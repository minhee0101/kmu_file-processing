#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

typedef struct BTNode {
    int n;             //key 개수
    int* k;            //key 값
    struct BTNode** p; //node 포인터 배열
    BTNode(int _n = 0, int* _k = 0, BTNode** _p = nullptr) : n(_n), k(_k), p(_p) {}
} BTNode;

BTNode* getBTNode(int m) {
    BTNode* node = new BTNode;
    node->n = 0;
    node->k = new int[m - 1]; //m-1개 데이터 생성
    node->p = new BTNode*[m]; //m개 노드 생성
    for (int i = 0; i < m; i++) {
        node->p[i] = nullptr;
    }
    return node;
}

typedef BTNode* BTree;

int binarySearch(int K[], int n, int key) {
    int a = 0;
    int b;
    int c = n - 1;
    while (a <= c) {
        b = (a + c) / 2;
        if (K[b] < key) a = b + 1;
        else
            c = b - 1;
    }
    if (K[c] < key) return c + 1;
    else
        return c;
}

/**
 * insertBT inserts newKey into T.
 * @param T: a B-tree
 * @param m: fanout of B-tree
 * @param newKey: a key to insert
 */
void insertBT(BTree* T, int m, int newKey) {
    //노드 비어있을때
    BTNode* node = *T;
    if (T == nullptr) {
        node = getBTNode(m);
        node->k[0] = newKey;
        node->n = 1;
        *T = node;
        return;
    }

    BTNode* x = *T;
    BTNode* y = nullptr;
    stack<BTNode*> s;
    stack<int> is;
    int i = 0;

    while (x != nullptr) {
        i = binarySearch(x->k, x->n, newKey);
        //삽입할 키랑 같은 키 있으면 리턴 (삽입x)
        if (i < x->n && newKey == x->k[i]) return;

        s.push(x);
        is.push(i);
        x = x->p[i]; //포인터 이동시키기
    }

    //키랑 노드 추가하고 부모노드 스텍에서 pop
    while (!s.empty()) {
        x = s.top();
        i = is.top();
        s.pop();
        is.pop();

        //overflow 아닐때
        if (x->n < m - 1) {
            for (int j = x->n - 1; j >= i; j--) x->k[j + 1] = x->k[j];

            x->k[i] = newKey;
            x->n += 1;

            if (y != nullptr) {
                for (int j = x->n; j > i; j--) x->p[j + 1] = x->p[j];
                x->p[i + 1] = y;
            }
            return;
        } else {
            //overflow 일때
            BTNode* tempnode = getBTNode(m + 1); //임시 노드

            //x노드의 key값 temp노드로 저장해놓기, 포인터도
            for (int j = 0; j < i; j++) tempnode->k[j] = x->k[j];
            for (int j = 0; j <= i; j++) tempnode->p[j] = x->p[j];

            tempnode->k[i] = newKey; //새로운 key값넣기
            tempnode->p[i + 1] = y;

            for (int j = i; j < x->n; j++) tempnode->k[j + 1] = x->k[j];
            for (int j = i + 1; j <= x->n; j++) tempnode->p[j + 1] = x->p[j];

            y = getBTNode(m);

            int mid = m / 2;
            //왼쪽 노드에 들어가는거
            for (int j = 0; j <= mid - 1; j++) x->k[j] = tempnode->k[j];
            for (int j = 0; j <= mid; j++) x->p[j] = tempnode->p[j];

            //오른쪽 노드에 들어가는거
            for (int j = mid + 1, l = 0; j < x->n + 1; j++, l++) y->k[l] = tempnode->k[j];
            for (int j = mid + 1, l = 0; j <= x->n + 1; j++, l++) y->p[l] = tempnode->p[j];

            newKey = tempnode->k[mid];
            x->n = mid;
            if (m % 2 == 1) y->n = mid;
            else
                y->n = mid - 1;

            delete tempnode;
        }
    }

    *T = getBTNode(m);
    (*T)->k[0] = newKey;
    (*T)->p[0] = x;
    (*T)->p[1] = y;
    (*T)->n = 1;
}

/**
 * deleteBT deletes oldKey from T.
 * @param T: a B-tree
 * @param m: fanout of B-tree
 * @param oldKey: a key to delete
 */
