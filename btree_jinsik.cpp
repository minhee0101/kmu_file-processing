/*
 * File processing, 2020
 * btree.cpp
 * implementation of B-tree
 */
using namespace std;
#include <iostream>
#include <stack>
#include <stdio.h>
#include <stdlib.h>

/**
  * BTNode represents a single node in B-tree.
  */
typedef struct BTNode {
    int n, *K;
    struct BTNode** P;
} BTNode;

/**
 * getBTNode retruns a new BTNode.
 * @param m: number of branch points in B-tree
 * @return a new BTNode
 */
BTNode* getBTNode(int m) {
    BTNode* node = (BTNode*)malloc(sizeof(BTNode));
    node->n = 0;
    node->K = (int*)malloc(sizeof(int) * (m - 1));
    node->P = (BTNode**)malloc(sizeof(BTNode*) * m);
    return node;
}

typedef BTNode* BTree;

/**
 * insertBT inserts newKey into T.
 * @param T: a B-tree
 * @param m: number of branch points in B-tree
 * @param newKey: a key to insert
 */
void insertBT(BTree* T, int m, int newKey) {
    /* write your code here */
    BTNode* x = *T;
    if (T == nullptr) {
        x = getBTNode(m);
        x->n = 1;
        x->K[0] = newKey;
        *T = x;
        return;
    }
    int i = 0;
    stack<BTNode> s;
    while (x != nullptr) {
        i = 0;
        while ((i < x->n) && (newKey > x->K[i])) {
            i += 1;
            if (i < x->n && newKey == x->K[i])
                break;
        }
        s.push(*x);
        if (x->P[i] != nullptr) {
            x = x->P[i];
        } else
            break;
    }
    BTNode* tempNode = getBTNode(m + 1);
    BTNode p = s.top();
    x = &p;
    s.pop();
    bool finished = false;
    while (!finished) {
        if (x->n < m - 1) {
            int j = 0;
            while (newKey > x->K[j]) {
                j += 1;
            }
            memcpy(&x->K[j + 1], &x->K[j], sizeof(BTNode) * ((m - 1) - (j + 1)));
            x->n += 1;
            x->K[j] = newKey;
            finished = true;
        }
        int j = 0;
        x->K = tempNode->K;
        while (newKey > tempNode->K[j]) {
            j += 1;
        }
        memcpy(&tempNode->K[j + 1], &tempNode->K[j], sizeof(BTNode) * ((m - 1) - (j + 1)));
        tempNode->K[j] = newKey;
        newKey = tempNode->K[m / 2];
        memcpy(&x->K, &tempNode->K[0], sizeof(BTNode) * (m / 2));
        BTNode* y = getBTNode(m);
        memcpy(&y->K, &tempNode->K[(m / 2) + 1], sizeof(BTNode) * ((m - 1) - (m / 2)));
        x->n = m / 2;
        y->n = (m - 1) - (m / 2);
        if (!s.empty()) {
            x = &s.top();
            s.pop();
        } else {
            BTNode* newlevel = getBTNode(m);
            newlevel->K[0] = newKey;
            newlevel->P[0] = x;
            newlevel->P[1] = y;
            *T = newlevel;
            finished = true;
        }
    }
}

/**
 * deleteBT deletes oldKey from T.
 * @param T: a B-tree
 * @param m: number of branch points in B-tree
 * @param oldKey: a key to delete
 */
void deleteBT(BTree* T, int m, int oldKey) {
    /* write your code here */
}

/**
 * inorderBT implements inorder traversal in T.
 * @param T: a B-tree
 */
void inorderBT(BTree T) {
    /* write your code here */
    for (int i = 0; i < T->n; ++i) {
        inorderBT(T->P[i]);
        cout << T->K[i] << " ";
    }
    inorderBT(T->P[(T->n) - 1]);
}

int main() {
    /* do not modify the code below */

    int insertTestCases[] = {40, 11, 77, 33, 20, 90, 99, 70, 88, 80, 66, 10, 22, 30, 44, 55, 50, 60, 100, 28, 18, 9, 5, 17, 6, 3, 1, 4, 2, 7, 8, 73, 12, 13, 14, 16, 15, 25, 24, 28, 45, 49, 42, 43, 41, 47, 48, 46, 63, 68, 61, 62, 64, 69, 67, 65, 54, 59, 58, 51, 53, 57, 52, 56, 83, 81, 82, 84, 75, 89};
    //int deleteTestCases[] = { 66, 10, 22, 30, 44, 55, 50, 60, 100, 28, 18, 9, 5, 17, 6, 3, 1, 4, 2, 7, 8, 73, 12, 13, 14, 16, 15, 25, 24, 28, 40, 11, 77, 33, 20, 90, 99, 70, 88, 80, 45, 49, 42, 43, 41, 47, 48, 46, 63, 68, 53, 57, 52, 56, 83, 81, 82, 84, 75, 89, 61, 62, 64, 69, 67, 65, 54, 59, 58, 51 };

    BTree T = nullptr;

    for (int tC : insertTestCases) {
        insertBT(&T, 3, tC);
        inorderBT(T);
        printf("\n");
    }
    //for (int tC : deleteTestCases) { deleteBT(&T, 3, tC); inorderBT(T); printf("\n"); }

    T = nullptr;

    for (int tC : insertTestCases) {
        insertBT(&T, 4, tC);
        inorderBT(T);
        printf("\n");
    }
    //for (int tC : deleteTestCases) { deleteBT(&T, 4, tC); inorderBT(T); printf("\n"); }
}