/*
 * File Processing, 2021
 * btree.cpp
 * B-tree implementation
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stack>
#include <stdio.h>
#include <stdlib.h>
/**
  * BTNode represents a node in B-tree.
  */
typedef struct BTNode {
    int n, *K, *A;
    struct BTNode** P;
} BTNode;

/**
 * getBTNode returns a new node.
 * @param m: fanout of B-tree
 * @return a new node
 */
BTNode* getBTNode(int m) {
    BTNode* node = (BTNode*)malloc(sizeof(BTNode));
    node->n = 0;
    node->K = (int*)malloc(sizeof(int) * (m - 1));
    node->A = NULL;
    node->P = (BTNode**)calloc(m, sizeof(BTNode*));
    return node;
}

typedef BTNode* BTree;

/**
 * binarySearch returns index i where K[i-1] < key <= K[i].
 * @param K: an array
 * @param n: size of array
 * @param key: a key to search
 * @return index of key in K
 */
int binarySearch(int K[], int n, int key) {
    int left = 0;
    int right = n - 1;
    int mid;

    while (left <= right) {
        mid = (left + right) / 2;
        if (K[mid] < key)
            left = mid + 1;
        else
            right = mid - 1;
    }

    if (K[right] < key)
        return right + 1;
    else
        return right;
}

/**
 * insertBT inserts newKey into T.
 * @param T: a B-tree
 * @param m: fanout of B-tree
 * @param newKey: a key to insert
 */
void insertBT(BTree* T, int m, int newKey) {
    BTNode* x = *T;
    BTNode* y = NULL;
    int i;
    std::stack<std::pair<BTNode*, int>> stack;

    while (x != NULL) {
        i = binarySearch(x->K, x->n, newKey);

        if (i < x->n && newKey == x->K[i])
            return;

        stack.push({x, i});
        x = x->P[i];
    }

    while (!stack.empty()) {
        x = stack.top().first;
        i = stack.top().second;
        stack.pop();

        // case of non overflow
        if (x->n < m - 1) {
            for (int j = x->n - 1; j >= i; j--) {
                x->K[j + 1] = x->K[j];
            }
            x->K[i] = newKey;
            x->n += 1;
            if (y != NULL) {
                for (int j = x->n; j > i; j--) {
                    x->P[j + 1] = x->P[j];
                }
                x->P[i + 1] = y;
            }
            return;
        }

        // case of overflow
        BTNode* tempNode = getBTNode(m + 1); // be careful about "m+1"

        // copy keys of x and newKey to tempNode
        for (int j = 0; j < i; j++)
            tempNode->K[j] = x->K[j];
        tempNode->K[i] = newKey;
        for (int j = i; j < x->n; j++)
            tempNode->K[j + 1] = x->K[j];

        // copy P of x and y(previous) to tempNode
        for (int j = 0; j <= i; j++)
            tempNode->P[j] = x->P[j];
        tempNode->P[i + 1] = y;
        for (int j = i + 1; j <= x->n; j++)
            tempNode->P[j + 1] = x->P[j];

        y = getBTNode(m);

        // paste keys
        for (int j = 0; j <= m / 2 - 1; j++)
            x->K[j] = tempNode->K[j];
        for (int j = m / 2 + 1, k = 0; j < x->n + 1; j++, k++)
            y->K[k] = tempNode->K[j];

        // paste P
        for (int j = 0; j <= m / 2; j++)
            x->P[j] = tempNode->P[j];
        for (int j = m / 2 + 1, k = 0; j <= x->n + 1; j++, k++)
            y->P[k] = tempNode->P[j];

        newKey = tempNode->K[m / 2];
        x->n = m / 2;
        if (m % 2 == 1)
            y->n = m / 2;
        else
            y->n = m / 2 - 1;

        delete tempNode;
    }

    *T = getBTNode(m);
    (*T)->K[0] = newKey;
    (*T)->P[0] = x;
    (*T)->P[1] = y;
    (*T)->n = 1;
}

/**
 * deleteBT deletes oldKey from T.
 * @param T: a B-tree
 * @param m: fanout of B-tree
 * @param oldKey: a key to delete
 */
