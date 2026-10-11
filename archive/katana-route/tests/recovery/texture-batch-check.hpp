#pragma once
// Independent PVM/allocator model for the two original Sonic texture files.
// It builds expected VRAM from file bytes, never from guest output descriptors.
// 669A00 computes 640*480*2 bytes per framebuffer. 65AB6C subtracts
// four 0x11C000 buffers and two framebuffers from 8 MiB;6671E0 reserves
// the remainder in bank0.667060 reserves both framebuffers in bank1.
struct GraphicsReservations {
    static constexpr unsigned frame=640*480*2;
    static constexpr unsigned used[2]={0x800000-4*0x11C000-2*frame,2*frame};
    unsigned free_nodes[2]{},nodes[2]{},owners[2]{};
    explicit GraphicsReservations(DCRuntime&rt){
        owners[0]=0x8CE3F000-0x80; //65ABC4 stack output argument, not live storage.
        owners[1]=dc_read32(rt,0x8C8A758C+28); //original framebuffer record argument
        require(owners[1]>=0x8C000000 && owners[1]+32<=0x8D000000,"Framebuffer owner outside RAM");
        for(unsigned bank=0;bank<2;++bank){
            const unsigned root=0x8C680020+bank*20;
            nodes[bank]=dc_read32(rt,root);free_nodes[bank]=dc_read32(rt,root+8);
        }
        check(rt);
    }
    void check(DCRuntime&rt)const{
        for(unsigned bank=0;bank<2;++bank){
            const unsigned root=0x8C680020+bank*20,node=nodes[bank],free=free_nodes[bank];
            require(node>=0x8C000000 && node+24<=0x8D000000 && free>=0x8C000000 && free+24<=0x8D000000,
                "Graphics VRAM node outside RAM");
            require(dc_read32(rt,root)==node && dc_read32(rt,root+4)==node
                && dc_read32(rt,root+8)==free && dc_read32(rt,root+12)==free
                && dc_read32(rt,root+16)==0x400000-used[bank],"Graphics VRAM bank accounting differs");
            require(dc_read16(rt,node)==(bank?0xD:0x2005) && dc_read32(rt,node+4)==0
                && dc_read32(rt,node+8)==0 && dc_read32(rt,node+12)==bank*0x400000
                && dc_read32(rt,node+16)==used[bank] && dc_read32(rt,node+20)==owners[bank],
                "Graphics VRAM reservation flags/extent/owner differs");
            require(dc_read32(rt,free+4)==0 && dc_read32(rt,free+8)==0
                && dc_read32(rt,free+12)==bank*0x400000+used[bank]
                && dc_read32(rt,free+16)==0x400000-used[bank],"Graphics VRAM free extent differs");
        }
    }
};
static void check_texture_batches(DCRuntime&rt,const std::vector<std::uint8_t>&before,const GraphicsReservations&graphics){
    auto expected_vram=before;
    require(expected_vram.size()==0x800000,"Unexpected VRAM size");
    struct Allocation{unsigned record,offset,size;};
    std::vector<Allocation> allocations[2];
    unsigned used[2]={graphics.used[0],graphics.used[1]},slot=0,total_bytes=0;
    auto expect=[&](unsigned address,unsigned value,const char*label){
        const auto actual=dc_read32(rt,address);
        if(actual!=value)throw std::runtime_error(std::string(label)+" slot="+std::to_string(slot)
            +" address="+std::to_string(address)+" expected="+std::to_string(value)+" actual="+std::to_string(actual));
    };
    for(unsigned file=0;file<2;++file){
        const auto*data=file?effect_decoded:sonic_decoded;
        const unsigned size=file?sizeof(effect_decoded):sizeof(sonic_decoded),count=file?29:28;
        const unsigned list=file?0x8C1CA528:0x8C1CA87C;
        auto u16=[&](unsigned off){require(off+2<=size,"PVM reference bounds");return unsigned(data[off])|(unsigned(data[off+1])<<8);};
        auto u32=[&](unsigned off){return u16(off)|(u16(off+2)<<16);};
        require(u32(0)==0x484D5650 && u16(8)==0x109 && u16(10)==count,"PVM identity differs");
        unsigned chunk=8+u32(4);
        for(unsigned i=0;i<count;++i,++slot){
            require(u32(chunk)==0x54525650,"Missing PVRT chunk");
            const unsigned format=u32(chunk+8),w=u16(chunk+12),h=u16(chunk+14);
            const bool mip=(format>>8)==2;
            require(((format>>8)==1 || mip) && (format&255)<=2 && w==h && w>=8 && w<=64,"Unhandled oracle texture format");
            unsigned bytes=w*h*2;
            if(mip){bytes=8;for(unsigned side=w;side>=2;side/=2)bytes+=side*side*2;}
            const unsigned aligned=(bytes+31)&~31u,bank=used[0]<=used[1]?0:1,offset=bank*0x400000+used[bank];
            const unsigned dest=offset+(mip?4:0),record=0x8C7649F4+slot*68;
            require(chunk+16+bytes<=size && bytes<=u32(chunk+4)-8 && dest+bytes<=offset+aligned,"Texture payload/extent bounds differ");
            const unsigned pixel=(format&255)<<27,flags=mip?5:4;
            unsigned dimension=0;for(unsigned side=w;side>8;side/=2)++dimension;
            expect(record,u32(12+i*34+30),"Global ID");expect(record+4,0xFFFFFFFF,"Secondary ID");
            expect(record+8,(dimension<<3)|dimension,"Texture size header");
            expect(record+12,(mip?0x80000000u:0u)|pixel|(offset>>3),"Texture control header");
            expect(record+16,2,"Texture registration flags");
            require(dc_read16(rt,record+64)==1,"Texture use count differs");
            expect(record+20,0x0CD00000+chunk+16,"Source descriptor");
            expect(record+24,(format<<16)|2,"Surface format");expect(record+28,1,"Surface type");
            expect(record+32,pixel,"Pixel format");expect(record+36,w,"Width");expect(record+40,h,"Height");
            expect(record+44,bytes,"Payload size");expect(record+48,flags,"Surface flags");expect(record+52,offset,"VRAM address");
            expect(list+i*12+8,record,"Permanent texture-list link");
            for(unsigned j=0;j<bytes;++j)expected_vram[dest+j]=data[chunk+16+j];
            allocations[bank].push_back({record,offset,aligned});used[bank]+=aligned;total_bytes+=bytes;
            chunk+=8+u32(chunk+4);
        }
        require(chunk==size,"PVM trailing bytes unaccounted");
    }
    require(slot==57,"Texture count differs");
    expect(0x8C677A58,slot,"Manager cursor");expect(0x8C893920,slot,"Manager count");
    for(unsigned i=slot;i<2048;++i){
        const unsigned record=0x8C7649F4+i*68;
        require(dc_read32(rt,record)==0xFFFFFFFF && dc_read32(rt,record+4)==0xFFFFFFFF
            && dc_read16(rt,record+64)==0,"Unused texture record changed");
    }
    for(unsigned bank=0;bank<2;++bank){
        const unsigned root=0x8C680020+bank*20,free=graphics.free_nodes[bank];
        expect(root+8,free,"Free head");expect(root+12,free,"Free tail");expect(root+16,0x400000-used[bank],"Free accounting");
        expect(free+4,0,"Free previous");expect(free+8,0,"Free next");
        expect(free+12,bank*0x400000+used[bank],"Free extent start");expect(free+16,0x400000-used[bank],"Free extent size");
        unsigned node=dc_read32(rt,root),previous=0;
        for(auto it=allocations[bank].rbegin();it!=allocations[bank].rend();++it){
            require(node>=0x8C000000 && node+24<=0x8D000000,"Used allocation outside main RAM");
            require(dc_read16(rt,node)==(bank?0x45:0x25),"Allocation bank flags differ");
            expect(node+4,previous,"Used previous");expect(node+12,it->offset,"Used extent offset");
            expect(node+16,it->size,"Used extent size");expect(node+20,it->record+24,"Used extent owner");
            previous=node;node=dc_read32(rt,node+8);
        }
        require(node==graphics.nodes[bank],"Graphics allocation missing after textures");
        expect(node+4,previous,"Graphics used previous");expect(node+8,0,"Graphics used next");
        expect(node+12,bank*0x400000,"Graphics offset");expect(node+16,graphics.used[bank],"Graphics size");
        expect(node+20,graphics.owners[bank],"Graphics owner");expect(root+4,node,"Used tail");
    }
    require(rt.vram==expected_vram,"Texture batch VRAM bytes or outside-range preservation differs");
    std::cout<<"PASS original texture batches: 28 Sonic + 29 effect textures; "<<total_bytes
        <<" exact VRAM bytes; all outside bytes unchanged; headers/list links/57 registrations; both banks/owners; allocations="
        <<used[0]<<','<<used[1]<<"; no displayed frame\n";
}
