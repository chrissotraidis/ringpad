// Actual original file loader -> verified AOT module -> original registration.
// Supplied-scene fixtures remain explicit; original startup records its own continuations.
#include "board-manifest.hpp"
#include "registration-check.hpp"
#include <iterator>
#include <functional>
#include <sstream>
#include "capture-storage.hpp"
#ifdef RECOVERY_CONTINUATION_PROGRESS
#include "startup-continuation-observer.hpp"
#endif
#ifdef RECOVERY_MOVIE_ENTRY
#include "movie-entry/movie-entry-manifest.hpp"
struct MovieActivationNeeded {};
static thread_local std::function<void(unsigned)> movie_entry_observer;
static thread_local dcrecomp_generated::RecompiledFunction original_movie_first=nullptr;
static void observed_movie_entry(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_entry_observer)movie_entry_observer(0);dcrecomp_generated::movie_recomp_8CEB0000(c,rt);}
static void observed_movie_first(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_entry_observer)movie_entry_observer(1);original_movie_first(c,rt);}
#endif
#ifdef RECOVERY_MOVIE_CALLBACK
#include "movie-callback/movie-callback-manifest.hpp"
static thread_local std::function<void(unsigned)> movie_callback_observer;
static thread_local dcrecomp_generated::RecompiledFunction original_movie_work=nullptr;
static void observed_movie_callback(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_callback_observer)movie_callback_observer(0);dcrecomp_generated::movie_recomp_8CEB06F4(c,rt);}
static void observed_movie_work(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_callback_observer)movie_callback_observer(1);original_movie_work(c,rt);}
#endif
#ifdef RECOVERY_MOVIE_SERVICE
namespace dcrecomp_generated {void movie_service_register(DCRuntime&);void movie_setup_register(DCRuntime&);}
static thread_local std::function<void(unsigned)> movie_service_observer;
static thread_local std::array<dcrecomp_generated::RecompiledFunction,3> original_movie_services{};
template<unsigned I>static void observed_movie_service(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_service_observer)movie_service_observer(I);original_movie_services[I](c,rt);}
#endif
#ifdef RECOVERY_MOVIE_TAIL
namespace dcrecomp_generated {void movie_tail_register(DCRuntime&);}
static thread_local std::function<void(unsigned)> movie_tail_observer;
static thread_local std::array<dcrecomp_generated::RecompiledFunction,2> original_movie_tails{};
template<unsigned I>static void observed_movie_tail(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_tail_observer)movie_tail_observer(I);original_movie_tails[I](c,rt);}
#endif
#ifdef RECOVERY_MOVIE_MODE
namespace dcrecomp_generated {void movie_mode_register(DCRuntime&);}
static thread_local std::function<void(unsigned)> movie_mode_observer;
static thread_local std::array<dcrecomp_generated::RecompiledFunction,2> original_movie_modes{};
template<unsigned I>static void observed_movie_mode(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_mode_observer)movie_mode_observer(I);original_movie_modes[I](c,rt);}
#endif
#ifdef RECOVERY_MOVIE_CONFIG
#include "movie-config/movie-config-manifest.hpp"
#include "movie-helper/movie-helper-manifest.hpp"
static thread_local std::function<void(unsigned)> movie_config_observer;
static thread_local dcrecomp_generated::RecompiledFunction original_movie_clear=nullptr;
static void observed_movie_config(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_config_observer)movie_config_observer(0);dcrecomp_generated::movie_recomp_8CECFCE4(c,rt);}
static void observed_movie_helper(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_config_observer)movie_config_observer(1);dcrecomp_generated::movie_recomp_8CED5614(c,rt);}
static void observed_movie_clear(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_config_observer)movie_config_observer(2);original_movie_clear(c,rt);}
#endif
#ifdef RECOVERY_MOVIE_COUNTER
#include "movie-counter/movie-counter-manifest.hpp"
static thread_local std::function<void(bool)> movie_counter_observer;
static void observed_movie_counter(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_counter_observer)movie_counter_observer(false);dcrecomp_generated::movie_recomp_8CED09E0(c,rt);if(movie_counter_observer)movie_counter_observer(true);}
#endif
#ifdef RECOVERY_MOVIE_INIT
#include "movie-clear-tail/movie-clear-tail-manifest.hpp"
#include "movie-init/movie-init-manifest.hpp"
static thread_local std::function<void(unsigned,bool)> movie_init_observer;
static thread_local dcrecomp_generated::RecompiledFunction original_movie_bulk_clear=nullptr;
static void observed_movie_clear_tail(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_init_observer)movie_init_observer(0,false);dcrecomp_generated::movie_recomp_8CED5712(c,rt);}
static void observed_movie_init(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_init_observer)movie_init_observer(1,false);dcrecomp_generated::movie_recomp_8CED5ADE(c,rt);}
static void observed_movie_bulk_clear(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_init_observer)movie_init_observer(2,false);original_movie_bulk_clear(c,rt);if(movie_init_observer)movie_init_observer(2,true);}
#endif
#ifdef RECOVERY_MOVIE_RECORD_INIT
#include "movie-record-init/movie-record-init-manifest.hpp"
static thread_local std::function<void(unsigned)> movie_record_init_observer;
static thread_local dcrecomp_generated::RecompiledFunction original_movie_record_tail=nullptr;
static void observed_movie_record_init(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_record_init_observer)movie_record_init_observer(0);dcrecomp_generated::movie_recomp_8CED5A4E(c,rt);}
static void observed_movie_record_tail(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_record_init_observer)movie_record_init_observer(1);original_movie_record_tail(c,rt);}
#endif
#ifdef RECOVERY_MOVIE_GUARDED_CLEAR
#include "movie-guarded-clear/movie-guarded-clear-manifest.hpp"
static thread_local std::function<void(unsigned)> movie_guard_observer;
static thread_local dcrecomp_generated::RecompiledFunction original_movie_guard_clear=nullptr;
static void observed_movie_guard(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_guard_observer)movie_guard_observer(0);dcrecomp_generated::movie_recomp_8CED0A2C(c,rt);if(movie_guard_observer)movie_guard_observer(2);}
static void observed_movie_guard_clear(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_guard_observer)movie_guard_observer(1);original_movie_guard_clear(c,rt);}
#endif
#ifdef RECOVERY_MOVIE_RETURN_ZERO
#include "movie-return-zero/movie-return-zero-manifest.hpp"
static thread_local std::function<void(bool)> movie_zero_observer;
static void observed_movie_zero(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_zero_observer)movie_zero_observer(false);dcrecomp_generated::movie_recomp_8CED0940(c,rt);if(movie_zero_observer)movie_zero_observer(true);}
#endif
#ifdef RECOVERY_MOVIE_RESIDENT_INIT
namespace dcrecomp_generated {void movie_resident_init_register(DCRuntime&);void movie_resident_count_register(DCRuntime&);void movie_resident_buffer_register(DCRuntime&);}
static thread_local std::function<void(unsigned)> movie_resident_init_observer;
static thread_local dcrecomp_generated::RecompiledFunction real_movie_resident_init=nullptr,real_movie_resident_clear=nullptr;
static void observed_movie_resident_init(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_resident_init_observer)movie_resident_init_observer(0);real_movie_resident_init(c,rt);if(movie_resident_init_observer)movie_resident_init_observer(2);}
static void observed_movie_resident_clear(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_resident_init_observer)movie_resident_init_observer(1);real_movie_resident_clear(c,rt);}
#endif
#ifdef RECOVERY_MOVIE_CRITICAL
namespace dcrecomp_generated {void movie_allocation_register(DCRuntime&);void movie_lock_enter_register(DCRuntime&);void movie_lock_leave_register(DCRuntime&);}
static thread_local std::function<void(unsigned)> movie_critical_observer;
static thread_local dcrecomp_generated::RecompiledFunction real_movie_allocation=nullptr,real_movie_lock_enter=nullptr;
static void observed_movie_allocation(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_critical_observer)movie_critical_observer(0);real_movie_allocation(c,rt);if(movie_critical_observer)movie_critical_observer(3);}
static void observed_movie_lock_enter(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_critical_observer)movie_critical_observer(1);real_movie_lock_enter(c,rt);if(movie_critical_observer)movie_critical_observer(2);}
#endif
#ifdef RECOVERY_MOVIE_SLOTS
namespace dcrecomp_generated {void movie_slot_search_register(DCRuntime&);void movie_slot_status_register(DCRuntime&);void movie_slot_mark_register(DCRuntime&);void movie_slot_argument_register(DCRuntime&);}
static thread_local std::function<void(unsigned)> movie_slots_observer;
static thread_local dcrecomp_generated::RecompiledFunction real_movie_slot_search=nullptr;
static void observed_movie_slot_search(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_slots_observer)movie_slots_observer(0);real_movie_slot_search(c,rt);if(movie_slots_observer)movie_slots_observer(1);}
#endif
#ifdef RECOVERY_MOVIE_STREAM
namespace dcrecomp_generated {void movie_stream_register(DCRuntime&);void movie_context_register(DCRuntime&);}
static thread_local std::function<void(unsigned)> movie_stream_observer;
static thread_local dcrecomp_generated::RecompiledFunction real_movie_stream=nullptr,real_movie_context=nullptr,real_movie_stream_search=nullptr;
static void observed_movie_stream(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_stream_observer)movie_stream_observer(0);real_movie_stream(c,rt);if(movie_stream_observer)movie_stream_observer(4);}
static void observed_movie_context(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_stream_observer)movie_stream_observer(1);real_movie_context(c,rt);if(movie_stream_observer)movie_stream_observer(2);}
static void observed_movie_stream_search(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_stream_observer)movie_stream_observer(3);real_movie_stream_search(c,rt);}
#endif
#ifdef RECOVERY_MOVIE_CALLBACK_INSTALL
namespace dcrecomp_generated {void movie_callback_install_register(DCRuntime&);}
static thread_local std::function<void(unsigned)> movie_install_observer;
static thread_local dcrecomp_generated::RecompiledFunction real_movie_install=nullptr,real_movie_install_helper=nullptr;
static void observed_movie_install(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_install_observer)movie_install_observer(0);real_movie_install(c,rt);if(movie_install_observer)movie_install_observer(3);}
static void observed_movie_install_helper(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_install_observer)movie_install_observer(1);real_movie_install_helper(c,rt);if(movie_install_observer)movie_install_observer(2);}
#endif
#ifdef RECOVERY_MOVIE_MODE_CONFIG
#include "movie-mode-config/movie-mode-config-manifest.hpp"
#include "movie-mode-tail/movie-mode-tail-manifest.hpp"
static thread_local std::function<void(unsigned)> movie_mode_config_observer;
static thread_local dcrecomp_generated::RecompiledFunction real_movie_mode_division=nullptr;
static void observed_movie_mode_config(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_mode_config_observer)movie_mode_config_observer(0);dcrecomp_generated::movie_recomp_8CECF096(c,rt);if(movie_mode_config_observer)movie_mode_config_observer(4);}
static void observed_movie_mode_division(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_mode_config_observer)movie_mode_config_observer(1);real_movie_mode_division(c,rt);if(movie_mode_config_observer)movie_mode_config_observer(2);}
static void observed_movie_mode_tail(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_mode_config_observer)movie_mode_config_observer(3);dcrecomp_generated::movie_recomp_8CED56EE(c,rt);}
#endif
#ifdef RECOVERY_MOVIE_CALLBACK_REGISTER
#include "movie-callback-register/movie-callback-register-manifest.hpp"
#include "movie-callback-enter/movie-callback-enter-manifest.hpp"
#include "movie-callback-leave/movie-callback-leave-manifest.hpp"
static thread_local std::function<void(unsigned)> movie_callback_register_observer;
static void observed_movie_callback_register(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_callback_register_observer)movie_callback_register_observer(0);dcrecomp_generated::movie_recomp_8CED53C2(c,rt);if(movie_callback_register_observer)movie_callback_register_observer(5);}
static void observed_movie_callback_enter(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_callback_register_observer)movie_callback_register_observer(1);dcrecomp_generated::movie_recomp_8CED5324(c,rt);if(movie_callback_register_observer)movie_callback_register_observer(2);}
static void observed_movie_callback_leave(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_callback_register_observer)movie_callback_register_observer(3);dcrecomp_generated::movie_recomp_8CED533C(c,rt);if(movie_callback_register_observer)movie_callback_register_observer(4);}
#endif
#ifdef RECOVERY_MOVIE_CALLBACK_REGISTER2
#include "movie-callback-register2/movie-callback-register2-manifest.hpp"
static thread_local std::function<void(unsigned)> movie_callback_register2_observer;
static void observed_movie_callback_register2(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_callback_register2_observer)movie_callback_register2_observer(0);dcrecomp_generated::movie_recomp_8CED5494(c,rt);if(movie_callback_register2_observer)movie_callback_register2_observer(5);}
static void observed_movie_callback_enter2(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_callback_register2_observer)movie_callback_register2_observer(1);observed_movie_callback_enter(c,rt);if(movie_callback_register2_observer)movie_callback_register2_observer(2);}
static void observed_movie_callback_leave2(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_callback_register2_observer)movie_callback_register2_observer(3);observed_movie_callback_leave(c,rt);if(movie_callback_register2_observer)movie_callback_register2_observer(4);}
#endif
#ifdef RECOVERY_MOVIE_CALLBACK_REGISTER3
#include "movie-callback-register3/movie-callback-register3-manifest.hpp"
static thread_local std::function<void(unsigned)> movie_callback_register3_observer;
static void observed_movie_callback_register3(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_callback_register3_observer)movie_callback_register3_observer(0);dcrecomp_generated::movie_recomp_8CED5556(c,rt);if(movie_callback_register3_observer)movie_callback_register3_observer(5);}
static void observed_movie_callback_enter3(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_callback_register3_observer)movie_callback_register3_observer(1);observed_movie_callback_enter2(c,rt);if(movie_callback_register3_observer)movie_callback_register3_observer(2);}
static void observed_movie_callback_leave3(dcrecomp_generated::SH4Context&c,dcrecomp_generated::DCRuntime&rt){if(movie_callback_register3_observer)movie_callback_register3_observer(3);observed_movie_callback_leave2(c,rt);if(movie_callback_register3_observer)movie_callback_register3_observer(4);}
#endif
#ifndef RECOVERY_STARTUP_CONTINUATION_LIMIT
#define RECOVERY_STARTUP_CONTINUATION_LIMIT 256
#endif
#ifdef RECOVERY_SBOARD_CALLBACK
#include "sboard-callback-check.hpp"
#endif
#ifdef RECOVERY_SBOARD_CAMERA
#include "sboard-camera-check.hpp"
#endif
#ifdef RECOVERY_SBOARD_CAMERA
#include <functional>
static thread_local RecompiledFunction original_sdk_request=nullptr;
static thread_local std::function<void(bool)> sdk_request_observer;
static thread_local RecompiledFunction original_sound_completion=nullptr;
static thread_local std::function<void(bool)> sound_completion_observer;
static thread_local RecompiledFunction original_module_init=nullptr;
static thread_local std::function<void(bool)> module_init_observer;
static thread_local RecompiledFunction original_nested_clear=nullptr;
static thread_local std::function<void(bool)> nested_clear_observer;
static thread_local RecompiledFunction original_critical_leave=nullptr;
static thread_local std::function<void(bool)> critical_leave_observer;
static thread_local RecompiledFunction original_timer_init=nullptr;
static thread_local std::function<void(bool)> timer_init_observer;
static thread_local RecompiledFunction original_startup_work=nullptr;
static thread_local std::function<void(bool)> startup_work_observer;
static thread_local RecompiledFunction original_task_scan=nullptr;
static thread_local std::function<void(bool)> task_scan_observer;
static thread_local RecompiledFunction original_task_critical=nullptr;
static thread_local std::function<void(bool)> task_critical_observer;
#ifdef RECOVERY_RECORD_ITERATION
namespace dcrecomp_generated {void parent_register(DCRuntime&);void record_helpers_register(DCRuntime&);void helper_register(DCRuntime&);void serializer_register(DCRuntime&);void transport_register(DCRuntime&);void command_register(DCRuntime&);void submission_register(DCRuntime&);void worker_register(DCRuntime&);}
static thread_local RecompiledFunction original_record_iteration=nullptr;
static thread_local std::function<void(bool)> record_iteration_observer;
static void observed_record_iteration(SH4Context&ctx,DCRuntime&rt){
    if(record_iteration_observer)record_iteration_observer(false);
    original_record_iteration(ctx,rt);
    if(record_iteration_observer)record_iteration_observer(true);
}
#endif
#ifdef RECOVERY_SEVENBYTE
namespace dcrecomp_generated {void sevenbyte_register(DCRuntime&);}
static thread_local RecompiledFunction original_sevenbyte=nullptr;
static thread_local std::function<void(bool)> sevenbyte_observer;
static void observed_sevenbyte(SH4Context&ctx,DCRuntime&rt){
    if(sevenbyte_observer)sevenbyte_observer(false);
    original_sevenbyte(ctx,rt);
    if(sevenbyte_observer)sevenbyte_observer(true);
}
#endif
static thread_local RecompiledFunction original_startup_counter=nullptr;
#ifdef RECOVERY_WORD_COPY
namespace dcrecomp_generated {void word_copy_register(DCRuntime&);}
static thread_local RecompiledFunction original_word_copy=nullptr;
static thread_local std::function<void(bool)> word_copy_observer;
static void observed_word_copy(SH4Context&ctx,DCRuntime&rt){
    if(word_copy_observer)word_copy_observer(false);
    original_word_copy(ctx,rt);
    if(word_copy_observer)word_copy_observer(true);
}
#endif
static thread_local std::function<void(bool)> startup_counter_observer;
#ifdef RECOVERY_PAYLOAD_VALIDATOR
namespace dcrecomp_generated {void payload_validator_register(DCRuntime&);}
static thread_local RecompiledFunction original_payload_validator=nullptr;
static thread_local std::function<void(bool)> payload_validator_observer;
static void observed_payload_validator(SH4Context&ctx,DCRuntime&rt){
    if(payload_validator_observer)payload_validator_observer(false);
    original_payload_validator(ctx,rt);
    if(payload_validator_observer)payload_validator_observer(true);
}
#endif
static void observed_startup_counter(SH4Context& ctx,DCRuntime& rt){
    if(startup_counter_observer)startup_counter_observer(false);
    original_startup_counter(ctx,rt);
    if(startup_counter_observer)startup_counter_observer(true);
}
#ifdef RECOVERY_TIMER_CALLBACK
namespace dcrecomp_generated {void timer_callback_register(DCRuntime&);void timer_tail_register(DCRuntime&);}
static thread_local RecompiledFunction original_timer_callback=nullptr;
static thread_local std::function<void(bool)> timer_callback_observer;
static void observed_timer_callback(SH4Context&c,DCRuntime&rt){if(timer_callback_observer)timer_callback_observer(false);original_timer_callback(c,rt);if(timer_callback_observer)timer_callback_observer(true);}
#endif
#ifdef RECOVERY_RECORD_UPDATE
namespace dcrecomp_generated {void record_update_register(DCRuntime&);void record_query_register(DCRuntime&);}
static thread_local RecompiledFunction original_record_update=nullptr;
static thread_local std::function<void(bool)> record_update_observer;
static void observed_record_update(SH4Context&c,DCRuntime&rt){if(record_update_observer)record_update_observer(false);original_record_update(c,rt);if(record_update_observer)record_update_observer(true);}
#endif
#ifdef RECOVERY_RECORD_RESET
namespace dcrecomp_generated {void record_reset_register(DCRuntime&);}
static thread_local RecompiledFunction original_record_reset=nullptr;
static thread_local std::function<void(bool)> record_reset_observer;
static void observed_record_reset(SH4Context&c,DCRuntime&rt){if(record_reset_observer)record_reset_observer(false);original_record_reset(c,rt);if(record_reset_observer)record_reset_observer(true);}
#endif
#ifdef RECOVERY_RECORD_COUNTER
namespace dcrecomp_generated {void record_counter_register(DCRuntime&);}
static thread_local RecompiledFunction original_record_counter=nullptr;
static thread_local std::function<void(bool)> record_counter_observer;
static void observed_record_counter(SH4Context&c,DCRuntime&rt){if(record_counter_observer)record_counter_observer(false);original_record_counter(c,rt);if(record_counter_observer)record_counter_observer(true);}
#endif
#ifdef RECOVERY_RECORD_BUFFER
namespace dcrecomp_generated {void record_buffer_register(DCRuntime&);void bit_expand_register(DCRuntime&);void rectangle_copy_register(DCRuntime&);}
static thread_local RecompiledFunction original_record_buffer=nullptr;
static thread_local std::function<void(bool)> record_buffer_observer;
static void observed_record_buffer(SH4Context&c,DCRuntime&rt){if(record_buffer_observer)record_buffer_observer(false);original_record_buffer(c,rt);if(record_buffer_observer)record_buffer_observer(true);}
#endif
#ifdef RECOVERY_RECORD_INIT
namespace dcrecomp_generated {void record_init_register(DCRuntime&);void record_info_register(DCRuntime&);void info_wrapper_register(DCRuntime&);void timestamp_register(DCRuntime&);void bcd_register(DCRuntime&);void count_blocks_register(DCRuntime&);}
static thread_local RecompiledFunction original_record_init=nullptr;
static thread_local std::function<void(bool)> record_init_observer;
static void observed_record_init(SH4Context&c,DCRuntime&rt){if(record_init_observer)record_init_observer(false);original_record_init(c,rt);if(record_init_observer)record_init_observer(true);}
#endif
#ifdef RECOVERY_GLOBAL_RESET
namespace dcrecomp_generated {void global_reset_register(DCRuntime&);void global_setup_register(DCRuntime&);void arena_init_register(DCRuntime&);void global_setter_register(DCRuntime&);}
static thread_local RecompiledFunction original_global_reset=nullptr,original_arena_init=nullptr;
static thread_local std::function<void(bool,bool)> global_reset_observer;
static void observed_global_reset(SH4Context&c,DCRuntime&rt){if(global_reset_observer)global_reset_observer(false,false);original_global_reset(c,rt);if(global_reset_observer)global_reset_observer(false,true);}
static void observed_arena_init(SH4Context&c,DCRuntime&rt){if(global_reset_observer)global_reset_observer(true,false);original_arena_init(c,rt);if(global_reset_observer)global_reset_observer(true,true);}
#endif
#ifdef RECOVERY_GLOBAL_WORD_COPY
namespace dcrecomp_generated {void global_word_copy_register(DCRuntime&);}
static thread_local RecompiledFunction original_global_word_copy=nullptr,original_global_setup_tail=nullptr,original_global_record_tail=nullptr;
static thread_local std::function<void(unsigned)> global_word_observer;
static void observed_global_word_copy(SH4Context&c,DCRuntime&rt){if(global_word_observer)global_word_observer(0);original_global_word_copy(c,rt);if(global_word_observer)global_word_observer(1);}
static void observed_global_setup_tail(SH4Context&c,DCRuntime&rt){original_global_setup_tail(c,rt);if(global_word_observer)global_word_observer(2);}
static void observed_global_record_tail(SH4Context&c,DCRuntime&rt){if(global_word_observer)global_word_observer(3);original_global_record_tail(c,rt);}
#endif
#ifdef RECOVERY_FRAME_DISPATCH
namespace dcrecomp_generated {void frame_dispatch_register(DCRuntime&);void input_status_register(DCRuntime&);void frame_adjust_register(DCRuntime&);}
static thread_local std::array<RecompiledFunction,6> original_frame_calls{};
static thread_local std::function<void(unsigned,bool)> frame_observer;
template<unsigned I> static void observed_frame_call(SH4Context&c,DCRuntime&rt){if(frame_observer)frame_observer(I,false);original_frame_calls[I](c,rt);if(frame_observer)frame_observer(I,true);}
#endif
#ifdef RECOVERY_FRAME_QUAD
namespace dcrecomp_generated {void frame_quad_register(DCRuntime&);void quad_state_register(DCRuntime&);}
static thread_local std::array<RecompiledFunction,4> original_quad_calls{};
static thread_local std::function<void(unsigned,bool)> quad_observer;
template<unsigned I> static void observed_quad_call(SH4Context&c,DCRuntime&rt){if(quad_observer)quad_observer(I,false);original_quad_calls[I](c,rt);if(quad_observer)quad_observer(I,true);}
#endif
#ifdef RECOVERY_QUAD_PACKETS
namespace dcrecomp_generated {void quad_header_register(DCRuntime&);void quad_strip_register(DCRuntime&);void strip_writer_register(DCRuntime&);}
static thread_local std::array<RecompiledFunction,4> original_packet_calls{};
static thread_local std::function<void(unsigned,bool)> packet_observer;
template<unsigned I> static void observed_packet_call(SH4Context&c,DCRuntime&rt){if(packet_observer)packet_observer(I,false);original_packet_calls[I](c,rt);if(packet_observer)packet_observer(I,true);}
#endif
#ifdef RECOVERY_FRAME_SPRITES
namespace dcrecomp_generated {void frame_sprites_register(DCRuntime&);void sprite_header_register(DCRuntime&);}
static thread_local std::array<RecompiledFunction,6> original_sprite_calls{};
static thread_local std::function<void(unsigned,bool)> sprite_observer;
static thread_local std::function<void()> frame_mode_exit_observer;
template<unsigned I> static void observed_sprite_call(SH4Context&c,DCRuntime&rt){if(sprite_observer)sprite_observer(I,false);original_sprite_calls[I](c,rt);if(sprite_observer)sprite_observer(I,true);}
#endif
#ifdef RECOVERY_MODE_LOADER
namespace dcrecomp_generated {void mode_loader_register(DCRuntime&);void loader_owner_register(DCRuntime&);}
static thread_local std::array<RecompiledFunction,3> original_loader_calls{};
static thread_local std::function<void(unsigned,bool)> loader_observer;
template<unsigned I> static void observed_loader_call(SH4Context&c,DCRuntime&rt){if(loader_observer)loader_observer(I,false);original_loader_calls[I](c,rt);if(loader_observer)loader_observer(I,true);}
#endif
#ifdef RECOVERY_FILE_TRANSFER
namespace dcrecomp_generated {void file_transfer_register(DCRuntime&);}
static thread_local std::array<RecompiledFunction,2> original_file_calls{};
static thread_local std::function<void(unsigned,bool)> file_transfer_observer;
template<unsigned I> static void observed_file_transfer_call(SH4Context&c,DCRuntime&rt){if(file_transfer_observer)file_transfer_observer(I,false);original_file_calls[I](c,rt);if(file_transfer_observer)file_transfer_observer(I,true);}
#endif
#ifdef RECOVERY_RECORD_STATE2
namespace dcrecomp_generated {void record_state2_register(DCRuntime&);}
static thread_local RecompiledFunction original_record_state2=nullptr;
static thread_local std::function<void(bool)> record_state2_observer;
static void observed_record_state2(SH4Context&c,DCRuntime&rt){if(record_state2_observer)record_state2_observer(false);original_record_state2(c,rt);if(record_state2_observer)record_state2_observer(true);}
#endif
#ifdef RECOVERY_RECORD_READY
namespace dcrecomp_generated {void record_ready_register(DCRuntime&);void ready_query_register(DCRuntime&);void ready_setter_register(DCRuntime&);}
static thread_local RecompiledFunction original_record_ready=nullptr;
static thread_local std::function<void(bool)> record_ready_observer;
static void observed_record_ready(SH4Context&c,DCRuntime&rt){if(record_ready_observer)record_ready_observer(false);original_record_ready(c,rt);if(record_ready_observer)record_ready_observer(true);}
#endif
#ifdef RECOVERY_BLOCK_ITERATION
namespace dcrecomp_generated {void block_iteration_register(DCRuntime&);}
static thread_local RecompiledFunction original_block_iteration=nullptr,original_block_read=nullptr,original_block_worker=nullptr,original_block_query=nullptr;
static thread_local std::function<void(unsigned)> block_iteration_observer;
static void observed_block_iteration(SH4Context&c,DCRuntime&rt){if(block_iteration_observer)block_iteration_observer(0);original_block_iteration(c,rt);if(block_iteration_observer)block_iteration_observer(5);}
static void observed_block_read(SH4Context&c,DCRuntime&rt){if(block_iteration_observer)block_iteration_observer(1);original_block_read(c,rt);}
static void observed_block_worker(SH4Context&c,DCRuntime&rt){if(block_iteration_observer)block_iteration_observer(2);original_block_worker(c,rt);}
static void observed_block_query(SH4Context&c,DCRuntime&rt){if(block_iteration_observer)block_iteration_observer(3);original_block_query(c,rt);if(block_iteration_observer)block_iteration_observer(4);}
#endif
#ifdef RECOVERY_LINKED_BLOCKS
namespace dcrecomp_generated {void linked_blocks_register(DCRuntime&);void next_block_register(DCRuntime&);}
static thread_local RecompiledFunction original_linked_blocks=nullptr,original_linked_read=nullptr,original_linked_worker=nullptr,original_linked_query=nullptr;
static thread_local std::function<void(unsigned)> linked_blocks_observer;
static void observed_linked_blocks(SH4Context&c,DCRuntime&rt){if(linked_blocks_observer)linked_blocks_observer(0);original_linked_blocks(c,rt);if(linked_blocks_observer)linked_blocks_observer(5);}
static void observed_linked_read(SH4Context&c,DCRuntime&rt){if(linked_blocks_observer)linked_blocks_observer(1);original_linked_read(c,rt);}
static void observed_linked_worker(SH4Context&c,DCRuntime&rt){if(linked_blocks_observer)linked_blocks_observer(2);original_linked_worker(c,rt);}
static void observed_linked_query(SH4Context&c,DCRuntime&rt){if(linked_blocks_observer)linked_blocks_observer(3);original_linked_query(c,rt);if(linked_blocks_observer)linked_blocks_observer(4);}
#endif
#ifdef RECOVERY_MOVIE_SIZE
#include "movie-size-wrapper/movie-size-wrapper-manifest.hpp"
#ifdef RECOVERY_MOVIE_CREATE
static thread_local std::function<void()> create_observer;
static void observe_create(){if(create_observer)create_observer();}
#else
static void observe_create(){}
#endif
#include "movie-stream-size/movie-stream-size-manifest.hpp"
#include "movie-frame-size/movie-frame-size-manifest.hpp"
#ifdef RECOVERY_MOVIE_SIZE_RETURN
#include "movie-dimensions/movie-dimensions-manifest.hpp"
#endif
static thread_local std::function<void(unsigned)> movie_size_observer;
static thread_local RecompiledFunction real_movie_size_division=nullptr;
static void observed_movie_size_wrapper(SH4Context&c,DCRuntime&rt){if(movie_size_observer)movie_size_observer(0);movie_recomp_8CED02BA(c,rt);
#ifdef RECOVERY_MOVIE_SIZE_RETURN
if(movie_size_observer)movie_size_observer(9);
#endif
}
static void observed_movie_size_stream(SH4Context&c,DCRuntime&rt){if(movie_size_observer)movie_size_observer(1);observe_create();movie_recomp_8CED01BC(c,rt);observe_create();if(movie_size_observer)movie_size_observer(4);}
static void observed_movie_size_frame(SH4Context&c,DCRuntime&rt){if(movie_size_observer)movie_size_observer(5);observe_create();movie_recomp_8CED0256(c,rt);observe_create();
#ifdef RECOVERY_MOVIE_SIZE_RETURN
if(movie_size_observer)movie_size_observer(8);
#endif
}
#ifdef RECOVERY_MOVIE_SIZE_RETURN
static void observed_movie_dimensions(SH4Context&c,DCRuntime&rt){if(movie_size_observer)movie_size_observer(6);observe_create();movie_recomp_8CED0222(c,rt);observe_create();if(movie_size_observer)movie_size_observer(7);}
#endif
static void observed_movie_size_division(SH4Context&c,DCRuntime&rt){if(movie_size_observer)movie_size_observer(2);real_movie_size_division(c,rt);if(movie_size_observer)movie_size_observer(3);}
#endif
#ifdef RECOVERY_MOVIE_SETUP_BIND
static thread_local std::function<void(unsigned)> setup_bind_observer;
#endif
#ifdef RECOVERY_MOVIE_SLOT_SETUP
#include "movie-slot-setup/movie-slot-setup-manifest.hpp"
#include "movie-setup-validation/movie-setup-validation-manifest.hpp"
static thread_local std::function<void(unsigned)> movie_slot_setup_observer;
static thread_local RecompiledFunction real_movie_setup_copy=nullptr;
static void observed_movie_slot_setup(SH4Context&c,DCRuntime&rt){if(movie_slot_setup_observer)movie_slot_setup_observer(0);movie_recomp_8CECF0F8(c,rt);}
static void observed_movie_setup_validation(SH4Context&c,DCRuntime&rt){if(movie_slot_setup_observer)movie_slot_setup_observer(1);movie_recomp_8CED0350(c,rt);
#ifdef RECOVERY_MOVIE_SETUP_BIND
if(setup_bind_observer)setup_bind_observer(4);
#endif
}
static void observed_movie_setup_copy(SH4Context&c,DCRuntime&rt){if(movie_slot_setup_observer)movie_slot_setup_observer(2);real_movie_setup_copy(c,rt);if(movie_slot_setup_observer)movie_slot_setup_observer(3);}
#endif
#ifdef RECOVERY_MOVIE_BINDING_RETURN
static thread_local std::function<void(unsigned)> binding_return_observer;
#endif
#ifdef RECOVERY_MOVIE_GUARD_FINAL
static thread_local std::function<void(unsigned)> guard_final_observer;
#endif
#ifdef RECOVERY_MOVIE_DESCRIPTOR_INIT
#include "movie-descriptor-init/movie-descriptor-init-manifest.hpp"
#include "movie-descriptor-fields/movie-descriptor-fields-manifest.hpp"
static thread_local std::function<void(unsigned)> movie_descriptor_observer;
static void observed_movie_descriptor_init(SH4Context&c,DCRuntime&rt){if(movie_descriptor_observer)movie_descriptor_observer(0);movie_recomp_8CED18E4(c,rt);
#ifdef RECOVERY_MOVIE_GUARD_FINAL
if(guard_final_observer)guard_final_observer(4);
#endif
}
static void observed_movie_descriptor_fields(SH4Context&c,DCRuntime&rt){if(movie_descriptor_observer)movie_descriptor_observer(1);movie_recomp_8CED1908(c,rt);
#ifdef RECOVERY_MOVIE_BINDING_RETURN
if(binding_return_observer)binding_return_observer(6);
#endif
}
#endif
#ifdef RECOVERY_MOVIE_WORKAREA
#include "movie-word-fill/movie-word-fill-manifest.hpp"
namespace dcrecomp_generated {void movie_bounded_string_register(DCRuntime&);}
static thread_local std::function<void(unsigned)> movie_workarea_observer;
static thread_local RecompiledFunction real_movie_bounded_string=nullptr;
#ifdef RECOVERY_MOVIE_INSTANCE_INITIALIZE
static thread_local std::function<void()> instance_initialize_observer;
static void observe_instance_initialize(){if(instance_initialize_observer)instance_initialize_observer();}
#else
static void observe_instance_initialize(){}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_RATE
static thread_local std::function<void()> second_fields_observer;
static void observe_second_fields(){if(second_fields_observer)second_fields_observer();}
#else
static void observe_second_fields(){}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_RESOURCES
static thread_local std::function<void()> instance_resources_observer;
static void observe_instance_resources(){if(instance_resources_observer)instance_resources_observer();}
#else
static void observe_instance_resources(){}
#endif
static void observed_movie_word_fill(SH4Context&c,DCRuntime&rt){if(movie_workarea_observer)movie_workarea_observer(0);observe_instance_initialize();observe_second_fields();observe_instance_resources();movie_recomp_8CED67F8(c,rt);observe_instance_resources();observe_second_fields();observe_instance_initialize();if(movie_workarea_observer)movie_workarea_observer(1);}
static void observed_movie_bounded_string(SH4Context&c,DCRuntime&rt){unsigned phase=c.pr==0x8CED192C?2:4;if(movie_workarea_observer)movie_workarea_observer(phase);real_movie_bounded_string(c,rt);if(movie_workarea_observer)movie_workarea_observer(phase+1);}
#endif
#ifdef RECOVERY_MOVIE_WORD_COPY
#include "movie-word-copy/movie-word-copy-manifest.hpp"
#include "movie-record-clear/movie-record-clear-manifest.hpp"
static thread_local std::function<void(unsigned)> movie_word_copy_observer;
static thread_local RecompiledFunction real_movie_descriptor_copy=nullptr;
static void observed_movie_word_copy(SH4Context&c,DCRuntime&rt){if(movie_word_copy_observer)movie_word_copy_observer(0);observe_second_fields();movie_recomp_8CED6708(c,rt);observe_second_fields();if(movie_word_copy_observer)movie_word_copy_observer(1);}
static void observed_movie_descriptor_copy(SH4Context&c,DCRuntime&rt){if(movie_word_copy_observer)movie_word_copy_observer(2);real_movie_descriptor_copy(c,rt);if(movie_word_copy_observer)movie_word_copy_observer(3);}
static void observed_movie_record_clear(SH4Context&c,DCRuntime&rt){if(movie_word_copy_observer)movie_word_copy_observer(4);observe_second_fields();movie_recomp_8CED1A90(c,rt);observe_second_fields();if(movie_word_copy_observer)movie_word_copy_observer(5);}
#endif
#ifdef RECOVERY_MOVIE_RATE_INIT
#include "movie-rate-init/movie-rate-init-manifest.hpp"
#include "movie-context-return/movie-context-return-manifest.hpp"
static thread_local std::function<void(unsigned)> movie_rate_observer;
static void observed_movie_rate_init(SH4Context&c,DCRuntime&rt){if(movie_rate_observer)movie_rate_observer(0);movie_recomp_8CED4620(c,rt);if(movie_rate_observer)movie_rate_observer(1);}
static void observed_movie_context_return(SH4Context&c,DCRuntime&rt){if(movie_rate_observer)movie_rate_observer(2);movie_recomp_8CED97A8(c,rt);if(movie_rate_observer)movie_rate_observer(3);}
static void observed_movie_buffer_fill(SH4Context&c,DCRuntime&rt){if(movie_rate_observer)movie_rate_observer(4);observed_movie_word_fill(c,rt);if(movie_rate_observer)movie_rate_observer(5);}
#endif
#ifdef RECOVERY_MOVIE_INTERFACE
#include "movie-interface-wrapper/movie-interface-wrapper-manifest.hpp"
#include "movie-interface-bind/movie-interface-bind-manifest.hpp"
static thread_local std::function<void(unsigned)> movie_interface_observer;
static thread_local RecompiledFunction real_movie_interface_copy=nullptr;
static void observed_movie_interface_wrapper(SH4Context&c,DCRuntime&rt){if(movie_interface_observer)movie_interface_observer(0);movie_recomp_8CEDA288(c,rt);
#ifdef RECOVERY_MOVIE_BINDING_RETURN
if(binding_return_observer)binding_return_observer(5);
#endif
}
#ifdef RECOVERY_MOVIE_INSTANCE_VIDEO_COMPLETE
static thread_local std::function<void()> video_complete_observer;
static void observe_video_complete(){if(video_complete_observer)video_complete_observer();}
#else
static void observe_video_complete(){}
#endif
static void observed_movie_interface_copy(SH4Context&c,DCRuntime&rt){if(movie_interface_observer)movie_interface_observer(1);observe_create();observe_instance_initialize();observe_video_complete();real_movie_interface_copy(c,rt);observe_video_complete();observe_instance_initialize();observe_create();if(movie_interface_observer)movie_interface_observer(2);}
static void observed_movie_interface_bind(SH4Context&c,DCRuntime&rt){if(movie_interface_observer)movie_interface_observer(3);movie_recomp_8CEDA2B2(c,rt);
#ifdef RECOVERY_MOVIE_BINDING_RETURN
if(binding_return_observer)binding_return_observer(4);
#endif
}
#endif
#ifdef RECOVERY_MOVIE_METHOD_INIT
#include "movie-method-init/movie-method-init-manifest.hpp"
static thread_local std::function<void(unsigned)> movie_method_observer;
static void observed_movie_method_init(SH4Context&c,DCRuntime&rt){if(movie_method_observer)movie_method_observer(0);movie_recomp_8CED1C10(c,rt);if(movie_method_observer)movie_method_observer(1);}
#endif
#ifdef RECOVERY_MOVIE_METHOD_SETUP
#include "movie-method-callback/movie-method-callback-manifest.hpp"
#include "movie-method-setup/movie-method-setup-manifest.hpp"
static thread_local std::function<void(unsigned)> movie_setup_observer;
static void observed_movie_method_callback(SH4Context&c,DCRuntime&rt){if(movie_setup_observer)movie_setup_observer(0);movie_recomp_8CED1CAC(c,rt);}
static void observed_movie_method_setup(SH4Context&c,DCRuntime&rt){if(movie_setup_observer)movie_setup_observer(1);movie_recomp_8CED78C4(c,rt);}
#endif
#ifdef RECOVERY_MOVIE_METHOD_WORKAREA
#include "movie-method-workarea/movie-method-workarea-manifest.hpp"
#include "movie-method-record/movie-method-record-manifest.hpp"
static thread_local std::function<void(unsigned)> movie_method_workarea_observer;
static thread_local RecompiledFunction real_method_bounded_string=nullptr;
static void observed_method_workarea(SH4Context&c,DCRuntime&rt){if(movie_method_workarea_observer)movie_method_workarea_observer(0);movie_recomp_8CED78EA(c,rt);if(movie_method_workarea_observer)movie_method_workarea_observer(9);}
static void observed_method_fill(SH4Context&c,DCRuntime&rt){if(movie_method_workarea_observer)movie_method_workarea_observer(1);observed_movie_buffer_fill(c,rt);if(movie_method_workarea_observer)movie_method_workarea_observer(2);}
static void observed_method_copy(SH4Context&c,DCRuntime&rt){unsigned phase=c.pr==0x8CED791A?3:5;if(movie_method_workarea_observer)movie_method_workarea_observer(phase);real_method_bounded_string(c,rt);if(movie_method_workarea_observer)movie_method_workarea_observer(phase+1);}
static void observed_method_record(SH4Context&c,DCRuntime&rt){if(movie_method_workarea_observer)movie_method_workarea_observer(7);observe_instance_resources();movie_recomp_8CED79A4(c,rt);observe_instance_resources();if(movie_method_workarea_observer)movie_method_workarea_observer(8);}
#endif
#ifdef RECOVERY_MOVIE_METHOD_RETURN
#include "movie-followup-return/movie-followup-return-manifest.hpp"
#include "movie-final-return/movie-final-return-manifest.hpp"
static thread_local std::function<void(unsigned)> method_return_observer;
static void observed_followup_return(SH4Context&c,DCRuntime&rt){if(method_return_observer)method_return_observer(0);movie_recomp_8CED6840(c,rt);if(method_return_observer)method_return_observer(1);}
static void observed_final_return(SH4Context&c,DCRuntime&rt){if(method_return_observer)method_return_observer(2);movie_recomp_8CED7800(c,rt);if(method_return_observer)method_return_observer(3);}
static void observed_return_setup(SH4Context&c,DCRuntime&rt){observed_movie_method_setup(c,rt);if(method_return_observer)method_return_observer(4);}
static void observed_return_callback(SH4Context&c,DCRuntime&rt){observed_movie_method_callback(c,rt);if(method_return_observer)method_return_observer(5);}
#endif
#ifdef RECOVERY_MOVIE_INITIALIZER_RETURN
static thread_local std::function<void(unsigned)> initializer_return_observer;
#endif
#ifdef RECOVERY_MOVIE_THIRD_SETUP
#include "movie-third-callback/movie-third-callback-manifest.hpp"
#include "movie-third-setup/movie-third-setup-manifest.hpp"
static thread_local std::function<void(unsigned)> movie_third_observer;
static void observed_movie_third_callback(SH4Context&c,DCRuntime&rt){if(movie_third_observer)movie_third_observer(0);movie_recomp_8CED256A(c,rt);
#ifdef RECOVERY_MOVIE_INITIALIZER_RETURN
if(initializer_return_observer)initializer_return_observer(5);
#endif
}
static void observed_movie_third_setup(SH4Context&c,DCRuntime&rt){if(movie_third_observer)movie_third_observer(1);movie_recomp_8CED80AC(c,rt);
#ifdef RECOVERY_MOVIE_INITIALIZER_RETURN
if(initializer_return_observer)initializer_return_observer(4);
#endif
}
#endif
#ifdef RECOVERY_MOVIE_THIRD_VALIDATION
#include "movie-third-validation/movie-third-validation-manifest.hpp"
#include "movie-third-guard/movie-third-guard-manifest.hpp"
static thread_local std::function<void(unsigned)> movie_validation_observer;
static void observed_movie_validation(SH4Context&c,DCRuntime&rt){if(movie_validation_observer)movie_validation_observer(0);movie_recomp_8CED811A(c,rt);}
static void observed_third_guard(SH4Context&c,DCRuntime&rt){if(movie_validation_observer)movie_validation_observer(1);movie_recomp_8CEDAB10(c,rt);if(movie_validation_observer)movie_validation_observer(2);}
#endif
#ifdef RECOVERY_MOVIE_PAIR_CHECK
#include "movie-pair-check/movie-pair-check-manifest.hpp"
#include "movie-pair-compare/movie-pair-compare-manifest.hpp"
static thread_local std::function<void(unsigned)> movie_pair_observer;
static void observed_movie_pair_check(SH4Context&c,DCRuntime&rt){if(movie_pair_observer)movie_pair_observer(0);movie_recomp_8CEDC35C(c,rt);if(movie_pair_observer)movie_pair_observer(9);}
static void observed_movie_pair_compare(SH4Context&c,DCRuntime&rt){unsigned i=c.pr==0x8CEDC366?1:c.pr==0x8CEDC370?3:c.pr==0x8CEDC37A?5:7;if(movie_pair_observer)movie_pair_observer(i);movie_recomp_8CEDC348(c,rt);if(movie_pair_observer)movie_pair_observer(i+1);}
#endif
#ifdef RECOVERY_MOVIE_CACHE_FLAGS
#include "movie-triple-check/movie-triple-check-manifest.hpp"
#include "movie-cache-flags/movie-cache-flags-manifest.hpp"
static thread_local std::function<void(unsigned)> movie_cache_observer;
static void observed_movie_triple_check(SH4Context&c,DCRuntime&rt){if(movie_cache_observer)movie_cache_observer(0);movie_recomp_8CEDC394(c,rt);if(movie_cache_observer)movie_cache_observer(7);}
static void observed_movie_cache_flags(SH4Context&c,DCRuntime&rt){if(movie_cache_observer)movie_cache_observer(9);movie_recomp_8CED81CC(c,rt);if(movie_cache_observer)movie_cache_observer(10);}
static void observed_movie_triple_compare(SH4Context&c,DCRuntime&rt){unsigned i=c.pr==0x8CEDC39E?1:c.pr==0x8CEDC3A8?3:c.pr==0x8CEDC3B2?5:99;if(i==99){observed_movie_pair_compare(c,rt);return;}if(movie_cache_observer)movie_cache_observer(i);movie_recomp_8CEDC348(c,rt);if(movie_cache_observer)movie_cache_observer(i+1);}
static void observed_movie_validation_return(SH4Context&c,DCRuntime&rt){observed_movie_validation(c,rt);if(movie_cache_observer)movie_cache_observer(8);}
#endif
#ifdef RECOVERY_MOVIE_WORK_CONFIG
#include "movie-work-pointer/movie-work-pointer-manifest.hpp"
#include "movie-work-config/movie-work-config-manifest.hpp"
static thread_local std::function<void(unsigned)> work_config_observer;
static void observed_work_pointer(SH4Context&c,DCRuntime&rt){if(work_config_observer)work_config_observer(0);movie_recomp_8CED8206(c,rt);if(work_config_observer)work_config_observer(1);}
static void observed_work_config(SH4Context&c,DCRuntime&rt){if(work_config_observer)work_config_observer(2);movie_recomp_8CED8348(c,rt);if(work_config_observer)work_config_observer(7);}
static void observed_work_config_fill(SH4Context&c,DCRuntime&rt){if(c.pr!=0x8CED8370){observed_method_fill(c,rt);return;}if(work_config_observer)work_config_observer(3);movie_recomp_8CED67F8(c,rt);if(work_config_observer)work_config_observer(4);}
static void observed_work_config_copy(SH4Context&c,DCRuntime&rt){if(c.pr!=0x8CED84EE){observed_movie_word_copy(c,rt);return;}if(work_config_observer)work_config_observer(5);movie_recomp_8CED6708(c,rt);if(work_config_observer)work_config_observer(6);}
#endif
#ifdef RECOVERY_MOVIE_WORK_RESET
#include "movie-work-clear/movie-work-clear-manifest.hpp"
#include "movie-work-reset/movie-work-reset-manifest.hpp"
static thread_local std::function<void(unsigned)> work_reset_observer;
static void observed_work_clear(SH4Context&c,DCRuntime&rt){if(work_reset_observer)work_reset_observer(0);movie_recomp_8CED7E44(c,rt);if(work_reset_observer)work_reset_observer(1);}
static void observed_work_reset(SH4Context&c,DCRuntime&rt){if(work_reset_observer)work_reset_observer(2);movie_recomp_8CED8074(c,rt);if(work_reset_observer)work_reset_observer(3);}
#endif
#ifdef RECOVERY_MOVIE_FIRST_TABLE
#include "movie-tail-helper/movie-tail-helper-manifest.hpp"
#include "movie-first-table/movie-first-table-manifest.hpp"
static thread_local std::function<void(unsigned)> first_table_observer;
static void observed_tail_helper(SH4Context&c,DCRuntime&rt){if(first_table_observer)first_table_observer(0);movie_recomp_8CEDAB14(c,rt);}
static void observed_first_table(SH4Context&c,DCRuntime&rt){if(first_table_observer)first_table_observer(1);movie_recomp_8CEDAB72(c,rt);if(first_table_observer)first_table_observer(2);}
#endif
#ifdef RECOVERY_MOVIE_SECOND_TABLE
#include "movie-second-table/movie-second-table-manifest.hpp"
static thread_local std::function<void(unsigned)> second_table_observer;
static void observed_second_table(SH4Context&c,DCRuntime&rt){if(second_table_observer)second_table_observer(0);movie_recomp_8CEDADAC(c,rt);if(second_table_observer)second_table_observer(1);}
#endif
#ifdef RECOVERY_MOVIE_THIRD_TABLE
#include "movie-third-table/movie-third-table-manifest.hpp"
static thread_local std::function<void(unsigned)> third_table_observer;
static void observed_third_table(SH4Context&c,DCRuntime&rt){if(third_table_observer)third_table_observer(0);movie_recomp_8CEDAFB6(c,rt);if(third_table_observer)third_table_observer(1);}
#endif
#ifdef RECOVERY_MOVIE_FOURTH_TABLE
#include "movie-fourth-table/movie-fourth-table-manifest.hpp"
static thread_local std::function<void(unsigned)> fourth_table_observer;
static void observed_fourth_table(SH4Context&c,DCRuntime&rt){if(fourth_table_observer)fourth_table_observer(0);movie_recomp_8CEDB1FC(c,rt);if(fourth_table_observer)fourth_table_observer(1);}
#endif
#ifdef RECOVERY_MOVIE_FIFTH_TABLE
#include "movie-fifth-table/movie-fifth-table-manifest.hpp"
static thread_local std::function<void(unsigned)> fifth_table_observer;
static void observed_fifth_table(SH4Context&c,DCRuntime&rt){if(fifth_table_observer)fifth_table_observer(0);movie_recomp_8CEDB26C(c,rt);if(fifth_table_observer)fifth_table_observer(1);}
#endif
#ifdef RECOVERY_MOVIE_SIXTH_TABLE
#include "movie-sixth-table/movie-sixth-table-manifest.hpp"
static thread_local std::function<void(unsigned)> sixth_table_observer;
static void observed_sixth_table(SH4Context&c,DCRuntime&rt){if(sixth_table_observer)sixth_table_observer(0);movie_recomp_8CEDB312(c,rt);if(sixth_table_observer)sixth_table_observer(1);}
#endif
#ifdef RECOVERY_MOVIE_SEVENTH_TABLE
#include "movie-seventh-table/movie-seventh-table-manifest.hpp"
static thread_local std::function<void(unsigned)> seventh_table_observer;
static void observed_seventh_table(SH4Context&c,DCRuntime&rt){if(seventh_table_observer)seventh_table_observer(0);movie_recomp_8CEDB486(c,rt);if(seventh_table_observer)seventh_table_observer(1);}
#endif
#ifdef RECOVERY_MOVIE_EIGHTH_TABLE
#include "movie-eighth-table/movie-eighth-table-manifest.hpp"
static thread_local std::function<void(unsigned)> eighth_table_observer;
static void observed_eighth_table(SH4Context&c,DCRuntime&rt){if(eighth_table_observer)eighth_table_observer(0);movie_recomp_8CEDBD9C(c,rt);if(eighth_table_observer)eighth_table_observer(1);}
#endif
#ifdef RECOVERY_MOVIE_NINTH_TABLE
#include "movie-ninth-table/movie-ninth-table-manifest.hpp"
static thread_local std::function<void(unsigned)> ninth_table_observer;
static void observed_ninth_table(SH4Context&c,DCRuntime&rt){if(ninth_table_observer)ninth_table_observer(0);movie_recomp_8CEDBE14(c,rt);if(ninth_table_observer)ninth_table_observer(1);}
#endif
#ifdef RECOVERY_MOVIE_TENTH_TABLE
#include "movie-tenth-table/movie-tenth-table-manifest.hpp"
static thread_local std::function<void(unsigned)> tenth_table_observer;
static void observed_tenth_table(SH4Context&c,DCRuntime&rt){if(tenth_table_observer)tenth_table_observer(0);movie_recomp_8CEDBEA2(c,rt);if(tenth_table_observer)tenth_table_observer(1);}
#endif
#ifdef RECOVERY_MOVIE_ELEVENTH_TABLE
#include "movie-eleventh-table/movie-eleventh-table-manifest.hpp"
static thread_local std::function<void(unsigned)> eleventh_table_observer;
static void observed_eleventh_table(SH4Context&c,DCRuntime&rt){if(eleventh_table_observer)eleventh_table_observer(0);movie_recomp_8CEDC040(c,rt);if(eleventh_table_observer)eleventh_table_observer(1);}
#endif
#ifdef RECOVERY_MOVIE_TABLE_TAIL
#include "movie-table-tail/movie-table-tail-manifest.hpp"
static thread_local std::function<void()> table_tail_observer;
static void observed_table_tail(SH4Context&c,DCRuntime&rt){if(table_tail_observer)table_tail_observer();movie_recomp_8CEDC0BA(c,rt);if(table_tail_observer)table_tail_observer();}
#endif
#ifdef RECOVERY_MOVIE_TABLE_FIRST
#include "movie-table-first/movie-table-first-manifest.hpp"
static thread_local std::function<void(unsigned)> table_first_observer;
static void observed_table_first(SH4Context&c,DCRuntime&rt){if(table_first_observer)table_first_observer(0);movie_recomp_8CEDC178(c,rt);if(table_first_observer)table_first_observer(1);}
#endif
#ifdef RECOVERY_MOVIE_TABLE_SECOND
#include "movie-table-second/movie-table-second-manifest.hpp"
static thread_local std::function<void(unsigned)> table_second_observer;
static void observed_table_second(SH4Context&c,DCRuntime&rt){if(table_second_observer)table_second_observer(0);movie_recomp_8CEDC218(c,rt);if(table_second_observer)table_second_observer(1);}
#endif
#ifdef RECOVERY_MOVIE_TABLE_THIRD
#include "movie-table-third/movie-table-third-manifest.hpp"
static thread_local std::function<void(unsigned)> table_third_observer;
static void observed_table_third(SH4Context&c,DCRuntime&rt){if(table_third_observer)table_third_observer(0);movie_recomp_8CEDC256(c,rt);if(table_third_observer)table_third_observer(1);}
#endif
#ifdef RECOVERY_MOVIE_TABLE_FOURTH
#include "movie-table-fourth/movie-table-fourth-manifest.hpp"
static thread_local std::function<void(unsigned)> table_fourth_observer;
static void observed_table_fourth(SH4Context&c,DCRuntime&rt){if(table_fourth_observer)table_fourth_observer(0);movie_recomp_8CEDC2F4(c,rt);if(table_fourth_observer)table_fourth_observer(1);}
#endif
#ifdef RECOVERY_MOVIE_WORK_FOLLOWUP
#include "movie-work-followup/movie-work-followup-manifest.hpp"
static thread_local std::function<void(unsigned)> work_followup_observer;
static void observed_work_followup(SH4Context&c,DCRuntime&rt){if(work_followup_observer)work_followup_observer(0);movie_recomp_8CEDA920(c,rt);if(work_followup_observer)work_followup_observer(1);}
#endif
#ifdef RECOVERY_MOVIE_WORK_TRANSFORM
#include "movie-work-final/movie-work-final-manifest.hpp"
#include "movie-work-transform/movie-work-transform-manifest.hpp"
static thread_local std::function<void(unsigned)> work_final_observer;
static void observed_work_final(SH4Context&c,DCRuntime&rt){if(work_final_observer)work_final_observer(0);movie_recomp_8CEDA9A4(c,rt);if(work_final_observer)work_final_observer(1);}
#endif
#ifdef RECOVERY_MOVIE_INITIALIZER_RETURN
#include "movie-post-work/movie-post-work-manifest.hpp"
#include "movie-post-buffer/movie-post-buffer-manifest.hpp"
#include "movie-final-setup/movie-final-setup-manifest.hpp"
#include "movie-fourth-callback/movie-fourth-callback-manifest.hpp"
static void observed_post_work(SH4Context&c,DCRuntime&rt){if(initializer_return_observer)initializer_return_observer(0);movie_recomp_8CED8216(c,rt);if(initializer_return_observer)initializer_return_observer(1);}
static void observed_final_setup(SH4Context&c,DCRuntime&rt){if(initializer_return_observer)initializer_return_observer(2);movie_recomp_8CEDA7C4(c,rt);if(initializer_return_observer)initializer_return_observer(3);}
static void observed_fourth_callback(SH4Context&c,DCRuntime&rt){if(initializer_return_observer)initializer_return_observer(6);movie_recomp_8CED5038(c,rt);if(initializer_return_observer)initializer_return_observer(7);}
#endif
#ifdef RECOVERY_MOVIE_BINDING_RETURN
#include "movie-fifth-callback/movie-fifth-callback-manifest.hpp"
#include "movie-sixth-callback/movie-sixth-callback-manifest.hpp"
static void observed_fifth_callback(SH4Context&c,DCRuntime&rt){if(binding_return_observer)binding_return_observer(0);movie_recomp_8CED0EEE(c,rt);if(binding_return_observer)binding_return_observer(1);}
static void observed_sixth_callback(SH4Context&c,DCRuntime&rt){if(binding_return_observer)binding_return_observer(2);movie_recomp_8CED16D4(c,rt);if(binding_return_observer)binding_return_observer(3);}
#endif
#ifdef RECOVERY_MOVIE_GUARD_FINAL
#include "movie-signature-guard/movie-signature-guard-manifest.hpp"
#include "movie-descriptor-final/movie-descriptor-final-manifest.hpp"
static void observed_signature_guard(SH4Context&c,DCRuntime&rt){if(guard_final_observer)guard_final_observer(0);movie_recomp_8CED38A0(c,rt);if(guard_final_observer)guard_final_observer(1);}
static void observed_descriptor_final(SH4Context&c,DCRuntime&rt){if(guard_final_observer)guard_final_observer(2);movie_recomp_8CED1BC4(c,rt);if(guard_final_observer)guard_final_observer(3);}
#endif
#ifdef RECOVERY_MOVIE_SETUP_BIND
#include "movie-setup-bind-prefix/movie-setup-bind-prefix-manifest.hpp"
#include "movie-setup-bind-child/movie-setup-bind-child-manifest.hpp"
static void observed_setup_bind(SH4Context&c,DCRuntime&rt){if(setup_bind_observer)setup_bind_observer(0);movie_recomp_8CED1AFA(c,rt);if(setup_bind_observer)setup_bind_observer(3);}
static void observed_setup_bind_child(SH4Context&c,DCRuntime&rt){if(setup_bind_observer)setup_bind_observer(1);movie_recomp_8CED1B5E(c,rt);if(setup_bind_observer)setup_bind_observer(2);}
#endif
#ifdef RECOVERY_MOVIE_CREATE
#include "movie-create-prefix/movie-create-prefix-manifest.hpp"
static void observed_movie_create(SH4Context&c,DCRuntime&rt){observe_create();movie_recomp_8CED03F8(c,rt);}
#endif
#ifdef RECOVERY_MOVIE_CREATE_INIT
#include "movie-create-init/movie-create-init-manifest.hpp"
#include "movie-create-second-init/movie-create-second-init-manifest.hpp"
static thread_local std::function<void()> create_init_observer;
static void observe_create_init(){if(create_init_observer)create_init_observer();}
static void observed_create_init(SH4Context&c,DCRuntime&rt){observe_create_init();movie_recomp_8CED2538(c,rt);observe_create_init();}
static void observed_create_second_init(SH4Context&c,DCRuntime&rt){observe_create_init();movie_recomp_8CED0EC4(c,rt);observe_create_init();}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE
#include "movie-create-instance/movie-create-instance-manifest.hpp"
#include "movie-create-instance-guard/movie-create-instance-guard-manifest.hpp"
#include "movie-instance-pool/movie-instance-pool-manifest.hpp"
static thread_local std::function<void()> instance_observer;
static void observe_instance(){if(instance_observer)instance_observer();}
static void observed_instance(SH4Context&c,DCRuntime&rt){observe_instance();movie_recomp_8CED3DC2(c,rt);}
static void observed_instance_guard(SH4Context&c,DCRuntime&rt){observe_instance();movie_recomp_8CED3DFE(c,rt);observe_instance();}
static void observed_instance_pool(SH4Context&c,DCRuntime&rt){observe_instance();movie_recomp_8CED3E0E(c,rt);observe_instance();}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_INITIALIZE
#include "movie-instance-initialize/movie-instance-initialize-manifest.hpp"
#include "movie-instance-fields/movie-instance-fields-manifest.hpp"
static void observed_instance_initialize(SH4Context&c,DCRuntime&rt){observe_instance_initialize();movie_recomp_8CED3E54(c,rt);}
static void observed_instance_fields(SH4Context&c,DCRuntime&rt){observe_instance_initialize();movie_recomp_8CED3EF8(c,rt);observe_instance_initialize();}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_RATE
#include "movie-instance-second-fields/movie-instance-second-fields-manifest.hpp"
#include "movie-instance-rate/movie-instance-rate-manifest.hpp"
static void observed_second_fields(SH4Context&c,DCRuntime&rt){observe_second_fields();movie_recomp_8CED3F22(c,rt);observe_second_fields();}
#ifdef RECOVERY_MOVIE_INSTANCE_RATE_RETURN
static thread_local std::function<void()> instance_rate_return_observer;
static void observe_instance_rate_return(){if(instance_rate_return_observer)instance_rate_return_observer();}
#else
static void observe_instance_rate_return(){}
#endif
static void observed_instance_rate(SH4Context&c,DCRuntime&rt){observe_second_fields();movie_recomp_8CED462E(c,rt);observe_instance_rate_return();}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_RATE_RETURN
#include "movie-instance-handler/movie-instance-handler-manifest.hpp"
#include "movie-instance-rate-fields/movie-instance-rate-fields-manifest.hpp"
static void observed_instance_handler(SH4Context&c,DCRuntime&rt){observe_instance_rate_return();movie_recomp_8CED4A54(c,rt);observe_instance_rate_return();}
static void observed_instance_rate_fields(SH4Context&c,DCRuntime&rt){observe_instance_rate_return();movie_recomp_8CED46DE(c,rt);observe_instance_rate_return();}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_STREAM_RETURN
static thread_local std::function<void()> stream_return_observer;
static void observe_stream_return(){if(stream_return_observer)stream_return_observer();}
#else
static void observe_stream_return(){}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_STREAM
#include "movie-instance-stream-prefix/movie-instance-stream-prefix-manifest.hpp"
#include "movie-instance-stream-descriptor/movie-instance-stream-descriptor-manifest.hpp"
static thread_local std::function<void()> instance_stream_observer;
static thread_local RecompiledFunction real_instance_remainder=nullptr;
static void observe_instance_stream(){if(instance_stream_observer)instance_stream_observer();}
static void observed_instance_stream(SH4Context&c,DCRuntime&rt){observe_instance_stream();movie_recomp_8CED97B0(c,rt);observe_stream_return();}
static void observed_instance_descriptor(SH4Context&c,DCRuntime&rt){observe_instance_stream();movie_recomp_8CED984E(c,rt);observe_instance_stream();}
static void observed_instance_remainder(SH4Context&c,DCRuntime&rt){observe_instance_stream();real_instance_remainder(c,rt);observe_instance_stream();}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_RECORDS
#include "movie-instance-stream-record/movie-instance-stream-record-manifest.hpp"
#include "movie-instance-small-record/movie-instance-small-record-manifest.hpp"
static thread_local std::function<void()> instance_records_observer;
static void observe_instance_records(){if(instance_records_observer)instance_records_observer();}
static void observed_instance_record(SH4Context&c,DCRuntime&rt){observe_instance_records();observe_stream_return();movie_recomp_8CED9864(c,rt);observe_stream_return();observe_instance_records();}
static void observed_instance_small_record(SH4Context&c,DCRuntime&rt){observe_instance_records();observe_stream_return();movie_recomp_8CED9918(c,rt);observe_stream_return();observe_instance_records();}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_STREAM_RETURN
#include "movie-instance-ring-record/movie-instance-ring-record-manifest.hpp"
static void observed_instance_ring_record(SH4Context&c,DCRuntime&rt){observe_stream_return();movie_recomp_8CED9978(c,rt);observe_stream_return();}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_TABLE_RETURN
static thread_local std::function<void()> table_return_observer;
static void observe_table_return(){if(table_return_observer)table_return_observer();}
#else
static void observe_table_return(){}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_TABLE
#include "movie-instance-table-prefix/movie-instance-table-prefix-manifest.hpp"
static thread_local std::function<void()> instance_table_observer;
static void observed_instance_table(SH4Context&c,DCRuntime&rt){if(instance_table_observer)instance_table_observer();movie_recomp_8CEDA304(c,rt);observe_table_return();}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_TABLE_RECORDS
#include "movie-instance-table-record/movie-instance-table-record-manifest.hpp"
static thread_local std::function<void()> table_records_observer;
static void observed_instance_table_record(SH4Context&c,DCRuntime&rt){if(table_records_observer)table_records_observer();movie_recomp_8CEDA362(c,rt);if(table_records_observer)table_records_observer();}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_NEXT_BRANCH
static thread_local std::function<void()> next_branch_observer;
static void observe_next_branch(){if(next_branch_observer)next_branch_observer();}
#else
static void observe_next_branch(){}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_SELECTOR
#include "movie-instance-table-final-prefix/movie-instance-table-final-prefix-manifest.hpp"
#include "movie-instance-selector/movie-instance-selector-manifest.hpp"
static thread_local std::function<void()> instance_selector_observer;
static void observed_instance_table_final(SH4Context&c,DCRuntime&rt){if(instance_selector_observer)instance_selector_observer();movie_recomp_8CEDA388(c,rt);observe_table_return();}
static void observed_instance_selector(SH4Context&c,DCRuntime&rt){if(instance_selector_observer)instance_selector_observer();observe_next_branch();observe_table_return();movie_recomp_8CEDA518(c,rt);observe_table_return();observe_next_branch();if(instance_selector_observer)instance_selector_observer();}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_SECOND_SELECTOR
#include "movie-instance-branch/movie-instance-branch-manifest.hpp"
#include "movie-instance-second-selector/movie-instance-second-selector-manifest.hpp"
static thread_local std::function<void()> second_selector_observer;
static void observed_instance_branch(SH4Context&c,DCRuntime&rt){if(second_selector_observer)second_selector_observer();observe_next_branch();observe_table_return();movie_recomp_8CEDA42C(c,rt);observe_table_return();observe_next_branch();if(second_selector_observer)second_selector_observer();}
static void observed_instance_second_selector(SH4Context&c,DCRuntime&rt){if(second_selector_observer)second_selector_observer();movie_recomp_8CEDA524(c,rt);if(second_selector_observer)second_selector_observer();}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_NEXT_BRANCH
#include "movie-instance-next-branch/movie-instance-next-branch-manifest.hpp"
static void observed_instance_next_branch(SH4Context&c,DCRuntime&rt){observe_next_branch();movie_recomp_8CEDA486(c,rt);observe_next_branch();}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_TABLE_RETURN
#include "movie-instance-third-selector/movie-instance-third-selector-manifest.hpp"
#include "movie-instance-third-branch/movie-instance-third-branch-manifest.hpp"
static void observed_instance_third_selector(SH4Context&c,DCRuntime&rt){observe_table_return();movie_recomp_8CEDA530(c,rt);observe_table_return();}
static void observed_instance_third_branch(SH4Context&c,DCRuntime&rt){observe_table_return();movie_recomp_8CEDA4CC(c,rt);observe_table_return();}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_DISPATCH
#include "movie-instance-final-thunk/movie-instance-final-thunk-manifest.hpp"
#include "movie-instance-handler-dispatch/movie-instance-handler-dispatch-manifest.hpp"
static thread_local std::function<void()> instance_dispatch_observer;
static void observed_instance_final_thunk(SH4Context&c,DCRuntime&rt){if(instance_dispatch_observer)instance_dispatch_observer();movie_recomp_8CED3FEA(c,rt);}
static void observed_instance_handler_dispatch(SH4Context&c,DCRuntime&rt){if(instance_dispatch_observer)instance_dispatch_observer();movie_recomp_8CEDA59E(c,rt);}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_HANDLERS
#include "movie-instance-first-handler/movie-instance-first-handler-manifest.hpp"
#include "movie-instance-second-handler/movie-instance-second-handler-manifest.hpp"
#include "movie-instance-handler-clear/movie-instance-handler-clear-manifest.hpp"
static thread_local std::function<void()> instance_handlers_observer;
static void observed_instance_first_handler(SH4Context&c,DCRuntime&rt){if(instance_handlers_observer)instance_handlers_observer();movie_recomp_8CED1C36(c,rt);if(instance_handlers_observer)instance_handlers_observer();}
static void observed_instance_second_handler(SH4Context&c,DCRuntime&rt){if(instance_handlers_observer)instance_handlers_observer();movie_recomp_8CED2444(c,rt);observe_video_complete();}
static void observed_instance_handler_clear(SH4Context&c,DCRuntime&rt){if(instance_handlers_observer)instance_handlers_observer();movie_recomp_8CED24A2(c,rt);if(instance_handlers_observer)instance_handlers_observer();}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_RESOURCE_COMPLETE
static thread_local std::function<void()> resource_complete_observer;
static void observe_resource_complete(){if(resource_complete_observer)resource_complete_observer();}
#else
static void observe_resource_complete(){}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_RESOURCES
#include "movie-instance-resource-wrapper/movie-instance-resource-wrapper-manifest.hpp"
#include "movie-instance-resource-search/movie-instance-resource-search-manifest.hpp"
#include "movie-instance-resource-init/movie-instance-resource-init-manifest.hpp"
static void observed_instance_resource_wrapper(SH4Context&c,DCRuntime&rt){observe_instance_resources();movie_recomp_8CED7ADC(c,rt);observe_resource_complete();}
static void observed_instance_resource_search(SH4Context&c,DCRuntime&rt){observe_instance_resources();movie_recomp_8CED7B0C(c,rt);observe_instance_resources();}
static void observed_instance_resource_init(SH4Context&c,DCRuntime&rt){observe_instance_resources();movie_recomp_8CED7B3E(c,rt);observe_resource_complete();}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_RESOURCE_COMPLETE
#include "movie-instance-resource-sentinels/movie-instance-resource-sentinels-manifest.hpp"
#include "movie-instance-resource-blocks/movie-instance-resource-blocks-manifest.hpp"
#include "movie-instance-resource-final/movie-instance-resource-final-manifest.hpp"
static void observed_resource_sentinels(SH4Context&c,DCRuntime&rt){observe_resource_complete();movie_recomp_8CED7B82(c,rt);observe_resource_complete();}
static void observed_resource_blocks(SH4Context&c,DCRuntime&rt){observe_resource_complete();movie_recomp_8CED7B8C(c,rt);observe_resource_complete();}
static void observed_resource_final(SH4Context&c,DCRuntime&rt){observe_resource_complete();movie_recomp_8CED7BA0(c,rt);observe_resource_complete();}
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_VIDEO_COMPLETE
#include "movie-instance-binding-prefix/movie-instance-binding-prefix-manifest.hpp"
static void observed_video_binding_prefix(SH4Context&c,DCRuntime&rt){observe_video_complete();movie_recomp_8CED7A2C(c,rt);observe_video_complete();}
#include "movie-instance-binding-validate/movie-instance-binding-validate-manifest.hpp"
static void observed_video_binding_validate(SH4Context&c,DCRuntime&rt){observe_video_complete();movie_recomp_8CED7AC4(c,rt);observe_video_complete();}
#include "movie-instance-binding-store/movie-instance-binding-store-manifest.hpp"
static void observed_video_binding_store(SH4Context&c,DCRuntime&rt){observe_video_complete();movie_recomp_8CED7A74(c,rt);observe_video_complete();}
#include "movie-instance-third-handler/movie-instance-third-handler-manifest.hpp"
static void observed_video_third_handler(SH4Context&c,DCRuntime&rt){observe_video_complete();movie_recomp_8CED2F4C(c,rt);observe_video_complete();}
#include "movie-instance-indexed-query/movie-instance-indexed-query-manifest.hpp"
static void observed_video_indexed_query(SH4Context&c,DCRuntime&rt){observe_video_complete();movie_recomp_8CED44FC(c,rt);observe_video_complete();}
#include "movie-instance-video-setup/movie-instance-video-setup-manifest.hpp"
static void observed_video_video_setup(SH4Context&c,DCRuntime&rt){observe_video_complete();movie_recomp_8CED2FDA(c,rt);observe_video_complete();}
#include "movie-instance-video-layout/movie-instance-video-layout-manifest.hpp"
static void observed_video_video_layout(SH4Context&c,DCRuntime&rt){observe_video_complete();movie_recomp_8CED3390(c,rt);observe_video_complete();}
#include "movie-instance-video-geometry/movie-instance-video-geometry-manifest.hpp"
static void observed_video_video_geometry(SH4Context&c,DCRuntime&rt){observe_video_complete();movie_recomp_8CED1BCC(c,rt);observe_video_complete();}
#include "movie-instance-video-return/movie-instance-video-return-manifest.hpp"
static void observed_video_video_return(SH4Context&c,DCRuntime&rt){observe_video_complete();movie_recomp_8CED1BE4(c,rt);observe_video_complete();}
#endif
static thread_local RecompiledFunction original_startup_word_add=nullptr;
static thread_local std::function<void(bool)> startup_word_add_observer;
static void observed_startup_word_add(SH4Context& ctx,DCRuntime& rt){
    if(startup_word_add_observer)startup_word_add_observer(false);
    original_startup_word_add(ctx,rt);
    if(startup_word_add_observer)startup_word_add_observer(true);
}
static thread_local RecompiledFunction original_startup_zero_guard=nullptr;
static thread_local std::function<void(bool)> startup_zero_guard_observer;
static void observed_startup_zero_guard(SH4Context& ctx,DCRuntime& rt){
    if(startup_zero_guard_observer)startup_zero_guard_observer(false);
    original_startup_zero_guard(ctx,rt);
    if(startup_zero_guard_observer)startup_zero_guard_observer(true);
}
static thread_local RecompiledFunction original_startup_query_reset=nullptr;
static thread_local std::function<void(bool)> startup_query_reset_observer;
static void observed_startup_query_reset(SH4Context& ctx,DCRuntime& rt){
    if(startup_query_reset_observer)startup_query_reset_observer(false);
    original_startup_query_reset(ctx,rt);
    if(startup_query_reset_observer)startup_query_reset_observer(true);
}
static thread_local RecompiledFunction original_startup_query_provider=nullptr;
static thread_local std::function<void(bool)> startup_query_provider_observer;
static void observed_startup_query_provider(SH4Context& ctx,DCRuntime& rt){
    if(startup_query_provider_observer)startup_query_provider_observer(false);
    original_startup_query_provider(ctx,rt);
    if(startup_query_provider_observer)startup_query_provider_observer(true);
}
static thread_local std::function<void()> startup_first_query_return_observer;
static thread_local RecompiledFunction original_startup_check_query=nullptr;
static thread_local std::function<void()> startup_check_query_observer;
static void observed_startup_check_query(SH4Context& ctx,DCRuntime& rt){
    if(startup_check_query_observer)startup_check_query_observer();
    original_startup_check_query(ctx,rt);
    if(startup_first_query_return_observer)startup_first_query_return_observer();
}
static thread_local RecompiledFunction original_startup_query_helper=nullptr;
static thread_local std::function<void()> startup_query_helper_observer;
static void observed_startup_query_helper(SH4Context& ctx,DCRuntime& rt){
    if(startup_query_helper_observer)startup_query_helper_observer();
    original_startup_query_helper(ctx,rt);
}
static thread_local RecompiledFunction original_startup_gate_return=nullptr;
static thread_local std::function<void(bool)> startup_gate_return_observer;
static void observed_startup_gate_return(SH4Context& ctx,DCRuntime& rt){
    if(startup_gate_return_observer)startup_gate_return_observer(false);
    original_startup_gate_return(ctx,rt);
    if(startup_gate_return_observer)startup_gate_return_observer(true);
}
static thread_local RecompiledFunction original_startup_owner_return=nullptr;
static thread_local std::function<void(bool)> startup_owner_return_observer;
static void observed_startup_owner_return(SH4Context& ctx,DCRuntime& rt){
    if(startup_owner_return_observer)startup_owner_return_observer(false);
    original_startup_owner_return(ctx,rt);
    if(startup_owner_return_observer)startup_owner_return_observer(true);
}
static thread_local RecompiledFunction original_startup_flag_return=nullptr;
static thread_local std::function<void(bool)> startup_flag_return_observer;
static void observed_startup_flag_return(SH4Context& ctx,DCRuntime& rt){
    if(startup_flag_return_observer)startup_flag_return_observer(false);
    original_startup_flag_return(ctx,rt);
    if(startup_flag_return_observer)startup_flag_return_observer(true);
}
static thread_local RecompiledFunction original_startup_checksum=nullptr;
static thread_local std::function<void(bool)> startup_checksum_observer;
static void observed_startup_checksum(SH4Context& ctx,DCRuntime& rt){
    if(startup_checksum_observer)startup_checksum_observer(false);
    original_startup_checksum(ctx,rt);
    if(startup_checksum_observer)startup_checksum_observer(true);
}
static thread_local RecompiledFunction original_startup_ninebytes=nullptr;
static thread_local std::function<void(bool)> startup_ninebytes_observer;
static void observed_startup_ninebytes(SH4Context& ctx,DCRuntime& rt){
    if(startup_ninebytes_observer)startup_ninebytes_observer(false);
    original_startup_ninebytes(ctx,rt);
    if(startup_ninebytes_observer)startup_ninebytes_observer(true);
}
static thread_local RecompiledFunction original_startup_triplet=nullptr;
static thread_local std::function<void(bool)> startup_triplet_observer;
static void observed_startup_triplet(SH4Context& ctx,DCRuntime& rt){
    if(startup_triplet_observer)startup_triplet_observer(false);
    original_startup_triplet(ctx,rt);
    if(startup_triplet_observer)startup_triplet_observer(true);
}
static thread_local RecompiledFunction original_startup_fourrecords=nullptr;
static thread_local std::function<void(bool)> startup_fourrecords_observer;
static void observed_startup_fourrecords(SH4Context& ctx,DCRuntime& rt){
    if(startup_fourrecords_observer)startup_fourrecords_observer(false);
    original_startup_fourrecords(ctx,rt);
    if(startup_fourrecords_observer)startup_fourrecords_observer(true);
}
static thread_local RecompiledFunction original_startup_sixrecords=nullptr;
static thread_local std::function<void(bool)> startup_sixrecords_observer;
static void observed_startup_sixrecords(SH4Context& ctx,DCRuntime& rt){
    if(startup_sixrecords_observer)startup_sixrecords_observer(false);
    original_startup_sixrecords(ctx,rt);
    if(startup_sixrecords_observer)startup_sixrecords_observer(true);
}
static thread_local RecompiledFunction original_startup_pair2=nullptr;
static thread_local std::function<void(bool)> startup_pair2_observer;
static void observed_startup_pair2(SH4Context& ctx,DCRuntime& rt){
    if(startup_pair2_observer)startup_pair2_observer(false);
    original_startup_pair2(ctx,rt);
    if(startup_pair2_observer)startup_pair2_observer(true);
}
static thread_local RecompiledFunction original_startup_pair=nullptr;
static thread_local std::function<void(bool)> startup_pair_observer;
static void observed_startup_pair(SH4Context& ctx,DCRuntime& rt){
    if(startup_pair_observer)startup_pair_observer(false);
    original_startup_pair(ctx,rt);
    if(startup_pair_observer)startup_pair_observer(true);
}
static thread_local RecompiledFunction original_startup_record=nullptr;
static thread_local std::function<void(bool)> startup_record_observer;
static void observed_startup_record(SH4Context& ctx,DCRuntime& rt){
    if(startup_record_observer)startup_record_observer(false);
    original_startup_record(ctx,rt);
    if(startup_record_observer)startup_record_observer(true);
}
static thread_local RecompiledFunction original_startup_vector=nullptr;
static thread_local std::function<void(bool)> startup_vector_observer;
static void observed_startup_vector(SH4Context& ctx,DCRuntime& rt){
    if(startup_vector_observer)startup_vector_observer(false);
    original_startup_vector(ctx,rt);
    if(startup_vector_observer)startup_vector_observer(true);
}
static thread_local RecompiledFunction original_startup_state=nullptr;
static thread_local std::function<void(bool)> startup_state_observer;
static void observed_startup_state(SH4Context& ctx,DCRuntime& rt){
    if(startup_state_observer)startup_state_observer(false);
    original_startup_state(ctx,rt);
    if(startup_state_observer)startup_state_observer(true);
}
static thread_local RecompiledFunction original_startup_global=nullptr;
static thread_local std::function<void(bool)> startup_global_observer;
static void observed_startup_global(SH4Context& ctx,DCRuntime& rt){
    if(startup_global_observer)startup_global_observer(false);
    original_startup_global(ctx,rt);
    if(startup_global_observer)startup_global_observer(true);
}
static thread_local RecompiledFunction original_startup_selector=nullptr;
static thread_local std::function<void(bool)> startup_selector_observer;
static void observed_startup_selector(SH4Context& ctx,DCRuntime& rt){
    if(startup_selector_observer)startup_selector_observer(false);
    original_startup_selector(ctx,rt);
    if(startup_selector_observer)startup_selector_observer(true);
}
static thread_local RecompiledFunction original_startup_buffer=nullptr;
static thread_local std::function<void(bool)> startup_buffer_observer;
static void observed_startup_buffer(SH4Context& ctx,DCRuntime& rt){
    if(startup_buffer_observer)startup_buffer_observer(false);
    original_startup_buffer(ctx,rt);
    if(startup_buffer_observer)startup_buffer_observer(true);
}
static thread_local RecompiledFunction original_startup_status=nullptr;
static thread_local std::function<void(bool)> startup_status_observer;
static void observed_startup_status(SH4Context& ctx,DCRuntime& rt){
    if(startup_status_observer)startup_status_observer(false);
    original_startup_status(ctx,rt);
    if(startup_status_observer)startup_status_observer(true);
}
static thread_local RecompiledFunction original_startup_flags=nullptr;
static thread_local std::function<void(bool)> startup_flags_observer;
static void observed_startup_flags(SH4Context& ctx,DCRuntime& rt){
    if(startup_flags_observer)startup_flags_observer(false);
    original_startup_flags(ctx,rt);
    if(startup_flags_observer)startup_flags_observer(true);
}
static thread_local RecompiledFunction original_startup_clear=nullptr;
static thread_local std::function<void(bool)> startup_clear_observer;
static void observed_startup_clear(SH4Context& ctx,DCRuntime& rt){
    if(startup_clear_observer)startup_clear_observer(false);
    original_startup_clear(ctx,rt);
    if(startup_clear_observer)startup_clear_observer(true);
}
static thread_local RecompiledFunction original_startup_table=nullptr;
static thread_local std::function<void(bool)> startup_table_observer;
static void observed_startup_table(SH4Context& ctx,DCRuntime& rt){
    if(startup_table_observer)startup_table_observer(false);
    original_startup_table(ctx,rt);
    if(startup_table_observer)startup_table_observer(true);
}
static thread_local RecompiledFunction original_startup_byte=nullptr;
static thread_local std::function<void(bool)> startup_byte_observer;
static void observed_startup_byte(SH4Context& ctx,DCRuntime& rt){
    if(startup_byte_observer)startup_byte_observer(false);
    original_startup_byte(ctx,rt);
    if(startup_byte_observer)startup_byte_observer(true);
}
static thread_local RecompiledFunction original_command_pack=nullptr;
static thread_local std::function<void(bool)> command_pack_observer;
static void observed_command_pack(SH4Context& ctx,DCRuntime& rt){
    if(command_pack_observer)command_pack_observer(false);
    original_command_pack(ctx,rt);
    if(command_pack_observer)command_pack_observer(true);
}
static thread_local RecompiledFunction original_command_lock=nullptr;
static thread_local std::function<void()> command_lock_observer;
static void observed_command_lock(SH4Context& ctx,DCRuntime& rt){
    if(command_lock_observer)command_lock_observer();
    original_command_lock(ctx,rt);
}
static thread_local RecompiledFunction original_command_wrapper=nullptr;
static thread_local std::function<void()> command_wrapper_observer;
static void observed_command_wrapper(SH4Context& ctx,DCRuntime& rt){
    if(command_wrapper_observer)command_wrapper_observer();
    original_command_wrapper(ctx,rt);
}
static thread_local RecompiledFunction original_channel_command=nullptr;
static thread_local std::function<void()> channel_command_observer;
static void observed_channel_command(SH4Context& ctx,DCRuntime& rt){
    if(channel_command_observer)channel_command_observer();
    original_channel_command(ctx,rt);
}
static thread_local RecompiledFunction original_channel_handler=nullptr;
static thread_local std::function<void()> channel_handler_observer;
static void observed_channel_handler(SH4Context& ctx,DCRuntime& rt){
    if(channel_handler_observer)channel_handler_observer();
    original_channel_handler(ctx,rt);
}
static thread_local RecompiledFunction original_channel_wrapper=nullptr;
static thread_local std::function<void()> channel_wrapper_observer;
static void observed_channel_wrapper(SH4Context& ctx,DCRuntime& rt){
    if(channel_wrapper_observer)channel_wrapper_observer();
    original_channel_wrapper(ctx,rt);
}
static thread_local RecompiledFunction original_channel_init=nullptr;
static thread_local std::function<void()> channel_init_observer;
static void observed_channel_init(SH4Context& ctx,DCRuntime& rt){
    if(channel_init_observer)channel_init_observer();
    original_channel_init(ctx,rt);
}
static thread_local RecompiledFunction original_vmu_fields=nullptr;
static thread_local std::function<void(bool)> vmu_fields_observer;
static void observed_vmu_fields(SH4Context& ctx,DCRuntime& rt){
    if(vmu_fields_observer)vmu_fields_observer(false);
    original_vmu_fields(ctx,rt);
    if(vmu_fields_observer)vmu_fields_observer(true);
}
static thread_local RecompiledFunction original_irq_word=nullptr;
static thread_local std::function<void(bool)> irq_word_observer;
static void observed_irq_word(SH4Context& ctx,DCRuntime& rt){
    if(irq_word_observer)irq_word_observer(false);
    original_irq_word(ctx,rt);
    if(irq_word_observer)irq_word_observer(true);
}
static thread_local RecompiledFunction original_language_init=nullptr;
static thread_local std::function<void()> language_init_observer;
static void observed_language_init(SH4Context& ctx,DCRuntime& rt){
    if(language_init_observer)language_init_observer();
    original_language_init(ctx,rt);
}
static thread_local RecompiledFunction original_startup_motion=nullptr;
static thread_local std::function<void(bool)> startup_motion_observer;
static void observed_startup_motion(SH4Context& ctx,DCRuntime& rt){
    if(startup_motion_observer)startup_motion_observer(false);
    original_startup_motion(ctx,rt);
    if(startup_motion_observer)startup_motion_observer(true);
}
static thread_local RecompiledFunction original_init_call=nullptr;
static thread_local std::function<void()> init_call_observer;
static void observed_init_call(SH4Context& ctx,DCRuntime& rt){
    if(init_call_observer)init_call_observer();
    original_init_call(ctx,rt);
}
static thread_local RecompiledFunction original_mode_dispatch=nullptr;
static thread_local std::function<void()> mode_dispatch_observer;
static void observed_mode_dispatch(SH4Context& ctx,DCRuntime& rt){
    if(mode_dispatch_observer)mode_dispatch_observer();
    original_mode_dispatch(ctx,rt);
#ifdef RECOVERY_FRAME_SPRITES
    if(frame_mode_exit_observer)frame_mode_exit_observer();
#endif
}
static thread_local RecompiledFunction original_main_guard=nullptr;
static thread_local std::function<void(bool)> main_guard_observer;
static void observed_main_guard(SH4Context& ctx,DCRuntime& rt){
    if(main_guard_observer)main_guard_observer(false);
    original_main_guard(ctx,rt);
    if(main_guard_observer)main_guard_observer(true);
}
static thread_local RecompiledFunction original_record32=nullptr;
static thread_local std::function<void(bool)> record32_observer;
static void observed_record32(SH4Context& ctx,DCRuntime& rt){
    if(record32_observer)record32_observer(false);
    original_record32(ctx,rt);
    if(record32_observer)record32_observer(true);
}
static thread_local RecompiledFunction original_record16=nullptr;
static thread_local std::function<void(bool)> record16_observer;
static void observed_record16(SH4Context& ctx,DCRuntime& rt){
    if(record16_observer)record16_observer(false);
    original_record16(ctx,rt);
    if(record16_observer)record16_observer(true);
}
static void observed_task_critical(SH4Context& ctx,DCRuntime& rt){
    if(task_critical_observer)task_critical_observer(false);
    original_task_critical(ctx,rt);
    if(task_critical_observer)task_critical_observer(true);
}
static void observed_startup_work(SH4Context& ctx,DCRuntime& rt){
    if(startup_work_observer)startup_work_observer(false);
    original_startup_work(ctx,rt);
    if(startup_work_observer)startup_work_observer(true);
}
static void observed_task_scan(SH4Context& ctx,DCRuntime& rt){
    if(task_scan_observer)task_scan_observer(false);
    original_task_scan(ctx,rt);
    if(task_scan_observer)task_scan_observer(true);
}
static void observed_timer_init(SH4Context& ctx,DCRuntime& rt){
    if(timer_init_observer)timer_init_observer(false);
    original_timer_init(ctx,rt);
    if(timer_init_observer)timer_init_observer(true);
}
static thread_local RecompiledFunction original_startup_dispatch=nullptr;
static thread_local std::function<void(bool)> startup_dispatch_observer;
static void observed_startup_dispatch(SH4Context& ctx,DCRuntime& rt){
    if(startup_dispatch_observer)startup_dispatch_observer(false);
    original_startup_dispatch(ctx,rt);
    if(startup_dispatch_observer)startup_dispatch_observer(true);
}
static thread_local RecompiledFunction original_file_pool=nullptr;
static thread_local std::function<void(bool)> file_pool_observer;
static void observed_file_pool(SH4Context& ctx,DCRuntime& rt){
    if(file_pool_observer)file_pool_observer(false);
    original_file_pool(ctx,rt);
    if(file_pool_observer)file_pool_observer(true);
}
static void observed_critical_leave(SH4Context& ctx,DCRuntime& rt){
    if(critical_leave_observer)critical_leave_observer(false);
    original_critical_leave(ctx,rt);
    if(critical_leave_observer)critical_leave_observer(true);
}
static void observed_nested_clear(SH4Context& ctx,DCRuntime& rt){
    if(nested_clear_observer)nested_clear_observer(false);
    original_nested_clear(ctx,rt);
    if(nested_clear_observer)nested_clear_observer(true);
}
static void observed_module_init(SH4Context& ctx,DCRuntime& rt){
    if(module_init_observer)module_init_observer(false);
    original_module_init(ctx,rt);
    if(module_init_observer)module_init_observer(true);
}
static void observed_sound_completion(SH4Context& ctx,DCRuntime& rt){
    if(sound_completion_observer)sound_completion_observer(false);
    original_sound_completion(ctx,rt);
    if(sound_completion_observer)sound_completion_observer(true);
}
#include "startup-hardware-capture-20261009.hpp"

static void observed_sdk_request(SH4Context& ctx,DCRuntime& rt){
    if(sdk_request_observer)sdk_request_observer(false);
    original_sdk_request(ctx,rt);
    if(sdk_request_observer)sdk_request_observer(true);
}
#include "/Users/chrissotraidis/GitHub/ringpad/docs/artifacts/recovery-aot/20261008-startup-movie-instance-buffer-acquire/movie-instance-buffer-acquire/movie-instance-buffer-acquire-manifest.hpp"
#include "/Users/chrissotraidis/GitHub/ringpad/docs/artifacts/recovery-aot/20261008-startup-movie-instance-buffer-validate/movie-instance-buffer-validate/movie-instance-buffer-validate-manifest.hpp"
#include "/Users/chrissotraidis/GitHub/ringpad/docs/artifacts/recovery-aot/20261008-startup-movie-instance-buffer-clear/movie-instance-buffer-clear/movie-instance-buffer-clear-manifest.hpp"
#include "/Users/chrissotraidis/GitHub/ringpad/docs/artifacts/recovery-aot/20261008-startup-movie-instance-buffer-config/movie-instance-buffer-config/movie-instance-buffer-config-manifest.hpp"
#include "/Users/chrissotraidis/GitHub/ringpad/docs/artifacts/recovery-aot/20261008-startup-movie-instance-buffer-config-return/movie-instance-buffer-config-return/movie-instance-buffer-config-return-manifest.hpp"
#include "/Users/chrissotraidis/GitHub/ringpad/docs/artifacts/recovery-aot/20261008-startup-movie-instance-buffer-links/movie-instance-buffer-links/movie-instance-buffer-links-manifest.hpp"
#include "/Users/chrissotraidis/GitHub/ringpad/docs/artifacts/recovery-aot/20261008-startup-movie-instance-buffer-return/movie-instance-buffer-return/movie-instance-buffer-return-manifest.hpp"
#include "/Users/chrissotraidis/GitHub/ringpad/docs/artifacts/recovery-aot/20261008-startup-movie-instance-buffer-binding-repair1/movie-instance-buffer-binding/movie-instance-buffer-binding-manifest.hpp"
#include "/Users/chrissotraidis/GitHub/ringpad/docs/artifacts/recovery-aot/20261008-startup-movie-instance-buffer-flags/movie-instance-buffer-flags/movie-instance-buffer-flags-manifest.hpp"
#include "/Users/chrissotraidis/GitHub/ringpad/docs/artifacts/recovery-aot/20261008-startup-movie-instance-fourth-handler/movie-instance-fourth-handler/movie-instance-fourth-handler-manifest.hpp"
#include "/Users/chrissotraidis/GitHub/ringpad/docs/artifacts/recovery-aot/20261008-startup-movie-instance-fourth-setup/movie-instance-fourth-setup/movie-instance-fourth-setup-manifest.hpp"
#include "/Users/chrissotraidis/GitHub/ringpad/docs/artifacts/recovery-aot/20261008-startup-movie-instance-fourth-create/movie-instance-fourth-create/movie-instance-fourth-create-manifest.hpp"
namespace dcrecomp_generated {void resident_pool_register(DCRuntime&);void resident_descriptor_register(DCRuntime&);void resident_descriptor_return_register(DCRuntime&);void resident_channel_register(DCRuntime&);void resident_channel_getters_register(DCRuntime&);void resident_work_pool_register(DCRuntime&);void resident_rate_pool_register(DCRuntime&);}
static std::function<void()> integration_entry_observer;
static bool integration_entry_seen=false;
static void observed_integration_acquire(SH4Context& c,DCRuntime& rt){
 if(!integration_entry_seen){integration_entry_seen=true;if(integration_entry_observer)integration_entry_observer();}
 movie_recomp_8CED8536(c,rt);
}
void run_original_startup(DCRuntime&rt,SH4Context&ctx){
resident_pool_register(rt);
resident_descriptor_register(rt);
resident_descriptor_return_register(rt);
resident_channel_register(rt);
resident_channel_getters_register(rt);
resident_work_pool_register(rt);
resident_rate_pool_register(rt);
require(!rt.targets.contains(0x8c65eb00),"FPU boundary unexpectedly registered");

#ifdef RECOVERY_MOVIE_ENTRY
    const char*movie_path=std::getenv("RINGPAD_RECOVERY_MOVIE_REFERENCE");require(movie_path,"Missing declared movie reference");
    std::ifstream movie_input(movie_path,std::ios::binary);require(bool(movie_input),"Cannot open declared movie reference");
    const std::vector<std::uint8_t> movie_reference{std::istreambuf_iterator<char>(movie_input),{}};require(movie_reference.size()==184592,"Movie reference size differs");
    dc_begin_code_module(rt,0x8CEB0000,movie_reference.size());bool movie_active=false;
    std::ofstream movie_activation(recovery_frame_directory+"/movie-activation.txt");require(bool(movie_activation),"Cannot record module activation");
    movie_activation<<"retired base=8ceb0000 bytes="<<movie_reference.size()<<" depth="<<rt.code_module_dispatch_depth<<std::endl;
#endif

#ifdef RECOVERY_FRAME_SPRITES
    frame_sprites_register(rt);sprite_header_register(rt);rt.pvr_ta_provenance_enabled=true;
#endif
#ifdef RECOVERY_MODE_LOADER
    mode_loader_register(rt);loader_owner_register(rt);
#endif
#ifdef RECOVERY_FILE_TRANSFER
    file_transfer_register(rt);
#endif
#ifdef RECOVERY_MOVIE_SERVICE
    movie_service_register(rt);movie_setup_register(rt);
#endif
#ifdef RECOVERY_MOVIE_TAIL
    movie_tail_register(rt);
#endif
#ifdef RECOVERY_MOVIE_MODE
    movie_mode_register(rt);
#endif
#ifdef RECOVERY_MOVIE_RESIDENT_INIT
    movie_resident_init_register(rt);movie_resident_count_register(rt);movie_resident_buffer_register(rt);
#endif
#ifdef RECOVERY_MOVIE_CRITICAL
    movie_allocation_register(rt);movie_lock_enter_register(rt);movie_lock_leave_register(rt);
#ifdef RECOVERY_MOVIE_SLOTS
    movie_slot_search_register(rt);movie_slot_status_register(rt);movie_slot_mark_register(rt);movie_slot_argument_register(rt);
#ifdef RECOVERY_MOVIE_STREAM
    movie_stream_register(rt);movie_context_register(rt);
#ifdef RECOVERY_MOVIE_CALLBACK_INSTALL
    movie_callback_install_register(rt);
#endif
#endif
#endif
#endif
    for(const auto&e:board_resident_entries)rt.register_target(e.address,e.function);
    for(const auto&e:callback_resident_entries)rt.register_target(e.address,e.function);
#ifdef RECOVERY_RECORD_ITERATION
    // Verified original bodies; all unregistered cold callees remain explicit faults.
    dcrecomp_generated::parent_register(rt);dcrecomp_generated::record_helpers_register(rt);
    dcrecomp_generated::helper_register(rt);dcrecomp_generated::serializer_register(rt);
    dcrecomp_generated::transport_register(rt);dcrecomp_generated::command_register(rt);
    dcrecomp_generated::submission_register(rt);dcrecomp_generated::worker_register(rt);
#endif
#ifdef RECOVERY_SEVENBYTE
    dcrecomp_generated::sevenbyte_register(rt);
#endif
#ifdef RECOVERY_WORD_COPY
    dcrecomp_generated::word_copy_register(rt);
#endif
#ifdef RECOVERY_PAYLOAD_VALIDATOR
    dcrecomp_generated::payload_validator_register(rt);
#endif
#ifdef RECOVERY_BLOCK_ITERATION
    dcrecomp_generated::block_iteration_register(rt);
#endif
#ifdef RECOVERY_LINKED_BLOCKS
    dcrecomp_generated::linked_blocks_register(rt);dcrecomp_generated::next_block_register(rt);
#endif
#ifdef RECOVERY_TIMER_CALLBACK
    dcrecomp_generated::timer_callback_register(rt);dcrecomp_generated::timer_tail_register(rt);
#endif
#ifdef RECOVERY_RECORD_UPDATE
    dcrecomp_generated::record_update_register(rt);dcrecomp_generated::record_query_register(rt);
#endif
#ifdef RECOVERY_RECORD_RESET
    dcrecomp_generated::record_reset_register(rt);
#endif
#ifdef RECOVERY_RECORD_COUNTER
    dcrecomp_generated::record_counter_register(rt);
#endif
#ifdef RECOVERY_RECORD_BUFFER
    dcrecomp_generated::record_buffer_register(rt);dcrecomp_generated::bit_expand_register(rt);dcrecomp_generated::rectangle_copy_register(rt);
#endif
#ifdef RECOVERY_RECORD_INIT
    dcrecomp_generated::record_init_register(rt);dcrecomp_generated::record_info_register(rt);dcrecomp_generated::info_wrapper_register(rt);dcrecomp_generated::timestamp_register(rt);dcrecomp_generated::bcd_register(rt);dcrecomp_generated::count_blocks_register(rt);
#endif
#ifdef RECOVERY_GLOBAL_RESET
    dcrecomp_generated::global_reset_register(rt);dcrecomp_generated::global_setup_register(rt);dcrecomp_generated::arena_init_register(rt);dcrecomp_generated::global_setter_register(rt);
#endif
#ifdef RECOVERY_GLOBAL_WORD_COPY
    dcrecomp_generated::global_word_copy_register(rt);
#endif
#ifdef RECOVERY_FRAME_DISPATCH
    dcrecomp_generated::frame_dispatch_register(rt);dcrecomp_generated::input_status_register(rt);dcrecomp_generated::frame_adjust_register(rt);
#endif
#ifdef RECOVERY_FRAME_QUAD
    dcrecomp_generated::frame_quad_register(rt);dcrecomp_generated::quad_state_register(rt);
#endif
#ifdef RECOVERY_QUAD_PACKETS
    dcrecomp_generated::quad_header_register(rt);dcrecomp_generated::quad_strip_register(rt);dcrecomp_generated::strip_writer_register(rt);
#endif
#ifdef RECOVERY_RECORD_STATE2
    dcrecomp_generated::record_state2_register(rt);
#endif
#ifdef RECOVERY_RECORD_READY
    dcrecomp_generated::record_ready_register(rt);dcrecomp_generated::ready_query_register(rt);dcrecomp_generated::ready_setter_register(rt);
#endif
    if(const char* path=std::getenv("RINGPAD_RECOVERY_SYSTEM_FLASH")){
        std::ifstream input(path,std::ios::binary);
        require(bool(input),"Cannot open declared system flash image");
        const std::vector<std::uint8_t> bytes{std::istreambuf_iterator<char>(input),{}};
        dc_set_system_flash_image(rt,bytes);
        std::cerr<<"DECLARED system flash image: "<<path
            <<"; BIOS service data only; flash-read success returns byte count\n";
    }
    // Run the declared driver's ARM7 interpreter when original code releases
    // reset. Keep KOS queue synthesis off and measure actual firmware state.
    dc_aica_enable_arm7(rt,true,128);
    rt.aica_arm7_idle_fast_forward=false;
    std::cerr<<"DECLARED AICA ARM7 interpreter enabled;128 instructions/slice; no synthesized queue; idle shortcut disabled\n";
    const auto capture=[&](const char*phase){
        if(std::string(phase)!="failure"&&std::string(phase)!="integration-entry")return;
        capture_startup_hardware(rt,recovery_frame_directory+"/original-startup-"+phase);
#ifdef RECOVERY_FRAME_SPRITES
        if(std::string(phase)=="first-sprites-entry"||std::string(phase)=="first-sprites-exit"){
            const auto stem=recovery_frame_directory+"/original-startup-"+phase;
            std::ofstream packets(stem+"-TA-packets.bin",std::ios::binary),sources(stem+"-TA-sources.bin",std::ios::binary),pcs(stem+"-TA-PCs.txt");
            for(const auto&packet:rt.pvr_ta_stream)packets.write(reinterpret_cast<const char*>(packet.bytes.data()),32);
            sources.write(reinterpret_cast<const char*>(rt.pvr_ta_stream_source.data()),rt.pvr_ta_stream_source.size());for(auto pc:rt.pvr_ta_stream_pc)pcs<<std::hex<<pc<<'\n';
            require(bool(packets)&&bool(sources)&&bool(pcs),"Cannot capture original sprite TA stream");
        }
#endif
        // Narrow source-state probe: observe the unchanged final boundary only.
        const std::string prefix=recovery_frame_directory+"/original-startup-"+phase;
        const char* reference_directory=std::getenv("RINGPAD_RECOVERY_CAPTURE_REFERENCE");
        auto write_capture=[&](const char* suffix,const auto& bytes){
            const std::string filename=std::string("original-startup-")+phase+suffix;
            const std::filesystem::path reference=reference_directory?std::filesystem::path(reference_directory)/filename:std::filesystem::path{};
            const bool shared=write_observed_capture(prefix+suffix,reference,bytes.data(),bytes.size());
            std::ofstream storage(recovery_frame_directory+"/capture-storage.tsv",std::ios::app);
            storage<<filename<<'\t'<<bytes.size()<<'\t'<<(shared?"clone-verified":"write-verified")<<'\n';
            require(bool(storage),"Cannot record capture storage");
        };
        write_capture("-ram.bin",rt.main_ram);
        write_capture("-aica-ram.bin",rt.aica_ram);
        write_capture("-system-flash.bin",rt.system_flash);
#ifdef RECOVERY_FRAME_QUAD
        if(std::string(phase)=="failure" || std::string(phase)=="first-quad-command-entry" || std::string(phase)=="first-quad-command-exit" || std::string(phase)=="first-quad-state-exit" || std::string(phase)=="first-quad-strip-entry" || std::string(phase)=="first-strip-writer-entry" || std::string(phase)=="first-strip-writer-exit" || std::string(phase)=="first-quad-strip-exit" || std::string(phase)=="first-quad-finalizer-entry" || std::string(phase)=="first-quad-exit" || std::string(phase)=="first-quad-draw-exit" || std::string(phase)=="first-sprites-entry" || std::string(phase)=="first-sprites-exit" || std::string(phase)=="first-sprite-texture-entry" || std::string(phase)=="first-sprite-texture-exit" || std::string(phase)=="first-sprite-entry" || std::string(phase)=="first-sprite-material-entry" || std::string(phase)=="first-sprite-material-exit" || std::string(phase)=="first-frame-exit" || std::string(phase)=="first-frame-tail-draw-entry" || std::string(phase)=="first-frame-tail-draw-exit" || std::string(phase)=="first-frame-counter-entry" || std::string(phase)=="first-frame-counter-exit" || std::string(phase)=="first-frame-mode-exit" || std::string(phase)=="first-mode-loader-entry" || std::string(phase)=="first-loader-owner-entry" || std::string(phase)=="first-loader-owner-exit" || std::string(phase)=="first-loader-service-entry" || std::string(phase)=="first-file-transfer-entry" || std::string(phase)=="first-movie-provider-entry" || std::string(phase)=="first-movie-provider-exit" || std::string(phase)=="first-movie-aot-entry" || std::string(phase)=="first-movie-resident-entry" || std::string(phase)=="first-movie-callback-entry" || std::string(phase)=="first-movie-work-entry" || std::string(phase)=="first-movie-service-entry" || std::string(phase)=="first-movie-setup-entry" || std::string(phase)=="first-movie-setup-callee-entry" || std::string(phase)=="first-movie-tail-entry" || std::string(phase)=="first-movie-tail-callee-entry" || std::string(phase)=="first-movie-mode-entry" || std::string(phase)=="first-movie-mode-callee-entry" || std::string(phase)=="first-movie-config-entry" || std::string(phase)=="first-movie-helper-entry" || std::string(phase)=="first-movie-clear-entry" || std::string(phase)=="first-movie-counter-entry" || std::string(phase)=="first-movie-counter-exit" || std::string(phase)=="first-movie-clear-tail-entry" || std::string(phase)=="first-movie-bulk-clear-entry" || std::string(phase)=="first-movie-bulk-clear-exit" || std::string(phase)=="first-movie-init-entry" || std::string(phase)=="first-movie-init-clear-entry" || std::string(phase)=="first-movie-init-clear-exit" || std::string(phase)=="first-movie-record-init-entry" || std::string(phase)=="first-movie-record-tail-entry" || std::string(phase)=="first-movie-guard-entry" || std::string(phase)=="first-movie-guard-clear-entry" || std::string(phase)=="first-movie-guard-exit" || std::string(phase)=="first-movie-zero-entry" || std::string(phase)=="first-movie-zero-exit" || std::string(phase)=="first-movie-resident-init-entry" || std::string(phase)=="first-movie-resident-clear-entry" || std::string(phase)=="first-movie-resident-init-exit" || std::string(phase)=="first-movie-allocation-entry" || std::string(phase)=="first-movie-lock-entry" || std::string(phase)=="first-movie-lock-exit" || std::string(phase)=="first-movie-slot-search-entry" || std::string(phase)=="first-movie-slot-search-exit" || std::string(phase)=="first-movie-allocation-exit" || std::string(phase)=="first-movie-stream-entry" || std::string(phase)=="first-movie-context-entry" || std::string(phase)=="first-movie-context-exit" || std::string(phase)=="first-movie-stream-search-entry" || std::string(phase)=="first-movie-stream-exit" || std::string(phase)=="first-movie-install-entry" || std::string(phase)=="first-movie-install-helper-entry" || std::string(phase)=="first-movie-install-helper-exit" || std::string(phase)=="first-movie-install-exit" || std::string(phase)=="first-movie-mode-config-entry" || std::string(phase)=="first-movie-mode-divide-entry" || std::string(phase)=="first-movie-mode-divide-exit" || std::string(phase)=="first-movie-mode-tail-entry" || std::string(phase)=="first-movie-mode-config-exit" || std::string(phase)=="first-movie-callback-register-entry" || std::string(phase)=="first-movie-callback-mask-enter-entry" || std::string(phase)=="first-movie-callback-mask-enter-exit" || std::string(phase)=="first-movie-callback-mask-leave-entry" || std::string(phase)=="first-movie-callback-mask-leave-exit" || std::string(phase)=="first-movie-callback-register-exit" || std::string(phase)=="first-movie-callback-register2-entry" || std::string(phase)=="first-movie-callback-mask-enter2-entry" || std::string(phase)=="first-movie-callback-mask-enter2-exit" || std::string(phase)=="first-movie-callback-mask-leave2-entry" || std::string(phase)=="first-movie-callback-mask-leave2-exit" || std::string(phase)=="first-movie-callback-register2-exit" || std::string(phase)=="first-movie-callback-register3-entry" || std::string(phase)=="first-movie-callback-mask-enter3-entry" || std::string(phase)=="first-movie-callback-mask-enter3-exit" || std::string(phase)=="first-movie-callback-mask-leave3-entry" || std::string(phase)=="first-movie-callback-mask-leave3-exit" || std::string(phase)=="first-movie-callback-register3-exit" || std::string(phase)=="first-movie-size-entry" || std::string(phase)=="first-movie-size-stream-entry" || std::string(phase)=="first-movie-size-divide-entry" || std::string(phase)=="first-movie-size-divide-exit" || std::string(phase)=="first-movie-size-stream-exit" || std::string(phase)=="first-movie-size-frame-entry" || std::string(phase)=="first-movie-dimensions-entry" || std::string(phase)=="first-movie-dimensions-exit" || std::string(phase)=="first-movie-size-frame-exit" || std::string(phase)=="first-movie-size-exit" || std::string(phase)=="first-movie-slot-setup-entry" || std::string(phase)=="first-movie-setup-validation-entry" || std::string(phase)=="first-movie-setup-copy-entry" || std::string(phase)=="first-movie-setup-copy-exit" || std::string(phase)=="first-movie-descriptor-init-entry" || std::string(phase)=="first-movie-descriptor-fields-entry" || std::string(phase)=="first-movie-word-fill-entry" || std::string(phase)=="first-movie-word-fill-exit" || std::string(phase)=="first-movie-string1-entry" || std::string(phase)=="first-movie-string1-exit" || std::string(phase)=="first-movie-string2-entry" || std::string(phase)=="first-movie-string2-exit" || std::string(phase)=="first-movie-word-copy-entry" || std::string(phase)=="first-movie-word-copy-exit" || std::string(phase)=="first-movie-work-descriptor-copy-entry" || std::string(phase)=="first-movie-work-descriptor-copy-exit" || std::string(phase)=="first-movie-record-clear-entry" || std::string(phase)=="first-movie-record-clear-exit" || std::string(phase)=="first-movie-rate-init-entry" || std::string(phase)=="first-movie-rate-init-exit" || std::string(phase)=="first-movie-context-return-entry" || std::string(phase)=="first-movie-context-return-exit" || std::string(phase)=="first-movie-buffer-fill-entry" || std::string(phase)=="first-movie-buffer-fill-exit" || std::string(phase)=="first-movie-interface-entry" || std::string(phase)=="first-movie-interface-copy-entry" || std::string(phase)=="first-movie-interface-copy-exit" || std::string(phase)=="first-movie-interface-bind-entry" || std::string(phase)=="first-movie-method-init-entry" || std::string(phase)=="first-movie-method-init-exit" || std::string(phase)=="first-movie-method-callback-entry" || std::string(phase)=="first-movie-method-setup-entry" || std::string(phase)=="first-method-workarea-entry" || std::string(phase)=="first-method-fill-entry" || std::string(phase)=="first-method-fill-exit" || std::string(phase)=="first-method-string1-entry" || std::string(phase)=="first-method-string1-exit" || std::string(phase)=="first-method-string2-entry" || std::string(phase)=="first-method-string2-exit" || std::string(phase)=="first-method-record-entry" || std::string(phase)=="first-method-record-exit" || std::string(phase)=="first-method-workarea-exit" || std::string(phase)=="first-method-followup-entry" || std::string(phase)=="first-method-followup-exit" || std::string(phase)=="first-method-final-entry" || std::string(phase)=="first-method-final-exit" || std::string(phase)=="first-method-setup-return" || std::string(phase)=="first-method-callback-return" || std::string(phase)=="first-movie-third-callback-entry" || std::string(phase)=="first-movie-third-setup-entry" || std::string(phase)=="first-third-validation-entry" || std::string(phase)=="first-third-guard-entry" || std::string(phase)=="first-third-guard-exit" || std::string(phase)=="first-pair-check-entry" || std::string(phase)=="first-pair1-entry" || std::string(phase)=="first-pair1-exit" || std::string(phase)=="first-pair2-entry" || std::string(phase)=="first-pair2-exit" || std::string(phase)=="first-pair3-entry" || std::string(phase)=="first-pair3-exit" || std::string(phase)=="first-pair4-entry" || std::string(phase)=="first-pair4-exit" || std::string(phase)=="first-pair-check-exit" || std::string(phase)=="first-triple-check-entry" || std::string(phase)=="first-triple1-entry" || std::string(phase)=="first-triple1-exit" || std::string(phase)=="first-triple2-entry" || std::string(phase)=="first-triple2-exit" || std::string(phase)=="first-triple3-entry" || std::string(phase)=="first-triple3-exit" || std::string(phase)=="first-triple-check-exit" || std::string(phase)=="first-third-validation-exit" || std::string(phase)=="first-cache-flags-entry" || std::string(phase)=="first-cache-flags-exit" || std::string(phase)=="first-work-pointer-entry" || std::string(phase)=="first-work-pointer-exit" || std::string(phase)=="first-work-config-entry" || std::string(phase)=="first-work-config-fill-entry" || std::string(phase)=="first-work-config-fill-exit" || std::string(phase)=="first-work-config-copy-entry" || std::string(phase)=="first-work-config-copy-exit" || std::string(phase)=="first-work-config-exit" || std::string(phase)=="first-work-clear-entry" || std::string(phase)=="first-work-clear-exit" || std::string(phase)=="first-work-reset-entry" || std::string(phase)=="first-work-reset-exit" || std::string(phase)=="first-tail-helper-entry" || std::string(phase)=="first-table-entry" || std::string(phase)=="first-table-exit" || std::string(phase)=="second-table-entry" || std::string(phase)=="second-table-exit" || std::string(phase)=="third-table-entry" || std::string(phase)=="third-table-exit" || std::string(phase)=="fourth-table-entry" || std::string(phase)=="fourth-table-exit" || std::string(phase)=="fifth-table-entry" || std::string(phase)=="fifth-table-exit" || std::string(phase)=="sixth-table-entry" || std::string(phase)=="sixth-table-exit" || std::string(phase)=="seventh-table-entry" || std::string(phase)=="seventh-table-exit" || std::string(phase)=="eighth-table-entry" || std::string(phase)=="eighth-table-exit" || std::string(phase)=="ninth-table-entry" || std::string(phase)=="ninth-table-exit" || std::string(phase)=="tenth-table-entry" || std::string(phase)=="tenth-table-exit" || std::string(phase)=="eleventh-table-entry" || std::string(phase)=="eleventh-table-exit" || std::string(phase)=="table-tail-entry" || std::string(phase)=="table-first-entry" || std::string(phase)=="table-first-exit" || std::string(phase)=="table-second-entry" || std::string(phase)=="table-second-exit" || std::string(phase)=="table-third-entry" || std::string(phase)=="table-third-exit" || std::string(phase)=="table-fourth-entry" || std::string(phase)=="table-fourth-exit" || std::string(phase)=="table-tail-return" || std::string(phase)=="work-followup-entry" || std::string(phase)=="work-followup-exit" || std::string(phase)=="work-final-entry" || std::string(phase)=="work-final-exit" || std::string(phase)=="post-work-entry" || std::string(phase)=="post-work-return" || std::string(phase)=="final-setup-entry" || std::string(phase)=="final-setup-exit" || std::string(phase)=="third-setup-return" || std::string(phase)=="third-callback-return" || std::string(phase)=="fourth-callback-entry" || std::string(phase)=="fourth-callback-return" || std::string(phase)=="fifth-callback-entry" || std::string(phase)=="fifth-callback-return" || std::string(phase)=="sixth-callback-entry" || std::string(phase)=="sixth-callback-return" || std::string(phase)=="binding-loop-return" || std::string(phase)=="interface-wrapper-return" || std::string(phase)=="descriptor-fields-return" || std::string(phase)=="signature-guard-entry" || std::string(phase)=="signature-guard-return" || std::string(phase)=="descriptor-final-entry" || std::string(phase)=="descriptor-final-return" || std::string(phase)=="descriptor-initializer-return" || std::string(phase)=="setup-bind-entry" || std::string(phase)=="setup-bind-child-entry" || std::string(phase)=="setup-bind-child-return" || std::string(phase)=="setup-bind-return" || std::string(phase)=="setup-validation-return" || std::string(phase)=="create-entry" || std::string(phase)=="create-stream-entry" || std::string(phase)=="create-stream-return" || std::string(phase)=="create-frame-entry" || std::string(phase)=="create-frame-dimensions-entry" || std::string(phase)=="create-frame-dimensions-return" || std::string(phase)=="create-frame-return" || std::string(phase)=="create-layout-dimensions-entry" || std::string(phase)=="create-layout-dimensions-return" || std::string(phase)=="create-copy-entry" || std::string(phase)=="create-copy-return" || std::string(phase)=="create-init-entry" || std::string(phase)=="create-init-return" || std::string(phase)=="create-second-init-entry" || std::string(phase)=="create-second-init-return" || std::string(phase)=="instance-entry" || std::string(phase)=="instance-guard-entry" || std::string(phase)=="instance-guard-return" || std::string(phase)=="instance-pool-entry" || std::string(phase)=="instance-pool-return" || std::string(phase)=="initialize-entry" || std::string(phase)=="initialize-fill-entry" || std::string(phase)=="initialize-fill-return" || std::string(phase)=="initialize-copy-entry" || std::string(phase)=="initialize-copy-return" || std::string(phase)=="initialize-fields-entry" || std::string(phase)=="initialize-fields-fill-entry" || std::string(phase)=="initialize-fields-fill-return" || std::string(phase)=="initialize-fields-return" || std::string(phase)=="second-fields-entry" || std::string(phase)=="second-fields-fill-entry" || std::string(phase)=="second-fields-fill-return" || std::string(phase)=="second-fields-return" || std::string(phase)=="second-record-entry" || std::string(phase)=="second-record-return" || std::string(phase)=="second-copy-entry" || std::string(phase)=="second-copy-return" || std::string(phase)=="instance-rate-entry" || std::string(phase)=="instance-handler-entry" || std::string(phase)=="instance-handler-first-return" || std::string(phase)=="instance-rate-fields-entry" || std::string(phase)=="instance-rate-fields-return" || std::string(phase)=="instance-rate-return" || std::string(phase)=="instance-stream-entry" || std::string(phase)=="instance-descriptor-entry" || std::string(phase)=="instance-descriptor-return" || std::string(phase)=="instance-remainder-entry" || std::string(phase)=="instance-remainder-return" || std::string(phase)=="instance-record-entry" || std::string(phase)=="instance-record-return" || std::string(phase)=="instance-record-second-entry" || std::string(phase)=="instance-record-second-return" || std::string(phase)=="instance-record-third-entry" || std::string(phase)=="instance-record-third-return" || std::string(phase)=="instance-small-record-entry" || std::string(phase)=="instance-small-record-return" || std::string(phase)=="instance-ring-entry" || std::string(phase)=="instance-ring-return" || std::string(phase)=="stream-tail-small-entry" || std::string(phase)=="stream-tail-small-return" || std::string(phase)=="stream-tail-ring-entry" || std::string(phase)=="stream-tail-ring-return" || std::string(phase)=="stream-tail-record-entry" || std::string(phase)=="stream-tail-record-return" || std::string(phase)=="instance-stream-return" || std::string(phase)=="instance-table-entry" || std::string(phase)=="instance-table-record-0-entry" || std::string(phase)=="instance-table-record-0-return" || std::string(phase)=="instance-table-record-1-entry" || std::string(phase)=="instance-table-record-1-return" || std::string(phase)=="instance-table-record-2-entry" || std::string(phase)=="instance-table-record-2-return" || std::string(phase)=="instance-table-record-3-entry" || std::string(phase)=="instance-table-record-3-return" || std::string(phase)=="instance-table-record-4-entry" || std::string(phase)=="instance-table-record-4-return" || std::string(phase)=="instance-table-record-5-entry" || std::string(phase)=="instance-table-record-5-return" || std::string(phase)=="instance-table-record-6-entry" || std::string(phase)=="instance-table-record-6-return" || std::string(phase)=="instance-table-record-7-entry" || std::string(phase)=="instance-table-record-7-return" || std::string(phase)=="instance-table-record-8-entry" || std::string(phase)=="instance-table-record-8-return" || std::string(phase)=="instance-table-final-entry" || std::string(phase)=="instance-selector-entry" || std::string(phase)=="instance-selector-return" || std::string(phase)=="instance-branch-entry" || std::string(phase)=="instance-branch-internal-entry" || std::string(phase)=="instance-branch-internal-return" || std::string(phase)=="instance-second-selector-entry" || std::string(phase)=="instance-second-selector-return" || std::string(phase)=="instance-next-branch-entry" || std::string(phase)=="instance-next-helper-entry" || std::string(phase)=="instance-next-helper-return" || std::string(phase)=="instance-next-selector-entry" || std::string(phase)=="instance-next-selector-return" || std::string(phase)=="instance-next-branch-return" || std::string(phase)=="instance-third-selector-entry" || std::string(phase)=="instance-third-selector-return" || std::string(phase)=="instance-third-branch-entry" || std::string(phase)=="instance-third-helper-entry" || std::string(phase)=="instance-third-helper-return" || std::string(phase)=="instance-third-branch-selector-entry" || std::string(phase)=="instance-third-branch-selector-return" || std::string(phase)=="instance-third-branch-return" || std::string(phase)=="instance-enclosing-branch-return" || std::string(phase)=="instance-table-final-return" || std::string(phase)=="instance-table-initializer-return" || std::string(phase)=="instance-final-thunk-entry" || std::string(phase)=="instance-handler-dispatch-entry" || std::string(phase)=="instance-first-handler-entry" || std::string(phase)=="instance-first-handler-return" || std::string(phase)=="instance-second-handler-entry" || std::string(phase)=="instance-handler-clear-entry" || std::string(phase)=="instance-handler-clear-return" || std::string(phase)=="instance-resource-wrapper-entry" || std::string(phase)=="instance-resource-search-entry" || std::string(phase)=="instance-resource-search-return" || std::string(phase)=="instance-resource-init-entry" || std::string(phase)=="instance-resource-fill-entry" || std::string(phase)=="instance-resource-fill-return" || std::string(phase)=="instance-resource-record-entry" || std::string(phase)=="instance-resource-record-return" || std::string(phase)=="resource-sentinels-entry" || std::string(phase)=="resource-sentinels-return" || std::string(phase)=="resource-blocks-entry" || std::string(phase)=="resource-block0-return" || std::string(phase)=="resource-block1-return" || std::string(phase)=="resource-block2-return" || std::string(phase)=="resource-block3-return" || std::string(phase)=="resource-final-entry" || std::string(phase)=="resource-final-return" || std::string(phase)=="resource-initializer-return" || std::string(phase)=="resource-wrapper-return" || std::string(phase)=="binding-entry" || std::string(phase)=="binding-validation-entry" || std::string(phase)=="binding-validation-return" || std::string(phase)=="binding-store-entry" || std::string(phase)=="binding-store-return" || std::string(phase)=="binding-return" || std::string(phase)=="second-handler-return" || std::string(phase)=="third-handler-entry" || std::string(phase)=="video-query-entry" || std::string(phase)=="video-query-return" || std::string(phase)=="video-setup-entry" || std::string(phase)=="video-copy-entry" || std::string(phase)=="video-copy-return" || std::string(phase)=="video-layout-entry" || std::string(phase)=="video-mask-entry" || std::string(phase)=="video-mask-return" || std::string(phase)=="video-restore-entry" || std::string(phase)=="video-restore-return" || std::string(phase)=="video-layout-return" || std::string(phase)=="video-setup-return"){
            std::ofstream sq(prefix+"-store-queues.bin",std::ios::binary);sq.write(reinterpret_cast<const char*>(rt.store_queues.data()),rt.store_queues.size());require(bool(sq),"Cannot capture store queues");
            std::ofstream qs(prefix+"-store-queue-state.txt");qs<<std::hex<<"qacr0="<<rt.qacr[0]<<"\nqacr1="<<rt.qacr[1]<<"\nqacr_writes="<<rt.qacr_writes<<"\npref_calls="<<rt.pref_calls<<"\npref_sq_calls="<<rt.pref_sq_calls<<"\nsq_commits="<<rt.sq_commits<<"\npref_ta_commits="<<rt.pref_ta_commits<<"\npref_texture_commits="<<rt.pref_texture_commits<<"\npref_other_commits="<<rt.pref_other_commits<<"\nlast_pref_address="<<rt.last_pref_address<<"\nlast_pref_target="<<rt.last_pref_target<<"\nlast_pref_source_pc="<<rt.last_pref_source_pc<<'\n';require(bool(qs),"Cannot capture store-queue state");
        }
#endif
        std::ofstream state(prefix+"-state.txt");
        state<<std::hex<<"pc="<<ctx.pc<<"\npr="<<ctx.pr<<"\nsr="<<ctx.sr
            <<"\nfpscr="<<ctx.fpscr<<"\nvbr="<<ctx.vbr<<"\nspc="<<ctx.spc<<'\n';
        for(unsigned i=0;i<16;++i)state<<"r"<<std::dec<<i<<'='<<std::hex<<ctx.r[i]<<'\n';
        state<<"gbr="<<ctx.gbr<<"\nmach="<<ctx.mach<<"\nmacl="<<ctx.macl<<"\nfpul="<<ctx.fpul<<'\n';
        for(unsigned i=0;i<16;++i)state<<"fr"<<std::dec<<i<<'='<<std::hex<<ctx.fr_bits[i]<<'\n';
        for(unsigned i=0;i<16;++i)state<<"xf"<<std::dec<<i<<'='<<std::hex<<ctx.xf_bits[i]<<'\n';
        state<<"mode="<<dc_read32(rt,0x8C7649F0)<<std::dec
            <<"\ncycles="<<rt.device_clock_sh4_cycles<<"\nsectors="<<rt.gd_sector_reads<<'\n';
        state<<"gd_bytes_read="<<rt.gd_bytes_read<<"\ncache_shadow="<<std::hex<<dc_read32(rt,0xFF00001C)<<std::dec<<'\n';
        state<<"arm7_enabled="<<rt.aica_arm7_enabled<<"\narm7_in_reset="<<rt.aica_arm7_in_reset
            <<"\narm7_instructions="<<rt.aica_arm.instructions<<"\narm7_faults="<<rt.aica_arm7_faults
            <<"\narm7_halted="<<rt.aica_arm.halted<<"\narm7_pc="<<std::hex<<rt.aica_arm.r[15]
            <<"\narm7_cpsr="<<rt.aica_arm.cpsr<<std::dec
            <<"\narm7_detail="<<rt.aica_arm7_last_detail<<"\naica_frames="<<rt.aica_native_frames
            <<"\naica_nonzero_frames="<<rt.aica_native_nonzero_frames<<'\n';
        state<<"g2_dma_runs="<<rt.g2_dma_runs<<"\ng2_dma_bytes="<<rt.g2_dma_bytes
            <<"\ng2_last_channel="<<rt.g2_last_channel<<std::hex
            <<"\ng2_last_g2="<<rt.g2_last_g2_addr<<"\ng2_last_sh4="<<rt.g2_last_sh4_addr
            <<"\ng2_last_size="<<rt.g2_last_size<<"\ng2_last_dir="<<rt.g2_last_dir<<std::dec<<'\n';
        for(unsigned i=0;i<rt.g2_dma.size();++i){
            const auto& dma=rt.g2_dma[i];
            state<<"g2_channel"<<i<<"="<<std::hex<<dma.g2_addr<<','<<dma.sh4_addr<<','
                <<dma.size<<','<<dma.dir<<','<<dma.trigger_select<<','<<dma.enable<<','
                <<dma.start<<','<<dma.suspend<<std::dec<<'\n';
        }
        state<<"tmu_tocr="<<std::hex<<unsigned(rt.tmu_tocr)<<"\ntmu_tstr="<<unsigned(rt.tmu_tstr)<<'\n';
        state<<"sh4_tick_pending_cycles="<<rt.sh4_tick_pending_cycles<<"\nsh4_tick_batch_cycles="<<rt.sh4_tick_batch_cycles<<'\n';
        for(unsigned i=0;i<rt.tmu.size();++i){
            const auto& ch=rt.tmu[i];
            state<<"tmu"<<std::dec<<i<<"_tcor="<<std::hex<<ch.tcor<<"\ntmu"<<std::dec<<i<<"_tcnt="<<std::hex<<ch.tcnt
                <<"\ntmu"<<std::dec<<i<<"_tcr="<<std::hex<<ch.tcr<<"\ntmu"<<std::dec<<i<<"_cycle_accumulator="<<std::hex<<ch.cycle_accumulator
                <<"\ntmu"<<std::dec<<i<<"_lazy_sh4_cycles="<<std::hex<<ch.lazy_sh4_cycles<<"\ntmu"<<std::dec<<i<<"_ticks="<<std::hex<<ch.ticks
                <<"\ntmu"<<std::dec<<i<<"_underflows="<<std::hex<<ch.underflows<<'\n';
        }
        state<<"dbr="<<std::hex<<ctx.dbr<<'\n';state<<"ssr="<<std::hex<<ctx.ssr<<"\nsgr="<<ctx.sgr<<'\n';
        for(unsigned i=0;i<8;++i)state<<"bank_r"<<std::dec<<i<<'='<<std::hex<<ctx.r_bank[i]<<'\n';
        state<<"holly_irq_taken="<<std::dec<<rt.holly_irq_taken<<"\nsh4_async_redirect="<<rt.sh4_async_redirect<<'\n';
        for(unsigned bank=0;bank<3;++bank){
            state<<"holly_pending"<<std::dec<<bank<<'='<<std::hex<<rt.holly_pending[bank]<<'\n';
            for(unsigned row=0;row<3;++row)state<<"holly_mask"<<std::dec<<row<<'_'<<bank<<'='<<std::hex<<rt.holly_masks[row][bank]<<'\n';
        }
        require(bool(state),"Cannot capture original startup state");
    };
    integration_entry_observer=[&]{capture("integration-entry");};
    struct ClearIntegrationObserver{~ClearIntegrationObserver(){integration_entry_observer={};}} clear_integration_observer;
    std::cerr<<"BEGIN original main entry010000; HLE post-BIOS state; IP handoff unverified; no supplied bank/video/task/mode state\n";
    const auto entry_return=ctx.pr;
    bool worker_entered=false,worker_yielded=false,worker_scheduler_returned=false;
    bool peripheral_request_captured=false,sound_batch_captured=false,g2_counters_captured=false,peripheral_setup_captured=false,peripheral_transfer_captured=false,peripheral_packet_captured=false;
    const auto observe_sound_batch=[&]{
        // Read-only first batch observation; no packet/ready/completion writes.
        const auto manager=dc_read32(rt,0x8C67FF10);
        if(!sound_batch_captured && manager && dc_read32(rt,manager+0x15C)==32 && dc_read32(rt,manager+0x160)==1){
            capture("first-sound-batch");sound_batch_captured=true;
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-sound-batch-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture first sound batch trace");
        }
    };
    const auto observe_g2_counters=[&]{
        // Read-only channel0 query outputs; no transfer/interrupt completion.
        if(!g2_counters_captured && dc_read32(rt,0x8C8948FC)==2 && dc_read32(rt,0x8C8948F8)==3){
            capture("first-g2-counter-queries");g2_counters_captured=true;
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-g2-counter-queries-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture first G2 counter query trace");
        }
    };
    const auto observe_peripheral_setup=[&]{
        // Original operation1 metadata only; no callback/completion injection.
        if(!peripheral_setup_captured && dc_read32(rt,0x8C8A0800)==2 && dc_read32(rt,0x8C8A0804)==17){
            capture("first-peripheral-operation-setup");peripheral_setup_captured=true;
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-peripheral-operation-setup-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture first peripheral operation setup trace");
        }
    };
    const auto observe_peripheral_transfer=[&]{
        // Observe progress-record writes and the actual SDK prerequisite boundary.
        if(!peripheral_transfer_captured && ctx.pc==0x8C64A87C && dc_read32(rt,0x8C6B613C)==1 && dc_read32(rt,0x8C6B6140)==17 && dc_read32(rt,0x8C6B6144)==1){
            capture("first-peripheral-transfer-attempt");peripheral_transfer_captured=true;
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-peripheral-transfer-attempt-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture first peripheral transfer trace");
        }
    };
    original_sdk_request=rt.targets.at(0x8C663550);
    require(original_sdk_request!=nullptr,"Original SDK request target absent");
    bool packet_dispatch_pending=false;
    bool command_pack_child_captured=false,command_pack_child_pending=false;
    sdk_request_observer=[&](bool after){
        // Observe the already registered provider without replacing its body.
        if(!after&&!command_pack_child_captured&&ctx.pc==0x8C663550&&ctx.pr==0x8C66427E&&ctx.r[4]==1&&ctx.r[5]==11){
            capture("first-command-pack-child-entry");command_pack_child_captured=true;command_pack_child_pending=true;
        }else if(after&&command_pack_child_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C66427E&&ctx.pr==0x8C66427E){
            capture("first-command-pack-child-exit");command_pack_child_pending=false;
        }

        if(!after && !peripheral_packet_captured && ctx.pc==0x8C663550 && ctx.pr==0x8C66421E && ctx.r[4]==1 && ctx.r[5]==10 && ctx.r[7]==2){
            const auto sdk=dc_read32(rt,0x8C8AC634),lock=dc_read32(rt,0x8C664534);
            require(dc_read32(rt,ctx.r[6])==2 && dc_read32(rt,ctx.r[6]+4)==0 && dc_read8(rt,lock)==0x80 && dc_read8(rt,sdk+40+44+13)==1,"Original request packet differs");
            capture("first-peripheral-request-packet");peripheral_packet_captured=true;packet_dispatch_pending=true;
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-peripheral-request-packet-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture first peripheral request packet trace");
        }else if(after && packet_dispatch_pending){
            capture("first-peripheral-request-dispatch-exit");packet_dispatch_pending=false;
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-peripheral-request-dispatch-exit-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture SDK request dispatch exit trace");
        }
    };
    original_sound_completion=rt.targets.at(0x8C66A810);
    require(original_sound_completion!=nullptr,"Original sound completion target absent");
    bool sound_completion_captured=false,sound_completion_pending=false;
    sound_completion_observer=[&](bool after){
        if(!after && !sound_completion_captured && ctx.pc==0x8C66A810 && ctx.pr==0x8C65E284){
            capture("first-sound-completion-entry");sound_completion_captured=true;sound_completion_pending=true;
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-sound-completion-entry-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture sound completion entry trace");
        }else if(after && sound_completion_pending){
            capture("first-sound-completion-exit");sound_completion_pending=false;
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-sound-completion-exit-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture sound completion exit trace");
        }
    };
    rt.register_target(0x8C66A810,observed_sound_completion);
    // A scoped read-only observer always calls the already registered original.
    // It changes no guest fields, payloads, return values or device responses.
    rt.register_target(0x8C663550,observed_sdk_request);
    original_module_init=rt.targets.at(0x8C6607D2);
    require(original_module_init!=nullptr,"Original module initializer target absent");
    bool module_init_captured=false,module_init_pending=false;
    module_init_observer=[&](bool after){
        if(!after && !module_init_captured && ctx.pc==0x8C6607D2 && ctx.pr==0x8C647D7C){
            capture("first-module-init-entry");module_init_captured=true;module_init_pending=true;
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-module-init-entry-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture module initializer entry trace");
        }else if(after && module_init_pending){
            capture("first-module-init-return");module_init_pending=false;
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-module-init-return-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture module initializer return trace");
        }
    };
    rt.register_target(0x8C6607D2,observed_module_init);
    original_nested_clear=rt.targets.at(0x8C65F328);
    require(original_nested_clear!=nullptr,"Original nested clear target absent");
    bool nested_clear_captured=false,nested_clear_pending=false;
    nested_clear_observer=[&](bool after){
        if(!after && !nested_clear_captured && ctx.pc==0x8C65F328 && ctx.pr==0x8C647DA0){
            nested_clear_captured=true;nested_clear_pending=true;
        }else if(after && nested_clear_pending){
            capture("first-nested-clear-return");nested_clear_pending=false;
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-nested-clear-return-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture nested clear return trace");
        }
    };
    rt.register_target(0x8C65F328,observed_nested_clear);
    original_critical_leave=rt.targets.at(0x8C65EACE);
    require(original_critical_leave!=nullptr,"Original critical leave target absent");
    bool critical_leave_captured=false,critical_leave_pending=false;
    critical_leave_observer=[&](bool after){
        if(!after && !critical_leave_captured && ctx.pc==0x8C65EACE && ctx.pr==0x8C05585C){
            capture("first-critical-leave-entry");critical_leave_captured=true;critical_leave_pending=true;
        }else if(after && critical_leave_pending){
            capture("first-critical-leave-exit");critical_leave_pending=false;
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-critical-leave-exit-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture critical leave exit trace");
        }
    };
    rt.register_target(0x8C65EACE,observed_critical_leave);
    original_file_pool=rt.targets.at(0x8C09F6D2);
    require(original_file_pool!=nullptr,"Original file pool target absent");
    bool file_pool_captured=false,file_pool_pending=false;
    file_pool_observer=[&](bool after){
        if(!after && !file_pool_captured && ctx.pc==0x8C09F6D2 && ctx.pr==0x8C055862){
            capture("first-file-pool-entry");file_pool_captured=true;file_pool_pending=true;
        }else if(after && file_pool_pending){
            capture("first-file-pool-exit");file_pool_pending=false;
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-file-pool-exit-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture file pool exit trace");
        }
    };
    rt.register_target(0x8C09F6D2,observed_file_pool);
    original_startup_dispatch=rt.targets.at(0x8C60549E);
    require(original_startup_dispatch!=nullptr,"Original startup dispatcher absent");
    bool startup_dispatch_captured=false,startup_dispatch_pending=false;
    startup_dispatch_observer=[&](bool after){
        if(!after && !startup_dispatch_captured && ctx.pc==0x8C60549E && ctx.pr==0x8C0556B6){
            capture("first-startup-dispatch-entry");startup_dispatch_captured=true;startup_dispatch_pending=true;
        }else if(after && startup_dispatch_pending){
            capture("first-startup-dispatch-exit");startup_dispatch_pending=false;
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-startup-dispatch-exit-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture startup dispatch return trace");
        }
    };
    rt.register_target(0x8C60549E,observed_startup_dispatch);
    original_timer_init=rt.targets.at(0x8C06DD60);
    require(original_timer_init!=nullptr,"Original timer initializer absent");
    bool timer_init_captured=false,timer_init_pending=false;
    timer_init_observer=[&](bool after){
        if(!after && !timer_init_captured && ctx.pc==0x8C06DD60 && ctx.pr==0x8C0556CE){
            capture("first-timer-init-entry");timer_init_captured=true;timer_init_pending=true;
        }else if(after && timer_init_pending){
            capture("first-timer-init-exit");timer_init_pending=false;
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-timer-init-exit-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture timer initializer return trace");
        }
    };
    rt.register_target(0x8C06DD60,observed_timer_init);
    original_startup_work=rt.targets.at(0x8C648422);
    original_task_scan=rt.targets.at(0x8C65F914);
    require(original_startup_work&&original_task_scan,"Original startup work/task scan absent");
    bool startup_work_captured=false,startup_work_pending=false,task_scan_captured=false,task_scan_pending=false;
    startup_work_observer=[&](bool after){
        if(!after&&!startup_work_captured&&ctx.pc==0x8C648422&&ctx.pr==0x8C0556D4){
            capture("first-startup-work-entry");startup_work_captured=true;startup_work_pending=true;
        }else if(after&&startup_work_pending){
            capture("first-startup-work-exit");startup_work_pending=false;
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-startup-work-exit-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture original timed work return trace");
        }
    };
    task_scan_observer=[&](bool after){
        if(!after&&!task_scan_captured&&ctx.pc==0x8C65F914&&ctx.pr==0x8C64843E){
            capture("first-task-scan-entry");task_scan_captured=true;task_scan_pending=true;
        }else if(after&&task_scan_pending){
            capture("first-task-scan-exit");task_scan_pending=false;
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-task-scan-exit-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture original task scan return trace");
        }
    };
    rt.register_target(0x8C648422,observed_startup_work);
    rt.register_target(0x8C65F914,observed_task_scan);
    struct RestoreWorkObservers{DCRuntime& rt;~RestoreWorkObservers(){
        rt.register_target(0x8C648422,original_startup_work);rt.register_target(0x8C65F914,original_task_scan);
        startup_work_observer={};task_scan_observer={};original_startup_work=nullptr;original_task_scan=nullptr;
    }}restore_work_observers{rt};
    original_task_critical=rt.targets.at(0x8C65F25A);
    require(original_task_critical!=nullptr,"Original critical task adapter absent");
    bool task_critical_captured=false,task_critical_pending=false;
    task_critical_observer=[&](bool after){
        if(!after&&!task_critical_captured&&ctx.pc==0x8C65F25A&&ctx.pr==0x8C648454){
            capture("first-task-critical-entry");task_critical_captured=true;task_critical_pending=true;
        }else if(after&&task_critical_pending){
            capture("first-task-critical-exit");task_critical_pending=false;
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-task-critical-exit-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture critical task return trace");
        }
    };
    rt.register_target(0x8C65F25A,observed_task_critical);
    struct RestoreTaskCritical{DCRuntime& rt;~RestoreTaskCritical(){
        rt.register_target(0x8C65F25A,original_task_critical);task_critical_observer={};original_task_critical=nullptr;
    }}restore_task_critical{rt};
#ifdef RECOVERY_RECORD_ITERATION
    original_record_iteration=rt.targets.at(0x8C10B992);
    bool record_iteration_entered=false,record_iteration_returned=false;
    record_iteration_observer=[&](bool after){
        if(!after&&!record_iteration_entered&&ctx.pc==0x8C10B992&&ctx.pr==0x8C0556EE){
            capture("first-record-iteration-entry");record_iteration_entered=true;
        }
        if(after&&!rt.sh4_async_redirect&&record_iteration_entered&&!record_iteration_returned&&ctx.pc==0x8C0556EE&&ctx.pr==0x8C0556EE){
            capture("first-record-iteration-exit");record_iteration_returned=true;
        }
    };
    rt.register_target(0x8C10B992,observed_record_iteration);
    struct RestoreRecordIteration{DCRuntime&rt;~RestoreRecordIteration(){
        rt.register_target(0x8C10B992,original_record_iteration);record_iteration_observer={};original_record_iteration=nullptr;
    }}restore_record_iteration{rt};
#endif
#ifdef RECOVERY_SEVENBYTE
    original_sevenbyte=rt.targets.at(0x8C647428);
    bool sevenbyte_entered=false,sevenbyte_returned=false;
    sevenbyte_observer=[&](bool after){
        if(!after&&!sevenbyte_entered&&ctx.pc==0x8C647428&&ctx.pr==0x8C0969A6){
            capture("first-sevenbyte-entry");sevenbyte_entered=true;
        }
        if(after&&!rt.sh4_async_redirect&&sevenbyte_entered&&!sevenbyte_returned&&ctx.pc==0x8C0969A6&&ctx.pr==0x8C0969A6){
            capture("first-sevenbyte-exit");sevenbyte_returned=true;
        }
    };
    rt.register_target(0x8C647428,observed_sevenbyte);
    struct RestoreSevenByte{DCRuntime&rt;~RestoreSevenByte(){
        rt.register_target(0x8C647428,original_sevenbyte);sevenbyte_observer={};original_sevenbyte=nullptr;
    }}restore_sevenbyte{rt};
#endif
#ifdef RECOVERY_WORD_COPY
    original_word_copy=rt.targets.at(0x8C66453C);
    bool word_copy_entered=false,word_copy_returned=false;
    word_copy_observer=[&](bool after){
        if(!after&&!word_copy_entered&&ctx.pc==0x8C66453C&&ctx.pr==0x8C6648BE){
            capture("first-word-copy-entry");word_copy_entered=true;
        }
        if(after&&!rt.sh4_async_redirect&&word_copy_entered&&!word_copy_returned&&ctx.pc==0x8C6648BE&&ctx.pr==0x8C6648BE){
            capture("first-word-copy-exit");word_copy_returned=true;
        }
    };
    rt.register_target(0x8C66453C,observed_word_copy);
    struct RestoreWordCopy{DCRuntime&rt;~RestoreWordCopy(){
        rt.register_target(0x8C66453C,original_word_copy);word_copy_observer={};original_word_copy=nullptr;
    }}restore_word_copy{rt};
#endif
#ifdef RECOVERY_PAYLOAD_VALIDATOR
    original_payload_validator=rt.targets.at(0x8C64E1F2);
    bool payload_validator_entered=false,payload_validator_returned=false;
    payload_validator_observer=[&](bool after){
        if(!after&&!payload_validator_entered&&ctx.pc==0x8C64E1F2&&ctx.pr==0x8C64EF68){
            capture("first-payload-validator-entry");payload_validator_entered=true;
        }
        if(after&&!rt.sh4_async_redirect&&payload_validator_entered&&!payload_validator_returned&&ctx.pc==0x8C64EF68&&ctx.pr==0x8C64EF68){
            capture("first-payload-validator-exit");payload_validator_returned=true;
        }
    };
    rt.register_target(0x8C64E1F2,observed_payload_validator);
    struct RestorePayloadValidator{DCRuntime&rt;~RestorePayloadValidator(){
        rt.register_target(0x8C64E1F2,original_payload_validator);payload_validator_observer={};original_payload_validator=nullptr;
    }}restore_payload_validator{rt};
#endif
#ifdef RECOVERY_BLOCK_ITERATION
    original_block_iteration=rt.targets.at(0x8C64EBA2);original_block_read=rt.targets.at(0x8C64B350);
    original_block_worker=rt.targets.at(0x8C6647BE);original_block_query=rt.targets.at(0x8C664B2C);
    unsigned block_phase=0;
    block_iteration_observer=[&](unsigned phase){
        const char* labels[]={"first-block-iteration-entry","first-block-read-entry","first-block-worker-entry","first-block-query-entry","first-block-query-exit","first-block-iteration-exit"};
        const std::uint32_t pcs[]={0x8C64EBA2,0x8C64B350,0x8C6647BE,0x8C664B2C,0x8C6647E6,0x8C64EF76};
        const std::uint32_t prs[]={0x8C64EF76,0x8C64EBDA,0x8C64B386,0x8C6647E6,0x8C6647E6,0x8C64EF76};
        if(phase==block_phase&&ctx.pc==pcs[phase]&&ctx.pr==prs[phase]&&!rt.sh4_async_redirect){capture(labels[phase]);++block_phase;}
    };
    rt.register_target(0x8C64EBA2,observed_block_iteration);rt.register_target(0x8C64B350,observed_block_read);
    rt.register_target(0x8C6647BE,observed_block_worker);rt.register_target(0x8C664B2C,observed_block_query);
    struct RestoreBlockIteration{DCRuntime&rt;~RestoreBlockIteration(){
        rt.register_target(0x8C64EBA2,original_block_iteration);rt.register_target(0x8C64B350,original_block_read);
        rt.register_target(0x8C6647BE,original_block_worker);rt.register_target(0x8C664B2C,original_block_query);
        block_iteration_observer={};original_block_iteration=nullptr;original_block_read=nullptr;original_block_worker=nullptr;original_block_query=nullptr;
    }}restore_block_iteration{rt};
#endif
#ifdef RECOVERY_LINKED_BLOCKS
    original_linked_blocks=rt.targets.at(0x8C64E6E8);original_linked_read=rt.targets.at(0x8C64B350);
    original_linked_worker=rt.targets.at(0x8C6647BE);original_linked_query=rt.targets.at(0x8C664B2C);
    unsigned linked_phase=0;
    linked_blocks_observer=[&](unsigned phase){
        const char* labels[]={"first-linked-iteration-entry","first-linked-read-entry","first-linked-worker-entry","first-linked-query-entry","first-linked-query-exit","first-linked-iteration-exit"};
        const std::uint32_t pcs[]={0x8C64E6E8,0x8C64B350,0x8C6647BE,0x8C664B2C,0x8C6647E6,0x8C64EF86};
        const std::uint32_t prs[]={0x8C64EF86,0x8C64E720,0x8C64B386,0x8C6647E6,0x8C6647E6,0x8C64EF86};
        if(phase==linked_phase&&ctx.pc==pcs[phase]&&ctx.pr==prs[phase]&&!rt.sh4_async_redirect){capture(labels[phase]);++linked_phase;}
    };
    rt.register_target(0x8C64E6E8,observed_linked_blocks);rt.register_target(0x8C64B350,observed_linked_read);
    rt.register_target(0x8C6647BE,observed_linked_worker);rt.register_target(0x8C664B2C,observed_linked_query);
    struct RestoreLinkedBlocks{DCRuntime&rt;~RestoreLinkedBlocks(){
        rt.register_target(0x8C64E6E8,original_linked_blocks);rt.register_target(0x8C64B350,original_linked_read);
        rt.register_target(0x8C6647BE,original_linked_worker);rt.register_target(0x8C664B2C,original_linked_query);
        linked_blocks_observer={};original_linked_blocks=nullptr;original_linked_read=nullptr;original_linked_worker=nullptr;original_linked_query=nullptr;
    }}restore_linked_blocks{rt};
#endif
#ifdef RECOVERY_TIMER_CALLBACK
    original_timer_callback=rt.targets.at(0x8C06DE78);
    bool timer_callback_entered=false,timer_callback_returned=false;
    timer_callback_observer=[&](bool after){
        if(!after&&!timer_callback_entered&&ctx.pc==0x8C06DE78&&ctx.pr==0x8C6562CC){capture("first-timer-callback-entry");timer_callback_entered=true;}
        if(after&&!rt.sh4_async_redirect&&timer_callback_entered&&!timer_callback_returned&&ctx.pc==0x8C6562CC&&ctx.pr==0x8C6562CC){capture("first-timer-callback-exit");timer_callback_returned=true;}
    };
    rt.register_target(0x8C06DE78,observed_timer_callback);
    struct RestoreTimerCallback{DCRuntime&rt;~RestoreTimerCallback(){rt.register_target(0x8C06DE78,original_timer_callback);timer_callback_observer={};original_timer_callback=nullptr;}}restore_timer_callback{rt};
#endif
#ifdef RECOVERY_RECORD_UPDATE
    original_record_update=rt.targets.at(0x8C10A4CE);
    bool record_update_entered=false,record_update_returned=false;
    record_update_observer=[&](bool after){
        if(!after&&!record_update_entered&&ctx.pc==0x8C10A4CE&&ctx.pr==0x8C10B39E){capture("first-record-update-entry");record_update_entered=true;}
        if(after&&!rt.sh4_async_redirect&&record_update_entered&&!record_update_returned&&ctx.pc==0x8C10B39E&&ctx.pr==0x8C10B39E){capture("first-record-update-exit");record_update_returned=true;}
    };
    rt.register_target(0x8C10A4CE,observed_record_update);
    struct RestoreRecordUpdate{DCRuntime&rt;~RestoreRecordUpdate(){rt.register_target(0x8C10A4CE,original_record_update);record_update_observer={};original_record_update=nullptr;}}restore_record_update{rt};
#endif
#ifdef RECOVERY_RECORD_RESET
    original_record_reset=rt.targets.at(0x8C10ABC0);
    bool record_reset_entered=false,record_reset_returned=false;
    record_reset_observer=[&](bool after){
        if(!after&&!record_reset_entered&&ctx.pc==0x8C10ABC0&&ctx.pr==0x8C10B410){capture("first-record-reset-entry");record_reset_entered=true;}
        if(after&&!rt.sh4_async_redirect&&record_reset_entered&&!record_reset_returned&&ctx.pc==0x8C10B410&&ctx.pr==0x8C10B410){capture("first-record-reset-exit");record_reset_returned=true;}
    };
    rt.register_target(0x8C10ABC0,observed_record_reset);
    struct RestoreRecordReset{DCRuntime&rt;~RestoreRecordReset(){rt.register_target(0x8C10ABC0,original_record_reset);record_reset_observer={};original_record_reset=nullptr;}}restore_record_reset{rt};
#endif
#ifdef RECOVERY_RECORD_COUNTER
    original_record_counter=rt.targets.at(0x8C109E9E);
    bool record_counter_entered=false,record_counter_returned=false;
    record_counter_observer=[&](bool after){
        if(!after&&!record_counter_entered&&ctx.pc==0x8C109E9E&&ctx.pr==0x8C10B41C){capture("first-record-counter-entry");record_counter_entered=true;}
        if(after&&!rt.sh4_async_redirect&&record_counter_entered&&!record_counter_returned&&ctx.pc==0x8C10B41C&&ctx.pr==0x8C10B41C){capture("first-record-counter-exit");record_counter_returned=true;}
    };
    rt.register_target(0x8C109E9E,observed_record_counter);
    struct RestoreRecordCounter{DCRuntime&rt;~RestoreRecordCounter(){rt.register_target(0x8C109E9E,original_record_counter);record_counter_observer={};original_record_counter=nullptr;}}restore_record_counter{rt};
#endif
#ifdef RECOVERY_RECORD_BUFFER
    original_record_buffer=rt.targets.at(0x8C109DB4);
    bool record_buffer_entered=false,record_buffer_returned=false;
    record_buffer_observer=[&](bool after){
        if(!after&&!record_buffer_entered&&ctx.pc==0x8C109DB4&&ctx.pr==0x8C10B422){capture("first-record-buffer-entry");record_buffer_entered=true;}
        if(after&&!rt.sh4_async_redirect&&record_buffer_entered&&!record_buffer_returned&&ctx.pc==0x8C10B422&&ctx.pr==0x8C10B422){capture("first-record-buffer-exit");record_buffer_returned=true;}
    };
    rt.register_target(0x8C109DB4,observed_record_buffer);
    struct RestoreRecordBuffer{DCRuntime&rt;~RestoreRecordBuffer(){rt.register_target(0x8C109DB4,original_record_buffer);record_buffer_observer={};original_record_buffer=nullptr;}}restore_record_buffer{rt};
#endif
#ifdef RECOVERY_RECORD_INIT
    original_record_init=rt.targets.at(0x8C010E9E);
    bool record_init_entered=false,record_init_returned=false;
    record_init_observer=[&](bool after){
        if(!after&&!record_init_entered&&ctx.pc==0x8C010E9E&&ctx.pr==0x8C64B8F4){capture("first-record-init-entry");record_init_entered=true;}
        if(after&&!rt.sh4_async_redirect&&record_init_entered&&!record_init_returned&&ctx.pc==0x8C64B8F4&&ctx.pr==0x8C64B8F4){capture("first-record-init-exit");record_init_returned=true;}
    };
    rt.register_target(0x8C010E9E,observed_record_init);
    struct RestoreRecordInit{DCRuntime&rt;~RestoreRecordInit(){rt.register_target(0x8C010E9E,original_record_init);record_init_observer={};original_record_init=nullptr;}}restore_record_init{rt};
#endif
#ifdef RECOVERY_GLOBAL_RESET
    original_global_reset=rt.targets.at(0x8C090F0C);original_arena_init=rt.targets.at(0x8C09A6B0);
    bool global_reset_entered=false,arena_init_entered=false,arena_init_returned=false,global_reset_returned=false;
    global_reset_observer=[&](bool arena,bool after){
        if(!arena&&!after&&!global_reset_entered&&ctx.pc==0x8C090F0C&&ctx.pr==0x8C0559F8){capture("first-global-reset-entry");global_reset_entered=true;}
        if(arena&&!after&&!arena_init_entered&&ctx.pc==0x8C09A6B0&&ctx.pr==0x8C090EBA){capture("first-arena-init-entry");arena_init_entered=true;}
        if(arena&&after&&arena_init_entered&&!arena_init_returned&&!rt.sh4_async_redirect&&ctx.pc==0x8C090EBA&&ctx.pr==0x8C090EBA){capture("first-arena-init-exit");arena_init_returned=true;}
        if(!arena&&after&&global_reset_entered&&!global_reset_returned&&!rt.sh4_async_redirect&&ctx.pc==0x8C0559F8&&ctx.pr==0x8C0559F8){capture("first-global-reset-exit");global_reset_returned=true;}
    };
    rt.register_target(0x8C090F0C,observed_global_reset);rt.register_target(0x8C09A6B0,observed_arena_init);
    struct RestoreGlobalReset{DCRuntime&rt;~RestoreGlobalReset(){rt.register_target(0x8C090F0C,original_global_reset);rt.register_target(0x8C09A6B0,original_arena_init);global_reset_observer={};original_global_reset=nullptr;original_arena_init=nullptr;}}restore_global_reset{rt};
#endif
#ifdef RECOVERY_RECORD_STATE2
    original_record_state2=rt.targets.at(0x8C10AB50);
    bool record_state2_entered=false,record_state2_returned=false;
    record_state2_observer=[&](bool after){
        if(!after&&!record_state2_entered&&ctx.pc==0x8C10AB50&&ctx.pr==0x8C10B3B6){capture("first-record-state2-entry");record_state2_entered=true;}
        if(after&&!rt.sh4_async_redirect&&record_state2_entered&&!record_state2_returned&&ctx.pc==0x8C10B3B6&&ctx.pr==0x8C10B3B6){capture("first-record-state2-exit");record_state2_returned=true;}
    };
    rt.register_target(0x8C10AB50,observed_record_state2);
    struct RestoreRecordState2{DCRuntime&rt;~RestoreRecordState2(){rt.register_target(0x8C10AB50,original_record_state2);record_state2_observer={};original_record_state2=nullptr;}}restore_record_state2{rt};
#endif
#ifdef RECOVERY_RECORD_READY
    original_record_ready=rt.targets.at(0x8C10AB1E);
    bool record_ready_entered=false,record_ready_returned=false;
    record_ready_observer=[&](bool after){
        if(!after&&!record_ready_entered&&ctx.pc==0x8C10AB1E&&ctx.pr==0x8C10B3AA){capture("first-record-ready-entry");record_ready_entered=true;}
        if(after&&!rt.sh4_async_redirect&&record_ready_entered&&!record_ready_returned&&ctx.pc==0x8C10B3AA&&ctx.pr==0x8C10B3AA){capture("first-record-ready-exit");record_ready_returned=true;}
    };
    rt.register_target(0x8C10AB1E,observed_record_ready);
    struct RestoreRecordReady{DCRuntime&rt;~RestoreRecordReady(){rt.register_target(0x8C10AB1E,original_record_ready);record_ready_observer={};original_record_ready=nullptr;}}restore_record_ready{rt};
#endif
    original_startup_counter=rt.targets.at(0x8C010B7A);
    require(original_startup_counter!=nullptr,"Original startup counter absent");
    bool startup_counter_entered=false,startup_counter_returned=false;
    startup_counter_observer=[&](bool after){
        if(!after&&!startup_counter_entered&&ctx.pc==0x8C010B7A&&ctx.pr==0x8C0556E4){
            capture("first-startup-counter-entry");startup_counter_entered=true;
        }
        if(after&&!rt.sh4_async_redirect&&startup_counter_entered&&!startup_counter_returned&&ctx.pc==0x8C0556E4&&ctx.pr==0x8C0556E4){
            capture("first-startup-counter-exit");startup_counter_returned=true;
        }
    };
    rt.register_target(0x8C010B7A,observed_startup_counter);
    struct RestoreStartupCounter{DCRuntime&rt;~RestoreStartupCounter(){
        rt.register_target(0x8C010B7A,original_startup_counter);startup_counter_observer={};original_startup_counter=nullptr;
    }}restore_startup_counter{rt};
    original_startup_word_add=rt.targets.at(0x8C08C25E);
    require(original_startup_word_add!=nullptr,"Original startup word add absent");
    bool startup_word_add_entered=false,startup_word_add_returned=false;
    startup_word_add_observer=[&](bool after){
        if(!after&&!startup_word_add_entered&&ctx.pc==0x8C08C25E&&ctx.pr==0x8C0561EE){
            capture("first-startup-word-add-entry");startup_word_add_entered=true;
        }
        if(after&&!rt.sh4_async_redirect&&startup_word_add_entered&&!startup_word_add_returned&&ctx.pc==0x8C0561EE&&ctx.pr==0x8C0561EE){
            capture("first-startup-word-add-exit");startup_word_add_returned=true;
        }
    };
    rt.register_target(0x8C08C25E,observed_startup_word_add);
    struct RestoreStartupWordAdd{DCRuntime&rt;~RestoreStartupWordAdd(){
        rt.register_target(0x8C08C25E,original_startup_word_add);startup_word_add_observer={};original_startup_word_add=nullptr;
    }}restore_startup_word_add{rt};
    original_startup_zero_guard=rt.targets.at(0x8C04562E);
    require(original_startup_zero_guard!=nullptr,"Original startup zero guard absent");
    bool startup_zero_guard_entered=false,startup_zero_guard_returned=false;
    startup_zero_guard_observer=[&](bool after){
        if(!after&&!startup_zero_guard_entered&&ctx.pc==0x8C04562E&&ctx.pr==0x8C0561E8){
            capture("first-startup-zero-guard-entry");startup_zero_guard_entered=true;
        }
        if(after&&!rt.sh4_async_redirect&&startup_zero_guard_entered&&!startup_zero_guard_returned&&ctx.pc==0x8C0561E8&&ctx.pr==0x8C0561E8){
            capture("first-startup-zero-guard-exit");startup_zero_guard_returned=true;
        }
    };
    rt.register_target(0x8C04562E,observed_startup_zero_guard);
    struct RestoreStartupZeroGuard{DCRuntime& rt;~RestoreStartupZeroGuard(){
        rt.register_target(0x8C04562E,original_startup_zero_guard);startup_zero_guard_observer={};original_startup_zero_guard=nullptr;
    }}restore_startup_zero_guard{rt};
    original_startup_query_reset=rt.targets.at(0x8C05A2E6);
    require(original_startup_query_reset!=nullptr,"Original startup query reset absent");
    bool startup_query_reset_entered=false,startup_query_reset_returned=false;
    startup_query_reset_observer=[&](bool after){
        if(!after&&!startup_query_reset_entered&&ctx.pc==0x8C05A2E6&&ctx.pr==0x8C05595A){
            capture("first-startup-query-reset-entry");startup_query_reset_entered=true;
        }
        if(after&&startup_query_reset_entered&&!startup_query_reset_returned&&ctx.pc==0x8C05595A&&ctx.pr==0x8C05595A){
            capture("first-startup-query-reset-exit");startup_query_reset_returned=true;
        }
    };
    rt.register_target(0x8C05A2E6,observed_startup_query_reset);
    struct RestoreStartupQueryReset{DCRuntime& rt;~RestoreStartupQueryReset(){
        rt.register_target(0x8C05A2E6,original_startup_query_reset);startup_query_reset_observer={};original_startup_query_reset=nullptr;
    }}restore_startup_query_reset{rt};
    original_startup_query_provider=rt.targets.at(0x8C606C9C);
    require(original_startup_query_provider!=nullptr,"Original startup query provider absent");
    bool startup_query_provider_entered=false,startup_query_provider_returned=false;
    startup_query_provider_observer=[&](bool after){
        if(!after&&!startup_query_provider_entered&&ctx.pc==0x8C606C9C&&ctx.pr==0x8C05577A){
            capture("first-startup-query-provider-entry");startup_query_provider_entered=true;
        }
        if(after&&startup_query_provider_entered&&!startup_query_provider_returned&&ctx.pc==0x8C05577A&&ctx.pr==0x8C05577A){
            capture("first-startup-query-provider-exit");startup_query_provider_returned=true;
        }
    };
    rt.register_target(0x8C606C9C,observed_startup_query_provider);
    struct RestoreStartupQueryProvider{DCRuntime& rt;~RestoreStartupQueryProvider(){
        rt.register_target(0x8C606C9C,original_startup_query_provider);startup_query_provider_observer={};original_startup_query_provider=nullptr;
    }}restore_startup_query_provider{rt};
    bool startup_first_query_return_captured=false;
    startup_first_query_return_observer=[&]{
        if(!startup_first_query_return_captured&&ctx.pc==0x8C055946&&ctx.pr==0x8C055946){
            capture("first-startup-first-query-exit");startup_first_query_return_captured=true;
        }
    };
    original_startup_check_query=rt.targets.at(0x8C055798);
    require(original_startup_check_query!=nullptr,"Original startup check_query absent");
    bool startup_check_query_captured=false;
    startup_check_query_observer=[&]{
        if(!startup_check_query_captured&&ctx.pc==0x8C055798&&ctx.pr==0x8C055946){
            capture("first-startup-check-query-entry");startup_check_query_captured=true;
        }
    };
    rt.register_target(0x8C055798,observed_startup_check_query);
    struct RestoreStartupcheck_query{DCRuntime& rt;~RestoreStartupcheck_query(){
        rt.register_target(0x8C055798,original_startup_check_query);startup_check_query_observer={};startup_first_query_return_observer={};original_startup_check_query=nullptr;
    }}restore_startup_check_query{rt};
    original_startup_query_helper=rt.targets.at(0x8C055768);
    require(original_startup_query_helper!=nullptr,"Original startup query_helper absent");
    bool startup_query_helper_captured=false;
    startup_query_helper_observer=[&]{
        if(!startup_query_helper_captured&&ctx.pc==0x8C055768&&ctx.pr==0x8C0557A6){
            capture("first-startup-query-helper-entry");startup_query_helper_captured=true;
        }
    };
    rt.register_target(0x8C055768,observed_startup_query_helper);
    struct RestoreStartupquery_helper{DCRuntime& rt;~RestoreStartupquery_helper(){
        rt.register_target(0x8C055768,original_startup_query_helper);startup_query_helper_observer={};original_startup_query_helper=nullptr;
    }}restore_startup_query_helper{rt};
    original_startup_gate_return=rt.targets.at(0x8C011238);
    require(original_startup_gate_return!=nullptr,"Original startup gate_return absent");
    bool startup_gate_return_captured=false,startup_gate_return_pending=false;
    startup_gate_return_observer=[&](bool after){
        if(!after&&!startup_gate_return_captured&&ctx.pc==0x8C011238&&ctx.pr==0x8C04E33E){
            capture("first-startup-gate-return-entry");startup_gate_return_captured=true;startup_gate_return_pending=true;
        }else if(after&&startup_gate_return_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C04E33E&&ctx.pr==0x8C04E33E){
            capture("first-startup-gate-return-exit");startup_gate_return_pending=false;
        }
    };
    rt.register_target(0x8C011238,observed_startup_gate_return);
    struct RestoreStartupgate_return{DCRuntime& rt;~RestoreStartupgate_return(){
        rt.register_target(0x8C011238,original_startup_gate_return);startup_gate_return_observer={};original_startup_gate_return=nullptr;
    }}restore_startup_gate_return{rt};
    original_startup_owner_return=rt.targets.at(0x8C10EFBC);
    require(original_startup_owner_return!=nullptr,"Original startup owner_return absent");
    bool startup_owner_return_captured=false,startup_owner_return_pending=false;
    startup_owner_return_observer=[&](bool after){
        if(!after&&!startup_owner_return_captured&&ctx.pc==0x8C10EFBC&&ctx.pr==0x8C055942){
            startup_owner_return_captured=true;startup_owner_return_pending=true;
        }else if(after&&startup_owner_return_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C055942&&ctx.pr==0x8C055942){
            capture("first-startup-owner-return-exit");startup_owner_return_pending=false;
        }
    };
    rt.register_target(0x8C10EFBC,observed_startup_owner_return);
    struct RestoreStartupowner_return{DCRuntime& rt;~RestoreStartupowner_return(){
        rt.register_target(0x8C10EFBC,original_startup_owner_return);startup_owner_return_observer={};original_startup_owner_return=nullptr;
    }}restore_startup_owner_return{rt};
    original_startup_flag_return=rt.targets.at(0x8C08BD14);
    require(original_startup_flag_return!=nullptr,"Original startup flagclear absent");
    bool startup_flag_return_captured=false,startup_flag_return_pending=false;
    startup_flag_return_observer=[&](bool after){
        if(!after&&!startup_flag_return_captured&&ctx.pc==0x8C08BD14&&ctx.pr==0x8C08A07A){
            capture("first-startup-flag-return-entry");startup_flag_return_captured=true;startup_flag_return_pending=true;
        }else if(after&&startup_flag_return_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C08A07A&&ctx.pr==0x8C08A07A){
            capture("first-startup-flag-return-exit");startup_flag_return_pending=false;
        }
    };
    rt.register_target(0x8C08BD14,observed_startup_flag_return);
    struct RestoreStartupFlagReturn{DCRuntime& rt;~RestoreStartupFlagReturn(){
        rt.register_target(0x8C08BD14,original_startup_flag_return);startup_flag_return_observer={};original_startup_flag_return=nullptr;
    }}restore_startup_flag_return{rt};
    original_startup_checksum=rt.targets.at(0x8C08A394);
    require(original_startup_checksum!=nullptr,"Original startup checksum absent");
    bool startup_checksum_captured=false,startup_checksum_pending=false;
    startup_checksum_observer=[&](bool after){
        if(!after&&!startup_checksum_captured&&ctx.pc==0x8C08A394&&ctx.pr==0x8C08A038){
            capture("first-startup-checksum-entry");startup_checksum_captured=true;startup_checksum_pending=true;
        }else if(after&&startup_checksum_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C08A038&&ctx.pr==0x8C08A038){
            capture("first-startup-checksum-exit");startup_checksum_pending=false;
        }
    };
    rt.register_target(0x8C08A394,observed_startup_checksum);
    struct RestoreStartupChecksum{DCRuntime& rt;~RestoreStartupChecksum(){
        rt.register_target(0x8C08A394,original_startup_checksum);startup_checksum_observer={};original_startup_checksum=nullptr;
    }}restore_startup_checksum{rt};
    original_startup_ninebytes=rt.targets.at(0x8C089E34);
    require(original_startup_ninebytes!=nullptr,"Original startup ninebyte initializer absent");
    bool startup_ninebytes_captured=false,startup_ninebytes_pending=false;
    startup_ninebytes_observer=[&](bool after){
        if(!after&&!startup_ninebytes_captured&&ctx.pc==0x8C089E34&&ctx.pr==0x8C089FD4){
            capture("first-startup-ninebytes-entry");startup_ninebytes_captured=true;startup_ninebytes_pending=true;
        }else if(after&&startup_ninebytes_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C089FD4&&ctx.pr==0x8C089FD4){
            capture("first-startup-ninebytes-exit");startup_ninebytes_pending=false;
        }
    };
    rt.register_target(0x8C089E34,observed_startup_ninebytes);
    struct RestoreStartupNinebytes{DCRuntime& rt;~RestoreStartupNinebytes(){
        rt.register_target(0x8C089E34,original_startup_ninebytes);startup_ninebytes_observer={};original_startup_ninebytes=nullptr;
    }}restore_startup_ninebytes{rt};
    original_startup_triplet=rt.targets.at(0x8C089E1E);
    require(original_startup_triplet!=nullptr,"Original startup triplet initializer absent");
    bool startup_triplet_captured=false,startup_triplet_pending=false;
    startup_triplet_observer=[&](bool after){
        if(!after&&!startup_triplet_captured&&ctx.pc==0x8C089E1E&&ctx.pr==0x8C089FD0){
            capture("first-startup-triplet-entry");startup_triplet_captured=true;startup_triplet_pending=true;
        }else if(after&&startup_triplet_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C089FD0&&ctx.pr==0x8C089FD0){
            capture("first-startup-triplet-exit");startup_triplet_pending=false;
        }
    };
    rt.register_target(0x8C089E1E,observed_startup_triplet);
    struct RestoreStartupTriplet{DCRuntime& rt;~RestoreStartupTriplet(){
        rt.register_target(0x8C089E1E,original_startup_triplet);startup_triplet_observer={};original_startup_triplet=nullptr;
    }}restore_startup_triplet{rt};
    original_startup_fourrecords=rt.targets.at(0x8C089DC0);
    require(original_startup_fourrecords!=nullptr,"Original startup four-record initializer absent");
    bool startup_fourrecords_captured=false,startup_fourrecords_pending=false;
    startup_fourrecords_observer=[&](bool after){
        if(!after&&!startup_fourrecords_captured&&ctx.pc==0x8C089DC0&&ctx.pr==0x8C089FCC){
            capture("first-startup-fourrecords-entry");startup_fourrecords_captured=true;startup_fourrecords_pending=true;
        }else if(after&&startup_fourrecords_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C089FCC&&ctx.pr==0x8C089FCC){
            capture("first-startup-fourrecords-exit");startup_fourrecords_pending=false;
        }
    };
    rt.register_target(0x8C089DC0,observed_startup_fourrecords);
    struct RestoreStartupFourRecords{DCRuntime& rt;~RestoreStartupFourRecords(){
        rt.register_target(0x8C089DC0,original_startup_fourrecords);startup_fourrecords_observer={};original_startup_fourrecords=nullptr;
    }}restore_startup_fourrecords{rt};
    original_startup_sixrecords=rt.targets.at(0x8C0A3724);
    require(original_startup_sixrecords!=nullptr,"Original startup six-record initializer absent");
    bool startup_sixrecords_captured=false,startup_sixrecords_pending=false;
    startup_sixrecords_observer=[&](bool after){
        if(!after&&!startup_sixrecords_captured&&ctx.pc==0x8C0A3724&&ctx.pr==0x8C089FC8){
            capture("first-startup-sixrecords-entry");startup_sixrecords_captured=true;startup_sixrecords_pending=true;
        }else if(after&&startup_sixrecords_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C089FC8&&ctx.pr==0x8C089FC8){
            capture("first-startup-sixrecords-exit");startup_sixrecords_pending=false;
        }
    };
    rt.register_target(0x8C0A3724,observed_startup_sixrecords);
    struct RestoreStartupSixRecords{DCRuntime& rt;~RestoreStartupSixRecords(){
        rt.register_target(0x8C0A3724,original_startup_sixrecords);startup_sixrecords_observer={};original_startup_sixrecords=nullptr;
    }}restore_startup_sixrecords{rt};
    original_startup_pair2=rt.targets.at(0x8C0A36F2);
    require(original_startup_pair2!=nullptr,"Original startup second pair initializer absent");
    bool startup_pair2_captured=false,startup_pair2_pending=false;
    startup_pair2_observer=[&](bool after){
        if(!after&&!startup_pair2_captured&&ctx.pc==0x8C0A36F2&&ctx.pr==0x8C089FC2){
            capture("first-startup-pair2-entry");startup_pair2_captured=true;startup_pair2_pending=true;
        }else if(after&&startup_pair2_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C089FC2&&ctx.pr==0x8C089FC2){
            capture("first-startup-pair2-exit");startup_pair2_pending=false;
        }
    };
    rt.register_target(0x8C0A36F2,observed_startup_pair2);
    struct RestoreStartupPair2{DCRuntime& rt;~RestoreStartupPair2(){
        rt.register_target(0x8C0A36F2,original_startup_pair2);startup_pair2_observer={};original_startup_pair2=nullptr;
    }}restore_startup_pair2{rt};
    original_startup_pair=rt.targets.at(0x8C0A36C0);
    require(original_startup_pair!=nullptr,"Original startup pair initializer absent");
    bool startup_pair_captured=false,startup_pair_pending=false;
    startup_pair_observer=[&](bool after){
        if(!after&&!startup_pair_captured&&ctx.pc==0x8C0A36C0&&ctx.pr==0x8C089FBC){
            capture("first-startup-pair-entry");startup_pair_captured=true;startup_pair_pending=true;
        }else if(after&&startup_pair_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C089FBC&&ctx.pr==0x8C089FBC){
            capture("first-startup-pair-exit");startup_pair_pending=false;
        }
    };
    rt.register_target(0x8C0A36C0,observed_startup_pair);
    struct RestoreStartupPair{DCRuntime& rt;~RestoreStartupPair(){
        rt.register_target(0x8C0A36C0,original_startup_pair);startup_pair_observer={};original_startup_pair=nullptr;
    }}restore_startup_pair{rt};
    original_startup_record=rt.targets.at(0x8C089F20);
    require(original_startup_record!=nullptr,"Original startup record initializer absent");
    bool startup_record_captured=false,startup_record_pending=false;
    startup_record_observer=[&](bool after){
        if(!after&&!startup_record_captured&&ctx.pc==0x8C089F20&&ctx.pr==0x8C04E338){
            capture("first-startup-record-entry");startup_record_captured=true;startup_record_pending=true;
        }else if(after&&startup_record_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C04E338&&ctx.pr==0x8C04E338){
            capture("first-startup-record-exit");startup_record_pending=false;
        }
    };
    rt.register_target(0x8C089F20,observed_startup_record);
    struct RestoreStartupRecord{DCRuntime& rt;~RestoreStartupRecord(){
        rt.register_target(0x8C089F20,original_startup_record);startup_record_observer={};original_startup_record=nullptr;
    }}restore_startup_record{rt};
    original_startup_vector=rt.targets.at(0x8C0547EA);
    require(original_startup_vector!=nullptr,"Original startup vector initializer absent");
    bool startup_vector_captured=false,startup_vector_pending=false;
    startup_vector_observer=[&](bool after){
        if(!after&&!startup_vector_captured&&ctx.pc==0x8C0547EA&&ctx.pr==0x8C04E626){
            capture("first-startup-vector-entry");startup_vector_captured=true;startup_vector_pending=true;
        }else if(after&&startup_vector_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C04E626&&ctx.pr==0x8C04E626){
            capture("first-startup-vector-exit");startup_vector_pending=false;
        }
    };
    rt.register_target(0x8C0547EA,observed_startup_vector);
    struct RestoreStartupVector{DCRuntime& rt;~RestoreStartupVector(){
        rt.register_target(0x8C0547EA,original_startup_vector);startup_vector_observer={};original_startup_vector=nullptr;
    }}restore_startup_vector{rt};
    original_startup_state=rt.targets.at(0x8C04E60E);
    require(original_startup_state!=nullptr,"Original startup state initializer absent");
    bool startup_state_captured=false,startup_state_pending=false;
    startup_state_observer=[&](bool after){
        if(!after&&!startup_state_captured&&ctx.pc==0x8C04E60E&&ctx.pr==0x8C04E332){
            capture("first-startup-state-entry");startup_state_captured=true;startup_state_pending=true;
        }else if(after&&startup_state_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C04E332&&ctx.pr==0x8C04E332){
            capture("first-startup-state-exit");startup_state_pending=false;
        }
    };
    rt.register_target(0x8C04E60E,observed_startup_state);
    struct RestoreStartupState{DCRuntime& rt;~RestoreStartupState(){
        rt.register_target(0x8C04E60E,original_startup_state);startup_state_observer={};original_startup_state=nullptr;
    }}restore_startup_state{rt};
    original_startup_global=rt.targets.at(0x8C04AE40);
    require(original_startup_global!=nullptr,"Original startup global initializer absent");
    bool startup_global_captured=false,startup_global_pending=false;
    startup_global_observer=[&](bool after){
        if(!after&&!startup_global_captured&&ctx.pc==0x8C04AE40&&ctx.pr==0x8C091A92){
            capture("first-startup-global-entry");startup_global_captured=true;startup_global_pending=true;
        }else if(after&&startup_global_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C091A92&&ctx.pr==0x8C091A92){
            capture("first-startup-global-exit");startup_global_pending=false;
        }
    };
    rt.register_target(0x8C04AE40,observed_startup_global);
    struct RestoreStartupGlobal{DCRuntime& rt;~RestoreStartupGlobal(){
        rt.register_target(0x8C04AE40,original_startup_global);startup_global_observer={};original_startup_global=nullptr;
    }}restore_startup_global{rt};
    original_startup_selector=rt.targets.at(0x8C0922E0);
    require(original_startup_selector!=nullptr,"Original startup selector absent");
    bool startup_selector_captured=false,startup_selector_pending=false;
    startup_selector_observer=[&](bool after){
        if(!after&&!startup_selector_captured&&ctx.pc==0x8C0922E0&&ctx.pr==0x8C091A8C){
            capture("first-startup-selector-entry");startup_selector_captured=true;startup_selector_pending=true;
        }else if(after&&startup_selector_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C091A8C&&ctx.pr==0x8C091A8C){
            capture("first-startup-selector-exit");startup_selector_pending=false;
        }
    };
    rt.register_target(0x8C0922E0,observed_startup_selector);
    struct RestoreStartupSelector{DCRuntime& rt;~RestoreStartupSelector(){
        rt.register_target(0x8C0922E0,original_startup_selector);startup_selector_observer={};original_startup_selector=nullptr;
    }}restore_startup_selector{rt};
    original_startup_buffer=rt.targets.at(0x8C091A20);
    require(original_startup_buffer!=nullptr,"Original startup buffer initializer absent");
    bool startup_buffer_captured=false,startup_buffer_pending=false;
    startup_buffer_observer=[&](bool after){
        if(!after&&!startup_buffer_captured&&ctx.pc==0x8C091A20&&ctx.pr==0x8C04E32A){
            capture("first-startup-buffer-entry");startup_buffer_captured=true;startup_buffer_pending=true;
        }else if(after&&startup_buffer_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C04E32A&&ctx.pr==0x8C04E32A){
            capture("first-startup-buffer-exit");startup_buffer_pending=false;
        }
    };
    rt.register_target(0x8C091A20,observed_startup_buffer);
    struct RestoreStartupBuffer{DCRuntime& rt;~RestoreStartupBuffer(){
        rt.register_target(0x8C091A20,original_startup_buffer);startup_buffer_observer={};original_startup_buffer=nullptr;
    }}restore_startup_buffer{rt};
    original_startup_status=rt.targets.at(0x8C08BCEA);
    require(original_startup_status!=nullptr,"Original startup status initializer absent");
    bool startup_status_captured=false,startup_status_pending=false;
    startup_status_observer=[&](bool after){
        if(!after&&!startup_status_captured&&ctx.pc==0x8C08BCEA&&ctx.pr==0x8C04E324){
            capture("first-startup-status-entry");startup_status_captured=true;startup_status_pending=true;
        }else if(after&&startup_status_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C04E324&&ctx.pr==0x8C04E324){
            capture("first-startup-status-exit");startup_status_pending=false;
        }
    };
    rt.register_target(0x8C08BCEA,observed_startup_status);
    struct RestoreStartupStatus{DCRuntime& rt;~RestoreStartupStatus(){
        rt.register_target(0x8C08BCEA,original_startup_status);startup_status_observer={};original_startup_status=nullptr;
    }}restore_startup_status{rt};
    original_startup_flags=rt.targets.at(0x8C08C218);
    require(original_startup_flags!=nullptr,"Original startup flag initializer absent");
    bool startup_flags_captured=false,startup_flags_pending=false;
    startup_flags_observer=[&](bool after){
        if(!after&&!startup_flags_captured&&ctx.pc==0x8C08C218&&ctx.pr==0x8C04E31E){
            capture("first-startup-flags-entry");startup_flags_captured=true;startup_flags_pending=true;
        }else if(after&&startup_flags_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C04E31E&&ctx.pr==0x8C04E31E){
            capture("first-startup-flags-exit");startup_flags_pending=false;
        }
    };
    rt.register_target(0x8C08C218,observed_startup_flags);
    struct RestoreStartupFlags{DCRuntime& rt;~RestoreStartupFlags(){
        rt.register_target(0x8C08C218,original_startup_flags);startup_flags_observer={};original_startup_flags=nullptr;
    }}restore_startup_flags{rt};
    original_startup_clear=rt.targets.at(0x8C026BE0);
    require(original_startup_clear!=nullptr,"Original startup clear initializer absent");
    bool startup_clear_captured=false,startup_clear_pending=false;
    startup_clear_observer=[&](bool after){
        if(!after&&!startup_clear_captured&&ctx.pc==0x8C026BE0&&ctx.pr==0x8C026ED0){
            capture("first-startup-clear-entry");startup_clear_captured=true;startup_clear_pending=true;
        }else if(after&&startup_clear_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C026ED0&&ctx.pr==0x8C026ED0){
            capture("first-startup-clear-exit");startup_clear_pending=false;
        }
    };
    rt.register_target(0x8C026BE0,observed_startup_clear);
    struct RestoreStartupClear{DCRuntime& rt;~RestoreStartupClear(){
        rt.register_target(0x8C026BE0,original_startup_clear);startup_clear_observer={};original_startup_clear=nullptr;
    }}restore_startup_clear{rt};
    original_startup_table=rt.targets.at(0x8C026E80);
    require(original_startup_table!=nullptr,"Original startup table initializer absent");
    bool startup_table_captured=false,startup_table_pending=false;
    startup_table_observer=[&](bool after){
        if(!after&&!startup_table_captured&&ctx.pc==0x8C026E80&&ctx.pr==0x8C04E318){
            capture("first-startup-table-entry");startup_table_captured=true;startup_table_pending=true;
        }else if(after&&startup_table_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C04E318&&ctx.pr==0x8C04E318){
            capture("first-startup-clear-parent-exit");startup_table_pending=false;
        }
    };
    rt.register_target(0x8C026E80,observed_startup_table);
    struct RestoreStartupTable{DCRuntime& rt;~RestoreStartupTable(){
        rt.register_target(0x8C026E80,original_startup_table);startup_table_observer={};original_startup_table=nullptr;
    }}restore_startup_table{rt};
    original_startup_byte=rt.targets.at(0x8C08BD1C);
    require(original_startup_byte!=nullptr,"Original startup byte writer absent");
    bool startup_byte_captured=false,startup_byte_pending=false;
    startup_byte_observer=[&](bool after){
        if(!after&&!startup_byte_captured&&ctx.pc==0x8C08BD1C&&ctx.pr==0x8C0513E6){
            capture("first-startup-byte-entry");startup_byte_captured=true;startup_byte_pending=true;
        }else if(after&&startup_byte_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C0513E6&&ctx.pr==0x8C0513E6){
            capture("first-startup-byte-exit");startup_byte_pending=false;
        }
    };
    rt.register_target(0x8C08BD1C,observed_startup_byte);
    struct RestoreStartupByte{DCRuntime& rt;~RestoreStartupByte(){
        rt.register_target(0x8C08BD1C,original_startup_byte);startup_byte_observer={};original_startup_byte=nullptr;
    }}restore_startup_byte{rt};
    original_command_pack=rt.targets.at(0x8C664258);
    require(original_command_pack!=nullptr,"Original command pack absent");
    bool command_pack_captured=false,command_pack_pending=false;
    command_pack_observer=[&](bool after){
        if(!after&&!command_pack_captured&&ctx.pc==0x8C664258&&ctx.pr==0x8C664248&&ctx.r[4]==1){
            capture("first-command-pack-entry");command_pack_captured=true;command_pack_pending=true;
        }else if(after&&command_pack_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C664248&&ctx.pr==0x8C664248){
            capture("first-command-pack-exit");command_pack_pending=false;
        }
    };
    rt.register_target(0x8C664258,observed_command_pack);
    struct RestoreCommandPack{DCRuntime& rt;~RestoreCommandPack(){
        rt.register_target(0x8C664258,original_command_pack);command_pack_observer={};original_command_pack=nullptr;
    }}restore_command_pack{rt};
    original_command_lock=rt.targets.at(0x8C664226);
    require(original_command_lock!=nullptr,"Original command lock absent");
    bool command_lock_captured=false;
    command_lock_observer=[&](){
        if(!command_lock_captured&&ctx.pc==0x8C664226&&ctx.pr==0x8C64A716&&ctx.r[4]==1){
            capture("first-command-lock-entry");command_lock_captured=true;
        }
    };
    rt.register_target(0x8C664226,observed_command_lock);
    struct RestoreCommandLock{DCRuntime& rt;~RestoreCommandLock(){
        rt.register_target(0x8C664226,original_command_lock);command_lock_observer={};original_command_lock=nullptr;
    }}restore_command_lock{rt};
    original_command_wrapper=rt.targets.at(0x8C64A708);
    require(original_command_wrapper!=nullptr,"Original command wrapper absent");
    bool command_wrapper_captured=false;
    command_wrapper_observer=[&](){
        if(!command_wrapper_captured&&ctx.pc==0x8C64A708&&ctx.pr==0x8C66484C&&ctx.r[4]==1){
            capture("first-command-wrapper-entry");command_wrapper_captured=true;
        }
    };
    rt.register_target(0x8C64A708,observed_command_wrapper);
    struct RestoreCommandWrapper{DCRuntime& rt;~RestoreCommandWrapper(){
        rt.register_target(0x8C64A708,original_command_wrapper);command_wrapper_observer={};original_command_wrapper=nullptr;
    }}restore_command_wrapper{rt};
    original_channel_command=rt.targets.at(0x8C6647BE);
    require(original_channel_command!=nullptr,"Original channel command callback absent");
    bool channel_command_captured=false;
    channel_command_observer=[&](){
        if(!channel_command_captured&&ctx.pc==0x8C6647BE&&ctx.pr==0x8C64B386&&ctx.r[4]==0){
            capture("first-channel-command-entry");channel_command_captured=true;
        }
    };
    rt.register_target(0x8C6647BE,observed_channel_command);
    struct RestoreChannelCommand{DCRuntime& rt;~RestoreChannelCommand(){
        rt.register_target(0x8C6647BE,original_channel_command);channel_command_observer={};original_channel_command=nullptr;
    }}restore_channel_command{rt};
    original_channel_handler=rt.targets.at(0x8C64B350);
    require(original_channel_handler!=nullptr,"Original channel handler absent");
    bool channel_handler_captured=false;
    channel_handler_observer=[&](){
        if(!channel_handler_captured&&ctx.pc==0x8C64B350&&ctx.pr==0x8C64EF5E&&ctx.r[4]==0){
            capture("first-channel-handler-entry");channel_handler_captured=true;
        }
    };
    rt.register_target(0x8C64B350,observed_channel_handler);
    struct RestoreChannelHandler{DCRuntime& rt;~RestoreChannelHandler(){
        rt.register_target(0x8C64B350,original_channel_handler);channel_handler_observer={};original_channel_handler=nullptr;
    }}restore_channel_handler{rt};
    original_channel_wrapper=rt.targets.at(0x8C64EB72);
    require(original_channel_wrapper!=nullptr,"Original channel wrapper absent");
    bool channel_wrapper_captured=false;
    channel_wrapper_observer=[&](){
        if(!channel_wrapper_captured&&ctx.pc==0x8C64EB72&&ctx.pr==0x8C64EF5E&&ctx.r[4]==0){
            capture("first-channel-wrapper-entry");channel_wrapper_captured=true;
        }
    };
    rt.register_target(0x8C64EB72,observed_channel_wrapper);
    struct RestoreChannelWrapper{DCRuntime& rt;~RestoreChannelWrapper(){
        rt.register_target(0x8C64EB72,original_channel_wrapper);channel_wrapper_observer={};original_channel_wrapper=nullptr;
    }}restore_channel_wrapper{rt};
    original_channel_init=rt.targets.at(0x8C64EF30);
    require(original_channel_init!=nullptr,"Original channel initializer absent");
    bool channel_init_captured=false;
    channel_init_observer=[&](){
        if(!channel_init_captured&&ctx.pc==0x8C64EF30&&ctx.pr==0x8C64B748&&ctx.r[4]==0){
            capture("first-channel-init-entry");channel_init_captured=true;
        }
    };
    rt.register_target(0x8C64EF30,observed_channel_init);
    struct RestoreChannelInit{DCRuntime& rt;~RestoreChannelInit(){
        rt.register_target(0x8C64EF30,original_channel_init);channel_init_observer={};original_channel_init=nullptr;
    }}restore_channel_init{rt};
    original_vmu_fields=rt.targets.at(0x8C64A8FC);
    require(original_vmu_fields!=nullptr,"Original VMU function-word decoder absent");
    bool vmu_fields_captured=false,vmu_fields_pending=false;
    vmu_fields_observer=[&](bool after){
        if(!after&&!vmu_fields_captured&&ctx.pc==0x8C64A8FC&&ctx.pr==0x8C66476A){
            capture("first-vmu-fields-entry");vmu_fields_captured=true;vmu_fields_pending=true;
        }else if(after&&vmu_fields_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C66476A&&ctx.pr==0x8C66476A){
            capture("first-vmu-fields-exit");vmu_fields_pending=false;
        }
    };
    rt.register_target(0x8C64A8FC,observed_vmu_fields);
    struct RestoreVMUFields{DCRuntime& rt;~RestoreVMUFields(){
        rt.register_target(0x8C64A8FC,original_vmu_fields);vmu_fields_observer={};original_vmu_fields=nullptr;
    }}restore_vmu_fields{rt};
    original_irq_word=rt.targets.at(0x8C664556);
    require(original_irq_word!=nullptr,"Original IRQ halfword accessor absent");
    bool irq_word_captured=false,irq_word_pending=false;
    irq_word_observer=[&](bool after){
        if(!after&&!irq_word_captured&&ctx.pc==0x8C664556&&ctx.pr==0x8C6646C4){
            capture("first-irq-word-entry");irq_word_captured=true;irq_word_pending=true;
        }else if(after&&irq_word_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C6646C4&&ctx.pr==0x8C6646C4){
            capture("first-irq-word-exit");irq_word_pending=false;
        }
    };
    rt.register_target(0x8C664556,observed_irq_word);
    struct RestoreIRQWord{DCRuntime& rt;~RestoreIRQWord(){
        rt.register_target(0x8C664556,original_irq_word);irq_word_observer={};original_irq_word=nullptr;
    }}restore_irq_word{rt};
    original_language_init=rt.targets.at(0x8C051354);
    original_startup_motion=rt.targets.at(0x8C09F5A0);
    require(original_language_init&&original_startup_motion,"Original language/motion initializer absent");
    bool language_captured=false,motion_captured=false,motion_pending=false;
    const auto capture_motion_return=[&](){
        if(motion_pending&&!rt.sh4_async_redirect&&ctx.pc==0x8C0513A2&&ctx.pr==0x8C0513A2){
            capture("first-startup-motion-exit");motion_pending=false;
        }
    };
    language_init_observer=[&](){
        if(!language_captured&&ctx.pc==0x8C051354&&ctx.pr==0x8C04E318){
            capture("first-language-init-entry");language_captured=true;
        }
        capture_motion_return();
    };
    startup_motion_observer=[&](bool after){
        if(!after&&!motion_captured&&ctx.pc==0x8C09F5A0&&ctx.pr==0x8C0513A2){
            capture("first-startup-motion-entry");motion_captured=true;motion_pending=true;
        }else if(after)capture_motion_return();
    };
    // Include original continuation bindings so an interrupt unwind remains observable.
    std::vector<std::uint32_t> language_observed_addresses;
    for(const auto& binding:rt.targets)if(binding.second==original_language_init)
        language_observed_addresses.push_back(binding.first);
    for(auto address:language_observed_addresses)rt.register_target(address,observed_language_init);
    rt.register_target(0x8C09F5A0,observed_startup_motion);
    struct RestoreLanguageMotion{DCRuntime& rt;std::vector<std::uint32_t>& addresses;~RestoreLanguageMotion(){
        for(auto address:addresses)rt.register_target(address,original_language_init);
        rt.register_target(0x8C09F5A0,original_startup_motion);
        language_init_observer={};startup_motion_observer={};original_language_init=nullptr;original_startup_motion=nullptr;
    }}restore_language_motion{rt,language_observed_addresses};
    original_init_call=rt.targets.at(0x8C04E310);
    require(original_init_call!=nullptr,"Original initialization caller absent");
    bool init_call_captured=false;
    init_call_observer=[&](){
        if(!init_call_captured&&ctx.pc==0x8C04E310&&ctx.pr==0x8C055942){
            capture("first-init-call-entry");init_call_captured=true;
        }
    };
    rt.register_target(0x8C04E310,observed_init_call);
    struct RestoreInitCall{DCRuntime& rt;~RestoreInitCall(){
        rt.register_target(0x8C04E310,original_init_call);init_call_observer={};original_init_call=nullptr;
    }}restore_init_call{rt};
    original_mode_dispatch=rt.targets.at(0x8C0558C0);
    require(original_mode_dispatch!=nullptr,"Original mode dispatcher absent");
    bool mode_dispatch_captured=false;
    mode_dispatch_observer=[&](){
        if(!mode_dispatch_captured&&ctx.pc==0x8C0558C0&&ctx.pr==0x8C0556E0){
            capture("first-mode-prefix-entry");mode_dispatch_captured=true;
        }
    };
    // Disc/IRQ yields can resume any original mode block, bypassing its entry binding.
    std::vector<std::uint32_t> mode_dispatch_addresses;
    for(const auto&binding:rt.targets)if(binding.second==original_mode_dispatch)mode_dispatch_addresses.push_back(binding.first);
    for(auto address:mode_dispatch_addresses)rt.register_target(address,observed_mode_dispatch);
    struct RestoreModeDispatch{DCRuntime& rt;std::vector<std::uint32_t>&addresses;~RestoreModeDispatch(){
        for(auto address:addresses)rt.register_target(address,original_mode_dispatch);mode_dispatch_observer={};original_mode_dispatch=nullptr;
    }}restore_mode_dispatch{rt,mode_dispatch_addresses};
    original_main_guard=rt.targets.at(0x8C010B58);
    require(original_main_guard!=nullptr,"Original signed main-loop guard absent");
    bool main_guard_captured=false,main_guard_pending=false;
    main_guard_observer=[&](bool after){
        if(!after&&!main_guard_captured&&ctx.pc==0x8C010B58&&ctx.pr==0x8C0556D8){
            capture("first-main-guard-entry");main_guard_captured=true;main_guard_pending=true;
        }else if(after&&main_guard_pending){
            capture("first-main-guard-exit");main_guard_pending=false;
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-main-guard-exit-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture original signed main guard return trace");
        }
    };
    rt.register_target(0x8C010B58,observed_main_guard);
    struct RestoreMainGuard{DCRuntime& rt;~RestoreMainGuard(){
        rt.register_target(0x8C010B58,original_main_guard);main_guard_observer={};original_main_guard=nullptr;
    }}restore_main_guard{rt};
    original_record32=rt.targets.at(0x8C65FEDE);
    require(original_record32!=nullptr,"Original thirty-two-record task scan absent");
    bool record32_captured=false,record32_pending=false;
    record32_observer=[&](bool after){
        if(!after&&!record32_captured&&ctx.pc==0x8C65FEDE&&ctx.pr==0x8C64847C){
            capture("first-record32-entry");record32_captured=true;record32_pending=true;
        }else if(after&&record32_pending){
            capture("first-record32-exit");record32_pending=false;
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-record32-exit-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture thirty-two-record scan return trace");
        }
    };
    rt.register_target(0x8C65FEDE,observed_record32);
    struct RestoreRecord32{DCRuntime& rt;~RestoreRecord32(){
        rt.register_target(0x8C65FEDE,original_record32);record32_observer={};original_record32=nullptr;
    }}restore_record32{rt};
    original_record16=rt.targets.at(0x8C10E4CC);
    require(original_record16!=nullptr,"Original sixteen-record task scan absent");
    bool record16_captured=false,record16_pending=false;
    record16_observer=[&](bool after){
        if(!after&&!record16_captured&&ctx.pc==0x8C10E4CC&&ctx.pr==0x8C648468){
            capture("first-record16-entry");record16_captured=true;record16_pending=true;
        }else if(after&&record16_pending){
            capture("first-record16-exit");record16_pending=false;
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-record16-exit-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture sixteen-record scan return trace");
        }
    };
    rt.register_target(0x8C10E4CC,observed_record16);
    struct RestoreRecord16{DCRuntime& rt;~RestoreRecord16(){
        rt.register_target(0x8C10E4CC,original_record16);record16_observer={};original_record16=nullptr;
    }}restore_record16{rt};
    struct RestoreObserver{DCRuntime& rt;~RestoreObserver(){rt.register_target(0x8C663550,original_sdk_request);rt.register_target(0x8C66A810,original_sound_completion);sdk_request_observer={};sound_completion_observer={};rt.register_target(0x8C6607D2,original_module_init);module_init_observer={};rt.register_target(0x8C65F328,original_nested_clear);nested_clear_observer={};rt.register_target(0x8C65EACE,original_critical_leave);critical_leave_observer={};rt.register_target(0x8C09F6D2,original_file_pool);file_pool_observer={};rt.register_target(0x8C60549E,original_startup_dispatch);startup_dispatch_observer={};rt.register_target(0x8C06DD60,original_timer_init);timer_init_observer={};original_timer_init=nullptr;original_startup_dispatch=nullptr;original_file_pool=nullptr;original_critical_leave=nullptr;original_nested_clear=nullptr;original_module_init=nullptr;original_sdk_request=nullptr;original_sound_completion=nullptr;}}restore_observer{rt};
#ifdef RECOVERY_FRAME_SPRITES
    const std::array<std::uint32_t,6> sprite_addresses={0x8C090E40,0x8C60CA0C,0x8C642BF4,0x8C60A1F0,0x8C04562E,0x8C08C25E};
    const std::array<std::uint32_t,6> sprite_returns={0x8C090F8E,0x8C090E5A,0x8C090E84,0x8C642C6A,0x8C0561E8,0x8C0561EE};
    const std::array<const char*,6> sprite_entries={"first-sprites-entry","first-sprite-texture-entry","first-sprite-entry","first-sprite-material-entry","first-frame-tail-draw-entry","first-frame-counter-entry"};
    const std::array<const char*,6> sprite_exits={"first-sprites-exit","first-sprite-texture-exit",nullptr,"first-sprite-material-exit","first-frame-tail-draw-exit","first-frame-counter-exit"};
    std::array<bool,12> sprite_seen{};bool mode_exit_seen=false;
    sprite_observer=[&](unsigned which,bool after){auto index=2*which+unsigned(after);const char*label=after?sprite_exits[which]:sprite_entries[which];if((which>=4&&!sprite_seen[1])||!label||sprite_seen[index]||ctx.pr!=sprite_returns[which]||ctx.pc!=(after?sprite_returns[which]:sprite_addresses[which])||(after&&rt.sh4_async_redirect))return;capture(label);sprite_seen[index]=true;};
#ifdef RECOVERY_MODE_PROGRESS
    std::ofstream mode_progress(recovery_frame_directory+"/mode-progress.txt");
    require(bool(mode_progress),"Cannot open mode progress log");
#endif
    frame_mode_exit_observer=[&]{
        if(!sprite_seen[1]||rt.sh4_async_redirect||ctx.pc!=0x8C0556E0||ctx.pr!=0x8C0556E0)return;
        if(!mode_exit_seen){capture("first-frame-mode-exit");mode_exit_seen=true;}
#ifdef RECOVERY_MODE_PROGRESS
        // Direct reads of known main RAM only: no MMIO accesses or guest ticks.
        auto word=[&](unsigned offset){std::uint32_t value=0;for(unsigned i=0;i<4;++i)value|=std::uint32_t(rt.main_ram[offset+i])<<(8*i);return value;};
        mode_progress<<"cycles="<<std::dec<<rt.device_clock_sh4_cycles
            <<" mode="<<word(0x79D834)<<" counter="<<word(0x79D83C)
            <<" adjustment="<<word(0x7A4D00)<<" parent="<<word(0x756C68)
            <<" ring="<<word(0x8A7328)<<" wait_counter="<<word(0x8A733C)
            <<" wait_flag="<<word(0x8A7340)<<" pc="<<std::hex<<ctx.pc
            <<" pr="<<ctx.pr<<" sr="<<ctx.sr<<" spc="<<ctx.spc<<std::dec<<std::endl;
#endif
    };
    const std::array<RecompiledFunction,6> sprite_wrappers={observed_sprite_call<0>,observed_sprite_call<1>,observed_sprite_call<2>,observed_sprite_call<3>,observed_sprite_call<4>,observed_sprite_call<5>};
    for(unsigned i=0;i<6;++i){original_sprite_calls[i]=rt.targets.at(sprite_addresses[i]);rt.register_target(sprite_addresses[i],sprite_wrappers[i]);}
    struct RestoreSprites{DCRuntime&rt;const std::array<std::uint32_t,6>&addresses;~RestoreSprites(){for(unsigned i=0;i<6;++i)rt.register_target(addresses[i],original_sprite_calls[i]);sprite_observer={};frame_mode_exit_observer={};original_sprite_calls={};}}restore_sprites{rt,sprite_addresses};
#endif
#ifdef RECOVERY_QUAD_PACKETS
    const std::array<std::uint32_t,4> packet_addresses={0x8C609C7C,0x8C642574,0x8C6424A0,0x8C60A6E4};
    const std::array<std::uint32_t,4> packet_returns={0x8C641FB8,0x8C642958,0x8C64260E,0x8C642960};
    const std::array<const char*,4> packet_entries={"first-quad-command-entry","first-quad-strip-entry","first-strip-writer-entry","first-quad-finalizer-entry"};
    const std::array<const char*,4> packet_exits={"first-quad-command-exit","first-quad-strip-exit","first-strip-writer-exit",nullptr};
    std::array<bool,8> packet_seen{};
    packet_observer=[&](unsigned which,bool after){
        auto index=2*which+unsigned(after);const char*label=after?packet_exits[which]:packet_entries[which];
        if(!label||packet_seen[index]||ctx.pr!=packet_returns[which]||ctx.pc!=(after?packet_returns[which]:packet_addresses[which])||(after&&rt.sh4_async_redirect))return;
        capture(label);packet_seen[index]=true;
    };
    const std::array<RecompiledFunction,4> packet_wrappers={observed_packet_call<0>,observed_packet_call<1>,observed_packet_call<2>,observed_packet_call<3>};
    for(unsigned i=0;i<4;++i){original_packet_calls[i]=rt.targets.at(packet_addresses[i]);rt.register_target(packet_addresses[i],packet_wrappers[i]);}
    struct RestorePackets{DCRuntime&rt;const std::array<std::uint32_t,4>&addresses;~RestorePackets(){for(unsigned i=0;i<4;++i)rt.register_target(addresses[i],original_packet_calls[i]);packet_observer={};original_packet_calls={};}}restore_packets{rt,packet_addresses};
#endif
#ifdef RECOVERY_FRAME_QUAD
    const std::array<std::uint32_t,4> quad_addresses={0x8C09D14C,0x8C642908,0x8C641F98,0x8C60A748};
    const std::array<std::uint32_t,4> quad_returns={0x8C090F8A,0x8C09D1CA,0x8C64293E,0x8C641FAC};
    const std::array<const char*,4> quad_entries={"first-frame-quad-entry","first-quad-draw-entry","first-quad-state-entry","first-quad-header-entry"};
    const std::array<const char*,4> quad_exits={"first-quad-exit","first-quad-draw-exit","first-quad-state-exit","first-quad-header-exit"};
    std::array<bool,8> quad_seen{};
    quad_observer=[&](unsigned which,bool after){
        auto index=2*which+unsigned(after);if(quad_seen[index]||ctx.pr!=quad_returns[which]||ctx.pc!=(after?quad_returns[which]:quad_addresses[which])||(after&&rt.sh4_async_redirect))return;
        capture(after?quad_exits[which]:quad_entries[which]);quad_seen[index]=true;
    };
    const std::array<RecompiledFunction,4> quad_wrappers={observed_quad_call<0>,observed_quad_call<1>,observed_quad_call<2>,observed_quad_call<3>};
    for(unsigned i=0;i<4;++i){original_quad_calls[i]=rt.targets.at(quad_addresses[i]);rt.register_target(quad_addresses[i],quad_wrappers[i]);}
    struct RestoreQuad{DCRuntime&rt;const std::array<std::uint32_t,4>&addresses;~RestoreQuad(){for(unsigned i=0;i<4;++i)rt.register_target(addresses[i],original_quad_calls[i]);quad_observer={};original_quad_calls={};}}restore_quad{rt,quad_addresses};
#endif
#ifdef RECOVERY_FRAME_DISPATCH
    const std::array<std::uint32_t,6> frame_addresses={0x8C090F2A,0x8C052700,0x8C052944,0x8C052766,0x8C09A544,0x8C09D0D0};
    const std::array<std::uint32_t,6> frame_returns={0x8C055A02,0x8C090F34,0x8C090F3A,0x8C090F40,0x8C090F56,0x8C090F70};
    const std::array<const char*,6> frame_entries={"first-frame-entry","first-frame-flag-entry","first-frame-input-entry","first-frame-status-entry","first-frame-common-entry","first-frame-adjust-entry"};
    const std::array<const char*,6> frame_exits={"first-frame-exit","first-frame-flag-exit",nullptr,"first-frame-status-exit",nullptr,"first-frame-adjust-exit"};
    std::array<bool,12> frame_seen{};
    frame_observer=[&](unsigned which,bool after){
        auto index=2*which+unsigned(after);const char*label=after?frame_exits[which]:frame_entries[which];
        if(!label||frame_seen[index]||ctx.pr!=frame_returns[which]||ctx.pc!=(after?frame_returns[which]:frame_addresses[which])||(after&&rt.sh4_async_redirect))return;
        capture(label);frame_seen[index]=true;
    };
    const std::array<RecompiledFunction,6> frame_wrappers={observed_frame_call<0>,observed_frame_call<1>,observed_frame_call<2>,observed_frame_call<3>,observed_frame_call<4>,observed_frame_call<5>};
    for(unsigned i=0;i<6;++i){original_frame_calls[i]=rt.targets.at(frame_addresses[i]);rt.register_target(frame_addresses[i],frame_wrappers[i]);}
    struct RestoreFrame{DCRuntime&rt;const std::array<std::uint32_t,6>&addresses;~RestoreFrame(){for(unsigned i=0;i<6;++i)rt.register_target(addresses[i],original_frame_calls[i]);frame_observer={};original_frame_calls={};}}restore_frame{rt,frame_addresses};
#endif
#ifdef RECOVERY_GLOBAL_WORD_COPY
    original_global_word_copy=rt.targets.at(0x8C045618);original_global_setup_tail=rt.targets.at(0x8C090EB0);original_global_record_tail=rt.targets.at(0x8C089F20);
    std::array<bool,4> global_word_seen{};
    global_word_observer=[&](unsigned phase){
        if(global_word_seen[phase])return;
        if(phase==0&&ctx.pc==0x8C045618&&ctx.pr==0x8C090F04){capture("first-global-word-copy-entry");global_word_seen[phase]=true;}
        if(phase==1&&!rt.sh4_async_redirect&&ctx.pc==0x8C090F04&&ctx.pr==0x8C090F04){capture("first-global-word-copy-exit");global_word_seen[phase]=true;}
        if(phase==2&&!rt.sh4_async_redirect&&ctx.pc==0x8C090F12&&ctx.pr==0x8C090F12){capture("first-global-setup-exit");global_word_seen[phase]=true;}
        if(phase==3&&ctx.pc==0x8C089F20&&ctx.pr==0x8C0559F8){capture("first-global-record-entry");global_word_seen[phase]=true;}
    };
    // Observe every original continuation binding because disc/IRQ yields may unwind the host frame.
    std::vector<std::uint32_t> global_setup_addresses;
    for(const auto&binding:rt.targets)if(binding.second==original_global_setup_tail)global_setup_addresses.push_back(binding.first);
    for(auto address:global_setup_addresses)rt.register_target(address,observed_global_setup_tail);
    rt.register_target(0x8C045618,observed_global_word_copy);rt.register_target(0x8C089F20,observed_global_record_tail);
    struct RestoreGlobalWord{DCRuntime&rt;std::vector<std::uint32_t>&addresses;~RestoreGlobalWord(){rt.register_target(0x8C045618,original_global_word_copy);rt.register_target(0x8C089F20,original_global_record_tail);for(auto address:addresses)rt.register_target(address,original_global_setup_tail);global_word_observer={};original_global_word_copy=nullptr;original_global_setup_tail=nullptr;original_global_record_tail=nullptr;}}restore_global_word{rt,global_setup_addresses};
#endif
#ifdef RECOVERY_CONTINUATION_PROGRESS
    StartupContinuationObserver progress(rt,ctx,recovery_frame_directory+"/continuation-progress.txt");
#endif
#ifdef RECOVERY_MODE_LOADER
    const std::array<std::uint32_t,3> loader_addresses={0x8C10D460,0x8C1096CA,0x8C609950};
    const std::array<std::uint32_t,3> loader_returns={0x8C055C60,0x8C10D46A,0x8C10D470};
    const std::array<const char*,3> loader_entries={"first-mode-loader-entry","first-loader-owner-entry","first-loader-service-entry"};
    std::array<bool,3> loader_seen{};bool owner_return_seen=false;
    loader_observer=[&](unsigned which,bool after){
        if(!after&&!loader_seen[which]&&ctx.pc==loader_addresses[which]&&ctx.pr==loader_returns[which]){capture(loader_entries[which]);loader_seen[which]=true;}
        if(which==1&&after&&loader_seen[1]&&!owner_return_seen&&!rt.sh4_async_redirect&&ctx.pc==loader_returns[1]&&ctx.pr==loader_returns[1]){capture("first-loader-owner-exit");owner_return_seen=true;}
    };
    const std::array<RecompiledFunction,3> loader_wrappers={observed_loader_call<0>,observed_loader_call<1>,observed_loader_call<2>};
    for(unsigned i=0;i<3;++i){original_loader_calls[i]=rt.targets.at(loader_addresses[i]);rt.register_target(loader_addresses[i],loader_wrappers[i]);}
    struct RestoreLoader{DCRuntime&rt;const std::array<std::uint32_t,3>&addresses;~RestoreLoader(){for(unsigned i=0;i<3;++i)rt.register_target(addresses[i],original_loader_calls[i]);loader_observer={};original_loader_calls={};}}restore_loader{rt,loader_addresses};
#endif
#ifdef RECOVERY_FILE_TRANSFER
    original_file_calls[0]=rt.targets.at(0x8C09F660);original_file_calls[1]=rt.targets.at(0x8C09E8A0);
    std::array<bool,2> file_entries{};bool movie_exit_seen=false;
    file_transfer_observer=[&](unsigned which,bool after){
#ifdef RECOVERY_MOVIE_ENTRY
        if(!after&&which==0&&movie_active&&(ctx.r[5]&0x1FFFFFFFu)==0x0CEB0000u)throw std::runtime_error("MOVIE reload requires quiescent retirement");
#endif

        if(!after&&!file_entries[which]&&ctx.pr==0x8C10D492&&ctx.pc==(which?0x8C09E8A0:0x8C09F660)){capture(which?"first-movie-provider-entry":"first-file-transfer-entry");file_entries[which]=true;}
        if(which==1&&after&&file_entries[1]&&!movie_exit_seen&&!rt.sh4_async_redirect&&ctx.pc==0x8C10D492&&ctx.pr==0x8C10D492){capture("first-movie-provider-exit");movie_exit_seen=true;
#ifdef RECOVERY_MOVIE_ENTRY
            throw MovieActivationNeeded{}; // Preserve the actual return context; activate only after unwinding.
#endif
}
    };
    std::vector<std::uint32_t> file_provider_aliases;for(const auto&binding:rt.targets)if(binding.second==original_file_calls[1])file_provider_aliases.push_back(binding.first);
    rt.register_target(0x8C09F660,observed_file_transfer_call<0>);for(auto address:file_provider_aliases)rt.register_target(address,observed_file_transfer_call<1>);
    struct RestoreFileTransfer{DCRuntime&rt;const std::vector<std::uint32_t>&aliases;~RestoreFileTransfer(){rt.register_target(0x8C09F660,original_file_calls[0]);for(auto address:aliases)rt.register_target(address,original_file_calls[1]);file_transfer_observer={};original_file_calls={};}}restore_file_transfer{rt,file_provider_aliases};
#endif
#ifdef RECOVERY_MOVIE_ENTRY
    bool movie_entry_seen=false,movie_resident_seen=false;
    movie_entry_observer=[&](unsigned which){
        if(which==0&&!movie_entry_seen&&ctx.pc==0x8CEB0000&&ctx.pr==0x8C10D4A2){capture("first-movie-aot-entry");movie_entry_seen=true;}
        if(which==1&&movie_entry_seen&&!movie_resident_seen&&ctx.pc==0x8C608F74&&ctx.pr==0x8CEB0090){capture("first-movie-resident-entry");movie_resident_seen=true;}
    };
    original_movie_first=rt.targets.at(0x8C608F74);rt.register_target(0x8C608F74,observed_movie_first);
    struct RestoreMovieObserver{DCRuntime&rt;~RestoreMovieObserver(){rt.register_target(0x8C608F74,original_movie_first);movie_entry_observer={};original_movie_first=nullptr;}}restore_movie_observer{rt};
#endif
#ifdef RECOVERY_MOVIE_CALLBACK
    bool movie_callback_seen=false,movie_work_seen=false;
    movie_callback_observer=[&](unsigned which){
        if(which==0&&!movie_callback_seen&&ctx.pc==0x8CEB06F4&&ctx.pr==0x8C649878){capture("first-movie-callback-entry");movie_callback_seen=true;}
        if(which==1&&movie_callback_seen&&!movie_work_seen&&ctx.pc==0x8C648422&&ctx.pr==0x8CEB06FC){capture("first-movie-work-entry");movie_work_seen=true;}
    };
    original_movie_work=rt.targets.at(0x8C648422);rt.register_target(0x8C648422,observed_movie_work);
    struct RestoreMovieCallbackObserver{DCRuntime&rt;~RestoreMovieCallbackObserver(){rt.register_target(0x8C648422,original_movie_work);movie_callback_observer={};original_movie_work=nullptr;}}restore_movie_callback_observer{rt};
#endif
#ifdef RECOVERY_MOVIE_SERVICE
    const std::array<std::uint32_t,3> movie_service_addresses={0x8C10D4C6,0x8C60C534,0x8C657AA8};
    const std::array<std::uint32_t,3> movie_service_returns={0x8CEB009C,0x8C10D4CE,0x8C60C572};
    const std::array<const char*,3> movie_service_labels={"first-movie-service-entry","first-movie-setup-entry","first-movie-setup-callee-entry"};
    std::array<bool,3> movie_services_seen{};
    movie_service_observer=[&](unsigned which){if(!movie_services_seen[which]&&ctx.pc==movie_service_addresses[which]&&ctx.pr==movie_service_returns[which]){capture(movie_service_labels[which]);movie_services_seen[which]=true;}};
    const std::array<RecompiledFunction,3> movie_service_wrappers={observed_movie_service<0>,observed_movie_service<1>,observed_movie_service<2>};
    for(unsigned i=0;i<3;++i){original_movie_services[i]=rt.targets.at(movie_service_addresses[i]);rt.register_target(movie_service_addresses[i],movie_service_wrappers[i]);}
    struct RestoreMovieServices{DCRuntime&rt;const std::array<std::uint32_t,3>&addresses;~RestoreMovieServices(){for(unsigned i=0;i<3;++i)rt.register_target(addresses[i],original_movie_services[i]);movie_service_observer={};original_movie_services={};}}restore_movie_services{rt,movie_service_addresses};
#endif
#ifdef RECOVERY_MOVIE_TAIL
    const std::array<std::uint32_t,2> movie_tail_addresses={0x8C644492,0x8C657AA8};
    const std::array<const char*,2> movie_tail_labels={"first-movie-tail-entry","first-movie-tail-callee-entry"};
    std::array<bool,2> movie_tails_seen{};
    movie_tail_observer=[&](unsigned which){if(!movie_tails_seen[which]&&ctx.pc==movie_tail_addresses[which]&&ctx.pr==0x8C10D4D4&&(which==0||ctx.r[4]==0x8C8947E4)){capture(movie_tail_labels[which]);movie_tails_seen[which]=true;}};
    const std::array<RecompiledFunction,2> movie_tail_wrappers={observed_movie_tail<0>,observed_movie_tail<1>};
    for(unsigned i=0;i<2;++i){original_movie_tails[i]=rt.targets.at(movie_tail_addresses[i]);rt.register_target(movie_tail_addresses[i],movie_tail_wrappers[i]);}
    struct RestoreMovieTails{DCRuntime&rt;const std::array<std::uint32_t,2>&addresses;~RestoreMovieTails(){for(unsigned i=0;i<2;++i)rt.register_target(addresses[i],original_movie_tails[i]);movie_tail_observer={};original_movie_tails={};}}restore_movie_tails{rt,movie_tail_addresses};
#endif
#ifdef RECOVERY_MOVIE_MODE
    const std::array<std::uint32_t,2> movie_mode_addresses={0x8C608700,0x8C6082D4};
    const std::array<std::uint32_t,2> movie_mode_returns={0x8C10D4EA,0x8C608776};
    const std::array<const char*,2> movie_mode_labels={"first-movie-mode-entry","first-movie-mode-callee-entry"};
    std::array<bool,2> movie_modes_seen{};
    movie_mode_observer=[&](unsigned which){if(!movie_modes_seen[which]&&ctx.pc==movie_mode_addresses[which]&&ctx.pr==movie_mode_returns[which]){capture(movie_mode_labels[which]);movie_modes_seen[which]=true;}};
    const std::array<RecompiledFunction,2> movie_mode_wrappers={observed_movie_mode<0>,observed_movie_mode<1>};
    for(unsigned i=0;i<2;++i){original_movie_modes[i]=rt.targets.at(movie_mode_addresses[i]);rt.register_target(movie_mode_addresses[i],movie_mode_wrappers[i]);}
    struct RestoreMovieModes{DCRuntime&rt;const std::array<std::uint32_t,2>&addresses;~RestoreMovieModes(){for(unsigned i=0;i<2;++i)rt.register_target(addresses[i],original_movie_modes[i]);movie_mode_observer={};original_movie_modes={};}}restore_movie_modes{rt,movie_mode_addresses};
#endif
#ifdef RECOVERY_MOVIE_CONFIG
    const std::array<std::uint32_t,3> movie_config_addresses={0x8CECFCE4,0x8CED5614,0x8C10EF3C};
    const std::array<std::uint32_t,3> movie_config_returns={0x8CEB00DA,0x8CECFCEE,0x8CED5630};
    const std::array<const char*,3> movie_config_labels={"first-movie-config-entry","first-movie-helper-entry","first-movie-clear-entry"};
    std::array<bool,3> movie_configs_seen{};
    movie_config_observer=[&](unsigned which){if(!movie_configs_seen[which]&&ctx.pc==movie_config_addresses[which]&&ctx.pr==movie_config_returns[which]){capture(movie_config_labels[which]);movie_configs_seen[which]=true;}};
    original_movie_clear=rt.targets.at(0x8C10EF3C);rt.register_target(0x8C10EF3C,observed_movie_clear);
    struct RestoreMovieConfig{DCRuntime&rt;~RestoreMovieConfig(){rt.register_target(0x8C10EF3C,original_movie_clear);movie_config_observer={};original_movie_clear=nullptr;}}restore_movie_config{rt};
#endif
#ifdef RECOVERY_MOVIE_COUNTER
    bool movie_counter_entered=false,movie_counter_returned=false;
    movie_counter_observer=[&](bool after){
        if(!after&&!movie_counter_entered&&ctx.pc==0x8CED09E0&&ctx.pr==0x8CECFCFA){capture("first-movie-counter-entry");movie_counter_entered=true;}
        if(after&&movie_counter_entered&&!movie_counter_returned&&ctx.pc==0x8CECFCFA&&ctx.pr==0x8CECFCFA&&!rt.sh4_async_redirect){capture("first-movie-counter-exit");movie_counter_returned=true;}
    };
    struct RestoreMovieCounter{~RestoreMovieCounter(){movie_counter_observer={};}}restore_movie_counter;
#endif
#ifdef RECOVERY_MOVIE_INIT
    const std::array<const char*,6> movie_init_labels={"first-movie-clear-tail-entry","first-movie-bulk-clear-entry","first-movie-bulk-clear-exit","first-movie-init-entry","first-movie-init-clear-entry","first-movie-init-clear-exit"};
    std::array<bool,6> movie_init_seen{};
    movie_init_observer=[&](unsigned which,bool after){unsigned index=6;
        if(which==0&&ctx.pc==0x8CED5712&&ctx.pr==0x8CECFD00)index=0;
        if(which==1&&ctx.pc==0x8CED5ADE&&ctx.pr==0x8CECFD00)index=3;
        if(which==2&&ctx.pr==0x8CED571E&&ctx.pc==(after?0x8CED571Eu:0x8C10EF3Cu))index=after?2:1;
        if(which==2&&ctx.pr==0x8CED5AEA&&ctx.pc==(after?0x8CED5AEAu:0x8C10EF3Cu))index=after?5:4;
        if(index<6&&!movie_init_seen[index]){capture(movie_init_labels[index]);movie_init_seen[index]=true;}
    };
    original_movie_bulk_clear=rt.targets.at(0x8C10EF3C);rt.register_target(0x8C10EF3C,observed_movie_bulk_clear);
    struct RestoreMovieInit{DCRuntime&rt;~RestoreMovieInit(){rt.register_target(0x8C10EF3C,original_movie_bulk_clear);movie_init_observer={};original_movie_bulk_clear=nullptr;}}restore_movie_init{rt};
#endif
#ifdef RECOVERY_MOVIE_RECORD_INIT
    bool movie_record_init_seen=false,movie_record_tail_seen=false;
    movie_record_init_observer=[&](unsigned which){
        if(which==0&&!movie_record_init_seen&&ctx.pc==0x8CED5A4E&&ctx.pr==0x8CED5AF0){capture("first-movie-record-init-entry");movie_record_init_seen=true;}
        if(which==1&&movie_record_init_seen&&!movie_record_tail_seen&&ctx.pc==0x8C658BA0&&ctx.pr==0x8CED5AF0){capture("first-movie-record-tail-entry");movie_record_tail_seen=true;}
    };
    original_movie_record_tail=rt.targets.at(0x8C658BA0);rt.register_target(0x8C658BA0,observed_movie_record_tail);
    struct RestoreMovieRecordInit{DCRuntime&rt;~RestoreMovieRecordInit(){rt.register_target(0x8C658BA0,original_movie_record_tail);movie_record_init_observer={};original_movie_record_tail=nullptr;}}restore_movie_record_init{rt};
#endif
#ifdef RECOVERY_MOVIE_GUARDED_CLEAR
    const std::array<std::uint32_t,3> movie_guard_addresses={0x8CED0A2C,0x8C10EF3C,0x8CECFD12};
    const std::array<const char*,3> movie_guard_labels={"first-movie-guard-entry","first-movie-guard-clear-entry","first-movie-guard-exit"};
    std::array<bool,3> movie_guard_seen{};
    movie_guard_observer=[&](unsigned which){if(!movie_guard_seen[which]&&ctx.pc==movie_guard_addresses[which]&&ctx.pr==0x8CECFD12&&!rt.sh4_async_redirect&&(which==0||movie_guard_seen[0])){capture(movie_guard_labels[which]);movie_guard_seen[which]=true;}};
    original_movie_guard_clear=rt.targets.at(0x8C10EF3C);rt.register_target(0x8C10EF3C,observed_movie_guard_clear);
    struct RestoreMovieGuard{DCRuntime&rt;~RestoreMovieGuard(){rt.register_target(0x8C10EF3C,original_movie_guard_clear);movie_guard_observer={};original_movie_guard_clear=nullptr;}}restore_movie_guard{rt};
#endif
#ifdef RECOVERY_MOVIE_RETURN_ZERO
    bool movie_zero_entered=false,movie_zero_returned=false;
    movie_zero_observer=[&](bool after){
        if(!after&&!movie_zero_entered&&ctx.pc==0x8CED0940&&ctx.pr==0x8CECFD18){capture("first-movie-zero-entry");movie_zero_entered=true;}
        if(after&&movie_zero_entered&&!movie_zero_returned&&ctx.pc==0x8CECFD18&&ctx.pr==0x8CECFD18&&!rt.sh4_async_redirect){capture("first-movie-zero-exit");movie_zero_returned=true;}
    };
    struct RestoreMovieZero{~RestoreMovieZero(){movie_zero_observer={};}}restore_movie_zero;
#endif
#ifdef RECOVERY_MOVIE_RESIDENT_INIT
    const std::array<std::uint32_t,3> resident_init_addresses={0x8C5FF860,0x8C10EF3C,0x8CECFD52};
    const std::array<std::uint32_t,3> resident_init_returns={0x8CECFD52,0x8C5FF892,0x8CECFD52};
    const std::array<const char*,3> resident_init_labels={"first-movie-resident-init-entry","first-movie-resident-clear-entry","first-movie-resident-init-exit"};
    std::array<bool,3> resident_init_seen{};
    movie_resident_init_observer=[&](unsigned which){if(!resident_init_seen[which]&&ctx.pc==resident_init_addresses[which]&&ctx.pr==resident_init_returns[which]&&!rt.sh4_async_redirect&&(which==0||resident_init_seen[0])){capture(resident_init_labels[which]);resident_init_seen[which]=true;}};
    real_movie_resident_init=rt.targets.at(0x8C5FF860);rt.register_target(0x8C5FF860,observed_movie_resident_init);
    real_movie_resident_clear=rt.targets.at(0x8C10EF3C);rt.register_target(0x8C10EF3C,observed_movie_resident_clear);
    struct RestoreMovieResidentInit{DCRuntime&rt;~RestoreMovieResidentInit(){rt.register_target(0x8C5FF860,real_movie_resident_init);rt.register_target(0x8C10EF3C,real_movie_resident_clear);movie_resident_init_observer={};real_movie_resident_init=nullptr;real_movie_resident_clear=nullptr;}}restore_movie_resident_init{rt};
#endif
#ifdef RECOVERY_MOVIE_CRITICAL
    const std::array<std::uint32_t,4> movie_critical_addresses={0x8C5FF932,0x8C5FFA5C,0x8C5FF952,0x8CECFD58};
    const std::array<std::uint32_t,4> movie_critical_returns={0x8CECFD58,0x8C5FF952,0x8C5FF952,0x8CECFD58};
    const std::array<const char*,4> movie_critical_labels={"first-movie-allocation-entry","first-movie-lock-entry","first-movie-lock-exit","first-movie-allocation-exit"};
    std::array<bool,4> movie_critical_seen{};
    movie_critical_observer=[&](unsigned which){if(!movie_critical_seen[which]&&ctx.pc==movie_critical_addresses[which]&&ctx.pr==movie_critical_returns[which]&&!rt.sh4_async_redirect&&(which==0||movie_critical_seen[0])){capture(movie_critical_labels[which]);movie_critical_seen[which]=true;}};
    real_movie_allocation=rt.targets.at(0x8C5FF932);rt.register_target(0x8C5FF932,observed_movie_allocation);
    real_movie_lock_enter=rt.targets.at(0x8C5FFA5C);rt.register_target(0x8C5FFA5C,observed_movie_lock_enter);
    struct RestoreMovieCritical{DCRuntime&rt;~RestoreMovieCritical(){rt.register_target(0x8C5FF932,real_movie_allocation);rt.register_target(0x8C5FFA5C,real_movie_lock_enter);movie_critical_observer={};real_movie_allocation=nullptr;real_movie_lock_enter=nullptr;}}restore_movie_critical{rt};
#endif
#ifdef RECOVERY_MOVIE_SLOTS
    const std::array<std::uint32_t,2> movie_slots_addresses={0x8C5FF980,0x8C5FF956};
    const std::array<const char*,2> movie_slots_labels={"first-movie-slot-search-entry","first-movie-slot-search-exit"};
    std::array<bool,2> movie_slots_seen{};
    movie_slots_observer=[&](unsigned which){if(!movie_slots_seen[which]&&ctx.pc==movie_slots_addresses[which]&&ctx.pr==0x8C5FF956&&!rt.sh4_async_redirect&&(which==0||movie_slots_seen[0])){capture(movie_slots_labels[which]);movie_slots_seen[which]=true;}};
    real_movie_slot_search=rt.targets.at(0x8C5FF980);rt.register_target(0x8C5FF980,observed_movie_slot_search);
    struct RestoreMovieSlots{DCRuntime&rt;~RestoreMovieSlots(){rt.register_target(0x8C5FF980,real_movie_slot_search);movie_slots_observer={};real_movie_slot_search=nullptr;}}restore_movie_slots{rt};
#endif
#ifdef RECOVERY_MOVIE_STREAM
    const std::array<std::uint32_t,5> movie_stream_addresses={0x8C5FF8D2,0x8C5FFA90,0x8C5FF8FE,0x8C5FF980,0x8CECFD68};
    const std::array<std::uint32_t,5> movie_stream_returns={0x8CECFD68,0x8C5FF8FE,0x8C5FF8FE,0x8C5FF90A,0x8CECFD68};
    const std::array<const char*,5> movie_stream_labels={"first-movie-stream-entry","first-movie-context-entry","first-movie-context-exit","first-movie-stream-search-entry","first-movie-stream-exit"};
    std::array<bool,5> movie_stream_seen{};
    movie_stream_observer=[&](unsigned which){if(!movie_stream_seen[which]&&ctx.pc==movie_stream_addresses[which]&&ctx.pr==movie_stream_returns[which]&&!rt.sh4_async_redirect&&(which==0||movie_stream_seen[0])){capture(movie_stream_labels[which]);movie_stream_seen[which]=true;}};
    real_movie_stream=rt.targets.at(0x8C5FF8D2);rt.register_target(0x8C5FF8D2,observed_movie_stream);
    real_movie_context=rt.targets.at(0x8C5FFA90);rt.register_target(0x8C5FFA90,observed_movie_context);
    real_movie_stream_search=rt.targets.at(0x8C5FF980);rt.register_target(0x8C5FF980,observed_movie_stream_search);
    struct RestoreMovieStream{DCRuntime&rt;~RestoreMovieStream(){rt.register_target(0x8C5FF8D2,real_movie_stream);rt.register_target(0x8C5FFA90,real_movie_context);rt.register_target(0x8C5FF980,real_movie_stream_search);movie_stream_observer={};real_movie_stream=nullptr;real_movie_context=nullptr;real_movie_stream_search=nullptr;}}restore_movie_stream{rt};
#endif
#ifdef RECOVERY_MOVIE_CALLBACK_INSTALL
    const std::array<std::uint32_t,4> movie_install_addresses={0x8C6453A2,0x8C645002,0x8C6453B6,0x8CECFD74};
    const std::array<std::uint32_t,4> movie_install_returns={0x8CECFD74,0x8C6453B6,0x8C6453B6,0x8CECFD74};
    const std::array<const char*,4> movie_install_labels={"first-movie-install-entry","first-movie-install-helper-entry","first-movie-install-helper-exit","first-movie-install-exit"};
    std::array<bool,4> movie_install_seen{};
    movie_install_observer=[&](unsigned which){if(!movie_install_seen[which]&&ctx.pc==movie_install_addresses[which]&&ctx.pr==movie_install_returns[which]&&!rt.sh4_async_redirect&&(which==0||movie_install_seen[0])){capture(movie_install_labels[which]);movie_install_seen[which]=true;}};
    real_movie_install=rt.targets.at(0x8C6453A2);rt.register_target(0x8C6453A2,observed_movie_install);
    real_movie_install_helper=rt.targets.at(0x8C645002);rt.register_target(0x8C645002,observed_movie_install_helper);
    struct RestoreMovieInstall{DCRuntime&rt;~RestoreMovieInstall(){rt.register_target(0x8C6453A2,real_movie_install);rt.register_target(0x8C645002,real_movie_install_helper);movie_install_observer={};real_movie_install=nullptr;real_movie_install_helper=nullptr;}}restore_movie_install{rt};
#endif
#ifdef RECOVERY_MOVIE_MODE_CONFIG
    const std::array<std::uint32_t,5> movie_mode_config_addresses={0x8CECF096,0x8C10E67C,0x8CECF0D0,0x8CED56EE,0x8CECFD82};
    const std::array<std::uint32_t,5> movie_mode_config_returns={0x8CECFD82,0x8CECF0D0,0x8CECF0D0,0x8CECFD82,0x8CECFD82};
    const std::array<const char*,5> movie_mode_config_labels={"first-movie-mode-config-entry","first-movie-mode-divide-entry","first-movie-mode-divide-exit","first-movie-mode-tail-entry","first-movie-mode-config-exit"};
    unsigned movie_mode_config_seen=0;
    movie_mode_config_observer=[&](unsigned which){if(which==movie_mode_config_seen&&ctx.pc==movie_mode_config_addresses[which]&&ctx.pr==movie_mode_config_returns[which]&&!rt.sh4_async_redirect){capture(movie_mode_config_labels[which]);++movie_mode_config_seen;}};
    real_movie_mode_division=rt.targets.at(0x8C10E67C);rt.register_target(0x8C10E67C,observed_movie_mode_division);
    struct RestoreMovieModeConfig{DCRuntime&rt;~RestoreMovieModeConfig(){rt.register_target(0x8C10E67C,real_movie_mode_division);movie_mode_config_observer={};real_movie_mode_division=nullptr;}}restore_movie_mode_config{rt};
#endif
#ifdef RECOVERY_MOVIE_CALLBACK_REGISTER
    const std::array<std::uint32_t,6> movie_callback_register_addresses={0x8CED53C2,0x8CED5324,0x8CED53CE,0x8CED533C,0x8CED5408,0x8CECFD92};
    const std::array<std::uint32_t,6> movie_callback_register_returns={0x8CECFD92,0x8CED53CE,0x8CED53CE,0x8CED5408,0x8CED5408,0x8CECFD92};
    const std::array<const char*,6> movie_callback_register_labels={"first-movie-callback-register-entry","first-movie-callback-mask-enter-entry","first-movie-callback-mask-enter-exit","first-movie-callback-mask-leave-entry","first-movie-callback-mask-leave-exit","first-movie-callback-register-exit"};
    unsigned movie_callback_register_seen=0;
    movie_callback_register_observer=[&](unsigned which){if(which==movie_callback_register_seen&&ctx.pc==movie_callback_register_addresses[which]&&ctx.pr==movie_callback_register_returns[which]&&!rt.sh4_async_redirect){capture(movie_callback_register_labels[which]);++movie_callback_register_seen;}};
    struct RestoreMovieCallbackRegister{~RestoreMovieCallbackRegister(){movie_callback_register_observer={};}}restore_movie_callback_register;
#endif
#ifdef RECOVERY_MOVIE_CALLBACK_REGISTER2
    const std::array<std::uint32_t,6> movie_callback_register2_addresses={0x8CED5494,0x8CED5324,0x8CED54A0,0x8CED533C,0x8CED54DA,0x8CECFD9A};
    const std::array<std::uint32_t,6> movie_callback_register2_returns={0x8CECFD9A,0x8CED54A0,0x8CED54A0,0x8CED54DA,0x8CED54DA,0x8CECFD9A};
    const std::array<const char*,6> movie_callback_register2_labels={"first-movie-callback-register2-entry","first-movie-callback-mask-enter2-entry","first-movie-callback-mask-enter2-exit","first-movie-callback-mask-leave2-entry","first-movie-callback-mask-leave2-exit","first-movie-callback-register2-exit"};
    unsigned movie_callback_register2_seen=0;
    movie_callback_register2_observer=[&](unsigned which){if(which==movie_callback_register2_seen&&ctx.pc==movie_callback_register2_addresses[which]&&ctx.pr==movie_callback_register2_returns[which]&&!rt.sh4_async_redirect){capture(movie_callback_register2_labels[which]);++movie_callback_register2_seen;}};
    struct RestoreMovieCallbackRegister2{~RestoreMovieCallbackRegister2(){movie_callback_register2_observer={};}}restore_movie_callback_register2;
#endif
#ifdef RECOVERY_MOVIE_CALLBACK_REGISTER3
    const std::array<std::uint32_t,6> movie_callback_register3_addresses={0x8CED5556,0x8CED5324,0x8CED5562,0x8CED533C,0x8CED559C,0x8CEB00DA};
    const std::array<std::uint32_t,6> movie_callback_register3_returns={0x8CEB00DA,0x8CED5562,0x8CED5562,0x8CED559C,0x8CED559C,0x8CEB00DA};
    const std::array<const char*,6> movie_callback_register3_labels={"first-movie-callback-register3-entry","first-movie-callback-mask-enter3-entry","first-movie-callback-mask-enter3-exit","first-movie-callback-mask-leave3-entry","first-movie-callback-mask-leave3-exit","first-movie-callback-register3-exit"};
    unsigned movie_callback_register3_seen=0;
    movie_callback_register3_observer=[&](unsigned which){if(which==movie_callback_register3_seen&&ctx.pc==movie_callback_register3_addresses[which]&&ctx.pr==movie_callback_register3_returns[which]&&!rt.sh4_async_redirect){capture(movie_callback_register3_labels[which]);++movie_callback_register3_seen;}};
    struct RestoreMovieCallbackRegister3{~RestoreMovieCallbackRegister3(){movie_callback_register3_observer={};}}restore_movie_callback_register3;
#endif
#ifdef RECOVERY_MOVIE_SIZE
#ifdef RECOVERY_MOVIE_SIZE_RETURN
    const std::array<std::uint32_t,10> movie_size_addresses={0x8CED02BA,0x8CED01BC,0x8C10E67C,0x8CED01F6,0x8CED02E4,0x8CED0256,0x8CED0222,0x8CED0274,0x8CED02F8,0x8CEB0166};
    const std::array<std::uint32_t,10> movie_size_returns={0x8CEB0166,0x8CED02E4,0x8CED01F6,0x8CED01F6,0x8CED02E4,0x8CED02F8,0x8CED0274,0x8CED0274,0x8CED02F8,0x8CEB0166};
    const std::array<const char*,10> movie_size_labels={"first-movie-size-entry","first-movie-size-stream-entry","first-movie-size-divide-entry","first-movie-size-divide-exit","first-movie-size-stream-exit","first-movie-size-frame-entry","first-movie-dimensions-entry","first-movie-dimensions-exit","first-movie-size-frame-exit","first-movie-size-exit"};
#else
    const std::array<std::uint32_t,6> movie_size_addresses={0x8CED02BA,0x8CED01BC,0x8C10E67C,0x8CED01F6,0x8CED02E4,0x8CED0256};
    const std::array<std::uint32_t,6> movie_size_returns={0x8CEB0166,0x8CED02E4,0x8CED01F6,0x8CED01F6,0x8CED02E4,0x8CED02F8};
    const std::array<const char*,6> movie_size_labels={"first-movie-size-entry","first-movie-size-stream-entry","first-movie-size-divide-entry","first-movie-size-divide-exit","first-movie-size-stream-exit","first-movie-size-frame-entry"};
#endif
    unsigned movie_size_seen=0;
    movie_size_observer=[&](unsigned which){if(which==movie_size_seen&&ctx.pc==movie_size_addresses[which]&&ctx.pr==movie_size_returns[which]&&!rt.sh4_async_redirect){capture(movie_size_labels[which]);++movie_size_seen;}};
    real_movie_size_division=rt.targets.at(0x8C10E67C);rt.register_target(0x8C10E67C,observed_movie_size_division);
    struct RestoreMovieSize{DCRuntime&rt;~RestoreMovieSize(){rt.register_target(0x8C10E67C,real_movie_size_division);movie_size_observer={};real_movie_size_division=nullptr;}}restore_movie_size{rt};
#endif
#ifdef RECOVERY_MOVIE_SLOT_SETUP
    const std::array<std::uint32_t,4> movie_slot_setup_addresses={0x8CECF0F8,0x8CED0350,0x8C10E958,0x8CED0370};
    const std::array<std::uint32_t,4> movie_slot_setup_returns={0x8CEB0174,0x8CECF188,0x8CED0370,0x8CED0370};
    const std::array<const char*,4> movie_slot_setup_labels={"first-movie-slot-setup-entry","first-movie-setup-validation-entry","first-movie-setup-copy-entry","first-movie-setup-copy-exit"};
    unsigned movie_slot_setup_seen=0;
    movie_slot_setup_observer=[&](unsigned which){if(which==movie_slot_setup_seen&&ctx.pc==movie_slot_setup_addresses[which]&&ctx.pr==movie_slot_setup_returns[which]&&!rt.sh4_async_redirect){capture(movie_slot_setup_labels[which]);++movie_slot_setup_seen;}};
    real_movie_setup_copy=rt.targets.at(0x8C10E958);rt.register_target(0x8C10E958,observed_movie_setup_copy);
    struct RestoreMovieSlotSetup{DCRuntime&rt;~RestoreMovieSlotSetup(){rt.register_target(0x8C10E958,real_movie_setup_copy);movie_slot_setup_observer={};real_movie_setup_copy=nullptr;}}restore_movie_slot_setup{rt};
#endif
#ifdef RECOVERY_MOVIE_DESCRIPTOR_INIT
    const std::array<std::uint32_t,2> movie_descriptor_addresses={0x8CED18E4,0x8CED1908};
    const std::array<std::uint32_t,2> movie_descriptor_returns={0x8CED0390,0x8CED18EA};
    const std::array<const char*,2> movie_descriptor_labels={"first-movie-descriptor-init-entry","first-movie-descriptor-fields-entry"};
    unsigned movie_descriptor_seen=0;
    movie_descriptor_observer=[&](unsigned which){if(which==movie_descriptor_seen&&ctx.pc==movie_descriptor_addresses[which]&&ctx.pr==movie_descriptor_returns[which]&&!rt.sh4_async_redirect){capture(movie_descriptor_labels[which]);++movie_descriptor_seen;}};
    struct RestoreMovieDescriptor{~RestoreMovieDescriptor(){movie_descriptor_observer={};}}restore_movie_descriptor;
#endif
#ifdef RECOVERY_MOVIE_WORKAREA
    const std::array<std::uint32_t,6> movie_workarea_addresses={0x8CED67F8,0x8CED1920,0x8C11157C,0x8CED192C,0x8C11157C,0x8CED1938};
    const std::array<std::uint32_t,6> movie_workarea_returns={0x8CED1920,0x8CED1920,0x8CED192C,0x8CED192C,0x8CED1938,0x8CED1938};
    const std::array<const char*,6> movie_workarea_labels={"first-movie-word-fill-entry","first-movie-word-fill-exit","first-movie-string1-entry","first-movie-string1-exit","first-movie-string2-entry","first-movie-string2-exit"};
    unsigned movie_workarea_seen=0;
    movie_workarea_observer=[&](unsigned which){if(which==movie_workarea_seen&&ctx.pc==movie_workarea_addresses[which]&&ctx.pr==movie_workarea_returns[which]&&!rt.sh4_async_redirect){capture(movie_workarea_labels[which]);++movie_workarea_seen;}};
    require(!rt.targets.contains(0x8C11157C),"BOOT bounded string already registered");movie_bounded_string_register(rt);
    real_movie_bounded_string=rt.targets.at(0x8C11157C);rt.register_target(0x8C11157C,observed_movie_bounded_string);
    struct RestoreMovieWorkarea{DCRuntime&rt;~RestoreMovieWorkarea(){rt.register_target(0x8C11157C,real_movie_bounded_string);movie_workarea_observer={};real_movie_bounded_string=nullptr;}}restore_movie_workarea{rt};
#endif
#ifdef RECOVERY_MOVIE_WORD_COPY
    const std::array<std::uint32_t,6> movie_word_copy_addresses={0x8CED6708,0x8CED1944,0x8C10E958,0x8CED1950,0x8CED1A90,0x8CED195C};
    const std::array<std::uint32_t,6> movie_word_copy_returns={0x8CED1944,0x8CED1944,0x8CED1950,0x8CED1950,0x8CED195C,0x8CED195C};
    const std::array<const char*,6> movie_word_copy_labels={"first-movie-word-copy-entry","first-movie-word-copy-exit","first-movie-work-descriptor-copy-entry","first-movie-work-descriptor-copy-exit","first-movie-record-clear-entry","first-movie-record-clear-exit"};
    unsigned movie_word_copy_seen=0;
    movie_word_copy_observer=[&](unsigned which){if(which==movie_word_copy_seen&&ctx.pc==movie_word_copy_addresses[which]&&ctx.pr==movie_word_copy_returns[which]&&!rt.sh4_async_redirect){capture(movie_word_copy_labels[which]);++movie_word_copy_seen;}};
    real_movie_descriptor_copy=rt.targets.at(0x8C10E958);rt.register_target(0x8C10E958,observed_movie_descriptor_copy);
    struct RestoreMovieWordCopy{DCRuntime&rt;~RestoreMovieWordCopy(){rt.register_target(0x8C10E958,real_movie_descriptor_copy);movie_word_copy_observer={};real_movie_descriptor_copy=nullptr;}}restore_movie_word_copy{rt};
#endif
#ifdef RECOVERY_MOVIE_RATE_INIT
    const std::array<std::uint32_t,6> movie_rate_addresses={0x8CED4620,0x8CED1966,0x8CED97A8,0x8CED196E,0x8CED67F8,0x8CED198A};
    const std::array<std::uint32_t,6> movie_rate_returns={0x8CED1966,0x8CED1966,0x8CED196E,0x8CED196E,0x8CED198A,0x8CED198A};
    const std::array<const char*,6> movie_rate_labels={"first-movie-rate-init-entry","first-movie-rate-init-exit","first-movie-context-return-entry","first-movie-context-return-exit","first-movie-buffer-fill-entry","first-movie-buffer-fill-exit"};
    unsigned movie_rate_seen=0;
    movie_rate_observer=[&](unsigned which){if(which==movie_rate_seen&&ctx.pc==movie_rate_addresses[which]&&ctx.pr==movie_rate_returns[which]&&!rt.sh4_async_redirect){capture(movie_rate_labels[which]);++movie_rate_seen;}};
    struct RestoreMovieRate{~RestoreMovieRate(){movie_rate_observer={};}}restore_movie_rate;
#endif
#ifdef RECOVERY_MOVIE_INTERFACE
    const std::array<std::uint32_t,4> movie_interface_addresses={0x8CEDA288,0x8C10E964,0x8CEDA298,0x8CEDA2B2};
    const std::array<std::uint32_t,4> movie_interface_returns={0x8CED19AC,0x8CEDA298,0x8CEDA298,0x8CEDA29E};
    const std::array<const char*,4> movie_interface_labels={"first-movie-interface-entry","first-movie-interface-copy-entry","first-movie-interface-copy-exit","first-movie-interface-bind-entry"};
    unsigned movie_interface_seen=0;
    movie_interface_observer=[&](unsigned which){if(which==movie_interface_seen&&ctx.pc==movie_interface_addresses[which]&&ctx.pr==movie_interface_returns[which]&&!rt.sh4_async_redirect){capture(movie_interface_labels[which]);++movie_interface_seen;}};
    real_movie_interface_copy=rt.targets.at(0x8C10E964);rt.register_target(0x8C10E964,observed_movie_interface_copy);
    struct RestoreMovieInterface{DCRuntime&rt;~RestoreMovieInterface(){rt.register_target(0x8C10E964,real_movie_interface_copy);movie_interface_observer={};real_movie_interface_copy=nullptr;}}restore_movie_interface{rt};
#endif
#ifdef RECOVERY_MOVIE_METHOD_INIT
    unsigned movie_method_seen=0;
    movie_method_observer=[&](unsigned which){if(which==movie_method_seen&&ctx.pc==(which?0x8CEDA2E6u:0x8CED1C10u)&&ctx.pr==0x8CEDA2E6u&&!rt.sh4_async_redirect){capture(which?"first-movie-method-init-exit":"first-movie-method-init-entry");++movie_method_seen;}};
    struct RestoreMovieMethod{~RestoreMovieMethod(){movie_method_observer={};}}restore_movie_method;
#endif
#ifdef RECOVERY_MOVIE_METHOD_SETUP
    unsigned movie_setup_seen=0;
    movie_setup_observer=[&](unsigned which){if(which==movie_setup_seen&&ctx.pc==(which?0x8CED78C4u:0x8CED1CACu)&&ctx.pr==(which?0x8CED1CB6u:0x8CEDA2E6u)&&!rt.sh4_async_redirect){capture(which?"first-movie-method-setup-entry":"first-movie-method-callback-entry");++movie_setup_seen;}};
    struct RestoreMovieSetup{~RestoreMovieSetup(){movie_setup_observer={};}}restore_movie_setup;
#endif
#ifdef RECOVERY_MOVIE_METHOD_WORKAREA
    const std::array<std::uint32_t,10> method_workarea_addresses={0x8CED78EA,0x8CED67F8,0x8CED7910,0x8C11157C,0x8CED791A,0x8C11157C,0x8CED7926,0x8CED79A4,0x8CED792C,0x8CED78CA};
    const std::array<std::uint32_t,10> method_workarea_returns={0x8CED78CA,0x8CED7910,0x8CED7910,0x8CED791A,0x8CED791A,0x8CED7926,0x8CED7926,0x8CED792C,0x8CED792C,0x8CED78CA};
    const std::array<const char*,10> method_workarea_labels={"first-method-workarea-entry","first-method-fill-entry","first-method-fill-exit","first-method-string1-entry","first-method-string1-exit","first-method-string2-entry","first-method-string2-exit","first-method-record-entry","first-method-record-exit","first-method-workarea-exit"};
    unsigned method_workarea_seen=0;
    movie_method_workarea_observer=[&](unsigned which){if(which==method_workarea_seen&&ctx.pc==method_workarea_addresses[which]&&ctx.pr==method_workarea_returns[which]&&!rt.sh4_async_redirect){capture(method_workarea_labels[which]);++method_workarea_seen;}};
    real_method_bounded_string=rt.targets.at(0x8C11157C);rt.register_target(0x8C11157C,observed_method_copy);
    struct RestoreMethodWorkarea{DCRuntime&rt;~RestoreMethodWorkarea(){rt.register_target(0x8C11157C,real_method_bounded_string);movie_method_workarea_observer={};real_method_bounded_string=nullptr;}}restore_method_workarea{rt};
#endif
#ifdef RECOVERY_MOVIE_METHOD_RETURN
    const std::array<std::uint32_t,6> method_return_addresses={0x8CED6840,0x8CED78DC,0x8CED7800,0x8CED78E2,0x8CED1CB6,0x8CEDA2E6};
    const std::array<std::uint32_t,6> method_return_returns={0x8CED78DC,0x8CED78DC,0x8CED78E2,0x8CED78E2,0x8CED1CB6,0x8CEDA2E6};
    const std::array<const char*,6> method_return_labels={"first-method-followup-entry","first-method-followup-exit","first-method-final-entry","first-method-final-exit","first-method-setup-return","first-method-callback-return"};
    unsigned method_return_seen=0;
    method_return_observer=[&](unsigned which){if(which==method_return_seen&&ctx.pc==method_return_addresses[which]&&ctx.pr==method_return_returns[which]&&!rt.sh4_async_redirect){capture(method_return_labels[which]);++method_return_seen;}};
    struct RestoreMethodReturn{~RestoreMethodReturn(){method_return_observer={};}}restore_method_return;
#endif
#ifdef RECOVERY_MOVIE_THIRD_SETUP
    unsigned movie_third_seen=0;
    movie_third_observer=[&](unsigned which){if(which==movie_third_seen&&ctx.pc==(which?0x8CED80ACu:0x8CED256Au)&&ctx.pr==(which?0x8CED2574u:0x8CEDA2E6u)&&!rt.sh4_async_redirect){capture(which?"first-movie-third-setup-entry":"first-movie-third-callback-entry");++movie_third_seen;}};
    struct RestoreMovieThird{~RestoreMovieThird(){movie_third_observer={};}}restore_movie_third;
#endif
#ifdef RECOVERY_MOVIE_THIRD_VALIDATION
    const std::array<std::uint32_t,3> movie_validation_addresses={0x8CED811A,0x8CEDAB10,0x8CED8144};
    const std::array<std::uint32_t,3> movie_validation_returns={0x8CED80B8,0x8CED8144,0x8CED8144};
    const std::array<const char*,3> movie_validation_labels={"first-third-validation-entry","first-third-guard-entry","first-third-guard-exit"};
    unsigned movie_validation_seen=0;
    movie_validation_observer=[&](unsigned which){if(which==movie_validation_seen&&ctx.pc==movie_validation_addresses[which]&&ctx.pr==movie_validation_returns[which]&&!rt.sh4_async_redirect){capture(movie_validation_labels[which]);++movie_validation_seen;}};
    struct RestoreMovieValidation{~RestoreMovieValidation(){movie_validation_observer={};}}restore_movie_validation;
#endif
#ifdef RECOVERY_MOVIE_PAIR_CHECK
    const std::array<std::uint32_t,10> movie_pair_addresses={0x8CEDC35C,0x8CEDC348,0x8CEDC366,0x8CEDC348,0x8CEDC370,0x8CEDC348,0x8CEDC37A,0x8CEDC348,0x8CEDC384,0x8CED8154};
    const std::array<std::uint32_t,10> movie_pair_returns={0x8CED8154,0x8CEDC366,0x8CEDC366,0x8CEDC370,0x8CEDC370,0x8CEDC37A,0x8CEDC37A,0x8CEDC384,0x8CEDC384,0x8CED8154};
    const std::array<const char*,10> movie_pair_labels={"first-pair-check-entry","first-pair1-entry","first-pair1-exit","first-pair2-entry","first-pair2-exit","first-pair3-entry","first-pair3-exit","first-pair4-entry","first-pair4-exit","first-pair-check-exit"};
    unsigned movie_pair_seen=0;
    movie_pair_observer=[&](unsigned which){if(which==movie_pair_seen&&ctx.pc==movie_pair_addresses[which]&&ctx.pr==movie_pair_returns[which]&&!rt.sh4_async_redirect){capture(movie_pair_labels[which]);++movie_pair_seen;}};
    struct RestoreMoviePair{~RestoreMoviePair(){movie_pair_observer={};}}restore_movie_pair;
#endif
#ifdef RECOVERY_MOVIE_CACHE_FLAGS
    const std::array<std::uint32_t,11> movie_cache_addresses={0x8CEDC394,0x8CEDC348,0x8CEDC39E,0x8CEDC348,0x8CEDC3A8,0x8CEDC348,0x8CEDC3B2,0x8CED8164,0x8CED80B8,0x8CED81CC,0x8CED80D6};
    const std::array<std::uint32_t,11> movie_cache_returns={0x8CED8164,0x8CEDC39E,0x8CEDC39E,0x8CEDC3A8,0x8CEDC3A8,0x8CEDC3B2,0x8CEDC3B2,0x8CED8164,0x8CED80B8,0x8CED80D6,0x8CED80D6};
    const std::array<const char*,11> movie_cache_labels={"first-triple-check-entry","first-triple1-entry","first-triple1-exit","first-triple2-entry","first-triple2-exit","first-triple3-entry","first-triple3-exit","first-triple-check-exit","first-third-validation-exit","first-cache-flags-entry","first-cache-flags-exit"};
    unsigned movie_cache_seen=0;
    movie_cache_observer=[&](unsigned which){if(which==movie_cache_seen&&ctx.pc==movie_cache_addresses[which]&&ctx.pr==movie_cache_returns[which]&&!rt.sh4_async_redirect){capture(movie_cache_labels[which]);++movie_cache_seen;}};
    struct RestoreMovieCache{~RestoreMovieCache(){movie_cache_observer={};}}restore_movie_cache;
#endif
#ifdef RECOVERY_MOVIE_WORK_CONFIG
    const std::array<std::uint32_t,8> work_config_addresses={0x8CED8206,0x8CED80DA,0x8CED8348,0x8CED67F8,0x8CED8370,0x8CED6708,0x8CED84EE,0x8CED80E0};
    const std::array<std::uint32_t,8> work_config_returns={0x8CED80DA,0x8CED80DA,0x8CED80E0,0x8CED8370,0x8CED8370,0x8CED84EE,0x8CED84EE,0x8CED80E0};
    const std::array<const char*,8> work_config_labels={"first-work-pointer-entry","first-work-pointer-exit","first-work-config-entry","first-work-config-fill-entry","first-work-config-fill-exit","first-work-config-copy-entry","first-work-config-copy-exit","first-work-config-exit"};
    unsigned work_config_seen=0;
    work_config_observer=[&](unsigned which){if(which==work_config_seen&&ctx.pc==work_config_addresses[which]&&ctx.pr==work_config_returns[which]&&!rt.sh4_async_redirect){capture(work_config_labels[which]);++work_config_seen;}};
    struct RestoreWorkConfig{~RestoreWorkConfig(){work_config_observer={};}}restore_work_config;
#endif
#ifdef RECOVERY_MOVIE_WORK_RESET
    const std::array<std::uint32_t,4> work_reset_addresses={0x8CED7E44,0x8CED80E8,0x8CED8074,0x8CED80EE};
    const std::array<std::uint32_t,4> work_reset_returns={0x8CED80E8,0x8CED80E8,0x8CED80EE,0x8CED80EE};
    const std::array<const char*,4> work_reset_labels={"first-work-clear-entry","first-work-clear-exit","first-work-reset-entry","first-work-reset-exit"};
    unsigned work_reset_seen=0;
    work_reset_observer=[&](unsigned which){if(which==work_reset_seen&&ctx.pc==work_reset_addresses[which]&&ctx.pr==work_reset_returns[which]&&!rt.sh4_async_redirect){capture(work_reset_labels[which]);++work_reset_seen;}};
    struct RestoreWorkReset{~RestoreWorkReset(){work_reset_observer={};}}restore_work_reset;
#endif
#ifdef RECOVERY_MOVIE_FIRST_TABLE
    const std::array<std::uint32_t,3> first_table_addresses={0x8CEDAB14,0x8CEDAB72,0x8CEDAB1C};
    const std::array<std::uint32_t,3> first_table_returns={0x8CED80F6,0x8CEDAB1C,0x8CEDAB1C};
    const std::array<const char*,3> first_table_labels={"first-tail-helper-entry","first-table-entry","first-table-exit"};
    unsigned first_table_seen=0;
    first_table_observer=[&](unsigned which){if(which==first_table_seen&&ctx.pc==first_table_addresses[which]&&ctx.pr==first_table_returns[which]&&!rt.sh4_async_redirect){capture(first_table_labels[which]);++first_table_seen;}};
    struct RestoreFirstTable{~RestoreFirstTable(){first_table_observer={};}}restore_first_table;
#endif
#ifdef RECOVERY_MOVIE_SECOND_TABLE
    const std::array<std::uint32_t,2> second_table_addresses={0x8CEDADAC,0x8CEDAB20};
    const std::array<std::uint32_t,2> second_table_returns={0x8CEDAB20,0x8CEDAB20};
    const std::array<const char*,2> second_table_labels={"second-table-entry","second-table-exit"};
    unsigned second_table_seen=0;
    second_table_observer=[&](unsigned which){if(which==second_table_seen&&ctx.pc==second_table_addresses[which]&&ctx.pr==second_table_returns[which]&&!rt.sh4_async_redirect){capture(second_table_labels[which]);++second_table_seen;}};
    struct RestoreSecondTable{~RestoreSecondTable(){second_table_observer={};}}restore_second_table;
#endif
#ifdef RECOVERY_MOVIE_THIRD_TABLE
    const std::array<std::uint32_t,2> third_table_addresses={0x8CEDAFB6,0x8CEDAB24};
    const std::array<std::uint32_t,2> third_table_returns={0x8CEDAB24,0x8CEDAB24};
    const std::array<const char*,2> third_table_labels={"third-table-entry","third-table-exit"};
    unsigned third_table_seen=0;
    third_table_observer=[&](unsigned which){if(which==third_table_seen&&ctx.pc==third_table_addresses[which]&&ctx.pr==third_table_returns[which]&&!rt.sh4_async_redirect){capture(third_table_labels[which]);++third_table_seen;}};
    struct RestoreThirdTable{~RestoreThirdTable(){third_table_observer={};}}restore_third_table;
#endif
#ifdef RECOVERY_MOVIE_FOURTH_TABLE
    const std::array<std::uint32_t,2> fourth_table_addresses={0x8CEDB1FC,0x8CEDAB28};
    const std::array<std::uint32_t,2> fourth_table_returns={0x8CEDAB28,0x8CEDAB28};
    const std::array<const char*,2> fourth_table_labels={"fourth-table-entry","fourth-table-exit"};
    unsigned fourth_table_seen=0;
    fourth_table_observer=[&](unsigned which){if(which==fourth_table_seen&&ctx.pc==fourth_table_addresses[which]&&ctx.pr==fourth_table_returns[which]&&!rt.sh4_async_redirect){capture(fourth_table_labels[which]);++fourth_table_seen;}};
    struct RestoreFourthTable{~RestoreFourthTable(){fourth_table_observer={};}}restore_fourth_table;
#endif
#ifdef RECOVERY_MOVIE_FIFTH_TABLE
    const std::array<std::uint32_t,2> fifth_table_addresses={0x8CEDB26C,0x8CEDAB2C};
    const std::array<std::uint32_t,2> fifth_table_returns={0x8CEDAB2C,0x8CEDAB2C};
    const std::array<const char*,2> fifth_table_labels={"fifth-table-entry","fifth-table-exit"};
    unsigned fifth_table_seen=0;
    fifth_table_observer=[&](unsigned which){if(which==fifth_table_seen&&ctx.pc==fifth_table_addresses[which]&&ctx.pr==fifth_table_returns[which]&&!rt.sh4_async_redirect){capture(fifth_table_labels[which]);++fifth_table_seen;}};
    struct RestoreFifthTable{~RestoreFifthTable(){fifth_table_observer={};}}restore_fifth_table;
#endif
#ifdef RECOVERY_MOVIE_SIXTH_TABLE
    const std::array<std::uint32_t,2> sixth_table_addresses={0x8CEDB312,0x8CEDAB30};
    const std::array<std::uint32_t,2> sixth_table_returns={0x8CEDAB30,0x8CEDAB30};
    const std::array<const char*,2> sixth_table_labels={"sixth-table-entry","sixth-table-exit"};
    unsigned sixth_table_seen=0;
    sixth_table_observer=[&](unsigned which){if(which==sixth_table_seen&&ctx.pc==sixth_table_addresses[which]&&ctx.pr==sixth_table_returns[which]&&!rt.sh4_async_redirect){capture(sixth_table_labels[which]);++sixth_table_seen;}};
    struct RestoreSixthTable{~RestoreSixthTable(){sixth_table_observer={};}}restore_sixth_table;
#endif
#ifdef RECOVERY_MOVIE_SEVENTH_TABLE
    const std::array<std::uint32_t,2> seventh_table_addresses={0x8CEDB486,0x8CEDAB34};
    const std::array<std::uint32_t,2> seventh_table_returns={0x8CEDAB34,0x8CEDAB34};
    const std::array<const char*,2> seventh_table_labels={"seventh-table-entry","seventh-table-exit"};
    unsigned seventh_table_seen=0;
    seventh_table_observer=[&](unsigned which){if(which==seventh_table_seen&&ctx.pc==seventh_table_addresses[which]&&ctx.pr==seventh_table_returns[which]&&!rt.sh4_async_redirect){capture(seventh_table_labels[which]);++seventh_table_seen;}};
    struct RestoreSeventhTable{~RestoreSeventhTable(){seventh_table_observer={};}}restore_seventh_table;
#endif
#ifdef RECOVERY_MOVIE_EIGHTH_TABLE
    const std::array<std::uint32_t,2> eighth_table_addresses={0x8CEDBD9C,0x8CEDAB3A};
    const std::array<std::uint32_t,2> eighth_table_returns={0x8CEDAB3A,0x8CEDAB3A};
    const std::array<const char*,2> eighth_table_labels={"eighth-table-entry","eighth-table-exit"};
    unsigned eighth_table_seen=0;
    eighth_table_observer=[&](unsigned which){if(which==eighth_table_seen&&ctx.pc==eighth_table_addresses[which]&&ctx.pr==eighth_table_returns[which]&&!rt.sh4_async_redirect){capture(eighth_table_labels[which]);++eighth_table_seen;}};
    struct RestoreEighthTable{~RestoreEighthTable(){eighth_table_observer={};}}restore_eighth_table;
#endif
#ifdef RECOVERY_MOVIE_NINTH_TABLE
    const std::array<std::uint32_t,2> ninth_table_addresses={0x8CEDBE14,0x8CEDAB40};
    const std::array<std::uint32_t,2> ninth_table_returns={0x8CEDAB40,0x8CEDAB40};
    const std::array<const char*,2> ninth_table_labels={"ninth-table-entry","ninth-table-exit"};
    unsigned ninth_table_seen=0;
    ninth_table_observer=[&](unsigned which){if(which==ninth_table_seen&&ctx.pc==ninth_table_addresses[which]&&ctx.pr==ninth_table_returns[which]&&!rt.sh4_async_redirect){capture(ninth_table_labels[which]);++ninth_table_seen;}};
    struct RestoreNinthTable{~RestoreNinthTable(){ninth_table_observer={};}}restore_ninth_table;
#endif
#ifdef RECOVERY_MOVIE_TENTH_TABLE
    const std::array<std::uint32_t,2> tenth_table_addresses={0x8CEDBEA2,0x8CEDAB46};
    const std::array<std::uint32_t,2> tenth_table_returns={0x8CEDAB46,0x8CEDAB46};
    const std::array<const char*,2> tenth_table_labels={"tenth-table-entry","tenth-table-exit"};
    unsigned tenth_table_seen=0;
    tenth_table_observer=[&](unsigned which){if(which==tenth_table_seen&&ctx.pc==tenth_table_addresses[which]&&ctx.pr==tenth_table_returns[which]&&!rt.sh4_async_redirect){capture(tenth_table_labels[which]);++tenth_table_seen;}};
    struct RestoreTenthTable{~RestoreTenthTable(){tenth_table_observer={};}}restore_tenth_table;
#endif
#ifdef RECOVERY_MOVIE_ELEVENTH_TABLE
    const std::array<std::uint32_t,2> eleventh_table_addresses={0x8CEDC040,0x8CEDAB4C};
    const std::array<std::uint32_t,2> eleventh_table_returns={0x8CEDAB4C,0x8CEDAB4C};
    const std::array<const char*,2> eleventh_table_labels={"eleventh-table-entry","eleventh-table-exit"};
    unsigned eleventh_table_seen=0;
    eleventh_table_observer=[&](unsigned which){if(which==eleventh_table_seen&&ctx.pc==eleventh_table_addresses[which]&&ctx.pr==eleventh_table_returns[which]&&!rt.sh4_async_redirect){capture(eleventh_table_labels[which]);++eleventh_table_seen;}};
    struct RestoreEleventhTable{~RestoreEleventhTable(){eleventh_table_observer={};}}restore_eleventh_table;
#endif
#ifdef RECOVERY_MOVIE_TABLE_TAIL
    unsigned table_tail_seen=0;
    table_tail_observer=[&]{if(!table_tail_seen&&ctx.pc==0x8CEDC0BA&&ctx.pr==0x8CED80F6&&!rt.sh4_async_redirect){capture("table-tail-entry");table_tail_seen=1;}else if(table_tail_seen==1&&ctx.pc==0x8CED80F6&&ctx.pr==0x8CED80F6&&!rt.sh4_async_redirect){capture("table-tail-return");table_tail_seen=2;}};
    struct RestoreTableTail{~RestoreTableTail(){table_tail_observer={};}}restore_table_tail;
#endif
#ifdef RECOVERY_MOVIE_TABLE_FIRST
    unsigned table_first_seen=0;
    const std::array<std::uint32_t,2> table_first_addresses={0x8CEDC178,0x8CEDC0C2};
    const std::array<const char*,2> table_first_labels={"table-first-entry","table-first-exit"};
    table_first_observer=[&](unsigned which){if(which==table_first_seen&&ctx.pc==table_first_addresses[which]&&ctx.pr==0x8CEDC0C2&&!rt.sh4_async_redirect){capture(table_first_labels[which]);++table_first_seen;}};
    struct RestoreTableFirst{~RestoreTableFirst(){table_first_observer={};}}restore_table_first;
#endif
#ifdef RECOVERY_MOVIE_TABLE_SECOND
    unsigned table_second_seen=0;
    const std::array<std::uint32_t,2> table_second_addresses={0x8CEDC218,0x8CEDC0C6};
    const std::array<const char*,2> table_second_labels={"table-second-entry","table-second-exit"};
    table_second_observer=[&](unsigned which){if(which==table_second_seen&&ctx.pc==table_second_addresses[which]&&ctx.pr==0x8CEDC0C6&&!rt.sh4_async_redirect){capture(table_second_labels[which]);++table_second_seen;}};
    struct RestoreTableSecond{~RestoreTableSecond(){table_second_observer={};}}restore_table_second;
#endif
#ifdef RECOVERY_MOVIE_TABLE_THIRD
    unsigned table_third_seen=0;
    const std::array<std::uint32_t,2> table_third_addresses={0x8CEDC256,0x8CEDC0CA};
    const std::array<const char*,2> table_third_labels={"table-third-entry","table-third-exit"};
    table_third_observer=[&](unsigned which){if(which==table_third_seen&&ctx.pc==table_third_addresses[which]&&ctx.pr==0x8CEDC0CA&&!rt.sh4_async_redirect){capture(table_third_labels[which]);++table_third_seen;}};
    struct RestoreTableThird{~RestoreTableThird(){table_third_observer={};}}restore_table_third;
#endif
#ifdef RECOVERY_MOVIE_TABLE_FOURTH
    unsigned table_fourth_seen=0;
    const std::array<std::uint32_t,2> table_fourth_addresses={0x8CEDC2F4,0x8CEDC0CE};
    const std::array<const char*,2> table_fourth_labels={"table-fourth-entry","table-fourth-exit"};
    table_fourth_observer=[&](unsigned which){if(which==table_fourth_seen&&ctx.pc==table_fourth_addresses[which]&&ctx.pr==0x8CEDC0CE&&!rt.sh4_async_redirect){capture(table_fourth_labels[which]);++table_fourth_seen;}};
    struct RestoreTableFourth{~RestoreTableFourth(){table_fourth_observer={};}}restore_table_fourth;
#endif
#ifdef RECOVERY_MOVIE_WORK_FOLLOWUP
    unsigned work_followup_seen=0;
    const std::array<std::uint32_t,2> work_followup_addresses={0x8CEDA920,0x8CED80FC};
    const std::array<const char*,2> work_followup_labels={"work-followup-entry","work-followup-exit"};
    work_followup_observer=[&](unsigned which){if(which==work_followup_seen&&ctx.pc==work_followup_addresses[which]&&ctx.pr==0x8CED80FC&&!rt.sh4_async_redirect){capture(work_followup_labels[which]);++work_followup_seen;}};
    struct RestoreWorkFollowup{~RestoreWorkFollowup(){work_followup_observer={};}}restore_work_followup;
#endif
#ifdef RECOVERY_MOVIE_WORK_TRANSFORM
    unsigned work_final_seen=0;
    const std::array<std::uint32_t,2> work_final_addresses={0x8CEDA9A4,0x8CED8104};
    const std::array<const char*,2> work_final_labels={"work-final-entry","work-final-exit"};
    work_final_observer=[&](unsigned which){if(which==work_final_seen&&ctx.pc==work_final_addresses[which]&&ctx.pr==0x8CED8104&&!rt.sh4_async_redirect){capture(work_final_labels[which]);++work_final_seen;}};
    struct RestoreWorkFinal{~RestoreWorkFinal(){work_final_observer={};}}restore_work_final;
#endif
#ifdef RECOVERY_MOVIE_INITIALIZER_RETURN
    unsigned initializer_return_seen=0;
    const std::array<std::uint32_t,8> initializer_return_addresses={0x8CED8216,0x8CED8108,0x8CEDA7C4,0x8CED8110,0x8CED2574,0x8CEDA2E6,0x8CED5038,0x8CEDA2E6};
    const std::array<std::uint32_t,8> initializer_return_pr={0x8CED8108,0x8CED8108,0x8CED8110,0x8CED8110,0x8CED2574,0x8CEDA2E6,0x8CEDA2E6,0x8CEDA2E6};
    const std::array<const char*,8> initializer_return_labels={"post-work-entry","post-work-return","final-setup-entry","final-setup-exit","third-setup-return","third-callback-return","fourth-callback-entry","fourth-callback-return"};
    initializer_return_observer=[&](unsigned which){if(which==initializer_return_seen&&which<8&&ctx.pc==initializer_return_addresses[which]&&ctx.pr==initializer_return_pr[which]&&!rt.sh4_async_redirect){capture(initializer_return_labels[which]);++initializer_return_seen;}};
    struct RestoreInitializerReturn{~RestoreInitializerReturn(){initializer_return_observer={};}}restore_initializer_return;
#endif
#ifdef RECOVERY_MOVIE_BINDING_RETURN
    unsigned binding_return_seen=0;
    const std::array<std::uint32_t,7> binding_return_addresses={0x8CED0EEE,0x8CEDA2E6,0x8CED16D4,0x8CEDA2E6,0x8CEDA29E,0x8CED19AC,0x8CED18EA};
    const std::array<std::uint32_t,7> binding_return_pr={0x8CEDA2E6,0x8CEDA2E6,0x8CEDA2E6,0x8CEDA2E6,0x8CEDA29E,0x8CED19AC,0x8CED18EA};
    const std::array<const char*,7> binding_return_labels={"fifth-callback-entry","fifth-callback-return","sixth-callback-entry","sixth-callback-return","binding-loop-return","interface-wrapper-return","descriptor-fields-return"};
    binding_return_observer=[&](unsigned which){if(which==binding_return_seen&&which<7&&ctx.pc==binding_return_addresses[which]&&ctx.pr==binding_return_pr[which]&&!rt.sh4_async_redirect){capture(binding_return_labels[which]);++binding_return_seen;}};
    struct RestoreBindingReturn{~RestoreBindingReturn(){binding_return_observer={};}}restore_binding_return;
#endif
#ifdef RECOVERY_MOVIE_GUARD_FINAL
    unsigned guard_final_seen=0;
    const std::array<std::uint32_t,5> guard_final_addresses={0x8CED38A0,0x8CED18FC,0x8CED1BC4,0x8CED1900,0x8CED0390};
    const std::array<std::uint32_t,5> guard_final_pr={0x8CED18FC,0x8CED18FC,0x8CED1900,0x8CED1900,0x8CED0390};
    const std::array<const char*,5> guard_final_labels={"signature-guard-entry","signature-guard-return","descriptor-final-entry","descriptor-final-return","descriptor-initializer-return"};
    guard_final_observer=[&](unsigned which){if(which==guard_final_seen&&which<5&&ctx.pc==guard_final_addresses[which]&&ctx.pr==guard_final_pr[which]&&!rt.sh4_async_redirect){capture(guard_final_labels[which]);++guard_final_seen;}};
    struct RestoreGuardFinal{~RestoreGuardFinal(){guard_final_observer={};}}restore_guard_final;
#endif
#ifdef RECOVERY_MOVIE_SETUP_BIND
    unsigned setup_bind_seen=0;
    const std::array<std::uint32_t,5> setup_bind_addresses={0x8CED1AFA,0x8CED1B5E,0x8CED1B54,0x8CED03A6,0x8CECF188};
    const std::array<std::uint32_t,5> setup_bind_pr={0x8CED03A6,0x8CED1B54,0x8CED1B54,0x8CED03A6,0x8CECF188};
    const std::array<const char*,5> setup_bind_labels={"setup-bind-entry","setup-bind-child-entry","setup-bind-child-return","setup-bind-return","setup-validation-return"};
    setup_bind_observer=[&](unsigned which){if(which==setup_bind_seen&&which<5&&ctx.pc==setup_bind_addresses[which]&&ctx.pr==setup_bind_pr[which]&&!rt.sh4_async_redirect){capture(setup_bind_labels[which]);++setup_bind_seen;}};
    struct RestoreSetupBind{~RestoreSetupBind(){setup_bind_observer={};}}restore_setup_bind;
#endif
#ifdef RECOVERY_MOVIE_CREATE
    unsigned create_seen=0;
    const std::array<std::uint32_t,11> create_addresses={0x8ced03f8,0x8ced01bc,0x8ced042e,0x8ced0256,0x8ced0222,0x8ced0274,0x8ced043c,0x8ced0222,0x8ced04dc,0x8c10e964,0x8ced052c};
    const std::array<std::uint32_t,11> create_pr={0x8cecf1b0,0x8ced042e,0x8ced042e,0x8ced043c,0x8ced0274,0x8ced0274,0x8ced043c,0x8ced04dc,0x8ced04dc,0x8ced052c,0x8ced052c};
    const std::array<const char*,11> create_labels={"create-entry","create-stream-entry","create-stream-return","create-frame-entry","create-frame-dimensions-entry","create-frame-dimensions-return","create-frame-return","create-layout-dimensions-entry","create-layout-dimensions-return","create-copy-entry","create-copy-return"};
    create_observer=[&]{if(create_seen<11&&ctx.pc==create_addresses[create_seen]&&ctx.pr==create_pr[create_seen]&&!rt.sh4_async_redirect){capture(create_labels[create_seen]);++create_seen;}};
    struct RestoreCreate{~RestoreCreate(){create_observer={};}}restore_create;
#endif
#ifdef RECOVERY_MOVIE_CREATE_INIT
    unsigned create_init_seen=0;
    const std::array<std::uint32_t,4> create_init_addresses={0x8ced2538,0x8ced05b2,0x8ced0ec4,0x8ced05ba};
    const std::array<std::uint32_t,4> create_init_pr={0x8ced05b2,0x8ced05b2,0x8ced05ba,0x8ced05ba};
    const std::array<const char*,4> create_init_labels={"create-init-entry","create-init-return","create-second-init-entry","create-second-init-return"};
    create_init_observer=[&]{if(create_init_seen<4&&ctx.pc==create_init_addresses[create_init_seen]&&ctx.pr==create_init_pr[create_init_seen]&&!rt.sh4_async_redirect){capture(create_init_labels[create_init_seen]);++create_init_seen;}};
    struct RestoreCreateInit{~RestoreCreateInit(){create_init_observer={};}}restore_create_init;
#endif
#ifdef RECOVERY_MOVIE_INSTANCE
    unsigned instance_seen=0;
    const std::array<std::uint32_t,5> instance_addresses={0x8ced3dc2,0x8ced3dfe,0x8ced3dce,0x8ced3e0e,0x8ced3de2};
    const std::array<std::uint32_t,5> instance_pr={0x8ced05c4,0x8ced3dce,0x8ced3dce,0x8ced3de2,0x8ced3de2};
    const std::array<const char*,5> instance_labels={"instance-entry","instance-guard-entry","instance-guard-return","instance-pool-entry","instance-pool-return"};
    instance_observer=[&]{if(instance_seen<5&&ctx.pc==instance_addresses[instance_seen]&&ctx.pr==instance_pr[instance_seen]&&!rt.sh4_async_redirect){capture(instance_labels[instance_seen]);++instance_seen;}};
    struct RestoreInstance{~RestoreInstance(){instance_observer={};}}restore_instance;
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_INITIALIZE
    unsigned instance_initialize_seen=0;
    const std::array<std::uint32_t,9> instance_initialize_addresses={0x8ced3e54,0x8ced67f8,0x8ced3e6c,0x8c10e964,0x8ced3e80,0x8ced3ef8,0x8ced67f8,0x8ced3f06,0x8ced3e9a};
    const std::array<std::uint32_t,9> instance_initialize_pr={0x8ced3df6,0x8ced3e6c,0x8ced3e6c,0x8ced3e80,0x8ced3e80,0x8ced3e9a,0x8ced3f06,0x8ced3f06,0x8ced3e9a};
    const std::array<const char*,9> instance_initialize_labels={"initialize-entry","initialize-fill-entry","initialize-fill-return","initialize-copy-entry","initialize-copy-return","initialize-fields-entry","initialize-fields-fill-entry","initialize-fields-fill-return","initialize-fields-return"};
    instance_initialize_observer=[&]{if(instance_initialize_seen<9&&ctx.pc==instance_initialize_addresses[instance_initialize_seen]&&ctx.pr==instance_initialize_pr[instance_initialize_seen]&&!rt.sh4_async_redirect){capture(instance_initialize_labels[instance_initialize_seen]);++instance_initialize_seen;}};
    struct RestoreInstanceInitialize{~RestoreInstanceInitialize(){instance_initialize_observer={};}}restore_instance_initialize;
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_RATE
    unsigned second_fields_seen=0;
    const std::array<std::uint32_t,9> second_fields_addresses={0x8ced3f22,0x8ced67f8,0x8ced3f30,0x8ced3ea0,0x8ced1a94,0x8ced3ea8,0x8ced6708,0x8ced3eb4,0x8ced462e};
    const std::array<std::uint32_t,9> second_fields_pr={0x8ced3ea0,0x8ced3f30,0x8ced3f30,0x8ced3ea0,0x8ced3ea8,0x8ced3ea8,0x8ced3eb4,0x8ced3eb4,0x8ced3ebe};
    const std::array<const char*,9> second_fields_labels={"second-fields-entry","second-fields-fill-entry","second-fields-fill-return","second-fields-return","second-record-entry","second-record-return","second-copy-entry","second-copy-return","instance-rate-entry"};
    second_fields_observer=[&]{if(second_fields_seen<9&&ctx.pc==second_fields_addresses[second_fields_seen]&&ctx.pr==second_fields_pr[second_fields_seen]&&!rt.sh4_async_redirect){capture(second_fields_labels[second_fields_seen]);++second_fields_seen;}};
    struct RestoreSecondFields{~RestoreSecondFields(){second_fields_observer={};}}restore_second_fields;
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_RATE_RETURN
    unsigned instance_rate_return_seen=0;
    const std::array<std::uint32_t,5> instance_rate_return_addresses={0x8ced4a54,0x8ced463e,0x8ced46de,0x8ced4668,0x8ced3ebe};
    const std::array<std::uint32_t,5> instance_rate_return_pr={0x8ced463e,0x8ced463e,0x8ced4668,0x8ced4668,0x8ced3ebe};
    const std::array<const char*,5> instance_rate_return_labels={"instance-handler-entry","instance-handler-first-return","instance-rate-fields-entry","instance-rate-fields-return","instance-rate-return"};
    instance_rate_return_observer=[&]{if(instance_rate_return_seen<5&&ctx.pc==instance_rate_return_addresses[instance_rate_return_seen]&&ctx.pr==instance_rate_return_pr[instance_rate_return_seen]&&!rt.sh4_async_redirect){capture(instance_rate_return_labels[instance_rate_return_seen]);++instance_rate_return_seen;}};
    struct RestoreInstanceRateReturn{~RestoreInstanceRateReturn(){instance_rate_return_observer={};}}restore_instance_rate_return;
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_STREAM
    unsigned instance_stream_seen=0;
    const std::array<std::uint32_t,5> instance_stream_addresses={0x8ced97b0,0x8ced984e,0x8ced97ce,0x8c10e7d8,0x8ced97d6};
    const std::array<std::uint32_t,5> instance_stream_pr={0x8ced3eca,0x8ced97ce,0x8ced97ce,0x8ced97d6,0x8ced97d6};
    const std::array<const char*,5> instance_stream_labels={"instance-stream-entry","instance-descriptor-entry","instance-descriptor-return","instance-remainder-entry","instance-remainder-return"};
    instance_stream_observer=[&]{if(instance_stream_seen<5&&ctx.pc==instance_stream_addresses[instance_stream_seen]&&ctx.pr==instance_stream_pr[instance_stream_seen]&&!rt.sh4_async_redirect){capture(instance_stream_labels[instance_stream_seen]);++instance_stream_seen;}};
    real_instance_remainder=rt.targets.at(0x8C10E7D8);rt.register_target(0x8C10E7D8,observed_instance_remainder);
    struct RestoreInstanceStream{DCRuntime&rt;~RestoreInstanceStream(){rt.register_target(0x8C10E7D8,real_instance_remainder);real_instance_remainder=nullptr;instance_stream_observer={};}}restore_instance_stream{rt};
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_RECORDS
    unsigned instance_records_seen=0;
    const std::array<std::uint32_t,8> instance_records_addresses={0x8ced9864,0x8ced97e8,0x8ced9864,0x8ced97f8,0x8ced9864,0x8ced9808,0x8ced9918,0x8ced9812};
    const std::array<std::uint32_t,8> instance_records_pr={0x8ced97e8,0x8ced97e8,0x8ced97f8,0x8ced97f8,0x8ced9808,0x8ced9808,0x8ced9812,0x8ced9812};
    const std::array<const char*,8> instance_records_labels={"instance-record-entry","instance-record-return","instance-record-second-entry","instance-record-second-return","instance-record-third-entry","instance-record-third-return","instance-small-record-entry","instance-small-record-return"};
    instance_records_observer=[&]{if(instance_records_seen<8&&ctx.pc==instance_records_addresses[instance_records_seen]&&ctx.pr==instance_records_pr[instance_records_seen]&&!rt.sh4_async_redirect){capture(instance_records_labels[instance_records_seen]);++instance_records_seen;}};
    struct RestoreInstanceRecords{~RestoreInstanceRecords(){instance_records_observer={};}}restore_instance_records;
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_STREAM_RETURN
    unsigned stream_return_seen=0;
    const std::array<std::uint32_t,9> stream_return_addresses={0x8ced9978,0x8ced981c,0x8ced9918,0x8ced9826,0x8ced9978,0x8ced9830,0x8ced9864,0x8ced9840,0x8ced3eca};
    const std::array<std::uint32_t,9> stream_return_pr={0x8ced981c,0x8ced981c,0x8ced9826,0x8ced9826,0x8ced9830,0x8ced9830,0x8ced9840,0x8ced9840,0x8ced3eca};
    const std::array<const char*,9> stream_return_labels={"instance-ring-entry","instance-ring-return","stream-tail-small-entry","stream-tail-small-return","stream-tail-ring-entry","stream-tail-ring-return","stream-tail-record-entry","stream-tail-record-return","instance-stream-return"};
    stream_return_observer=[&]{if(stream_return_seen<9&&ctx.pc==stream_return_addresses[stream_return_seen]&&ctx.pr==stream_return_pr[stream_return_seen]&&!rt.sh4_async_redirect){capture(stream_return_labels[stream_return_seen]);++stream_return_seen;}};
    struct RestoreStreamReturn{~RestoreStreamReturn(){stream_return_observer={};}}restore_stream_return;
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_TABLE
    bool instance_table_seen=false;
    instance_table_observer=[&]{if(!instance_table_seen&&ctx.pc==0x8ceda304&&ctx.pr==0x8ced3ed8&&!rt.sh4_async_redirect){capture("instance-table-entry");instance_table_seen=true;}};
    struct RestoreInstanceTable{~RestoreInstanceTable(){instance_table_observer={};}}restore_instance_table;
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_TABLE_RECORDS
    unsigned table_records_seen=0;
    const std::array<const char*,18> table_records_labels={"instance-table-record-0-entry","instance-table-record-0-return","instance-table-record-1-entry","instance-table-record-1-return","instance-table-record-2-entry","instance-table-record-2-return","instance-table-record-3-entry","instance-table-record-3-return","instance-table-record-4-entry","instance-table-record-4-return","instance-table-record-5-entry","instance-table-record-5-return","instance-table-record-6-entry","instance-table-record-6-return","instance-table-record-7-entry","instance-table-record-7-return","instance-table-record-8-entry","instance-table-record-8-return"};
    table_records_observer=[&]{if(table_records_seen<18&&ctx.pc==(table_records_seen%2?0x8ceda32a:0x8ceda362)&&ctx.pr==0x8ceda32a&&!rt.sh4_async_redirect){capture(table_records_labels[table_records_seen]);++table_records_seen;}};
    struct RestoreTableRecords{~RestoreTableRecords(){table_records_observer={};}}restore_table_records;
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_SELECTOR
    unsigned instance_selector_seen=0;
    const std::array<std::uint32_t,3> instance_selector_pc={0x8ceda388,0x8ceda518,0x8ceda3a2};
    const std::array<std::uint32_t,3> instance_selector_pr={0x8ceda336,0x8ceda3a2,0x8ceda3a2};
    const std::array<const char*,3> instance_selector_labels={"instance-table-final-entry","instance-selector-entry","instance-selector-return"};
    instance_selector_observer=[&]{if(instance_selector_seen<3&&ctx.pc==instance_selector_pc[instance_selector_seen]&&ctx.pr==instance_selector_pr[instance_selector_seen]&&!rt.sh4_async_redirect){capture(instance_selector_labels[instance_selector_seen]);++instance_selector_seen;}};
    struct RestoreInstanceSelector{~RestoreInstanceSelector(){instance_selector_observer={};}}restore_instance_selector;
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_SECOND_SELECTOR
    unsigned second_selector_seen=0;
    const std::array<std::uint32_t,5> second_selector_pc={0x8ceda42c,0x8ceda57c,0x8ceda43c,0x8ceda524,0x8ceda44a};
    const std::array<std::uint32_t,5> second_selector_pr={0x8ceda3a8,0x8ceda43c,0x8ceda43c,0x8ceda44a,0x8ceda44a};
    const std::array<const char*,5> second_selector_labels={"instance-branch-entry","instance-branch-internal-entry","instance-branch-internal-return","instance-second-selector-entry","instance-second-selector-return"};
    second_selector_observer=[&]{if(second_selector_seen<5&&ctx.pc==second_selector_pc[second_selector_seen]&&ctx.pr==second_selector_pr[second_selector_seen]&&!rt.sh4_async_redirect){capture(second_selector_labels[second_selector_seen]);++second_selector_seen;}};
    struct RestoreSecondSelector{~RestoreSecondSelector(){second_selector_observer={};}}restore_second_selector;
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_NEXT_BRANCH
    unsigned next_branch_seen=0;
    const std::array<std::uint32_t,6> next_branch_pc={0x8ceda486,0x8ceda57c,0x8ceda498,0x8ceda518,0x8ceda4a0,0x8ceda450};
    const std::array<std::uint32_t,6> next_branch_pr={0x8ceda450,0x8ceda498,0x8ceda498,0x8ceda4a0,0x8ceda4a0,0x8ceda450};
    const std::array<const char*,6> next_branch_labels={"instance-next-branch-entry","instance-next-helper-entry","instance-next-helper-return","instance-next-selector-entry","instance-next-selector-return","instance-next-branch-return"};
    next_branch_observer=[&]{if(next_branch_seen<6&&ctx.pc==next_branch_pc[next_branch_seen]&&ctx.pr==next_branch_pr[next_branch_seen]&&!rt.sh4_async_redirect){capture(next_branch_labels[next_branch_seen]);++next_branch_seen;}};
    struct RestoreNextBranch{~RestoreNextBranch(){next_branch_observer={};}}restore_next_branch;
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_TABLE_RETURN
    unsigned table_return_seen=0;
    const std::array<std::uint32_t,11> table_return_pc={0x8ceda530,0x8ceda45e,0x8ceda4cc,0x8ceda57c,0x8ceda4de,0x8ceda518,0x8ceda4e6,0x8ceda464,0x8ceda3a8,0x8ceda336,0x8ced3ed8};
    const std::array<std::uint32_t,11> table_return_pr={0x8ceda45e,0x8ceda45e,0x8ceda464,0x8ceda4de,0x8ceda4de,0x8ceda4e6,0x8ceda4e6,0x8ceda464,0x8ceda3a8,0x8ceda336,0x8ced3ed8};
    const std::array<const char*,11> table_return_labels={"instance-third-selector-entry","instance-third-selector-return","instance-third-branch-entry","instance-third-helper-entry","instance-third-helper-return","instance-third-branch-selector-entry","instance-third-branch-selector-return","instance-third-branch-return","instance-enclosing-branch-return","instance-table-final-return","instance-table-initializer-return"};
    table_return_observer=[&]{if(table_return_seen<11&&ctx.pc==table_return_pc[table_return_seen]&&ctx.pr==table_return_pr[table_return_seen]&&!rt.sh4_async_redirect){capture(table_return_labels[table_return_seen]);++table_return_seen;}};
    struct RestoreTableReturn{~RestoreTableReturn(){table_return_observer={};}}restore_table_return;
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_DISPATCH
    unsigned instance_dispatch_seen=0;
    instance_dispatch_observer=[&]{const unsigned pcs[]={0x8ced3fea,0x8ceda59e};const char*labels[]={"instance-final-thunk-entry","instance-handler-dispatch-entry"};if(instance_dispatch_seen<2&&ctx.pc==pcs[instance_dispatch_seen]&&ctx.pr==0x8ced3edc&&!rt.sh4_async_redirect){capture(labels[instance_dispatch_seen]);++instance_dispatch_seen;}};
    struct RestoreInstanceDispatch{~RestoreInstanceDispatch(){instance_dispatch_observer={};}}restore_instance_dispatch;
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_HANDLERS
    unsigned instance_handlers_seen=0;
    instance_handlers_observer=[&]{const unsigned pcs[]={0x8ced1c36,0x8ceda5da,0x8ced2444,0x8ced24a2,0x8ced245e};const unsigned prs[]={0x8ceda5da,0x8ceda5da,0x8ceda5da,0x8ced245e,0x8ced245e};const char*labels[]={"instance-first-handler-entry","instance-first-handler-return","instance-second-handler-entry","instance-handler-clear-entry","instance-handler-clear-return"};if(instance_handlers_seen<5&&ctx.pc==pcs[instance_handlers_seen]&&ctx.pr==prs[instance_handlers_seen]&&!rt.sh4_async_redirect){capture(labels[instance_handlers_seen]);++instance_handlers_seen;}};
    struct RestoreInstanceHandlers{~RestoreInstanceHandlers(){instance_handlers_observer={};}}restore_instance_handlers;
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_RESOURCES
    unsigned instance_resources_seen=0;
    instance_resources_observer=[&]{const unsigned pcs[]={0x8ced7adc,0x8ced7b0c,0x8ced7ae2,0x8ced7b3e,0x8ced67f8,0x8ced7b4c,0x8ced79a8,0x8ced7b56};const unsigned prs[]={0x8ced2464,0x8ced7ae2,0x8ced7ae2,0x8ced7af2,0x8ced7b4c,0x8ced7b4c,0x8ced7b56,0x8ced7b56};const char*labels[]={"instance-resource-wrapper-entry","instance-resource-search-entry","instance-resource-search-return","instance-resource-init-entry","instance-resource-fill-entry","instance-resource-fill-return","instance-resource-record-entry","instance-resource-record-return"};if(instance_resources_seen<8&&ctx.pc==pcs[instance_resources_seen]&&ctx.pr==prs[instance_resources_seen]&&!rt.sh4_async_redirect){capture(labels[instance_resources_seen]);++instance_resources_seen;}};
    struct RestoreInstanceResources{~RestoreInstanceResources(){instance_resources_observer={};}}restore_instance_resources;
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_RESOURCE_COMPLETE
    unsigned resource_complete_seen=0;
    resource_complete_observer=[&]{const unsigned pcs[]={0x8ced7b82,0x8ced7b5c,0x8ced7b8c,0x8ced7b62,0x8ced7b68,0x8ced7b6e,0x8ced7b74,0x8ced7ba0,0x8ced7b7a,0x8ced7af2,0x8ced2464};const unsigned prs[]={0x8ced7b5c,0x8ced7b5c,0x8ced7b62,0x8ced7b62,0x8ced7b68,0x8ced7b6e,0x8ced7b74,0x8ced7b7a,0x8ced7b7a,0x8ced7af2,0x8ced2464};const char*labels[]={"resource-sentinels-entry","resource-sentinels-return","resource-blocks-entry","resource-block0-return","resource-block1-return","resource-block2-return","resource-block3-return","resource-final-entry","resource-final-return","resource-initializer-return","resource-wrapper-return"};if(resource_complete_seen<11&&ctx.pc==pcs[resource_complete_seen]&&ctx.pr==prs[resource_complete_seen]&&!rt.sh4_async_redirect){capture(labels[resource_complete_seen]);++resource_complete_seen;}};
    struct RestoreResourceComplete{~RestoreResourceComplete(){resource_complete_observer={};}}restore_resource_complete;
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_VIDEO_COMPLETE
    unsigned video_complete_seen=0;
    video_complete_observer=[&]{const unsigned pcs[]={0x8ced7a2c,0x8ced7ac4,0x8ced7a50,0x8ced7a74,0x8ced7a6a,0x8ced247a,0x8ceda5da,0x8ced2f4c,0x8ced44fc,0x8ced2f5c,0x8ced2fda,0x8c10e964,0x8ced3008,0x8ced3390,0x8ced1bcc,0x8ced33a8,0x8ced1be4,0x8ced3482,0x8ced3014,0x8ced2f72};const unsigned prs[]={0x8ced247a,0x8ced7a50,0x8ced7a50,0x8ced7a6a,0x8ced7a6a,0x8ced247a,0x8ceda5da,0x8ceda5da,0x8ced2f5c,0x8ced2f5c,0x8ced2f72,0x8ced3008,0x8ced3008,0x8ced3014,0x8ced33a8,0x8ced33a8,0x8ced3482,0x8ced3482,0x8ced3014,0x8ced2f72};const char*labels[]={"binding-entry","binding-validation-entry","binding-validation-return","binding-store-entry","binding-store-return","binding-return","second-handler-return","third-handler-entry","video-query-entry","video-query-return","video-setup-entry","video-copy-entry","video-copy-return","video-layout-entry","video-mask-entry","video-mask-return","video-restore-entry","video-restore-return","video-layout-return","video-setup-return"};if(video_complete_seen<20&&ctx.pc==pcs[video_complete_seen]&&ctx.pr==prs[video_complete_seen]&&!rt.sh4_async_redirect){capture(labels[video_complete_seen]);++video_complete_seen;}};
    struct RestoreVideoComplete{~RestoreVideoComplete(){video_complete_observer={};}}restore_video_complete;
#endif
    auto dispatch_original=[&]{
#ifdef RECOVERY_MOVIE_ENTRY
        try{call_recompiled(ctx,rt,ctx.pc);}catch(const MovieActivationNeeded&){
            require(!movie_active&&movie_exit_seen&&ctx.pc==0x8C10D492&&ctx.pr==0x8C10D492&&!rt.sh4_async_redirect&&rt.code_module_dispatch_depth==0,"Movie activation is not at a quiescent original return");
            const auto cycles=rt.device_clock_sh4_cycles;
            std::vector<CodeModuleEntry> entries(std::begin(movie_entry_entries),std::end(movie_entry_entries));for(auto&e:entries)e.function=observed_movie_entry;
#ifdef RECOVERY_MOVIE_INSTANCE_VIDEO_COMPLETE
            for(const auto&e:movie_instance_binding_prefix_entries)entries.push_back({e.address,observed_video_binding_prefix});
            for(const auto&e:movie_instance_binding_validate_entries)entries.push_back({e.address,observed_video_binding_validate});
            for(const auto&e:movie_instance_binding_store_entries)entries.push_back({e.address,observed_video_binding_store});
            for(const auto&e:movie_instance_third_handler_entries)entries.push_back({e.address,observed_video_third_handler});
            for(const auto&e:movie_instance_indexed_query_entries)entries.push_back({e.address,observed_video_indexed_query});
            for(const auto&e:movie_instance_video_setup_entries)entries.push_back({e.address,observed_video_video_setup});
            for(const auto&e:movie_instance_video_layout_entries)entries.push_back({e.address,observed_video_video_layout});
            for(const auto&e:movie_instance_video_geometry_entries)entries.push_back({e.address,observed_video_video_geometry});
            for(const auto&e:movie_instance_video_return_entries)entries.push_back({e.address,observed_video_video_return});
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_RESOURCE_COMPLETE
            for(const auto&e:movie_instance_resource_sentinels_entries)entries.push_back({e.address,observed_resource_sentinels});
            for(const auto&e:movie_instance_resource_blocks_entries)entries.push_back({e.address,observed_resource_blocks});
            for(const auto&e:movie_instance_resource_final_entries)entries.push_back({e.address,observed_resource_final});
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_RESOURCES
            for(const auto&e:movie_instance_resource_wrapper_entries)entries.push_back({e.address,observed_instance_resource_wrapper});
            for(const auto&e:movie_instance_resource_search_entries)entries.push_back({e.address,observed_instance_resource_search});
            for(const auto&e:movie_instance_resource_init_entries)entries.push_back({e.address,observed_instance_resource_init});
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_HANDLERS
            for(const auto&e:movie_instance_first_handler_entries)entries.push_back({e.address,observed_instance_first_handler});
            for(const auto&e:movie_instance_second_handler_entries)entries.push_back({e.address,observed_instance_second_handler});
            for(const auto&e:movie_instance_handler_clear_entries)entries.push_back({e.address,observed_instance_handler_clear});
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_DISPATCH
            for(const auto&e:movie_instance_final_thunk_entries)entries.push_back({e.address,observed_instance_final_thunk});
            for(const auto&e:movie_instance_handler_dispatch_entries)entries.push_back({e.address,observed_instance_handler_dispatch});
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_TABLE_RETURN
            for(const auto&e:movie_instance_third_selector_entries)entries.push_back({e.address,observed_instance_third_selector});
            for(const auto&e:movie_instance_third_branch_entries)entries.push_back({e.address,observed_instance_third_branch});
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_NEXT_BRANCH
            for(const auto&e:movie_instance_next_branch_entries)entries.push_back({e.address,observed_instance_next_branch});
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_SECOND_SELECTOR
            for(const auto&e:movie_instance_branch_entries)entries.push_back({e.address,observed_instance_branch});
            for(const auto&e:movie_instance_second_selector_entries)entries.push_back({e.address,observed_instance_second_selector});
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_SELECTOR
            for(const auto&e:movie_instance_table_final_prefix_entries)entries.push_back({e.address,observed_instance_table_final});
            for(const auto&e:movie_instance_selector_entries)entries.push_back({e.address,observed_instance_selector});
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_TABLE_RECORDS
            for(const auto&e:movie_instance_table_record_entries)entries.push_back({e.address,observed_instance_table_record});
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_TABLE
            for(const auto&e:movie_instance_table_prefix_entries)entries.push_back({e.address,observed_instance_table});
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_STREAM_RETURN
            for(const auto&e:movie_instance_ring_record_entries)entries.push_back({e.address,observed_instance_ring_record});
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_RECORDS
            for(const auto&e:movie_instance_stream_record_entries)entries.push_back({e.address,observed_instance_record});
            for(const auto&e:movie_instance_small_record_entries)entries.push_back({e.address,observed_instance_small_record});
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_STREAM
            for(const auto&e:movie_instance_stream_prefix_entries)entries.push_back({e.address,observed_instance_stream});
            for(const auto&e:movie_instance_stream_descriptor_entries)entries.push_back({e.address,observed_instance_descriptor});
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_RATE_RETURN
            for(const auto&e:movie_instance_handler_entries)entries.push_back({e.address,observed_instance_handler});
            for(const auto&e:movie_instance_rate_fields_entries)entries.push_back({e.address,observed_instance_rate_fields});
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_RATE
            for(const auto&e:movie_instance_second_fields_entries)entries.push_back({e.address,observed_second_fields});
            for(const auto&e:movie_instance_rate_entries)entries.push_back({e.address,observed_instance_rate});
#endif
#ifdef RECOVERY_MOVIE_INSTANCE_INITIALIZE
            for(const auto&e:movie_instance_initialize_entries)entries.push_back({e.address,observed_instance_initialize});
            for(const auto&e:movie_instance_fields_entries)entries.push_back({e.address,observed_instance_fields});
#endif
#ifdef RECOVERY_MOVIE_INSTANCE
            for(const auto&e:movie_create_instance_entries)entries.push_back({e.address,observed_instance});
            for(const auto&e:movie_create_instance_guard_entries)entries.push_back({e.address,observed_instance_guard});
            for(const auto&e:movie_instance_pool_entries)entries.push_back({e.address,observed_instance_pool});
#endif
#ifdef RECOVERY_MOVIE_CREATE_INIT
            for(const auto&e:movie_create_init_entries)entries.push_back({e.address,observed_create_init});
            for(const auto&e:movie_create_second_init_entries)entries.push_back({e.address,observed_create_second_init});
#endif
#ifdef RECOVERY_MOVIE_CREATE
            for(const auto&e:movie_create_prefix_entries)entries.push_back({e.address,observed_movie_create});
#endif
#ifdef RECOVERY_MOVIE_SETUP_BIND
            for(const auto&e:movie_setup_bind_prefix_entries)entries.push_back({e.address,observed_setup_bind});
            for(const auto&e:movie_setup_bind_child_entries)entries.push_back({e.address,observed_setup_bind_child});
#endif
#ifdef RECOVERY_MOVIE_GUARD_FINAL
            for(const auto&e:movie_signature_guard_entries)entries.push_back({e.address,observed_signature_guard});
            for(const auto&e:movie_descriptor_final_entries)entries.push_back({e.address,observed_descriptor_final});
#endif
#ifdef RECOVERY_MOVIE_BINDING_RETURN
            for(const auto&e:movie_fifth_callback_entries)entries.push_back({e.address,observed_fifth_callback});
            for(const auto&e:movie_sixth_callback_entries)entries.push_back({e.address,observed_sixth_callback});
#endif
#ifdef RECOVERY_MOVIE_INITIALIZER_RETURN
            for(const auto&e:movie_post_work_entries)entries.push_back({e.address,observed_post_work});
            for(const auto&e:movie_post_buffer_entries)entries.push_back(e);
            for(const auto&e:movie_final_setup_entries)entries.push_back({e.address,observed_final_setup});
            for(const auto&e:movie_fourth_callback_entries)entries.push_back({e.address,observed_fourth_callback});
#endif
#ifdef RECOVERY_MOVIE_WORK_TRANSFORM
            for(const auto&e:movie_work_final_entries)entries.push_back({e.address,observed_work_final});
            for(const auto&e:movie_work_transform_entries)entries.push_back(e);
#endif
#ifdef RECOVERY_MOVIE_WORK_FOLLOWUP
            for(const auto&e:movie_work_followup_entries)entries.push_back({e.address,observed_work_followup});
#endif
#ifdef RECOVERY_MOVIE_TABLE_FOURTH
            for(const auto&e:movie_table_fourth_entries)entries.push_back({e.address,observed_table_fourth});
#endif
#ifdef RECOVERY_MOVIE_TABLE_THIRD
            for(const auto&e:movie_table_third_entries)entries.push_back({e.address,observed_table_third});
#endif
#ifdef RECOVERY_MOVIE_TABLE_SECOND
            for(const auto&e:movie_table_second_entries)entries.push_back({e.address,observed_table_second});
#endif
#ifdef RECOVERY_MOVIE_TABLE_FIRST
            for(const auto&e:movie_table_first_entries)entries.push_back({e.address,observed_table_first});
#endif
#ifdef RECOVERY_MOVIE_TABLE_TAIL
            for(const auto&e:movie_table_tail_entries)entries.push_back({e.address,observed_table_tail});
#endif
#ifdef RECOVERY_MOVIE_ELEVENTH_TABLE
            for(const auto&e:movie_eleventh_table_entries)entries.push_back({e.address,observed_eleventh_table});
#endif
#ifdef RECOVERY_MOVIE_TENTH_TABLE
            for(const auto&e:movie_tenth_table_entries)entries.push_back({e.address,observed_tenth_table});
#endif
#ifdef RECOVERY_MOVIE_NINTH_TABLE
            for(const auto&e:movie_ninth_table_entries)entries.push_back({e.address,observed_ninth_table});
#endif
#ifdef RECOVERY_MOVIE_EIGHTH_TABLE
            for(const auto&e:movie_eighth_table_entries)entries.push_back({e.address,observed_eighth_table});
#endif
#ifdef RECOVERY_MOVIE_SEVENTH_TABLE
            for(const auto&e:movie_seventh_table_entries)entries.push_back({e.address,observed_seventh_table});
#endif
#ifdef RECOVERY_MOVIE_SIXTH_TABLE
            for(const auto&e:movie_sixth_table_entries)entries.push_back({e.address,observed_sixth_table});
#endif
#ifdef RECOVERY_MOVIE_FIFTH_TABLE
            for(const auto&e:movie_fifth_table_entries)entries.push_back({e.address,observed_fifth_table});
#endif
#ifdef RECOVERY_MOVIE_FOURTH_TABLE
            for(const auto&e:movie_fourth_table_entries)entries.push_back({e.address,observed_fourth_table});
#endif
#ifdef RECOVERY_MOVIE_THIRD_TABLE
            for(const auto&e:movie_third_table_entries)entries.push_back({e.address,observed_third_table});
#endif
#ifdef RECOVERY_MOVIE_SECOND_TABLE
            for(const auto&e:movie_second_table_entries)entries.push_back({e.address,observed_second_table});
#endif
#ifdef RECOVERY_MOVIE_FIRST_TABLE
            for(const auto&e:movie_tail_helper_entries)entries.push_back({e.address,observed_tail_helper});
            for(const auto&e:movie_first_table_entries)entries.push_back({e.address,observed_first_table});
#endif
#ifdef RECOVERY_MOVIE_WORK_RESET
            for(const auto&e:movie_work_clear_entries)entries.push_back({e.address,observed_work_clear});
            for(const auto&e:movie_work_reset_entries)entries.push_back({e.address,observed_work_reset});
#endif
#ifdef RECOVERY_MOVIE_WORK_CONFIG
            for(const auto&e:movie_work_pointer_entries)entries.push_back({e.address,observed_work_pointer});
            for(const auto&e:movie_work_config_entries)entries.push_back({e.address,observed_work_config});
#endif
#ifdef RECOVERY_MOVIE_CACHE_FLAGS
            for(const auto&e:movie_triple_check_entries)entries.push_back({e.address,observed_movie_triple_check});
            for(const auto&e:movie_cache_flags_entries)entries.push_back({e.address,observed_movie_cache_flags});
#endif
#ifdef RECOVERY_MOVIE_PAIR_CHECK
            for(const auto&e:movie_pair_check_entries)entries.push_back({e.address,observed_movie_pair_check});
#ifdef RECOVERY_MOVIE_CACHE_FLAGS
            for(const auto&e:movie_pair_compare_entries)entries.push_back({e.address,observed_movie_triple_compare});
#else
            for(const auto&e:movie_pair_compare_entries)entries.push_back({e.address,observed_movie_pair_compare});
#endif
#endif
#ifdef RECOVERY_MOVIE_THIRD_VALIDATION
#ifdef RECOVERY_MOVIE_CACHE_FLAGS
            for(const auto&e:movie_third_validation_entries)entries.push_back({e.address,observed_movie_validation_return});
#else
            for(const auto&e:movie_third_validation_entries)entries.push_back({e.address,observed_movie_validation});
#endif
            for(const auto&e:movie_third_guard_entries)entries.push_back({e.address,observed_third_guard});
#endif
#ifdef RECOVERY_MOVIE_THIRD_SETUP
            for(const auto&e:movie_third_callback_entries)entries.push_back({e.address,observed_movie_third_callback});
            for(const auto&e:movie_third_setup_entries)entries.push_back({e.address,observed_movie_third_setup});
#endif
#ifdef RECOVERY_MOVIE_METHOD_RETURN
            for(const auto&e:movie_followup_return_entries)entries.push_back({e.address,observed_followup_return});
            for(const auto&e:movie_final_return_entries)entries.push_back({e.address,observed_final_return});
#endif
#ifdef RECOVERY_MOVIE_METHOD_WORKAREA
            for(const auto&e:movie_method_workarea_entries)entries.push_back({e.address,observed_method_workarea});
            for(const auto&e:movie_method_record_entries)entries.push_back({e.address,observed_method_record});
#endif
#ifdef RECOVERY_MOVIE_METHOD_SETUP
#ifdef RECOVERY_MOVIE_METHOD_RETURN
            for(const auto&e:movie_method_callback_entries)entries.push_back({e.address,observed_return_callback});
            for(const auto&e:movie_method_setup_entries)entries.push_back({e.address,observed_return_setup});
#else
            for(const auto&e:movie_method_callback_entries)entries.push_back({e.address,observed_movie_method_callback});
            for(const auto&e:movie_method_setup_entries)entries.push_back({e.address,observed_movie_method_setup});
#endif
#endif
#ifdef RECOVERY_MOVIE_METHOD_INIT
            for(const auto&e:movie_method_init_entries)entries.push_back({e.address,observed_movie_method_init});
#endif
#ifdef RECOVERY_MOVIE_INTERFACE
            for(const auto&e:movie_interface_wrapper_entries)entries.push_back({e.address,observed_movie_interface_wrapper});
            for(const auto&e:movie_interface_bind_entries)entries.push_back({e.address,observed_movie_interface_bind});
#endif
#ifdef RECOVERY_MOVIE_RATE_INIT
            for(const auto&e:movie_rate_init_entries)entries.push_back({e.address,observed_movie_rate_init});
            for(const auto&e:movie_context_return_entries)entries.push_back({e.address,observed_movie_context_return});
#endif
#ifdef RECOVERY_MOVIE_WORD_COPY
#ifdef RECOVERY_MOVIE_WORK_CONFIG
            for(const auto&e:movie_word_copy_entries)entries.push_back({e.address,observed_work_config_copy});
#else
            for(const auto&e:movie_word_copy_entries)entries.push_back({e.address,observed_movie_word_copy});
#endif
            for(const auto&e:movie_record_clear_entries)entries.push_back({e.address,observed_movie_record_clear});
#endif
#ifdef RECOVERY_MOVIE_WORKAREA
#ifdef RECOVERY_MOVIE_WORK_CONFIG
            for(const auto&e:movie_word_fill_entries)entries.push_back({e.address,observed_work_config_fill});
#elif defined(RECOVERY_MOVIE_METHOD_WORKAREA)
            for(const auto&e:movie_word_fill_entries)entries.push_back({e.address,observed_method_fill});
#elif defined(RECOVERY_MOVIE_RATE_INIT)
            for(const auto&e:movie_word_fill_entries)entries.push_back({e.address,observed_movie_buffer_fill});
#else
            for(const auto&e:movie_word_fill_entries)entries.push_back({e.address,observed_movie_word_fill});
#endif
#endif
#ifdef RECOVERY_MOVIE_DESCRIPTOR_INIT
            for(const auto&e:movie_descriptor_init_entries)entries.push_back({e.address,observed_movie_descriptor_init});
            for(const auto&e:movie_descriptor_fields_entries)entries.push_back({e.address,observed_movie_descriptor_fields});
#endif
#ifdef RECOVERY_MOVIE_SLOT_SETUP
            for(const auto&e:movie_slot_setup_entries)entries.push_back({e.address,observed_movie_slot_setup});
            for(const auto&e:movie_setup_validation_entries)entries.push_back({e.address,observed_movie_setup_validation});
#endif
#ifdef RECOVERY_MOVIE_SIZE
            for(const auto&e:movie_size_wrapper_entries)entries.push_back({e.address,observed_movie_size_wrapper});
            for(const auto&e:movie_stream_size_entries)entries.push_back({e.address,observed_movie_size_stream});
            for(const auto&e:movie_frame_size_entries)entries.push_back({e.address,observed_movie_size_frame});
#ifdef RECOVERY_MOVIE_SIZE_RETURN
            for(const auto&e:movie_dimensions_entries)entries.push_back({e.address,observed_movie_dimensions});
#endif
#endif
#ifdef RECOVERY_MOVIE_CALLBACK
            for(const auto&e:movie_callback_entries)entries.push_back({e.address,observed_movie_callback});
#endif
#ifdef RECOVERY_MOVIE_CONFIG
            for(const auto&e:movie_config_entries)entries.push_back({e.address,observed_movie_config});
            for(const auto&e:movie_helper_entries)entries.push_back({e.address,observed_movie_helper});
#endif
#ifdef RECOVERY_MOVIE_COUNTER
            for(const auto&e:movie_counter_entries)entries.push_back({e.address,observed_movie_counter});
#endif
#ifdef RECOVERY_MOVIE_INIT
            for(const auto&e:movie_clear_tail_entries)entries.push_back({e.address,observed_movie_clear_tail});
            for(const auto&e:movie_init_entries)entries.push_back({e.address,observed_movie_init});
#endif
#ifdef RECOVERY_MOVIE_RECORD_INIT
            for(const auto&e:movie_record_init_entries)entries.push_back({e.address,observed_movie_record_init});
#endif
#ifdef RECOVERY_MOVIE_GUARDED_CLEAR
            for(const auto&e:movie_guarded_clear_entries)entries.push_back({e.address,observed_movie_guard});
#endif
#ifdef RECOVERY_MOVIE_RETURN_ZERO
            for(const auto&e:movie_return_zero_entries)entries.push_back({e.address,observed_movie_zero});
#endif
#ifdef RECOVERY_MOVIE_MODE_CONFIG
            for(const auto&e:movie_mode_config_entries)entries.push_back({e.address,observed_movie_mode_config});
            for(const auto&e:movie_mode_tail_entries)entries.push_back({e.address,observed_movie_mode_tail});
#endif
#ifdef RECOVERY_MOVIE_CALLBACK_REGISTER
            for(const auto&e:movie_callback_register_entries)entries.push_back({e.address,observed_movie_callback_register});
#ifdef RECOVERY_MOVIE_CALLBACK_REGISTER2
#ifdef RECOVERY_MOVIE_CALLBACK_REGISTER3
            for(const auto&e:movie_callback_enter_entries)entries.push_back({e.address,observed_movie_callback_enter3});
            for(const auto&e:movie_callback_leave_entries)entries.push_back({e.address,observed_movie_callback_leave3});
            for(const auto&e:movie_callback_register3_entries)entries.push_back({e.address,observed_movie_callback_register3});
#else
            for(const auto&e:movie_callback_enter_entries)entries.push_back({e.address,observed_movie_callback_enter2});
            for(const auto&e:movie_callback_leave_entries)entries.push_back({e.address,observed_movie_callback_leave2});
#endif
            for(const auto&e:movie_callback_register2_entries)entries.push_back({e.address,observed_movie_callback_register2});
#else
            for(const auto&e:movie_callback_enter_entries)entries.push_back({e.address,observed_movie_callback_enter});
            for(const auto&e:movie_callback_leave_entries)entries.push_back({e.address,observed_movie_callback_leave});
#endif
#endif
            for(const auto&e:movie_instance_buffer_acquire_entries)entries.push_back(e);
            for(const auto&e:movie_instance_buffer_validate_entries)entries.push_back(e);
            for(const auto&e:movie_instance_buffer_clear_entries)entries.push_back(e);
            for(const auto&e:movie_instance_buffer_config_entries)entries.push_back(e);
            for(const auto&e:movie_instance_buffer_config_return_entries)entries.push_back(e);
            for(const auto&e:movie_instance_buffer_links_entries)entries.push_back(e);
            for(const auto&e:movie_instance_buffer_return_entries)entries.push_back(e);
            for(const auto&e:movie_instance_buffer_binding_entries)entries.push_back(e);
            for(const auto&e:movie_instance_buffer_flags_entries)entries.push_back(e);
            for(const auto&e:movie_instance_fourth_handler_entries)entries.push_back(e);
            for(const auto&e:movie_instance_fourth_setup_entries)entries.push_back(e);
            for(const auto&e:movie_instance_fourth_create_entries)entries.push_back(e);
            require(entries.size()==1287,"Integrated module entry count differs");
            for(auto& e:entries)if(e.address==0x8ced8536)e.function=&observed_integration_acquire;
            dc_activate_code_module(rt,0x8CEB0000,movie_reference.data(),movie_reference.size(),entries.data(),entries.size());
            require(rt.device_clock_sh4_cycles==cycles,"Activation advanced guest time");movie_active=true;
            movie_activation<<"activated entries="<<entries.size()<<" depth="<<rt.code_module_dispatch_depth<<" cycles="<<cycles<<" pc="<<std::hex<<ctx.pc<<" pr="<<ctx.pr<<std::dec<<std::endl;
        }
#else
        call_recompiled(ctx,rt,ctx.pc);
#endif
    };
    ctx.pc=0x8C010000;capture("before");dc_device_clock_enable(rt,true);
    try{
        dispatch_original();
        for(unsigned continuations=0;ctx.pc!=entry_return || rt.sh4_async_redirect;++continuations){
            // Observe the first original worker transfer without changing guest state.
            if(!worker_entered && ctx.pc==0x8C64B64E){capture("first-worker-entry");worker_entered=true;}
            if(worker_entered && !worker_yielded && ctx.pc==0x8C66CBD2){capture("first-worker-yield");worker_yielded=true;}
            if(worker_yielded && !worker_scheduler_returned && ctx.pc==0x8C606A28){capture("first-worker-scheduler-return");worker_scheduler_returned=true;}
            if(!peripheral_request_captured && dc_read32(rt,0x8C8983BC)==1 && dc_read32(rt,0x8C8983C0)==1){
                capture("first-peripheral-request");peripheral_request_captured=true;
                std::ofstream trace(recovery_frame_directory+"/original-startup-first-peripheral-request-trace.txt");
                dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture first peripheral request trace");
            }
            observe_sound_batch();observe_g2_counters();observe_peripheral_setup();observe_peripheral_transfer();
#ifdef RECOVERY_CONTINUATION_PROGRESS
            progress.snapshot("continuation",ctx.pc);
#endif
            require(continuations<RECOVERY_STARTUP_CONTINUATION_LIMIT,"Original startup continuation budget exhausted");
            std::cerr<<"ORIGINAL STARTUP CONTINUE pc="<<std::hex<<ctx.pc<<" spc="<<ctx.spc
                <<" sr="<<ctx.sr<<" redirect="<<rt.sh4_async_redirect<<std::dec<<" cycles="<<rt.device_clock_sh4_cycles<<'\n';
            rt.sh4_async_redirect=false;dispatch_original();
        }
        if(rt.aica_arm7_faults)throw std::runtime_error("Original startup AICA firmware fault: "+rt.aica_arm7_last_detail);
        capture("returned");dc_device_clock_enable(rt,false);
    }catch(...){
        observe_sound_batch();observe_g2_counters();observe_peripheral_setup();observe_peripheral_transfer();

        // Read-only missing module-initializer boundary after original store/guard/reset.
        if(ctx.pc==0x8C65F208 && ctx.pr==0x8C647D7C && ctx.r[3]==0x8C6607D2){
            capture("first-service-init-prerequisite");
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-service-init-prerequisite-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture service initializer prerequisite trace");
        }
        if(ctx.pc==0x8C66B8F0 && ctx.pr==0x8C660800 && ctx.r[2]==0x8C66E8DC){
            capture("first-module-init-prerequisite");
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-module-init-prerequisite-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture module initializer tail prerequisite trace");
        }
        if(ctx.pc==0x8C647D82 && ctx.pr==0x8C647D88 && ctx.r[2]==0x8C10F6AC){
            capture("first-critical-init-prerequisite");
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-critical-init-prerequisite-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture critical initializer prerequisite trace");
        }
        if(ctx.pc==0x8C647D8E && ctx.pr==0x8C647D94 && ctx.r[2]==0x8C661E00){
            capture("first-guarded-init-prerequisite");
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-guarded-init-prerequisite-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture guarded initializer prerequisite trace");
        }
        if(ctx.pc==0x8C647D9A && ctx.pr==0x8C647DA0 && ctx.r[2]==0x8C65F328){
            capture("first-final-buffer-prerequisite");
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-final-buffer-prerequisite-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture final-buffer prerequisite trace");
        }
        if(ctx.pc==0x8C66B0B0 && ctx.pr==0x8C66B0B8 && ctx.r[3]==0x8C6488A4){
            capture("first-init-prologue-prerequisite");
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-init-prologue-prerequisite-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture initializer prologue prerequisite trace");
        }
        if(ctx.pc==0x8C647DC8 && ctx.pr==0x8C05585C && ctx.r[2]==0x8C65EACE){
            capture("first-critical-leave-prerequisite");
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-critical-leave-prerequisite-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture critical leave prerequisite trace");
        }
        if(ctx.pc==0x8C05585C && ctx.pr==0x8C055862 && ctx.r[3]==0x8C09F6D2){
            capture("first-file-pool-prerequisite");
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-file-pool-prerequisite-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture file pool prerequisite trace");
        }
        if(ctx.pc==0x8C055862 && ctx.pr==0x8C0556B6 && ctx.r[2]==0x8C60549E){
            capture("first-startup-tail-prerequisite");
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-startup-tail-prerequisite-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture startup tail prerequisite trace");
        }
        if(ctx.pc==0x8C0556CA && ctx.pr==0x8C0556CE && ctx.r[13]==0x8C06DD60){
            capture("first-main-loop-prerequisite");
            std::ofstream trace(recovery_frame_directory+"/original-startup-first-main-loop-prerequisite-trace.txt");
            dc_trace_print_history(rt,trace);require(bool(trace),"Cannot capture main loop prerequisite trace");
        }
        capture("failure");dc_device_clock_enable(rt,false);
        if(rt.aica_arm7_faults)throw std::runtime_error("Original startup AICA firmware fault: "+rt.aica_arm7_last_detail);
        throw;
    }
}
#endif
void verify_loaded_sboard(DCRuntime& rt, SH4Context& ctx) {
    for(const auto& e:board_resident_entries)rt.register_target(e.address,e.function);
    auto loader=[&](unsigned filename){
        ctx.pc=0x8C09F664;ctx.pr=0xFFFFFFFF;ctx.r[15]=0x8CE3F000;
        ctx.r[4]=filename;ctx.r[5]=0x0C900000;const auto before=ctx;
        call_recompiled(ctx,rt,ctx.pc);
        require(ctx.pc==before.pr && ctx.pr==before.pr && ctx.r[15]==before.r[15],"SBOARD loader return differs");
        for(unsigned n=8;n<15;++n)require(ctx.r[n]==before.r[n],"SBOARD loader preserved state differs");
        return ctx.r[0];
    };
    auto reject_entry=[&]{
        SH4Context rejected{};rejected.pc=0x8C900040;rejected.pr=0xFFFFFFFF;rejected.r[15]=0x8CE3F000;
        bool rejected_ok=false;
        try{call_recompiled(rejected,rt,rejected.pc);}
        catch(const std::runtime_error& e){rejected_ok=std::string(e.what()).find("Inactive or unregistered")!=std::string::npos;}
        require(rejected_ok,"Unactivated SBOARD entry executed");
    };
    for(unsigned round=0;round<2;++round){
        dc_begin_code_module(rt,0x8C900000,0x200000);reject_entry();
        for(unsigned n=0;n<16;++n){dc_write8(rt,0x8C900000-16+n,0xA5);dc_write8(rt,0x8C900000+sizeof(board_bytes)+n,0xA5);}
        const auto reads=rt.gd_sector_reads;
        require(loader(0x8C15D030)==0,"Original SBOARD filename load failed");
        require(rt.gd_sector_reads-reads==147,"SBOARD sector count differs");
        for(unsigned n=0;n<sizeof(board_bytes);++n)require(dc_read8(rt,0x8C900000+n)==board_bytes[n],"Loaded SBOARD bytes differ");
        for(unsigned n=0;n<16;++n)require(dc_read8(rt,0x8C900000-16+n)==0xA5 && dc_read8(rt,0x8C900000+sizeof(board_bytes)+n)==0xA5,"SBOARD loader guard changed");
        const auto handle=dc_read32(rt,0x8C7A4D04);
        require(handle && dc_read16(rt,handle+76)==0,"SBOARD handle not closed");
        reject_entry(); // Correct bytes alone do not register native code.
        dc_activate_code_module(rt,0xAC900000,board_bytes,sizeof(board_bytes),board_module_entries,std::size(board_module_entries));
#ifdef RECOVERY_SBOARD_CAMERA
        check_sboard_camera(rt,ctx);
#else
        check_registration(rt,256,true,true,true);check_registration(rt,32,true,true,true);
        check_registration(rt,16,true,false,true);check_registration(rt,256,false,false,true);
#ifdef RECOVERY_SBOARD_CALLBACK
        check_sboard_callback(rt);
#endif
#endif
        std::vector<std::uint8_t> before_failed_load(sizeof(board_bytes));
#ifdef RECOVERY_SBOARD_CAMERA
        //09B3F0 copies index and manager pointer into each permanent terrain
        //texture-list entry.60C5FC frees the pointed records, not these words.
        for(unsigned i=0;i<50;++i){
            const unsigned entry=0x8C90A61C+i*12,record=0x8C7649F4+(58+i)*68;
            require(dc_read32(rt,entry+4)==i && dc_read32(rt,entry+8)==record,
                "Released terrain texture-list metadata differs");
            require(dc_read32(rt,record)==0xFFFFFFFF && dc_read16(rt,record+64)==0,
                "Released terrain metadata points to a live texture");
        }
        std::cerr<<"POST CLEANUP queued_packets="<<rt.pvr_ta_stream.size()<<'\n';
        {
            std::ofstream image(recovery_frame_directory+"/module-after-cleanup-"+std::to_string(round)+".bin",std::ios::binary);
            require(bool(image),"Cannot capture released module");
            for(unsigned n=0;n<sizeof(board_bytes);++n){const char b=dc_read8(rt,0x8C900000+n);image.write(&b,1);}
            require(bool(image),"Released module capture failed");
        }
#endif
        for(unsigned n=0;n<sizeof(board_bytes);++n){
            before_failed_load[n]=dc_read8(rt,0x8C900000+n);
#ifdef RECOVERY_SBOARD_CAMERA
            // Exact original initializer writes: two pointers and seven byte counters.
            if(n>=0x7C030 && n<0x7C038)continue;
            if(n==0x7BDC8 || n==0x7BE0C || n==0x7BE50 || n==0x7BEBC
                || n==0x7BC4C || n==0x7BC68 || n==0x7BC84){
                require(before_failed_load[n]==0,"Resource counter differs");continue;
            }
            if(n>=0xA61C && n<0xA61C+50*12){
                const unsigned index=(n-0xA61C)/12,offset=(n-0xA61C)%12;
                if(offset>=4){
                    const unsigned expected=offset<8?index:0x8C7649F4+(58+index)*68;
                    require(before_failed_load[n]==((expected>>(8*(offset%4)))&255),
                        "Terrain metadata byte differs");continue;
                }
            }
#endif
            if(before_failed_load[n]!=board_bytes[n])std::cerr<<"MODULE DIFFERENCE offset="<<std::hex<<n
                <<" expected="<<unsigned(board_bytes[n])<<" actual="<<unsigned(before_failed_load[n])<<std::dec<<'\n';
            require(before_failed_load[n]==board_bytes[n],"Unexpected module byte mutation");
        }
        dc_begin_code_module(rt,0x0C900000,0x200000);
        const auto before_missing=rt.gd_sector_reads;
        require(loader(0x8CE00400)==0xFFFFFFFF && rt.gd_sector_reads==before_missing,"Missing overlay did not fail without reading");
        reject_entry(); // Prior bytes remain, but the retired image stays inactive.
        for(unsigned n=0;n<sizeof(board_bytes);++n)require(dc_read8(rt,0x8C900000+n)==before_failed_load[n],"Missing overlay changed prior bytes");
    }
    std::cout<<"PASS loaded SBOARD registration and reload: 147 sectors -> 508012 exact bytes; "
#ifdef RECOVERY_SBOARD_CAMERA
        <<"six joined camera/task/heap cases per load; "
#else
        <<"four task/heap cases per load; "
#endif
        <<"inactive/failed-load rejection; ABI/guards/closed handle; scheduler not booted\n";
}
