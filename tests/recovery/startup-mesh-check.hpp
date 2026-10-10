#pragma once
// Bounded first-mesh projection oracle. Source vertices are snapshotted before
// display. The guest-produced XF matrix/projection state are explicit inputs;
// this does not independently validate the camera or the full model hierarchy.
struct StartupMeshReference {
    static constexpr unsigned model=0x8C1D2274, count=43;
    unsigned source;
    std::array<unsigned,count*3> words{};
    explicit StartupMeshReference(DCRuntime&rt):source(dc_read32(rt,model)) {
        require(source==0x8C1D1E6C && dc_read32(rt,model+8)==count,
            "First mesh source layout differs");
        for(unsigned i=0;i<words.size();++i)words[i]=dc_read32(rt,source+i*4);
    }
    double check(DCRuntime&rt,const SH4Context&ctx)const {
        require(ctx.gbr==0x8C8FFE00 && dc_read32(rt,ctx.gbr+60)==0x8C7B0B60,
            "First mesh projection workspace differs");
        const auto f=[&](unsigned address){return double(std::bit_cast<float>(dc_read32(rt,address)));};
        const double cx=f(ctx.gbr),cy=f(ctx.gbr+4),focal=f(ctx.gbr+8),aspect=f(ctx.gbr+12);
        double max_error=0;
        for(unsigned n=0;n<count;++n){
            double input[4]={0,0,0,1},view[3]{};
            for(unsigned k=0;k<3;++k){
                require(dc_read32(rt,source+(n*3+k)*4)==words[n*3+k],"First mesh source changed during display");
                input[k]=std::bit_cast<float>(words[n*3+k]);
            }
            for(unsigned row=0;row<3;++row)for(unsigned k=0;k<4;++k)
                view[row]+=double(std::bit_cast<float>(ctx.xf_bits[k*4+row]))*input[k];
            require(std::isfinite(view[2]) && std::abs(view[2])>0.000001,"First mesh projection singular");
            const double expected[]={cx+view[0]*focal/view[2],cy+view[1]*focal*aspect/view[2],1/view[2]};
            for(unsigned k=0;k<3;++k){
                const double actual=f(0x8C7B0B60+n*16+k*4),error=std::abs(actual-expected[k]);
                max_error=std::max(max_error,error);
                if(!std::isfinite(actual) || error>0.0003+0.00002*std::abs(expected[k]))
                    throw std::runtime_error("First mesh projected value differs vertex="+std::to_string(n)+
                        " component="+std::to_string(k)+" expected="+std::to_string(expected[k])+" actual="+std::to_string(actual));
            }
        }
        return max_error;
    }
};

// First textured mesh only. Strip indices/UVs are captured before guest display;
// projected coordinates have the independent oracle above. Lighting values and
// the material header remain guest-produced inputs to this serialization check.
struct StartupStripReference {
    struct Vertex{unsigned index;std::int16_t u,v;bool last;};
    std::vector<Vertex> vertices;
    unsigned mesh,indices,uvs,output;
    std::vector<DCRuntime::PVRTAStreamRaw> prefix;
    std::vector<std::uint16_t> source_indices;
    float uv_scale;
    explicit StartupStripReference(DCRuntime&rt){
        mesh=dc_read32(rt,StartupMeshReference::model+12);
        require(mesh==0x8C1D1E3C && dc_read16(rt,mesh)==0xC000 && dc_read16(rt,mesh+2)==12,
            "First textured strip layout differs");
        indices=dc_read32(rt,mesh+4);uvs=dc_read32(rt,mesh+20);
        require(indices==0x8C1D1B7C && uvs==0x8C1D1C7C,"First strip source pointers differ");
        unsigned cursor=indices,uv=uvs;
        for(unsigned strip=0;strip<12;++strip){
            const auto raw=dc_read16(rt,cursor);cursor+=2;source_indices.push_back(raw);
            const unsigned length=raw&0x7FFF;
            require(length>=3 && length<=43,"First strip length outside source vertex count");
            for(unsigned i=0;i<length;++i){
                const unsigned index=dc_read16(rt,cursor);cursor+=2;source_indices.push_back(index);
                require(index<43,"First strip vertex index outside mesh");
                vertices.push_back({index,std::bit_cast<std::int16_t>(std::uint16_t(dc_read16(rt,uv))),
                    std::bit_cast<std::int16_t>(std::uint16_t(dc_read16(rt,uv+2))),i+1==length});uv+=4;
            }
        }
        require(vertices.size()==98,"First strip vertex total differs");
        const unsigned record=dc_read32(rt,0x8C893F54);
        output=dc_read32(rt,record); //609AEC+16 ->0377D8: direct opaque FIFO
        require(output==0x10000000,"First strip output is not original direct TA FIFO");
        prefix=rt.pvr_ta_stream;
        uv_scale=std::bit_cast<float>(dc_read32(rt,0x8C0391B0));
        require(std::isfinite(uv_scale) && std::abs(uv_scale-1.0f/255)<1e-8,"Original UV scale differs");
    }
    void check(DCRuntime&rt,const SH4Context&ctx)const{
        require(ctx.r[2]==mesh && dc_read32(rt,ctx.gbr+84)==0
            && dc_read32(rt,ctx.gbr+56)==0x8C03DA00,"First strip branch/input state differs");
        require((ctx.r[0]&0x03FFFFFF)==((output+32+98*32)&0x03FFFFFF),"First strip output cursor differs");
        require(rt.pvr_ta_stream.size()==prefix.size()+99,"First strip FIFO packet count differs");
        for(unsigned k=0;k<prefix.size();++k)require(rt.pvr_ta_stream[k].bytes==prefix[k].bytes,"Existing FIFO prefix changed");
        const auto packet_word=[&](unsigned packet,unsigned word){
            const auto&bytes=rt.pvr_ta_stream[prefix.size()+packet].bytes;
            const unsigned off=word*4;
            return unsigned(bytes[off])|(unsigned(bytes[off+1])<<8)|(unsigned(bytes[off+2])<<16)|(unsigned(bytes[off+3])<<24);
        };
        for(unsigned k=0;k<source_indices.size();++k)
            require(dc_read16(rt,indices+k*2)==source_indices[k],"First strip indices changed");
        for(unsigned k=0;k<4;++k){
            unsigned expected=dc_read32(rt,0x8C894384+k*4);
            if(k==2)expected=(expected&0x03EFFFFF)|0x20080000;
            require(packet_word(0,k)==expected,"First material header serialization differs");
        }
        for(unsigned n=0;n<vertices.size();++n){
            const auto&v=vertices[n];const unsigned packet=n+1,projected=0x8C7B0B60+v.index*16;
            require(packet_word(packet,0)==(v.last?0xF0000000u:0xE0000000u),"First strip terminator differs");
            for(unsigned k=0;k<3;++k)require(packet_word(packet,1+k)==dc_read32(rt,projected+k*4),
                "First strip projected coordinate serialization differs");
            require(dc_read16(rt,uvs+n*4)==std::bit_cast<std::uint16_t>(v.u)
                && dc_read16(rt,uvs+n*4+2)==std::bit_cast<std::uint16_t>(v.v),"First strip UV source changed");
            require(packet_word(packet,4)==std::bit_cast<unsigned>(float(v.u)*uv_scale)
                && packet_word(packet,5)==std::bit_cast<unsigned>(float(v.v)*uv_scale),"First strip UV conversion differs");
            require(packet_word(packet,6)==dc_read32(rt,projected+12)
                && packet_word(packet,7)==dc_read32(rt,0x8C03DA00+v.index*4),"First strip lighting serialization differs");
        }
    }
};

