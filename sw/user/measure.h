#include "../machine/encoding.h"

#ifndef MEASURE_H
#define MEASURE_H

#define NPU_BASE ((volatile unsigned int *)0xc0000000)
unsigned long long rdl_cycle(){
    unsigned long cycle_low = (unsigned long)read_csr(cycle);
    unsigned long cycle_high = (unsigned long)read_csr(cycleh);
    return ((unsigned long long)cycle_high << 32) | cycle_low;
}


#endif
