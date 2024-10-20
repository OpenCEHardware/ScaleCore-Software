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

// Enable support for an Altera JTAG UART console device.
// Unsupported in simulation.
//
// M_DEV_ALTERA_JTAG_UART_BASE: base address defined in Platform Designer
//#define M_DEV_ALTERA_JTAG_UART
//#define M_DEV_ALTERA_JTAG_UART_BASE 0x800030d8

#endif
