#!/usr/bin/env python3
"""Bounded original file-provider initialization, without provider-state overrides."""
import importlib.util
import json
from pathlib import Path
import re
import struct
import sys
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('aot',ROOT/'scripts/run-recovery-aot.py')
aot=importlib.util.module_from_spec(spec);spec.loader.exec_module(aot)
prs_spec=importlib.util.spec_from_file_location('prs',ROOT/'scripts/decode-recovery-prs.py')
prs=importlib.util.module_from_spec(prs_spec);prs_spec.loader.exec_module(prs)

def main():
    flash=None
    if '--system-flash' in sys.argv:
        index=sys.argv.index('--system-flash')
        if index+1>=len(sys.argv):raise ValueError('--system-flash requires an image path')
        flash=Path(sys.argv[index+1]).resolve(strict=True);del sys.argv[index:index+2]
        if flash.stat().st_size!=131072:raise ValueError('System flash image must be exactly128KiB')
    aot.ENV.pop('SONICPAD_RECOVERY_SYSTEM_FLASH',None)
    if flash:aot.ENV['SONICPAD_RECOVERY_SYSTEM_FLASH']=str(flash)
    original_startup='--original-startup' in sys.argv
    if original_startup:sys.argv.remove('--original-startup')
    if flash and not original_startup:raise ValueError('--system-flash requires --original-startup')
    aot.REPORT['system_flash_image']=str(flash) if flash else None
    aot.REPORT['original_main_entry_experiment']=original_startup
    aot.ENV.pop('SONICPAD_RECOVERY_ORIGINAL_STARTUP',None)
    if original_startup:aot.ENV['SONICPAD_RECOVERY_ORIGINAL_STARTUP']='1'
    outer_loop='--startup-loop' in sys.argv
    if outer_loop:sys.argv.remove('--startup-loop')
    aot.REPORT['original_outer_loop_experiment']=outer_loop
    aot.ENV.pop('SONICPAD_RECOVERY_OUTER_LOOP',None)
    if outer_loop:aot.ENV['SONICPAD_RECOVERY_OUTER_LOOP']='1'
    board=None
    camera='--sboard-camera' in sys.argv
    if camera:sys.argv.remove('--sboard-camera')
    if original_startup and (not camera or outer_loop):raise ValueError('--original-startup requires --sboard-camera and excludes --startup-loop')
    if outer_loop and not camera:raise ValueError('--startup-loop requires --sboard-camera')
    bank1='--startup-bank1' in sys.argv
    if bank1:sys.argv.remove('--startup-bank1')
    if bank1 and not camera:raise ValueError('--startup-bank1 requires --sboard-camera')
    aot.REPORT['supplied_startup_bank1_hypothesis']=bank1 and not original_startup
    aot.REPORT['baseline_startup_bank1_compiled']=bank1
    callback='--sboard-callback' in sys.argv
    if callback:sys.argv.remove('--sboard-callback')
    callback=callback or camera
    if len(sys.argv)==8 and sys.argv[6]=='--sboard':
        board=Path(sys.argv[7]).resolve(strict=True);sys.argv=sys.argv[:6]
    if len(sys.argv)!=6:raise ValueError('usage: run-recovery-provider.py BOOT IP DISC_MAP FUNCTION_MAP EXPECTED_SECTORS [--sboard DECODED_SBOARD [--sboard-callback | --sboard-camera [--startup-bank1] [--startup-loop | --original-startup [--system-flash IMAGE]]]]')
    boot,ip,discmap,functions,expected=[Path(p).resolve(strict=True) for p in sys.argv[1:]]
    if callback and not board:raise ValueError('--sboard-callback requires --sboard DECODED_SBOARD')
    fields=dict(line.split('=',1) for line in discmap.read_text().splitlines() if '=' in line)
    disc=Path(fields['image']).resolve(strict=True)
    if aot.sha256(boot)!='68478ff33fe6c4d48a423b363b3e14669106040d01284d889aa4db69ab2e0b29':raise ValueError('Wrong owner boot')
    if aot.sha256(disc)!='105b9a8849feeabf4b7b00a6f5d101d18789af82dfeba407952d884a726b9533':raise ValueError('Wrong owner disc')
    identities={str(p):aot.sha256(p) for p in (boot,ip,discmap,functions,disc,expected)}
    if flash:identities[str(flash)]=aot.sha256(flash)
    if board:
        if aot.sha256(board)!='428dce0260d89a2e524691456cfb228bdc8cc4247631cd5e850eff69a28ff3ac':raise ValueError('Wrong decoded SBOARD')
        identities[str(board)]=aot.sha256(board)
    camera_inputs=[]
    stage_map=ROOT/'docs/artifacts/recovery-aot/20261005-121754/functions.csv'
    if camera:
        if aot.sha256(stage_map)!='f0774e00496328c0f8ae4a26042b576e56bb7cf79cac20c92875fa508be22f26':raise ValueError('Audited SBOARD map changed')
        identities[str(stage_map)]=aot.sha256(stage_map)
        for index in (97,98):
            path=ROOT/f'work/dreamcast-recovery/bin-candidates/candidate-{index:04}/decoded.bin'
            if aot.sha256(path)!='df73ce45a74fbe42a43337485023dcda07cee4a3a905e93a193038982255f636':raise ValueError('Wrong SBOARD camera reference')
            identities[str(path)]=aot.sha256(path);camera_inputs.append(path)
        inventory=ROOT/'docs/artifacts/recovery-aot/20261005-103149/disc-files.json'
        if aot.sha256(inventory)!='3b67f17c08aa533a10b30794a8ecce8a16527d8f81b053201e74a4a63ca28bf7':raise ValueError('Owner disc inventory changed')
        identities[str(inventory)]=aot.sha256(inventory)
        texture_references={}
        for stem,size,decoded_size,encoded_hash,decoded_hash in (
            ('SONIC',36861,108000,'da8c4ad821e83c9b4c9e16ddfb528e4991a24e14b5dc6faed7846474faef3f59','892414bdb1cdff5dfaa59b01914adb62d02deae70c692f311a173c42fa37e094'),
            ('SON_EFF',5676,23264,'63e7b82de0a2a3bcea6656c13ee26c46c16fbff8ed9e7ab6f717f528372bbf51','e77857878c65f35a12ee688eff810254bb49dd5a29775648808f68d4017a3c12'),
            ('S_MOT',214890,488608,'52398564204238a3dcaff24a01ad255f0d805a21b334bc48f328a08b620a57ca','239a25a00e6314b81bf3c2bc37784efdb6550420b3d31b8c48bc6819c47c0f94'),
            ('S_SBMOT',117125,261688,'57d77145c15e78fdc99dddc227d077d23b49063d826670fe5faea2ae6a2fc977','b1d3897a4636ea3affed824451455ab59d77c41c453354287b138bce4b65a2b6'),
            ('M_SBMOT',74957,165232,'88716f917861ebe9ca6209835b2a71a602f45bb052801365bd17edb132644121','7c15418fbfa5e89671f4a0cf1acb916e5964c184dcde14eeb125b626bd856ef2')):
            item=next(row for row in json.loads(inventory.read_text()) if row['path']==f'SONICADV/{stem}.PRS')
            if item['size']!=size or item['multi_extent']:raise ValueError(f'Unexpected {stem} texture extent')
            prefix=f"track.{fields['boot_track']}."
            encoded=bytearray()
            with disc.open('rb') as image:
                for n in range((size+2047)//2048):
                    image.seek(int(fields[prefix+'file_base'])+(item['extent']-(int(fields[prefix+'start_fad'])-150)+n)*int(fields[prefix+'sector_size'])+int(fields[prefix+'user_offset']))
                    encoded+=image.read(min(2048,size-len(encoded)))
            decoded,consumed=prs.decode_prs(encoded)
            if consumed!=size or len(decoded)!=decoded_size:raise ValueError(f'{stem} texture size differs')
            label={'SONIC':'sonic','SON_EFF':'effect'}.get(stem,stem.lower())
            for suffix,data,expected_hash in (('encoded',encoded,encoded_hash),('decoded',decoded,decoded_hash)):
                path=aot.LOGS/f'{label}-{suffix}.bin';path.write_bytes(data)
                if aot.sha256(path)!=expected_hash:raise ValueError(f'{stem} texture identity differs')
                identities[str(path)]=expected_hash
                texture_references[f'{label}_{suffix}']=data
        # Independent bytes for original lighting and streamed snowboard texture loads.
        for filename,size,digest in (
            ('PL1C0B.BIN',12672,'003ae42e2f6e5aa1b5cf208baaf8f12d4c94801f27ff43f7ac0c6fdad1414b5b'),
            ('SL1C0B.BIN',1920,'528ad07114a141852a0af402164e7835b590a7bb39ddb6140550480fa3b20671'),
            ('MIW_B001.PVR',10952,'72551086cbd33c4600140fa18eb8c52d7aedaaa1db5fe4dcbe3645ca3d02a46c')):
            stem=Path(filename).stem
            item=next(row for row in json.loads(inventory.read_text()) if row['path']==f'SONICADV/{filename}')
            if item['size']!=size or item['multi_extent']:raise ValueError('Reference extent differs')
            data=bytearray();prefix=f"track.{fields['boot_track']}."
            with disc.open('rb') as image:
                for n in range((size+2047)//2048):
                    image.seek(int(fields[prefix+'file_base'])+(item['extent']-(int(fields[prefix+'start_fad'])-150)+n)*int(fields[prefix+'sector_size'])+int(fields[prefix+'user_offset']))
                    data+=image.read(min(2048,size-len(data)))
            path=aot.LOGS/filename;path.write_bytes(data)
            if aot.sha256(path)!=digest:raise ValueError('Reference identity differs')
            identities[str(path)]=digest;texture_references[stem.lower()+'_reference']=data
    aot.REPORT['inputs']=identities
    prefix=f"track.{fields['boot_track']}."
    with disc.open('rb') as image:
        original_ip=bytearray()
        for n in range(16):
            image.seek(int(fields[prefix+'file_base'])+n*int(fields[prefix+'sector_size'])+int(fields[prefix+'user_offset']))
            original_ip+=image.read(2048)
    if ip.read_bytes()!=original_ip:raise ValueError('IP does not match mapped owner disc')
    if aot.sha256(expected)!='5bbe2c96088b2ab860412fd25c2aacc9f88f35d783095edbf6df6d607734c8a0':raise ValueError('Expected Mini Cart sectors differ')
    raw=aot.LOGS/'bios-init.bin';raw.write_bytes(boot.read_bytes()[0xFF594:0xFF5A8])
    sys.argv=[sys.argv[0],'--coverage',str(raw),'--base','0x8C10F594'];aot.main()
    aot.REPORT.update(passed=False,scope='Original provider and combined file/decompression loader; public BIOS HLE, MMIO shadow; no gameplay',r3_complete=False,r4_complete=False,inputs=identities)
    # Reviewed low disc-service table, plus actual provider callback/entry targets.
    seeds=set(struct.unpack_from('<20I',boot.read_bytes(),0x670F80-0x10000))
    seeds.update([0x8C603D78,0x8C603A68,0x8C603A16,0x8C603A30,0x8C604214,0x8C09E8A0,0x8C654DE4,0x8C604B1C,0x8C603FE2,0x8C09F664])
    if camera:seeds.update([0x8C60623A,0x8C605774,0x8C606328,0x8C649B20,0x8C605E68,0x8C605FC8,0x8C608250,0x8C608442,0x8C0196FE,0x8C0194FC,0x8C09E98E,0x8C604BBA,0x8C604C5E,0x8C6050E4,0x8C605240,0x8C09F5A0])
    seedfile=aot.LOGS/'provider-seeds.txt';seedfile.write_text(''.join(f'0x{v:08X}\n' for v in sorted(seeds)))
    aot.REPORT['seed_sha256']=aot.sha256(seedfile)
    aot.REPORT['source_sha256']={str(p):aot.sha256(p) for p in (Path(__file__).resolve(),ROOT/'scripts/run-recovery-aot.py',ROOT/'scripts/decode-recovery-prs.py',ROOT/'tests/recovery/provider-generator.cpp',ROOT/'tests/recovery/provider-execution.cpp',ROOT/'tests/recovery/system-flash-check.hpp',ROOT/'tests/recovery/system-flash-write-check.hpp')}
    generator=aot.LOGS/'provider-generator';generated=aot.LOGS/'provider-generated';build=aot.LOGS/'provider-build'
    aot.run('build-provider-generator',['/usr/bin/clang++','-std=c++20','-O2','-arch','arm64','-mmacosx-version-min=13.0','-I',aot.SOURCE/'include',ROOT/'tests/recovery/provider-generator.cpp',aot.BUILD/'libdcrecomp_core.a','-o',generator])
    aot.REPORT['provider_closure']=json.loads(aot.run('generate-provider',[generator,boot,functions,seedfile,generated],timeout=120))
    extras=[]
    if board:
        board_gen=aot.LOGS/'board-generator';board_out=aot.LOGS/'board-generated'
        aot.run('build-board-generator',['/usr/bin/clang++','-std=c++20','-O2','-arch','arm64','-mmacosx-version-min=13.0','-I',aot.SOURCE/'include',ROOT/'tests/recovery/registration-generator.cpp',aot.BUILD/'libdcrecomp_core.a','-o',board_gen])
        aot.REPORT['board_closure']=json.loads(aot.run('generate-board',[board_gen,boot,board,board_out]))
        names=re.findall(r'void (recomp_[0-9A-F]+)\(', (board_out/'generated_program.hpp').read_text())
        if set(names)!={'recomp_8C09A426','recomp_8C09A720','recomp_8C0A1690','recomp_8C900040','recomp_8C903140'}:raise ValueError('Board registration closure changed')
        definitions=['register_recompiled_program=board_register']+[f'{name}=board_{name}' for name in names]
        source=board_out/'generated_program.cpp'
        (board_out/'module-sources.cmake').write_text(f'target_sources(recovery_oracle PRIVATE "{source}")\nset_source_files_properties("{source}" PROPERTIES COMPILE_DEFINITIONS "'+ ';'.join(definitions)+'")\n')
        manifest='#pragma once\n#include "dc_runtime.hpp"\nnamespace dcrecomp_generated {\n'
        manifest+=''.join(f'void board_{name}(SH4Context&, DCRuntime&);\n' for name in names)
        manifest+='inline constexpr std::uint8_t board_bytes[] = {'+','.join(map(str,board.read_bytes()))+'};\n'
        for label,selected in [('resident',[n for n in names if int(n[7:],16)<0x8C900000]),('module',[n for n in names if int(n[7:],16)>=0x8C900000])]:
            manifest+=f'inline const CodeModuleEntry board_{label}_entries[] = {{'+','.join(f'{{0x{n[7:]}u,&board_{n}}}' for n in selected)+'};\n'
        (board_out/'board-manifest.hpp').write_text(manifest+'}\n')
        aot.REPORT['board_generated_sha256']={str(p):aot.sha256(p) for p in (source,board_out/'board-manifest.hpp',board_out/'module-sources.cmake')}
        aot.REPORT['source_sha256'].update({str(p):aot.sha256(p) for p in (ROOT/'tests/recovery/registration-generator.cpp',ROOT/'tests/recovery/board-join.cpp',ROOT/'tests/recovery/registration-check.hpp',ROOT/'tests/recovery/CMakeLists.txt')})
        extras=[f'-DMODULE_JOIN_DIR={board_out}']
        if callback:
            callback_gen=aot.LOGS/'callback-generator';callback_out=aot.LOGS/'callback-generated'
            aot.run('build-callback-generator',['/usr/bin/clang++','-std=c++20','-O2','-arch','arm64','-mmacosx-version-min=13.0','-I',aot.SOURCE/'include',ROOT/'tests/recovery/sboard-callback-generator.cpp',aot.BUILD/'libdcrecomp_core.a','-o',callback_gen])
            aot.REPORT['callback_closure']=json.loads(aot.run('generate-callback',[callback_gen,boot,board,callback_out,*(['--stage-map',stage_map] if camera else [])]))
            callback_names=re.findall(r'void (recomp_[0-9A-F]+)\(', (callback_out/'generated_program.hpp').read_text())
            callback_sources=[callback_out/'generated_program.cpp',*sorted(callback_out.glob('generated_program_part_*.cpp'))]
            callback_source_list=' '.join(f'"{p}"' for p in callback_sources)
            definitions=['register_recompiled_program=callback_register']+[f'{n}=callback_{n}' for n in callback_names]
            with (board_out/'module-sources.cmake').open('a') as f:
                f.write(f'target_sources(recovery_oracle PRIVATE {callback_source_list})\nset_source_files_properties({callback_source_list} PROPERTIES COMPILE_DEFINITIONS "'+ ';'.join(definitions)+'")\ntarget_compile_definitions(recovery_oracle PRIVATE RECOVERY_SBOARD_CALLBACK=1)\n')
            manifest='#pragma once\n#include "dc_runtime.hpp"\nnamespace dcrecomp_generated {\n'
            manifest+=''.join(f'void callback_{n}(SH4Context&, DCRuntime&);\n' for n in callback_names)
            # Keep the emitter's resident continuation labels, not only function
            # starts: an interrupt may unwind native calls and resume at a block.
            bindings=[(int(address,16),name) for address,name in re.findall(
                r'runtime\.register_target\(0x([0-9A-F]+)u, &(recomp_[0-9A-F]+)\)',callback_sources[0].read_text())]
            resident=[(address,name) for address,name in bindings if address<0x8C900000]
            expected_resident={(int(n[7:],16),n) for n in callback_names if int(n[7:],16)<0x8C900000}
            if not expected_resident.issubset(set(resident)) or any(n not in callback_names for _,n in resident):
                raise ValueError('Resident callback binding/continuation coverage differs')
            modules=[(int(n[7:],16),n) for n in callback_names if int(n[7:],16)>=0x8C900000]
            for label,selected in [('resident',resident),('module',modules)]:
                manifest+=f'inline const CodeModuleEntry callback_{label}_entries[] = {{'+','.join(f'{{0x{address:08X}u,&callback_{name}}}' for address,name in selected)+'};\n'
            aot.REPORT['callback_resident_function_and_block_bindings']=len(resident)
            (board_out/'callback-manifest.hpp').write_text(manifest+'}\n')
            aot.REPORT['callback_generated_sha256']={str(p):aot.sha256(p) for p in (*callback_sources,callback_out/'generated_program.hpp',board_out/'callback-manifest.hpp',board_out/'module-sources.cmake')}
            aot.REPORT['board_generated_sha256'][str(board_out/'module-sources.cmake')]=aot.sha256(board_out/'module-sources.cmake')
            aot.REPORT['source_sha256'].update({str(p):aot.sha256(p) for p in (ROOT/'tests/recovery/sboard-callback-generator.cpp',ROOT/'tests/recovery/sboard-callback-check.hpp')})
        if camera:
            with (board_out/'module-sources.cmake').open('a') as f:f.write('target_compile_definitions(recovery_oracle PRIVATE RECOVERY_SBOARD_CAMERA=1)\n')
            if bank1:
                with (board_out/'module-sources.cmake').open('a') as f:f.write('target_compile_definitions(recovery_oracle PRIVATE RECOVERY_STARTUP_BANK1=1)\n')
            (board_out/'camera-reference.hpp').write_text('#pragma once\ninline constexpr unsigned char camera_reference[] = {'+','.join(map(str,camera_inputs[0].read_bytes()))+'};\n')
            texture_header=board_out/'sonic-texture-reference.hpp'
            texture_header.write_text('#pragma once\n'+''.join('inline constexpr unsigned char '+name+'[] = {'+','.join(map(str,data))+'};\n' for name,data in texture_references.items()))
            aot.REPORT['camera_generated_sha256']={str(p):aot.sha256(p) for p in (board_out/'camera-reference.hpp',board_out/'module-sources.cmake')}
            aot.REPORT['camera_generated_sha256'][str(texture_header)]=aot.sha256(texture_header)
            for field in ('board_generated_sha256','callback_generated_sha256'):
                aot.REPORT[field][str(board_out/'module-sources.cmake')]=aot.sha256(board_out/'module-sources.cmake')
            aot.REPORT['source_sha256'][str(ROOT/'tests/recovery/sboard-camera-check.hpp')]=aot.sha256(ROOT/'tests/recovery/sboard-camera-check.hpp')
            for name in ('startup-check.hpp','texture-batch-check.hpp','startup-model-check.hpp','startup-motion-check.hpp','startup-mesh-check.hpp'):
                path=ROOT/'tests/recovery'/name
                aot.REPORT['source_sha256'][str(path)]=aot.sha256(path)
            # Large original character dispatch functions exceed the bounded optimizer budget.
            # Keep this isolated execution experiment unoptimized; no performance claim.
            with (board_out/'module-sources.cmake').open('a') as f:
                f.write(f'set_source_files_properties({callback_source_list} PROPERTIES COMPILE_OPTIONS "-O0")\n')
            aot.REPORT['callback_optimization']='-O0 diagnostic; 17710-instruction slice exceeded 180s optimized build budget'
            for field in ('board_generated_sha256','callback_generated_sha256','camera_generated_sha256'):
                aot.REPORT[field][str(board_out/'module-sources.cmake')]=aot.sha256(board_out/'module-sources.cmake')
    aot.configure('configure-provider',ROOT/'tests/recovery',build,[f'-DFIXTURE_DIR={generated}','-DFIXTURE=9',f'-DORACLE_SOURCE={ROOT / "tests/recovery/provider-execution.cpp"}',*extras])
    aot.run('build-provider',['cmake','--build',build,'--target','recovery_oracle','--parallel','4'],timeout=180)
    binary=build/'recovery_oracle'
    if 'Mach-O 64-bit executable arm64' not in aot.run('identity-provider',['file',binary]):raise ValueError('Not ARM64')
    imports=aot.run('imports-provider',['/usr/bin/nm','-u',binary])
    if re.search(r'\b_(?:mmap|mprotect|pthread_jit_write_protect_np|dlopen|system|popen|posix_spawn\w*|exec\w*)\b',imports):raise ValueError('Review executable-memory/process imports')
    decoded,consumed=prs.decode_prs(expected.read_bytes()[:331320])
    if consumed!=331320:raise ValueError('PRS consumption mismatch')
    decoded_file=aot.LOGS/'expected-decoded.bin';decoded_file.write_bytes(decoded)
    if aot.sha256(decoded_file)!='002ab2a1f083d1a41902ced39c2e47e929fae68b33fa7094b5c748a8d22b09b6':raise ValueError('Decoded identity mismatch')
    aot.REPORT['expected_decoded_sha256']=aot.sha256(decoded_file)
    result=aot.run('execute-provider',[binary,discmap,ip,expected,decoded_file],timeout=20)
    if 'PASS original filename read' not in result or 'PASS missing filename' not in result or 'PASS combined loader' not in result or 'PASS missing module load' not in result:raise ValueError('Missing file verification result')
    if board and 'PASS loaded SBOARD registration and reload' not in result:raise ValueError('Missing loaded SBOARD proof')
    if callback and not camera and (result.count('PASS SBOARD callback character=')!=8 or result.count('NEXT SBOARD boundary:')!=4):raise ValueError('Missing callback cases or explicit next boundary')
    if camera and (result.count('PASS joined SBOARD camera act=')!=12 or 'PASS original video initialization' not in result):raise ValueError('Missing video/camera join cases')
    if camera:
        for label,count in {'PASS isolated original RNG:':2,'PASS isolated original drawing header:':2,'PASS isolated original controller decoder:':2,'PASS original task pool:':4,'PASS original character selector:':4,
                            'PASS original common player-work producer:':2,'PASS original producers ->':2,
                            'PASS original collision center:':2,'PASS original descriptor:':2,'PASS original Sonic texture load:':2,
                            'PASS original texture manager:':2,'PASS original graphics setup:':2,'PASS original vertex workspace:':2,'PASS original group1 return:':2,'PASS original texture batches:':2,
                            'PASS original VRAM reset:':2,'PASS original camera phase0:':2,
                            'PASS original camera phase1:':2,'PASS original camera phase2:':2,
                            'PASS original TMU startup:':1,'PASS original exception vector copy:':1,
                            'PASS original frame interrupt continuation:':6,
                            'PASS original full update:':2,'PASS original full display/frame:':2,'PASS original terrain release:':2,
                            'PASS original polygon setup:':2,'PASS original graphics VRAM reservations:':2,
                            'PASS first textured strip packets:':2,'PASS first strip negative control:':2,
                            'PASS first mesh projection:':2,'PASS first mesh negative control:':2,
                            'PASS original post-camera visibility:':2,'PASS original character display return:':2,
                            'PASS original mesh pointer updates:':2,'PASS original lighting file bytes:':2,
                            'PASS original Maple startup:':1,'PASS original frame handoff:':2,
                            'PASS original character motion file:':2,'PASS original snowboard file bytes:':2,
                            'PASS live stage join:':2,'PASS original snowboard constructor and texture:':2,
                            'PASS original group2 return:':2,'PASS original group3 return:':2,'PASS original group3 model release:':2,'PASS original snowboard texture release:':2,
                            'PASS original first motion pose:':2,'PASS original model setup:':2,'PASS original auxiliary tasks:':2,
                            'PASS original texture release:':2,
                            'PASS original scheduler prefix:':2}.items():
            if result.count(label)!=count:raise ValueError(f'Missing original startup cases: {label}')
    aot.REPORT['original_scheduler_prefix_verified']=camera
    aot.REPORT['sonic_texture_loader_verified']=camera
    aot.REPORT['sonic_pvm_metadata_and_first_descriptor_verified']=camera
    aot.REPORT['sonic_first_texture_allocation_verified']=camera
    aot.REPORT['sonic_first_texture_transfer_bytes_verified']=camera
    aot.REPORT['sonic_first_texture_completion_verified']=camera
    aot.REPORT['sonic_first_texture_registration_verified']=camera
    aot.REPORT['original_vram_reset_verified']=camera
    aot.REPORT['sonic_texture_upload_verified']=camera
    aot.REPORT['sonic_and_effect_57_texture_batches_verified']=camera
    aot.REPORT['sonic_and_effect_texture_release_verified']=camera
    aot.REPORT['original_auxiliary_tasks_verified']=camera
    aot.REPORT['original_model_setup_verified']=camera
    aot.REPORT['animation_blend_values_verified']=camera
    aot.REPORT['first_pose_73_matrices_verified']=camera
    aot.REPORT['original_matrix_stack_producer_verified']=camera
    aot.REPORT['original_collision_center_verified']=camera
    aot.REPORT['original_outer_graphics_setup_verified']=camera
    aot.REPORT['original_vertex_workspace_producer_verified']=camera
    aot.REPORT['original_mesh_material_entry_reached']=camera
    aot.REPORT['original_polygon_setup_returned']=camera
    aot.REPORT['original_exception_vector_copy_verified']=camera
    aot.REPORT['one_original_graphics_interrupt_continuation_verified']=camera and result.count('PASS original interrupt continuation:')==1
    aot.REPORT['original_frame_interrupt_continuations_verified']=camera
    aot.REPORT['nonframe_interrupt_continuation_count']=result.count('PASS original interrupt continuation:')
    aot.REPORT['graphics_vram_reservations_verified']=camera
    aot.REPORT['first_mesh_projection_given_guest_matrix_verified']=camera
    aot.REPORT['first_textured_mesh_98_vertex_packets_verified']=camera
    aot.REPORT['original_character_display_callback_return_verified']=camera
    aot.REPORT['original_53_material_pointer_updates_verified']=camera
    aot.REPORT['display_test_observer_delegates_to_original_body']=camera
    aot.REPORT['original_stage_lighting_file_bytes_verified']=camera
    aot.REPORT['original_frame_dma_and_return_verified']=camera
    aot.REPORT['nonblack_offscreen_fixture_observed']=camera
    aot.REPORT['original_character_motion_load_verified']=camera
    aot.REPORT['live_stage_character_camera_join_verified']=camera
    aot.REPORT['original_snowboard_assets_and_streamed_texture_verified']=camera
    aot.REPORT['original_group2_first_invocation_verified']=camera
    aot.REPORT['original_group3_first_invocation_verified']=camera
    if camera and result.count('PASS original auxiliary angle release:')!=2:raise ValueError('Auxiliary model angle cleanup repetitions differ')
    aot.REPORT['original_auxiliary_model_angle_release_verified']=camera
    aot.REPORT['original_group3_model_release_verified']=camera
    aot.REPORT['original_snowboard_texture_release_verified']=camera
    aot.REPORT['full_task_tree_teardown_verified']=False
    aot.REPORT['complete_model_packet_values_verified']=False
    aot.REPORT['sonic_rasterized_frame_verified']=False
    aot.REPORT['lighting_values_independently_verified']=False
    aot.REPORT['original_background_packet_verified']=camera
    aot.REPORT['sonic_draw_submission_verified']=False
    aot.REPORT['deformed_mesh_values_verified']=False
    aot.REPORT['original_group1_invocation_verified']=camera
    aot.REPORT['collision_registration_and_history_verified']=camera
    aot.REPORT['original_camera_three_invocations_returned']=camera
    aot.REPORT['camera_pose_values_independently_verified']=False
    aot.REPORT['original_post_camera_visibility_verified']=camera
    aot.REPORT['supplied_update_and_display_return_verified']=camera
    aot.REPORT['original_terrain_texture_release_verified']=camera
    aot.REPORT['scheduler_tick_complete']=False
    aot.REPORT['normal_boot_verified']=False
    aot.REPORT['original_startup_producer_slice_verified']=camera
    aot.REPORT['sboard_camera_join_verified']=camera
    aot.REPORT['sboard_callback_verified']=callback
    aot.REPORT['loaded_sboard_registration_verified']=bool(board)
    if any(aot.sha256(Path(p))!=h for p,h in identities.items()):raise ValueError('Original input changed')
    aot.REPORT.update(passed=True,result=result,binary_sha256=aot.sha256(binary),provider_complete=False,combined_loader_bytes_verified=True,cache_side_effects_verified=False,filename_read_verified='PASS original filename read' in result)
    print('\n'.join(line for line in result.splitlines() if line.startswith(('BEGIN','WORK_AREA','RETURN','DIRECTORY','PASS'))),flush=True)

if __name__=='__main__':
    aot.LOGS.mkdir(parents=True);(aot.WORK/'tmp').mkdir(parents=True,exist_ok=True)
    try:main()
    except Exception as error:aot.REPORT.update(passed=False,error=str(error));print(error,file=sys.stderr)
    finally:
        aot.REPORT['original_inputs_unchanged']=all(aot.sha256(Path(p))==h for p,h in aot.REPORT.get('inputs',{}).items())
        if not aot.REPORT['original_inputs_unchanged']:aot.REPORT.update(passed=False,error='Original input changed')
        (aot.LOGS/'report.json').write_text(json.dumps(aot.REPORT,indent=2)+'\n');print(f'Evidence: {aot.LOGS}',flush=True)
    sys.exit(0 if aot.REPORT['passed'] else 1)