void deleteBT(BTree* T, int m, int oldKey) {
    int t = m % 2 == 1 ? m / 2 : m / 2 - 1; // minimum number of keys (underflow)
    BTNode* x = *T;
    BTNode* internalNode = NULL;
    BTNode* y = NULL; // parentNode
    std::stack<std::pair<BTNode*, int>> stack;
    int i;

    while (x != NULL) {
        i = binarySearch(x->K, x->n, oldKey);

        if (i < x->n && oldKey == x->K[i])
            break;

        stack.push({x, i});
        x = x->P[i];
    }

    if (x == NULL)
        return;

    if (x->P[i + 1] != NULL) // case of x is internal node
    {
        internalNode = x;
        stack.push({x, i + 1});
        x = x->P[i + 1];

        while (x != NULL) {
            stack.push({x, 0});
            x = x->P[0];
        }
    }

    if (x == NULL) {
        x = stack.top().first;
        internalNode->K[i] = x->K[0];
        x->K[0] = oldKey;
        i = stack.top().second;
        stack.pop();
    }

    // delete oldKey from x (x is leafNode)
    x->n -= 1;
    for (int j = i; j < x->n; j++)
        x->K[j] = x->K[j + 1];

    while (!stack.empty()) {
        if (x->n >= t)
            return;

        y = stack.top().first;
        i = stack.top().second;
        stack.pop();

        BTNode* bestSibling = NULL;
        bool isRight = false; // check whether bestSibling is on the rigth side of x

        if (0 < i && i < y->n - 1) {
            if (y->P[i - 1]->n < y->P[i + 1]->n) {
                bestSibling = y->P[i + 1];
                isRight = true;
            } else {
                bestSibling = y->P[i - 1];
                isRight = false;
            }
        } else if (i == 0) {
            bestSibling = y->P[i + 1];
            isRight = true;
        } else {
            bestSibling = y->P[i - 1];
            isRight = false;
        }

        // case of redistribution
        if (bestSibling->n > t) {
            if (isRight == false) // bestSibling is on the left side of x
            {
                for (int j = x->n; j > 0; j--)
                    x->K[j] = x->K[j - 1];
                for (int j = x->n + 1; j > 0; j--)
                    x->P[j] = x->P[j - 1];

                x->K[0] = y->K[i - 1];

                y->K[i - 1] = bestSibling->K[bestSibling->n - 1];

                if (x->P[0] != NULL) // case of x is internal node
                    x->P[0] = bestSibling->P[bestSibling->n];
            } else {
                x->K[t - 1] = y->K[i];

                if (x->P[0] != NULL) // case of x in internal node
                    x->P[t] = bestSibling->P[0];

                y->K[i] = bestSibling->K[0];

                for (int j = 0; j < bestSibling->n - 1; j++)
                    bestSibling->K[j] = bestSibling->K[j + 1];
                for (int j = 0; j < bestSibling->n; j++)
                    bestSibling->P[j] = bestSibling->P[j + 1];
            }
            x->n += 1;
            bestSibling->n -= 1;
            return;
        }

        // case of merge
        if (isRight == false) // standard point of merge is bestSibling
        {
            bestSibling->K[bestSibling->n] = y->K[i - 1];
            for (int j = 0; j < x->n; j++)
                bestSibling->K[bestSibling->n + 1 + j] = x->K[j];
            for (int j = 0; j <= x->n; j++)
                bestSibling->P[bestSibling->n + 1 + j] = x->P[j];

            for (int j = i - 1; j < y->n - 1; j++)
                y->K[j] = y->K[j + 1];
            for (int j = i; j <= y->n - 1; j++)
                y->P[j] = y->P[j + 1];

            y->n -= 1;
            bestSibling->n += x->n + 1;

            delete x;
        } else {
            x->K[t - 1] = y->K[i];
            for (int j = 0; j < bestSibling->n; j++)
                x->K[t + j] = bestSibling->K[j];
            for (int j = 0; j <= bestSibling->n; j++)
                x->P[t + j] = bestSibling->P[j];

            for (int j = i; j < y->n - 1; j++)
                y->K[j] = y->K[j + 1];
            for (int j = i + 1; j <= y->n - 1; j++)
                y->P[j] = y->P[j + 1];

            y->n -= 1;
            x->n += bestSibling->n + 1;

            delete bestSibling;
        }

        x = y;
    }

    if (x->n == 0) {
        *T = x->P[0];
        delete x;
    }
}

/**
 * inorderBT implements inorder traversal in T.
 * @param T: a B-tree
 */
void inorderBT(BTree T) {
    if (T == NULL)
        return;
    int i;
    for (i = 0; i < T->n; i++) {
        if (T->P[0] != NULL)
            inorderBT(T->P[i]);
        printf("%d ", T->K[i]);
    }
    if (T->P[0] != NULL)
        inorderBT(T->P[i]);
}

int main() {
    /* DO NOT MODIFY CODE BELOW */
    int insertTestCases[] = {40, 11, 77, 33, 20, 90, 99, 70, 88, 80, 66, 10, 22, 30, 44, 55, 50, 60, 100, 28, 18, 9, 5, 17, 6, 3, 1, 4, 2, 7, 8, 73, 12, 13, 14, 16, 15, 25, 24, 28, 45, 49, 42, 43, 41, 47, 48, 46, 63, 68, 61, 62, 64, 69, 67, 65, 54, 59, 58, 51, 53, 57, 52, 56, 83, 81, 82, 84, 75, 89};
    int deleteTestCases[] = {66, 10, 22, 30, 44, 55, 50, 60, 100, 28, 18, 9, 5, 17, 6, 3, 1, 4, 2, 7, 8, 73, 12, 13, 14, 16, 15, 25, 24, 28, 40, 11, 77, 33, 20, 90, 99, 70, 88, 80, 45, 49, 42, 43, 41, 47, 48, 46, 63, 68, 53, 57, 52, 56, 83, 81, 82, 84, 75, 89, 61, 62, 64, 69, 67, 65, 54, 59, 58, 51};

    BTree T = nullptr;

    for (int i = 0; i < 70; i++) {
        insertBT(&T, 3, insertTestCases[i]);
        inorderBT(T);
        printf("\n");
    }
    for (int i = 0; i < 70; i++) {
        deleteBT(&T, 3, insertTestCases[i]);
        inorderBT(T);
        printf("\n");
    }
}