#ifndef __FHQ_H__
#define __FHQ_H__

#include <stdint.h>

typedef struct Node {
    int key; // 键
    void* value; // 值
    int priority; // 优先级
    struct Node *left, *right;
} Node;

Node* insert(Node* root, int key, void* value);

Node* search(Node* root, int key);

void freeTree(Node* root);

#endif