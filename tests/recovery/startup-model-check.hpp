#pragma once
#include <algorithm>
// Read original source records before guest initialization writes output indices
// or changes model links. Expected work bytes come from source vertex/normal
// arrays, not from the generated work buffers being checked.
struct StartupModelReference {
    struct Row { unsigned address,mode,first,second; std::vector<unsigned> words; };
    std::vector<Row> rows;
    explicit StartupModelReference(DCRuntime&rt){
        std::vector<unsigned> nodes,pending{0x8C1D24A4};
        while(!pending.empty()){
            const auto node=pending.back();pending.pop_back();
            require(node>=0x8C010000 && node+52<0x8C6803C0,"Model node outside resident source");
            require(nodes.size()<111 && std::find(nodes.begin(),nodes.end(),node)==nodes.end(),"Model tree cycle/bound");
            nodes.push_back(node);
            for(unsigned off:{48u,44u})if(auto child=dc_read32(rt,node+off))pending.push_back(child);
        }
        require(nodes.size()==73,"Source model node count differs");
        auto index=[&](unsigned target){
            auto it=std::find(nodes.begin(),nodes.end(),target);
            // Original search returns -1; the caller stores its low byte.
            return it==nodes.end()?255u:unsigned(it-nodes.begin());
        };
        for(unsigned i=0;i<34;++i){
            const unsigned a=0x8C1C9CDC+i*24,root=dc_read32(rt,a);
            Row row{a,dc_read8(rt,a+13),0,0,{}};
            if(root==0x0C909D2C)row.mode=0; // Original stage38 disables these rows.
            else {
                require(root==0x8C1D24A4,"Unexpected geometry source root");
                row.first=index(dc_read32(rt,a+4));
                if(row.mode==2){
                    row.second=index(dc_read32(rt,a+8));
                    const unsigned count=dc_read8(rt,a+12),config=dc_read32(rt,a+20);
                    for(unsigned j=0;j<count;++j)for(unsigned side=0;side<2;++side){
                        const auto model=dc_read32(rt,dc_read32(rt,a+4+side*4)+4);
                        const auto vertex=dc_read16(rt,config+j*4+side*2);
                        for(unsigned channel=0;channel<2;++channel){
                            const auto source=dc_read32(rt,model+channel*4)+vertex*12;
                            for(unsigned k=0;k<3;++k)row.words.push_back(dc_read32(rt,source+k*4));
                        }
                    }
                }else require(row.mode>=4 && row.mode<=9,"Unexpected geometry mode");
            }
            rows.push_back(row);
        }
        require(dc_read32(rt,0x8C1C9CDC+34*24)==0,"Geometry source terminator differs");
    }
    void check(DCRuntime&rt,unsigned work)const{
        unsigned copied=0,disabled=0;
        require(dc_read16(rt,work+290)==1 && dc_read32(rt,work+324)==0x8C1C9CDC,"Geometry control state differs");
        for(const auto&row:rows){
            require(dc_read8(rt,row.address+13)==row.mode,"Geometry stage mode differs");
            if(!row.mode){++disabled;continue;}
            require(dc_read8(rt,row.address+14)==row.first && dc_read8(rt,row.address+15)==row.second,"Geometry preorder indices differ");
            const auto buffer=dc_read32(rt,row.address+16);
            if(row.words.empty())require(buffer==0,"Index-only geometry allocated unexpected buffer");
            else {
                require(buffer>=0x8CE40000 && buffer+row.words.size()*4<0x8CEC0000,"Geometry work outside heap");
                for(unsigned i=0;i<row.words.size();++i)require(dc_read32(rt,buffer+i*4)==row.words[i],"Geometry source vertex/normal copy differs");
                copied+=row.words.size()*4;
            }
        }
        require(disabled==12 && copied==3264,"Geometry oracle scope differs");
        std::cout<<"PASS original model setup: 73 source nodes; 34 records; 12 disabled; 3264 exact vertex/normal bytes; preorder indices; geometry state1\n";
    }
};
static void check_startup_auxiliary(DCRuntime&rt,unsigned pool){
    const unsigned heads[]={dc_read32(rt,0x8C7A32D4),dc_read32(rt,0x8C7A32D8)};
    require(heads[0]==pool+96 && heads[1]==pool+192,"Auxiliary task queue heads differ");
    for(unsigned i=1;i<=4;++i){
        const auto task=pool+i*48,main=dc_read32(rt,task+32);
        const unsigned callback=i==1?0x8C0B564E:(i==2?0x8C0B450E:(i==3?0x8C0B3F58:0x8C0B3F96));
        require(dc_read32(rt,task+16)==callback && main>=0x8CE40000 && main+64<0x8CEC0000
            && dc_read8(rt,main+8)==0,"Auxiliary task callback/work/player differs");
        require(dc_read32(rt,task)==(i==2?pool+48:(i==4?pool+144:0))
            && dc_read32(rt,task+4)==(i==1?pool+96:(i==3?pool+192:0)),"Auxiliary queue links differ");
    }
    const auto aux=dc_read32(rt,pool+96+36);
    require(aux>=0x8CE40000 && aux+136<0x8CEC0000,"Auxiliary work extent differs");
    for(unsigned i=0;i<3;++i)require(dc_read32(rt,aux+88+i*4)==dc_read32(rt,0x8C1C9C40+i*4),"Auxiliary model pointer copy differs");
    require(dc_read32(rt,0x8C7A32C8)==pool+240,"Auxiliary task pool consumption differs");
    std::cout<<"PASS original auxiliary tasks: two group2 plus two group3 tasks; callbacks/links/player0; 136-byte work and three source pointers; five task nodes consumed\n";
}
