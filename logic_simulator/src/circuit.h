#ifndef CIRCUIT_H
#define CIRCUIT_H

typedef enum {
    INPUT,
    AND, OR, NOT, NAND, NOR, XOR
} GateType;

typedef struct Node {
    char name[16];               // Name of the gate (e.g., "A", "B", "C")
    GateType type;               // Type of gate
    struct Node* inputs[2];      // Pointers to input nodes (up to 2)
    int input_count;             // Number of inputs
    int value;                   // 0 or 1: evaluated value
} Node;

typedef struct {
    Node* nodes[64];             // Array of all nodes in the circuit
    int node_count;              // Total number of nodes
} Circuit;

// Functions we'll define later
Node* create_node(const char* name, GateType type);
void add_node(Circuit* circuit, Node* node);
Node* find_node(Circuit* circuit, const char* name);
void evaluate(Circuit* circuit);

#endif