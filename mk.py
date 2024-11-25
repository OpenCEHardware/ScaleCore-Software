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

hsv_host_sw    = CrossSoftwarePackage('hsv_host_sw')
hsv_user_sw    = CrossSoftwarePackage('hsv_user_sw')
hsv_machine_sw = CrossSoftwarePackage('hsv_machine_sw')
hsv_picolibc   = MesonPackage('hsv_picolibc')

M_CFLAGS = ['-g', f'-march={RISCV_M_ISA}', f'-mabi={RISCV_M_ABI}', '--specs=$(obj)/picolibc.specs']
U_CFLAGS = ['-g', f'-march={RISCV_U_ISA}', f'-mabi={RISCV_U_ABI}', '--specs=$(obj)/picolibc.specs']

if hsv_user_sw.make_flags.opt:
    U_CFLAGS += ['-ffast-math']

hsv_host_sw.requires      (hsv_machine_sw)
hsv_host_sw.requires      (hsv_user_sw)
hsv_host_sw.requires      (hsv_picolibc, outputs=['picolibc.specs'])
hsv_host_sw.cross_triplet (CROSS)
hsv_host_sw.cc_flags      (M_CFLAGS)
hsv_host_sw.executable    ()
hsv_host_sw.ld_flags      (['--oslib=semihost'])

cross_spec = find_files('cross-riscv32-none-elf.txt')

hsv_picolibc.requires    (cross_spec)
hsv_picolibc.meson_src   ('picolibc')
hsv_picolibc.meson_args  (['-Dprefix=$(src)/$(obj)'])
hsv_picolibc.meson_args  (['-Dincludedir=include', '-Dlibdir=lib', '-Dspecsdir=.', '-Dmultilib=false'])
hsv_picolibc.meson_args  ([f'--cross-file=$(src)/$(obj)/{next(iter(cross_spec))}'])
hsv_picolibc.add_outputs (['picolibc.specs', 'picolibcpp.specs'])

hsv_user_sw.requires      (hsv_picolibc, outputs=['picolibc.specs'])
hsv_user_sw.requires      ('machine/*.h') #FIXME
hsv_user_sw.requires      ('user/*.h')
hsv_user_sw.cross_triplet (CROSS)
hsv_user_sw.cc_files      (['user/*.c', 'user/*.S'])
hsv_user_sw.cc_flags      (U_CFLAGS)
hsv_user_sw.static_lib    ()

hsv_machine_sw.requires      (hsv_picolibc, outputs=['picolibc.specs'])
hsv_machine_sw.requires      ('machine/*.h')
hsv_machine_sw.cross_triplet (CROSS)
hsv_machine_sw.cc_files      (['machine/*.c', 'machine/*.S'])
hsv_machine_sw.cc_flags      (M_CFLAGS)
hsv_machine_sw.static_lib    ()
