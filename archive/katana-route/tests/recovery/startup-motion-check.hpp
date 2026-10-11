#pragma once
#include <array>
#include <cmath>
// Independent first-pose reference. Source key zero is sampled before execution;
// parent*T*Rz*Ry*Rx*S is evaluated in host doubles, without SH-4 helpers.
struct StartupMotionReference {
    using Matrix=std::array<double,16>;
    struct Node {unsigned address,flags; std::array<unsigned,3> position,rotation,scale; bool keyed[2]; Matrix matrix;};
    std::vector<Node> nodes;
    static Matrix identity(){Matrix m{};for(unsigned i=0;i<4;++i)m[i*5]=1;return m;}
    static Matrix multiply(const Matrix&a,const Matrix&b){
        Matrix c{};for(unsigned col=0;col<4;++col)for(unsigned row=0;row<4;++row)
            for(unsigned k=0;k<4;++k)c[col*4+row]+=a[k*4+row]*b[col*4+k];return c;
    }
    explicit StartupMotionReference(DCRuntime&rt){
        require(dc_read32(rt,0x8C1C92D8)==0x8C201EDC && dc_read32(rt,0x8C201ED0)==0x8C201A40
            && dc_read16(rt,0x8C201ED8)==3,"First-pose source layout differs");
        struct Pending{unsigned address;Matrix parent;};std::vector<Pending> todo{{0x8C1D24A4,identity()}};
        while(!todo.empty()){
            auto p=todo.back();todo.pop_back();const unsigned a=p.address,index=nodes.size();
            require(index<73,"Motion model tree exceeds reference");
            Node n{};n.address=a;n.flags=dc_read32(rt,a);
            for(unsigned k=0;k<3;++k){n.position[k]=dc_read32(rt,a+8+k*4);n.rotation[k]=dc_read32(rt,a+20+k*4);n.scale[k]=dc_read32(rt,a+32+k*4);}
            for(unsigned channel=0;channel<2;++channel){
                const unsigned record=0x8C201A40+index*16,key=dc_read32(rt,record+channel*4);
                n.keyed[channel]=key!=0;
                if(key){
                    require(dc_read32(rt,record+8+channel*4)>0 && dc_read32(rt,key)==0,"First-pose key zero absent");
                    for(unsigned k=0;k<3;++k)(channel?n.rotation:n.position)[k]=dc_read32(rt,key+4+k*4);
                }
            }
            Matrix local=identity();for(unsigned k=0;k<3;++k)local[12+k]=std::bit_cast<float>(n.position[k]);
            if(n.keyed[1] || !(n.flags&2))for(unsigned axis:{2u,1u,0u}){
                const double angle=(n.rotation[axis]&65535)*6.2831853071795864769/65536.0;
                const double c=std::cos(angle),s=std::sin(angle);Matrix r=identity();
                const unsigned i=(axis+1)%3,j=(axis+2)%3;
                r[i*4+i]=c;r[j*4+j]=c;r[i*4+j]=s;r[j*4+i]=-s;
                local=multiply(local,r);
            }
            if(!(n.flags&4))for(unsigned col=0;col<3;++col)for(unsigned row=0;row<4;++row)
                local[col*4+row]*=std::bit_cast<float>(n.scale[col]);
            n.matrix=multiply(p.parent,local);nodes.push_back(n);
            if(auto sibling=dc_read32(rt,a+48))todo.push_back({sibling,p.parent});
            if(auto child=dc_read32(rt,a+44))todo.push_back({child,n.matrix});
        }
        require(nodes.size()==73,"First-pose tree count differs");
    }
    void check(DCRuntime&rt,unsigned work)const{
        const unsigned control=work+288,action=dc_read32(rt,control+40),motion=dc_read32(rt,action+4);
        require(dc_read16(rt,control)==2 && dc_read16(rt,control+4)==0 && dc_read16(rt,control+6)==0
            && dc_read32(rt,control+16)==0x3D800000,"First blend selection/time differs");
        require(action>=0x8CE40000 && action+8<0x8CEC0000 && dc_read32(rt,action)==0x8C1D24A4
            && motion>=0x8CE40000 && motion+12<0x8CEC0000,"First blend action/motion extent differs");
        require(dc_read32(rt,motion+4)==2 && dc_read16(rt,motion+8)==3 && dc_read16(rt,motion+10)==2,
            "First blend frame/channel layout differs");
        const unsigned records=dc_read32(rt,motion);unsigned channels=0;double max_error=0;
        require(records>=0x8CE40000 && records+73*16<0x8CEC0000,"First blend records outside heap");
        for(unsigned i=0;i<nodes.size();++i){const auto&n=nodes[i];
            for(unsigned channel=0;channel<2;++channel){
                const unsigned record=records+i*16,key=dc_read32(rt,record+channel*4),count=dc_read32(rt,record+8+channel*4);
                if(!n.keyed[channel]){require(key==0 && count==0,"Unexpected blend channel");continue;}
                ++channels;require(count==2 && key>=0x8CE40000 && key+32<0x8CEC0000,"Blend key allocation differs");
                for(unsigned frame=0;frame<2;++frame){
                    require(dc_read32(rt,key+frame*16)==frame,"Blend frame index differs");
                    for(unsigned k=0;k<3;++k){const unsigned actual=dc_read32(rt,key+frame*16+4+k*4);
                        if(channel)require((actual&65535)==(n.rotation[k]&65535),"Blend first-pose angle differs");
                        else require(actual==n.position[k],"Blend first-pose position differs");
                    }
                }
            }
            for(unsigned k=0;k<16;++k){
                const double actual=std::bit_cast<float>(dc_read32(rt,0x8C78D4C8+i*64+k*4)),expected=n.matrix[k];
                const double error=std::abs(actual-expected);max_error=std::max(max_error,error);
                if(!std::isfinite(actual) || error>0.0002+0.00002*std::abs(expected))
                    throw std::runtime_error("Pose matrix differs node="+std::to_string(i)+" element="+std::to_string(k)
                        +" expected="+std::to_string(expected)+" actual="+std::to_string(actual));
            }
        }
        require(channels==19 && dc_read32(rt,0x8C78F4C8)==73 && dc_read32(rt,0x8C78F4CC)==74,"Pose channel/traversal counts differ");
        require(dc_read32(rt,0x8C89391C)==1 && dc_read32(rt,0x8C893878)==0x8C7F0B60,"Matrix stack not restored");
        std::cout<<"PASS original first motion pose: 19 keyed channels/two frames; 73 hierarchical matrices/1168 finite elements; max_error="<<max_error<<"; stack restored\n";
    }
};
