#include <stdio.h>
#include <stdlib.h>
#include "yes_npu.c"
#include "no_npu.c"

int main() {

    // Make inference without NPU
    printf("No NPU results:\n\n");
    no_npu_inference();

    printf("\n");
    printf("\n");

    // Make inference with CPU 
    printf("With NPU results:\n\n");
    npu_inference();

  return 0;

}
