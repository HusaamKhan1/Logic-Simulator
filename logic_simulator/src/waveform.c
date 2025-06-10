#include <stdio.h>
#include <string.h>
#include "waveform.h"

void print_bar(int* values, int count, const char* label) {
    printf("%-5s: ", label);
    for (int i = 0; i < count; i++) {
        printf(values[i] ? "──" : "__");
        if (i != count - 1) printf(" ");
    }
    printf("\n");
}

void print_waveform(Circuit* circuit, const char* output_name) {
    Node* inputs[16];
    int input_count = 0;
    for (int i = 0; i < circuit->node_count; i++) {
        if (circuit->nodes[i]->type == INPUT)
            inputs[input_count++] = circuit->nodes[i];
    }

    int rows = 1 << input_count;
    int values[rows];
    int output_values[rows];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < input_count; j++) {
            inputs[j]->value = (i >> (input_count - j - 1)) & 1;
        }
        evaluate(circuit);
        Node* out = find_node(circuit, output_name);
        output_values[i] = out ? out->value : 0;
    }

    for (int i = 0; i < input_count; i++) {
        for (int j = 0; j < rows; j++) {
            values[j] = (j >> (input_count - i - 1)) & 1;
        }
        print_bar(values, rows, inputs[i]->name);
    }

    print_bar(output_values, rows, output_name);
}