void deleteBT(BTree* T, int m, int oldKey) {
    BTNode* x = *T;
    BTNode* y = nullptr;
    BTNode* internalNode = nullptr;
    stack<BTNode*> s;
    stack<int> is;
    int i = 0;
    while (x != nullptr) {
        i = binarySearch(x->k, x->n, oldKey);
        if (i < x->n && oldKey == x->k[i]) break;

        s.push(x);
        is.push(i);
        x = x->p[i];
    }
    if (x == nullptr) return;
    if (x->p[i + 1] != nullptr) {
        internalNode = x;
        s.push(x);
        is.push(i + 1);
        x = x->p[i + 1];
        while (x != nullptr) {
            s.push(x);
            is.push(0);
            x = x->p[0];
        }
    }
    if (x == nullptr) {
        x = s.top();
        internalNode->k[i] = x->k[0];
        x->k[0] = oldKey;
        i = is.top();
        s.pop();
        is.pop();
    }

    //delete oldKey form x (x is leafnode)
    x->n -= 1;
    for (int j = i; j < x->n; j++) {
        x->k[j] = x->k[j + 1];
    }
    int a = m % 2 == 1 ? m / 2 : m / 2 - 1; //a
    while (!s.empty()) {
        if (x->n >= a) return;

        //case of underflow
        y = s.top();
        s.pop();
        i = is.top();
        is.pop();

        BTNode* bestSibling = nullptr;
        bool leftisgood = true;
        //bestSibling <-best sibling of x;
        //hey what's up brother
        if (i < y->n - 1 && 0 < i) {
            if (y->p[i - 1]->n < y->p[i + 1]->n) {
                bestSibling = y->p[i + 1];
                leftisgood = false;
            }

            else {
                bestSibling = y->p[i - 1];
                leftisgood = true;
            }
        } else if (i == 0) {
            bestSibling = y->p[i + 1];
            leftisgood = false;
        } else {
            bestSibling = y->p[i - 1];
            leftisgood = true;
        }

        //case of key redistribution  //key jae buun bae
        if (bestSibling->n > a) {
            if (leftisgood == true) {
                for (int j = x->n; j > 0; j--) {
                    x->k[j] = y->k[j - 1];
                }
                for (int j = x->n + 1; j > 0; j--) {
                    x->p[j] = x->p[j - 1];
                }
                x->k[0] = y->k[i - 1];
                y->k[i - 1] = bestSibling->k[bestSibling->n - 1];

                if (x->p[0] != nullptr) x->p[0] = bestSibling->p[bestSibling->n];
            } else {
                x->k[a - 1] = y->k[i];

                if (x->p[0] != nullptr) x->p[a] = bestSibling->p[0];

                y->k[i] = bestSibling->k[0];

                for (int j = 0; j < bestSibling->n - 1; j++) bestSibling->k[j] = bestSibling->k[j + 1];
                for (int j = 0; j < bestSibling->n; j++) bestSibling->p[j] = bestSibling->p[j + 1];
            }
            x->n += 1;
            bestSibling->n -= 1;

            return;
        }
        //merge
        if (leftisgood == true) {
            bestSibling->k[bestSibling->n] = y->k[i - 1];

            for (int j = 0; j < x->n; j++) bestSibling->k[bestSibling->n + j + 1] = x->k[j];
            for (int j = 0; j <= x->n; j++) bestSibling->p[bestSibling->n + j + 1] = x->p[j];

            for (int j = i - 1; j < y->n - 1; j++) y->k[j] = y->k[j + 1];
            for (int j = i; j <= y->n - 1; j++) y->p[j] = y->p[j + 1];

            y->n -= 1;
            bestSibling->n += x->n + 1;

            delete x;
        } else {
            x->k[a - 1] = y->k[i];

            for (int j = 0; j < bestSibling->n; j++) x->k[a + j] = bestSibling->k[j];
            for (int j = 0; j <= bestSibling->n; j++) x->p[a + j] = bestSibling->p[j];

            for (int j = i; j < y->n - 1; j++) y->k[j] = y->k[j + 1];
            for (int j = i + 1; j <= y->n - 1; j++) y->p[j] = y->p[j + 1];

            y->n -= 1;
            x->n += bestSibling->n + 1;
            delete bestSibling;
        }
        x = y;
    }
    if (x->n == 0) {
        *T = x->p[0];
        delete x;
    }
}

/**
 * inorderBT implements inorder traversal in T.
 * @param T: a B-tree
 */
void inorderBT(BTree T) {
    int i = 0;
    if (T == nullptr) return;
    for (i; i < T->n; i++) {
        if (T->p[0] != nullptr) inorderBT(T->p[i]);
        printf("%d ", T->k[i]);
    }
    if (T->p[0] != nullptr) inorderBT(T->p[i]);
}

int main() {
    /* DO NOT MODIFY CODE BELOW */
    FILE* f;
    for (int m = 3; m <= 4; m++) {
        BTree T = nullptr;

        f = fopen("./insertSequence.txt", "r");
        for (int n; !feof(f);) {
            fscanf(f, "%d", &n);
            insertBT(&T, m, n);
            inorderBT(T);
            printf("\n");
        }
        fclose(f);

        f = fopen("./deleteSequence.txt", "r");
        for (int n; !feof(f);) {
            fscanf(f, "%d", &n);
            deleteBT(&T, m, n);
            inorderBT(T);
            printf("\n");
        }
        fclose(f);
    }
}