#ifndef __TRAINER__
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>

typedef struct TNode {
    int type;
    double value;
    struct TNode * left, * right;
} TNode;

// use direct stored value
#define OP_VALUE 0

// basic arithmetic
#define OP_PLUS 1 
#define OP_MINUS 2
#define OP_MULTIPLY 3
#define OP_DIVIDE 4

// left^right power
#define OP_POWER 5
#endif

// allocate tree node on heap, fill it with data and return pointer to it
// no argument checking needed, test env will always pass correct data
TNode * createNode(int type, double value, TNode * left, TNode * right) {
    TNode * result = (TNode *)malloc(sizeof(TNode));
    (*result).type = type;
    (*result).value = value;
    (*result).left = left;
    (*result).right = right;
    return result;
}

// free given (sub)tree recursively, including the node
void freeNode(TNode * node) {
    if (node->left)  freeNode(node->left);
    if (node->right) freeNode(node->right);
    node->left = NULL;
    node->right = NULL;
    free(node);
}

// evaluate the tree, return value of expression
double evaluate(TNode * node) {
    //printf("(%d) %f\n", node->type, node->value);
    if (node == NULL) return NAN;
    if (node->type == 0) {
        return node->value;
    }
    else if (node->type == 1) {
        return (evaluate(node->left) + evaluate(node->right));
    }
    else if (node->type == 2) {
        return (evaluate(node->left) - evaluate(node->right));
    }
    else if (node->type == 3) {
        return (evaluate(node->left) * evaluate(node->right));
    }
    else if (node->type == 4) {
        return (evaluate(node->left) / evaluate(node->right));
    }
    else if (node->type == 5) {
        return ( pow(evaluate(node->left), evaluate(node->right)) );
    } else return NAN;
}

// print the tree
void print(FILE * f, TNode * node) {
    if (node == NULL) return;
    if (node->type == 0) {
        fprintf(f, "%g", node->value);
    }
    else if (node->type == 1) {
        fprintf(f, "(");
        print(f,node->left);
        fprintf(f, " + ");
        print(f,node->right);
        fprintf(f, ")");
    }
    else if (node->type == 2) {
        fprintf(f, "(");
        print(f,node->left);
        fprintf(f, " - ");
        print(f,node->right);
        fprintf(f, ")");
    }
    else if (node->type == 3) {
        fprintf(f, "(");
        print(f,node->left);
        fprintf(f, " * ");
        print(f,node->right);
        fprintf(f, ")");
    }
    else if (node->type == 4) {
        fprintf(f, "(");
        print(f,node->left);
        fprintf(f, " / ");
        print(f,node->right);
        fprintf(f, ")");
    }
    else if (node->type == 5) {
        fprintf(f, "(");
        print(f,node->left);
        fprintf(f, " ^ ");
        print(f,node->right);
        fprintf(f, ")");
    } else return;
}

#ifndef __TRAINER__
int main() {

    TNode * root = nullptr;
    
    root = createNode(OP_PLUS, 0, 
        createNode(OP_VALUE, 42, nullptr, nullptr),
        createNode(OP_VALUE, 13, nullptr, nullptr)
    );

    assert(evaluate(root) == 55);
    freeNode(root);

    root = createNode(OP_DIVIDE, 0, 
        createNode(OP_VALUE, 1000, nullptr, nullptr),
        createNode(OP_VALUE, 4, nullptr, nullptr)
    );
    printf("\n\n\nzaciatok\n");
    assert(evaluate(root) == 250);
    printf("mid\n");
    printf("%g_konec\n\n\n\n", evaluate(root));
    
    root->right->value = 4000;
    assert(evaluate(root) == 0.25);
    root->right->value = 0;
    assert(!isfinite(evaluate(root)));
    freeNode(root);

    root = createNode(OP_POWER, 0,
        createNode(OP_VALUE, 2, nullptr, nullptr),
        createNode(OP_MINUS, 0,
            createNode(OP_VALUE, 7, nullptr, nullptr),
            createNode(OP_MULTIPLY, 0, 
                createNode(OP_VALUE, 1.5, nullptr, nullptr),
                createNode(OP_VALUE, 6, nullptr, nullptr)
            )
        )
    );

    assert(evaluate(root) == 0.25);
    root->left->value = 1;
    assert(evaluate(root) == 1);
    root->left->value = 0.5;
    assert(evaluate(root) == 4);
    printf("chhillujem\n");
    char buf[100];
    FILE * f = fmemopen(buf, sizeof(buf), "w");
    print(f, root);
    printf("nechillujem1\n");
    fclose(f);
    assert(strcmp(buf, "(0.5 ^ (7 - (1.5 * 6)))") == 0);
    printf("nechillujem2\n");
    
    freeNode(root);
    printf("nechillujem3\n");

    //freeNode(nullptr);
    printf("nechillujem4\n");
    
    return EXIT_SUCCESS;
}
#endif