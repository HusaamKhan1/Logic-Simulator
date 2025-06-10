#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "circuit.h"

// Trim leading and trailing spaces in-place
void trim(char* str) {
    while (*str == ' ') memmove(str, str + 1, strlen(str));
    size_t len = strlen(str);
    while (len > 0 && str[len - 1] == ' ') {
        str[len - 1] = '\0';
        len--;
    }
}

GateType parse_type(const char* s) {
    if (strcmp(s, "INPUT") == 0) return INPUT;
    if (strcmp(s, "AND") == 0) return AND;
    if (strcmp(s, "OR") == 0) return OR;
    if (strcmp(s, "NOT") == 0) return NOT;
    if (strcmp(s, "NAND") == 0) return NAND;
    if (strcmp(s, "NOR") == 0) return NOR;
    if (strcmp(s, "XOR") == 0) return XOR;
    return INPUT; // fallback
}

void parse_file(const char* filename, Circuit* circuit, char* output_name) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        perror("Could not open circuit file");
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = 0;

        char name[16], op[16], in1[16], in2[16];

        // 1. 2-input gate: C = AND(A, B), XOR, NAND, NOR
        if (sscanf(line, "%[^=]= %[^ (](%[^,], %[^)])", name, op, in1, in2) == 4) {
            trim(name); trim(op); trim(in1); trim(in2);
            Node* node = create_node(name, parse_type(op));
            Node* in_node1 = find_node(circuit, in1);
            Node* in_node2 = find_node(circuit, in2);
            if (in_node1 == NULL || in_node2 == NULL) continue;
            node->inputs[0] = in_node1;
            node->inputs[1] = in_node2;
            node->input_count = 2;
            add_node(circuit, node);
        }

        // 2. 1-input gate: D = NOT(C)
        else if (sscanf(line, "%[^=]= %[^ (](%[^)])", name, op, in1) == 3) {
            trim(name); trim(op); trim(in1);
            Node* node = create_node(name, parse_type(op));
            Node* input_node = find_node(circuit, in1);
            if (input_node == NULL) continue;
            node->inputs[0] = input_node;
            node->input_count = 1;
            add_node(circuit, node);
        }

        // 3. OUTPUT = D
        else if (strstr(line, "OUTPUT") != NULL) {
            char* equals = strchr(line, '=');
            if (equals != NULL) {
                sscanf(equals + 1, " %s", output_name);
                trim(output_name);
            }
        }

        // 4. A = INPUT
        else if (sscanf(line, "%[^=]= %s", name, op) == 2) {
            trim(name); trim(op);
            Node* node = create_node(name, parse_type(op));
            add_node(circuit, node);
        }
    }

    fclose(f);
}