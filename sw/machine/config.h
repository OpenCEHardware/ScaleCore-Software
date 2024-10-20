#ifndef MACHINE_CONFIG_H
#define MACHINE_CONFIG_H

// M-mode configuration
// To enable a given feature, uncomment the corresponding #define(s)


///////////////////
// General debug //
///////////////////

// Enable mnemonic decoding for illegal instructions.
// Adds a large lookup table that includes every RISC-V instruction.
//#define M_DEBUG_INSN

// Similar to M_DEBUG_INSN, but for illegal CSR accesses.
// Adds a large lookup table that includes every RISC-V CSR.
//#define M_DEBUG_CSR


////////////////////
// Device drivers //
////////////////////

// Enable SPIKE's "tohost" protocol, required for simulation I/O to work.
// Note: this is useless on actual hardware (e.g. FPGA synthesis).
//
// https://github.com/riscv-software-src/riscv-isa-sim/issues/364
#define M_DEV_TOHOST

#endif
