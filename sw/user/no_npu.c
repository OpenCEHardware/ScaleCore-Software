// #include <stdio.h>
// #include <math.h>
// #include <limits.h>
// #include <stdlib.h>
// #include <string.h>
#include <stdint.h>
#include "measure.h"

// Define the input size, hidden layers size, and output size
#define INPUT_SIZE 4
#define HIDDEN_SIZE 8
#define OUTPUT_SIZE 3

// Activation function: ReLU
uint8_t relu(short x) {
    short result = (x > 0) ? x : 0; 
    result = result >> 7;
    return result;
}

// Function to perform matrix multiplication and add biases
void dense_layer1(const int8_t *inputs, const int8_t *weights, const int8_t *biases, short *output, int hidden_size , int input_size) {
    for (int j = 0; j < hidden_size; j++) {
        output[j] = biases[j]; // Start with the bias
        for (int i = 0; i < input_size; i++) {
            // Perform the dot product and add it to the bias
            output[j] += inputs[i] * weights[i * hidden_size + j];
        }
    }
}

void dense_layer2(const short *inputs, const int8_t *weights, const int8_t *biases, short *output, int hidden_size , int input_size) {
    for (int j = 0; j < hidden_size; j++) {
        output[j] = biases[j]; // Start with the bias
        for (int i = 0; i < input_size; i++) {
            // Perform the dot product and add it to the bias
            output[j] += inputs[i] * weights[i * hidden_size + j];
        }
    }
}

void dense_out(const short *inputs, const int8_t *weights, const int8_t *biases, short *output, int hidden_size , int input_size) {
    for (int j = 0; j < hidden_size; j++) {
        output[j] = biases[j]; // Start with the bias
        for (int i = 0; i < input_size; i++) {
            // Perform the dot product and add it to the bias
            output[j] += inputs[i] * weights[i * hidden_size + j];
        }
    }
}

void no_npu_inference() {

    // Capture initial cycles
    unsigned long init_cycles = rdl_cycle();

    // Define inputs (scaled to 16-bit short)
    const int8_t input_matrix[4 * INPUT_SIZE] = {
        77, -36, 54, -72,
        96, -26, 29, -93,
        95, -37, 33, -78,
        110, 32, -59, -122,
    };
        
    const int8_t dense1_weights[INPUT_SIZE * HIDDEN_SIZE]= {
        85, 37, -3, -115, -110, -98, 95, -14,
        98, 10, 13, -15, -43, 69, 68, 37,
        -35, -128, -26, 53, -20, -5, 127, -81,
        -58, -47, 43, -57, -53, 94, 34, 109
    };

    const int8_t dense1_biases[HIDDEN_SIZE] = {
    -128, -91, 10, -89, 10, 10, 127, 10
    };

    const int8_t dense2_weights[HIDDEN_SIZE * HIDDEN_SIZE]= {
        74, -91, -82, 37, -49, 126, -57, -59,
        78, -8, -53, -124, 127, 55, 107, 47,
        101, -10, -75, 65, -25, -74, -53, -14,
        88, -68, -106, -4, -16, -80, 71, -38,
        49, -114, 32, 42, -16, -123, -128, -118,
        66, 68, -1, -35, 96, 70, 110, -19,
        -124, 78, 19, -51, -61, -98, -79, -9,
        56, -57, -103, 82, 88, -37, 44, 62
    };

    const int8_t dense2_biases[HIDDEN_SIZE] = {
    -128, 41, 18, -23, 127, 86, 119, 15
    };

    const int8_t output_weights[HIDDEN_SIZE * OUTPUT_SIZE]  = {
        16, -110, -90,
        -51, -96, 94,
        94, -34, -84,
        -108, 47, -70,
        63, -127, 33,
        -128, 3, -39,
        127, -103, -27,
        -109, -119, -113
    };

    const int8_t output_biases[OUTPUT_SIZE] = {
    -128, -12, 127
    };

    short results[INPUT_SIZE*OUTPUT_SIZE];

    for (int i = 0; i < 4; i++)
    {
        
        int8_t input[INPUT_SIZE];
        for (int j = 0; j < INPUT_SIZE; j++) {
            input[j] = input_matrix[i*INPUT_SIZE + j];
        }

        // Define arrays to hold intermediate results
        short hidden1[HIDDEN_SIZE];
        short hidden2[HIDDEN_SIZE];
        short output[OUTPUT_SIZE];

        // Forward pass through the network
        dense_layer1(input, dense1_weights, dense1_biases, hidden1, HIDDEN_SIZE, INPUT_SIZE);

        // Apply ReLU activation on hidden1
        for (int i = 0; i < HIDDEN_SIZE; i++) {
            hidden1[i] = relu(hidden1[i]);
        }
        
        dense_layer2(hidden1, dense2_weights, dense2_biases, hidden2, HIDDEN_SIZE, HIDDEN_SIZE);

        // Apply ReLU activation on hidden2
        for (int i = 0; i < HIDDEN_SIZE; i++) {
            hidden2[i] = relu(hidden2[i]);
        }

        // Output layer (dense)
        dense_layer2(hidden2, output_weights, output_biases, output, OUTPUT_SIZE, HIDDEN_SIZE);

        for (int k = 0; k < OUTPUT_SIZE; k++) {
            results[(i*OUTPUT_SIZE)+k] = output[k];
        }

    }
    unsigned long final_cycles = rdl_cycle();

    for (int i = 0; i < OUTPUT_SIZE*INPUT_SIZE; i++) {
      printf("%d ", results[i]);
      // Print a newline after every 3 elements
      if ((i + 1) % OUTPUT_SIZE == 0) {
          printf("\n");
      }
    }
    printf("\n");

    // Show cycle results
    printf("Intial cycles: %lu\n",init_cycles);
    printf("End cycles: %lu\n",final_cycles);
    unsigned long total_cycles = final_cycles - init_cycles;
    printf("Total cycles: %lu\n",total_cycles);
}