// Test-only observation of the original output-pointer routine. Every call
// delegates to its generated body; no return, guest PC, or result is supplied.
// Disable direct bypass only for this scope and restore registration on exit.
struct StartupDisplayObservation {
    inline static StartupDisplayObservation* active=nullptr;
    DCRuntime& runtime;
    StartupMeshReference mesh_reference;
    StartupStripReference strip_reference;
    RecompiledFunction original;
    bool direct_enabled;
    unsigned updates=0;
    explicit StartupDisplayObservation(DCRuntime&rt):runtime(rt),mesh_reference(rt),strip_reference(rt),
        original(rt.targets.at(0x8C037950)),direct_enabled(rt.direct_dispatch_enabled){
        require(active==nullptr && original!=nullptr,"Nested or missing display observer target");
        active=this;
        rt.direct_dispatch_enabled=false;dc_refresh_dispatch_ready(rt);
        rt.register_target(0x8C037950,observe);
    }
    ~StartupDisplayObservation(){
        runtime.register_target(0x8C037950,original);
        runtime.direct_dispatch_enabled=direct_enabled;dc_refresh_dispatch_ready(runtime);
        active=nullptr;
    }
    StartupDisplayObservation(const StartupDisplayObservation&)=delete;
    static void observe(SH4Context&ctx,DCRuntime&rt){
        require(active && &active->runtime==&rt,"Display observer runtime differs");
        auto&self=*active;
        if(self.updates==0)self.check_first(rt,ctx);
        const auto destination=dc_read32(rt,0x8C8943AC);
        const auto expected=(ctx.r[14]&0x03FFFFFF)|dc_read32(rt,0x8C8943A8);
        self.original(ctx,rt);
        require(!rt.sh4_async_redirect && dc_read32(rt,destination)==expected,
            "Original mesh output-pointer update differs");
        ++self.updates;
    }
    void check_first(DCRuntime&rt,const SH4Context&ctx){
        const double mesh_error=mesh_reference.check(rt,ctx);
        std::cout<<"PASS first mesh projection: 43 vertices/129 coordinates; source snapshot unchanged; guest matrix/projection inputs; max_error="<<mesh_error<<'\n';
        const auto saved_coordinate=dc_read32(rt,0x8C7B0B60);
        dc_write32(rt,0x8C7B0B60,0x7FC00000); //negative control: nonfinite X
        bool corrupt_rejected=false;
        try{mesh_reference.check(rt,ctx);}catch(const std::runtime_error&){corrupt_rejected=true;}
        dc_write32(rt,0x8C7B0B60,saved_coordinate);
        require(corrupt_rejected,"First mesh oracle accepted corrupt coordinate");
        mesh_reference.check(rt,ctx);
        std::cout<<"PASS first mesh negative control: corrupt coordinate rejected; exact original restored\n";
        strip_reference.check(rt,ctx);
        std::cout<<"PASS first textured strip packets: 12 strips/98 vertices/3136 bytes; projected XYZ; source signed UVs; end flags; guest lighting/header serialization; FIFO count/prefix checked; deferred parse\n";
        auto&flag_byte=rt.pvr_ta_stream[strip_reference.prefix.size()+1].bytes[3];
        const auto first_flag=flag_byte;flag_byte^=0x10;
        bool strip_corruption_rejected=false;
        try{strip_reference.check(rt,ctx);}catch(const std::runtime_error&){strip_corruption_rejected=true;}
        flag_byte=first_flag;
        require(strip_corruption_rejected,"First strip oracle accepted corrupt end flag");
        strip_reference.check(rt,ctx);
        std::cout<<"PASS first strip negative control: corrupt end flag rejected; exact original restored\n";
    }
};
