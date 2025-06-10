#include "truth_table.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

void generate_truth_table(Circuit* circuit, const char* output_name) {
    int input_count = 0;
    Node* inputs[16];

    // Collect all INPUT nodes
    for (int i = 0; i < circuit->node_count; i++) {
        if (circuit->nodes[i]->type == INPUT)
            inputs[input_count++] = circuit->nodes[i];
    }

    // Open file for writing
    FILE* f = fopen("truth_table.csv", "w");
    if (!f) {
        perror("Failed to create output file");
        return;
    }

    // Print header
    for (int i = 0; i < input_count; i++) {
        printf("%s ", inputs[i]->name);
        fprintf(f, "%s,", inputs[i]->name);
    }
    printf("| %s\n", output_name);
    fprintf(f, "%s\n", output_name); // Last column no comma

    int rows = 1 << input_count;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < input_count; j++) {
            int val = (i >> (input_count - j - 1)) & 1;
            inputs[j]->value = val;
            printf("%d ", val);
            fprintf(f, "%d,", val);
        }

        evaluate(circuit);
        Node* output = find_node(circuit, output_name);
        printf("| %d\n", output->value);
        fprintf(f, "%d\n", output->value);
    }

    fclose(f);
}