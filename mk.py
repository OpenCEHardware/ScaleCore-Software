from mk import *

# M_ISA and M_ABI must match hardware-supported features exactly. Adding ISA extensions
# not implemented in hardware will make M-mode behave incorrectly and eventually crash.
#
# U_ISA and U_ABI can safely include any valid RISC-V extension, even those not supported
# by hardware. M-mode will catch any illegal instructions, potentially emulating them if
# it chooses to do so.
CROSS       = 'riscv32-none-elf' # Target arch-OS-binfmt triplet (i.e. compiler executable is riscv32-none-elf-gcc)
RISCV_M_ISA = 'rv32i_zicsr'
RISCV_M_ABI = 'ilp32'
RISCV_U_ISA = 'rv32i_zicsr'
RISCV_U_ABI = 'ilp32'

hsv_machine_sw = CrossSoftwarePackage('hsv_machine_sw')
hsv_picolibc   = MesonPackage('hsv_picolibc')

M_CFLAGS = ['-g', f'-march={RISCV_M_ISA}', f'-mabi={RISCV_M_ABI}', '--specs=$(obj)/picolibc.specs']
U_CFLAGS = ['-g', f'-march={RISCV_U_ISA}', f'-mabi={RISCV_U_ABI}', '--specs=$(obj)/picolibc.specs']

cross_spec = find_files('cross-riscv32-none-elf.txt')

hsv_picolibc.requires    (cross_spec)
hsv_picolibc.meson_src   ('picolibc')
hsv_picolibc.meson_args  (['-Dprefix=$(src)/$(obj)'])
hsv_picolibc.meson_args  (['-Dincludedir=include', '-Dlibdir=lib', '-Dspecsdir=.', '-Dmultilib=false'])
hsv_picolibc.meson_args  ([f'--cross-file=$(src)/$(obj)/{next(iter(cross_spec))}'])
hsv_picolibc.add_outputs (['picolibc.specs', 'picolibcpp.specs'])

hsv_machine_sw.requires      (hsv_picolibc, outputs=['picolibc.specs'])
hsv_machine_sw.requires      ('*.h')
hsv_machine_sw.requires      ('machine/*.h')
hsv_machine_sw.cross_triplet (CROSS)
hsv_machine_sw.cc_files      (['machine/*.c', 'machine/*.S'])
hsv_machine_sw.cc_flags      (M_CFLAGS)
hsv_machine_sw.static_lib    ()

def make_hsv_user_app(name):
    rom     = CrossSoftwarePackage(f'hsv_rom_{name}')
    user_sw = CrossSoftwarePackage(f'hsv_user_sw_{name}')

    user_sw.requires      (hsv_picolibc, outputs=['picolibc.specs'])
    user_sw.requires      ('*.h')
    user_sw.requires      (f'user/{name}/*.h')
    user_sw.cross_triplet (CROSS)
    user_sw.cc_files      ([f'user/{name}/*.c', f'user/{name}/*.S'])
    user_sw.cc_flags      (U_CFLAGS)
    user_sw.static_lib    ()

    if user_sw.make_flags.opt:
        user_sw.cc_flags  (['-ffast-math'])

    rom.requires      (hsv_machine_sw)
    rom.requires      (user_sw)
    rom.requires      (hsv_picolibc, outputs=['picolibc.specs'])
    rom.cross_triplet (CROSS)
    rom.cc_flags      (M_CFLAGS)
    rom.executable    ()
    rom.ld_flags      (['--oslib=semihost'])

    return rom, user_sw

hsv_rom_iris,     hsv_user_sw_iris     = make_hsv_user_app('iris')
hsv_rom_bogomips, hsv_user_sw_bogomips = make_hsv_user_app('bogomips')
