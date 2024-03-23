#include "fhq.h"
#include <stdio.h>
#include <stdlib.h>

static Node* createNode(int key, void* value)
{
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    node->priority = rand(); // 随机优先级
    node->left = node->right = NULL;
    return node;
}

static void split(Node* t, int key, Node** left, Node** right)
{
    if (!t) {
        *left = *right = NULL;
    } else if (key < t->key) {
        split(t->left, key, left, &t->left);
        *right = t;
    } else {
        split(t->right, key, &t->right, right);
        *left = t;
    }
}

static Node* merge(Node* left, Node* right)
{
    if (!left || !right)
        return left ? left : right;
    if (left->priority > right->priority) {
        left->right = merge(left->right, right);
        return left;
    } else {
        right->left = merge(left, right->left);
        return right;
    }
}

// 插入或更新一个键值对
Node* insert(Node* root, int key, void* value)
{
    Node *left, *right;
    split(root, key, &left, &right);
    Node* newNode = createNode(key, value);
    return merge(merge(left, newNode), right);
}

// 查找操作
Node* search(Node* root, int key)
{
    if (!root)
        return NULL; // 找不到
    if (root->key == key)
        return root; // 找到
    if (key < root->key)
        return search(root->left, key); // 向左查找
    return search(root->right, key); // 向右查找
}

// 内存释放操作
void freeTree(Node* root)
{
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

#if 0

int main()
{
    Node* root = NULL;
    root = insert(root, 50, (void*)500);
    root = insert(root, 30, (void*)300);
    root = insert(root, 20, (void*)200);
    root = insert(root, 40, (void*)400);
    root = insert(root, 70, (void*)700);

    // 查找示例
    int keysToFind[] = { 20, 55, 70 };
    for (int i = 0; i < 3; i++) {
        Node* result = search(root, keysToFind[i]);
        if (result) {
            printf("Key %d found with value %d\n", keysToFind[i], (int)result->value);
        } else {
            printf("Key %d not found\n", keysToFind[i]);
        }
    }

    // 内存释放
    freeTree(root);

    return 0;
}

#endif