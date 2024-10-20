#include "../machine/encoding.h"

#ifndef MEASURE_H
#define MEASURE_H

#define NPU_BASE ((volatile unsigned int *)0xc0000000)
unsigned long rdl_cycle(){
    return (unsigned long)read_csr(cycleh) << 32 | (unsigned long)read_csr(cycle);
}

#endif
