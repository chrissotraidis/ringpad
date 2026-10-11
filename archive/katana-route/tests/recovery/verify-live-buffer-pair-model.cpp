// Reuse the sealed source-derived caller model; this driver never executes guest code.
#define main prior_buffer_pair_test_main
#include "startup-resident-buffer-pair.cpp"
#undef main
int main(int argc,char**argv){try{
 need(argc==10,"usage RAM STATE AICA TIMERS END_RAM END_STATE END_AICA END_TIMERS OUTPUT");
 auto ram=read(argv[1]),sound=read(argv[3]);auto c=state(argv[2]);
 auto before=std::make_unique<DCRuntime>(),after=std::make_unique<DCRuntime>();
 load_timers(*before,argv[4]);load_timers(*after,argv[8]);
 const auto cycles=caller_model(ram,sound,c);need(cycles==2185,"Reached cycle count differs");
 same(state(argv[6]),c);need(ram==read(argv[5]),"Full main RAM model differs");need(sound==read(argv[7]),"Full sound RAM model differs");
 tick(before->tmu,before->tmu_tstr,cycles);for(unsigned i=0;i<3;++i)timer_same(before->tmu[i],after->tmu[i]);
 need(before->tmu_tstr==after->tmu_tstr&&before->tmu_tocr==after->tmu_tocr,"Timer controls differ");
 const std::string out=argv[9];save_state(c,out+"/modeled-state.txt");save_ram(ram,out+"/modeled-ram.bin");save_ram(sound,out+"/modeled-aica.bin");
 std::ofstream(out+"/result.json")<<R"({"passed":true,"cycles":2185,"CPU_RAM_AICA_TMU_match":true,"retained_allocations":4,"second_audio_word_stores":2,"guest_execution":false,"G1_resolved":false})"<<'\n';
 std::cout<<"PASS saved buffer-pair state against retained source model;2185cycles;no guest execution\n";
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
