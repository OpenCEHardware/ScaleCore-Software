#include <stdio.h>
#include <stdlib.h>
#include "yes_npu.h"
#include "no_npu.h"

int main() {

    // Make inference without NPU
    printf("No NPU results:\n\n");
    no_npu_inference();

    printf("\n");
    printf("\n");

    // Make inference with CPU 
    printf("With NPU results:\n\n");
    npu_inference();

  exit(0);
  return 0;
}
