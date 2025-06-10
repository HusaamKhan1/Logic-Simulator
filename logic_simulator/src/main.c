#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "truth_table.h"
#include "waveform.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <circuit_file>\n", argv[0]);
        return 1;
    }

    const char* circuit_file = argv[1];
    Circuit circuit = { .node_count = 0 };
    char output_name[16] = {0};

    parse_file(circuit_file, &circuit, output_name);

    Node* output_node = find_node(&circuit, output_name);
    if (output_node == NULL) {
        printf("Error: Could not find output node named '%s'\n", output_name);
        return 1;
    }

    // Truth Table
    generate_truth_table(&circuit, output_name);

    // Waveform
    printf("\nWaveform View:\n");
    print_waveform(&circuit, output_name);

    return 0;
}