#include <stdio.h>
#include <stdlib.h>
#include "measure.h"

volatile int flag = 0;

void timer_isr(void /*void *context, alt_u32 id*/) {

    if (NPU_BASE[16]){
        NPU_BASE[16] = 1;
        flag = 1;
    }
    return;
}

void npu_inference() {

 // Capture initial cycles
 unsigned long init_cycles = rdl_cycle();

 // ------ LAYER 1 ------

  int npu_data[32];

  // Load first layer weigths
  
  npu_data[0] = 0xC72BD1C6;
  npu_data[1] = 0x6D225ECB;
  npu_data[2] = 0x35E680DD;
  npu_data[3] = 0xAF7FFBEC;
  npu_data[4] = 0xF10D0A62;
  npu_data[5] = 0x254445D5;
  npu_data[6] = 0x8DFD2555;
  npu_data[7] = 0xF25F9E92;

  // Load inputs
  npu_data[8] =  0x00000000;
  npu_data[9] =  0xB836DC4D;
  npu_data[10] = 0x00000000;
  npu_data[11] = 0xA31DE660;
  npu_data[12] = 0x00000000;
  npu_data[13] = 0xB221DB5F;
  npu_data[14] = 0x00000000;
  npu_data[15] = 0x86C5206E;

  // Load bias
  npu_data[16] = -128;
  npu_data[17] =  -91;
  npu_data[18] =   10;
  npu_data[19] =  -89;
  npu_data[20] =   10;
  npu_data[21] =   10;
  npu_data[22] =  127;
  npu_data[23] =   10;

  int npu_result[32];

  // Set up for first layer

  NPU_BASE[2] = 4;  // INROWS
  NPU_BASE[3] = 8;  // INCOLS
  NPU_BASE[4] = 4;  // WGHTROWS
  NPU_BASE[5] = 8;  // WGHTCOLS
  NPU_BASE[6] = 0;  // REINPUTS
  NPU_BASE[7] = 0;  // REWEIGHTS
  NPU_BASE[8] = 1;  // SAVEOUT
  NPU_BASE[9] = 1;  // USEBIAS
  NPU_BASE[10] = 0;  // USESUMM
  NPU_BASE[11] = 7;  // SHIFTAMT
  NPU_BASE[12] = 1;  // ACTFN
  NPU_BASE[13] = (unsigned int)npu_data;   // BASE
  NPU_BASE[14] = (unsigned int)npu_result; // RESULT

  flag = 0;
  NPU_BASE[15] = 1;  // INIT
  do {
    asm volatile ("wfi");
  } while (!flag);
  
    
 // ------ LAYER 2 ------

  int npu_data_2[24];

  // Load layer weigths
  
  npu_data_2[0] = 0x5299C738;
  npu_data_2[1] = 0x3E2CDB58;
  npu_data_2[2] = 0xCD134E84;
  npu_data_2[3] = 0xF7B19EC3;
  npu_data_2[4] = 0xDDFF4442;
  npu_data_2[5] = 0xED6E4660;
  npu_data_2[6] = 0x2A208E31;
  npu_data_2[7] = 0x8A8085F0;
  npu_data_2[8] = 0xFC96BC58;
  npu_data_2[9] = 0xDA47B0F0;
  npu_data_2[10] = 0x41B5F665;
  npu_data_2[11] = 0xF2CBB6E7;
  npu_data_2[12] = 0x84CBF84E;
  npu_data_2[13] = 0x2F6B377F;
  npu_data_2[14] = 0x25AEA54A;
  npu_data_2[15] = 0xC5C77ECF;

  // Load bias
  npu_data_2[16] = -128;
  npu_data_2[17] =   41;
  npu_data_2[18] =   18;
  npu_data_2[19] =  -23;
  npu_data_2[20] =  127;
  npu_data_2[21] =   86;
  npu_data_2[22] =  119;
  npu_data_2[23] =   15;


  int npu_result_2[32];

  // Set up for first layer

  NPU_BASE[2] = 4;  // INROWS
  NPU_BASE[3] = 8;  // INCOLS
  NPU_BASE[4] = 8;  // WGHTROWS
  NPU_BASE[5] = 8;  // WGHTCOLS
  NPU_BASE[6] = 1;  // REINPUTS
  NPU_BASE[7] = 0;  // REWEIGHTS
  NPU_BASE[8] = 1;  // SAVEOUT
  NPU_BASE[9] = 1;  // USEBIAS
  NPU_BASE[10] = 0;  // USESUMM
  NPU_BASE[11] = 7;  // SHIFTAMT
  NPU_BASE[12] = 1;  // ACTFN
  NPU_BASE[13] = (unsigned int)npu_data_2;   // BASE
  NPU_BASE[14] = (unsigned int)npu_result_2; // RESULT

  flag = 0;
  NPU_BASE[15] = 1;  // INIT
  do {
    asm volatile ("wfi");
  } while (!flag);
    
 // ------ LAYER 3 ------


  int npu_data_3[24];

  // Load layer weigths
  npu_data_3[0] = 0x008F8993;
  npu_data_3[1] = 0x00000000;
  npu_data_3[2] = 0x00E5997F;
  npu_data_3[3] = 0x00000000;
  npu_data_3[4] = 0x00D90380;
  npu_data_3[5] = 0x00000000;
  npu_data_3[6] = 0x0021813F;
  npu_data_3[7] = 0x00000000;
  npu_data_3[8] = 0x00BA2F94;
  npu_data_3[9] = 0x00000000;
  npu_data_3[10] = 0x00ACDE5E;
  npu_data_3[11] = 0x00000000;
  npu_data_3[12] = 0x005EA0CD;
  npu_data_3[13] = 0x00000000;
  npu_data_3[14] = 0x00A69210;
  npu_data_3[15] = 0x00000000;

  // Load bias
  npu_data_3[16] = -128;
  npu_data_3[17] =  -12;
  npu_data_3[18] =  127;
  npu_data_3[19] =    0;
  npu_data_3[20] =    0;
  npu_data_3[21] =    0;
  npu_data_3[22] =    0;
  npu_data_3[23] =    0;


  int npu_result_3[32];

  // Set up for first layer

  NPU_BASE[2] = 4;  // INROWS
  NPU_BASE[3] = 8;  // INCOLS
  NPU_BASE[4] = 8;  // WGHTROWS
  NPU_BASE[5] = 8;  // WGHTCOLS
  NPU_BASE[6] = 1;  // REINPUTS
  NPU_BASE[7] = 0;  // REWEIGHTS
  NPU_BASE[8] = 1;  // SAVEOUT
  NPU_BASE[9] = 1;  // USEBIAS
  NPU_BASE[10] = 0;  // USESUMM
  NPU_BASE[11] = 0;  // SHIFTAMT
  NPU_BASE[12] = 0;  // ACTFN
  NPU_BASE[13] = (unsigned int)npu_data_3;   // BASE
  NPU_BASE[14] = (unsigned int)npu_result_3; // RESULT

  flag = 0;
  NPU_BASE[15] = 1;  // INIT
  do {
    asm volatile ("wfi");
  } while (!flag);
  unsigned long final_cycles = rdl_cycle();

  int size_3 = sizeof(npu_result_3) / sizeof(npu_result_3[0]);  // Calculate the size of the array
  // Print elements starting from index 2
   for (int i = 0; i < size_3; i++) {
      printf("%d ", npu_result_3[i]);

      // Print a newline after every 8 elements
      if ((i + 1) % 8 == 0) {
          printf("\n");
      }
   }

   printf("\n");  // New line after printing the array

    // Show cycle results
    printf("Intial cycles: %lu\n",init_cycles);
    printf("End cycles: %lu\n",final_cycles);
    unsigned long total_cycles = final_cycles - init_cycles;
    printf("Total cycles: %lu\n",total_cycles);

}
