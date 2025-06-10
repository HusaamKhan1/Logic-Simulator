#include "circuit.h"
#include <stdlib.h>
#include <string.h>

Node* create_node(const char* name, GateType type) {
    Node* node = malloc(sizeof(Node));
    strcpy(node->name, name);
    node->type = type;
    node->input_count = 0;
    node->value = 0;
    return node;
}

void add_node(Circuit* circuit, Node* node) {
    circuit->nodes[circuit->node_count++] = node;
}

Node* find_node(Circuit* circuit, const char* name) {
    for (int i = 0; i < circuit->node_count; i++) {
        if (strcmp(circuit->nodes[i]->name, name) == 0)
            return circuit->nodes[i];
    }
    return NULL;
}

void evaluate_node(Node* node) {
    int a = node->input_count > 0 ? node->inputs[0]->value : 0;
    int b = node->input_count > 1 ? node->inputs[1]->value : 0;

    switch (node->type) {
        case INPUT: break;
        case AND: node->value = a & b; break;
        case OR: node->value = a | b; break;
        case NOT: node->value = !a; break;
        case NAND: node->value = !(a & b); break;
        case NOR: node->value = !(a | b); break;
        case XOR: node->value = a ^ b; break;
    }
}

void evaluate(Circuit* circuit) {
    for (int i = 0; i < circuit->node_count; i++) {
        evaluate_node(circuit->nodes[i]);
    }
}