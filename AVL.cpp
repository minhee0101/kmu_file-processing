#include <iostream>
using namespace std;

struct Treenode {
    int key;
    int bf;

    Treenode* left;
    Treenode* right;
    Treenode(int _key = 0, int _bf = 0, Treenode* _left = nullptr, Treenode* _right = nullptr) : key(_key), bf(_bf), left(_left), right(_right) {}
};

struct AVL {
    Treenode* root = nullptr;
    Treenode* getNode() {
        return new Treenode();
    }
    Treenode* searchParentBST(int searchkey) {
        return searchParentBST(root, searchkey);
    }
    Treenode* searchParentBST(Treenode* T, int searchkey) {
        Treenode* p = T;
        Treenode* q = 0;
        while (p != 0) {
            if (p->key < searchkey) {
                q = p;
                p = p->right;
            } else if (p->key > searchkey) {
                q = p;
                p = p->left;
            } else
                return q;
        }
        return q;
    }
    void inorderAVL(Treenode* T) {
        if (T == nullptr) return;
        inorderAVL(T->left);
        cout << T->key << " ";
        inorderAVL(T->right);
    }
    Treenode* insertBST(Treenode* T, int newkey) {
        Treenode* p;
        Treenode* q;

        p = T;
        q = nullptr;
        while (p != nullptr) {
            if (newkey == p->key) return nullptr;
            q = p;
            if (newkey < p->key) p = p->left;
            else
                p = p->right;
        }

        Treenode* newNode = getNode();
        newNode->key = newkey;

        if (T == nullptr) root = newNode;
        else if (newkey < q->key)
            q->left = newNode;
        else
            q->right = newNode;
        return newNode;
    }

    void insertAVL(Treenode* T, int newkey) {
        Treenode* x;
        Treenode* f;
        string rotationType;
        Treenode* y = insertBST(T, newkey);

        checkBalance(root, y, rotationType, x, f);
        rotateTree(&root, y, rotationType, x, f);
        updateALLBF(&root);
    }

    void deleteAVL(Treenode* T, int deletekey) {
        Treenode* p;
        Treenode* q;
        Treenode* f;
        Treenode* x;

        string rotationType;

        x = nullptr;
        f = nullptr;
        p = T;
        q = nullptr;

        while (p != nullptr && deletekey != p->key) {
            q = p;
            if (deletekey < p->key) p = p->left;
            else
                p = p->right;
        }
        if (p == nullptr) return;

        // ????????? node ?????? 2
        if (p->left != nullptr && p->right != nullptr) {
            Treenode* tempnode;
            tempnode = p;
            //case1.?????? ?????? ?????? ???????????? ??? ?????????
            if (height(p->left) < height(p->right)) {
                p = minnode(p->right);
            }
            //case2.?????? ?????? ?????? ????????? ??? ?????????
            else if (height(p->left) > height(p->right)) {
                p = maxnode(p->left);
            }
            //case3.?????? ?????? ?????? ?????????
            else {
                //case3_1.subtree ??????????????? ??? ????????? ?????? ???
                if (nonode(p->left) >= nonode(p->right)) {
                    p = maxnode(p->left);
                }
                //case3_2. subtree ????????? ????????? ??? ?????????
                else {
                    p = minnode(p->right);
                }
            }
            q = searchParentBST(p->key);
            tempnode->key = p->key;
        }
        //????????? node ?????? 0
        if (p->left == nullptr && p->right == nullptr) {
            if (q == nullptr) root = nullptr;
            else if (q->left == p)
                q->left = nullptr;
            else
                q->right = nullptr;
        }

        // ????????? node ?????? 1
        else {
            if (p->left != nullptr) { //p ???????????? nullptr
                if (q == nullptr) root = root->left;
                else if (q->left == p)
                    q->left = p->left;
                else
                    q->right = p->left;
            } else { // p ????????? nullptr
                if (q == nullptr) root = root->right;
                else if (q->left == p)
                    q->left = p->right;
                else
                    q->right = p->right;
            }
        }
        Treenode* y;
        y = p;

        checkBalance(root, y, rotationType, x, f);
        rotateTree(&root, y, rotationType, x, f);
        updateALLBF(&root);
        delete p;
    }
    //?????? ?????? ????????? ??????
    int height(Treenode* T) {
        if (T == nullptr) return 0;
        int leftsub = height(T->left);
        int rightsub = height(T->right);

        return 1 + max(leftsub, rightsub);
    }

    //?????? ?????? ?????? ?????? ??????
    int nonode(Treenode* T) {
        if (T == nullptr) return 0;
        int leftsub = nonode(T->left);
        int rightsub = nonode(T->right);
        return 1 + leftsub + rightsub;
    }

    //?????? ?????? ????????? ?????????
    Treenode* maxnode(Treenode* T) {
        if (T->right == nullptr) {
            return T;
        } else {
            return maxnode(T->right);
        }
    }

    //?????? ?????? ????????? ?????????
    Treenode* minnode(Treenode* T) {
        if (T->left == nullptr) {
            return T;
        } else {
            return minnode(T->left);
        }
    }

    //bf ???????????? ??????
    int getBF(Treenode* T) {
        if (T == nullptr) return 0;
        return height(T->left) - height(T->right);
    }

    void checkBalance(Treenode* T, Treenode* y, string& rotationType, Treenode*& p, Treenode*& q) {
        Treenode* x;
        Treenode* f;
        updateBF(T, y, &p, &q);
        x = p;
        f = q;

        if (x == nullptr) {
            cout << "NO\n";
            rotationType = "NO";
            return;
        }
        //rebalance tree
        if (x->bf > 1) {
            if (getBF(x->left) < 0) {
                cout << "LR\n";
                rotationType = "LR";
            } else {
                cout << "LL\n";
                rotationType = "LL";
            }
        } else if (x->bf < -1) {
            if (getBF(x->right) > 0) {
                cout << "RL\n";
                rotationType = "RL";

            } else {
                cout << "RR\n";
                rotationType = "RR";
            }
        } else {
            //x = nullptr;
            //f = nullptr;
            cout << "NO\n";
            rotationType = "NO";
        }
    }

    void rotateTree(Treenode** T, Treenode* y, string rotationType, Treenode* p, Treenode* q) {
        Treenode* x;
        Treenode* f;
        x = p;
        f = q;

        if (rotationType == "NO") {
            return;
        }
        if (rotationType == "LL")
            return rotateLL(T, p, q);
        else if (rotationType == "RR")
            return rotateRR(T, p, q);
        else if (rotationType == "LR")
            return rotateLR(T, p, q);
        else
            return rotateRL(T, p, q);
        updateBF(*T, y, &p, &q);
    }
    //BF ????????????
    void updateBF(Treenode* T, Treenode* y, Treenode** x, Treenode** f) {
        Treenode* p = T;
        Treenode* q = nullptr;

        int key = y->key;
        *x = nullptr;
        *f = nullptr;

        while (p != nullptr) {
            p->bf = getBF(p);
            if (p->bf > 1 || p->bf < -1) {
                *f = q;
                *x = p;
            }
            if (p == y) break;
            q = p;

            if (key < p->key) p = p->left;
            else
                p = p->right;
        }
    }
    //?????? BF ????????????
    void updateALLBF(Treenode** T) {
        /* write your code here */
        Treenode* p = *T;
        if (p == nullptr) {
            return;
        }
        if (p->left == nullptr && p->right == nullptr) {
            p->bf = 0;
        } else if (p->left == nullptr || p->right == nullptr) {
            if (p->left == nullptr) {
                p->bf = -height(p->right);
            } else if (p->right == nullptr) {
                p->bf = height(p->left);
            }
        } else {
            p->bf = height(p->left) - height(p->right);
        }
        updateALLBF(&p->left);
        updateALLBF(&p->right);
    }

    //override
    void updateBF(Treenode* T, int deletekey, Treenode** x, Treenode** f) {
        Treenode* p = T;
        Treenode* q = nullptr;

        int key = deletekey;
        *x = nullptr;
        *f = nullptr;

        while (p != nullptr) {
            p->bf = height(p->left) - height(p->right);
            if (p->bf > 1 || p->bf < -1) {
                *f = q;
                *x = p;
            }
            if (p->left == nullptr && p->right == nullptr) break;
            q = p;

            if (key < p->key) p = p->left;
            else
                p = p->right;
        }
    }
    // T: AVL tree
    // x:bf 2??? ??????
    // f:x??? ????????????
    void rotateLL(Treenode** T, Treenode* x, Treenode* p) {
        bool check = true;
        if (p == nullptr) {

            Treenode* sub = x->left;
            x->left = sub->right;
            sub->right = x;
            *T = sub;
            return;
        }
        if (p->right == x) { check = false; }

        Treenode* sub = x->left;
        x->left = sub->right;
        sub->right = x;

        check ? p->left = sub : p->right = sub;
    }
    void rotateRR(Treenode** T, Treenode* x, Treenode* p) {
        bool check = true;
        if (p == nullptr) {
            Treenode* sub = x->right;
            x->right = sub->left;
            sub->left = x;
            *T = sub;
            return;
        }
        if (p->right == x) { check = false; }

        Treenode* sub = x->right;
        x->right = sub->left;
        sub->left = x;

        check ? p->left = sub : p->right = sub;
    }
    //z:x??? ????????? ????????? sub????????? ??????
    void rotateLR(Treenode** T, Treenode* x, Treenode* p) {
        Treenode* sub = x->left;
        bool check = true;

        if (p == nullptr) { // Node x??? root??? ??????

            Treenode* q = x->left->right;
            rotateRR(T, sub, x);
            rotateLL(T, x, p);
            *T = q;
            return;
        }
        if (p->right == x) { check = false; }

        rotateRR(T, sub, x);
        rotateLL(T, x, p);
    }
    void rotateRL(Treenode** T, Treenode* x, Treenode* p) {
        Treenode* sub = x->right;
        bool check = true;
        if (p == nullptr) { // Node x??? root??? ??????

            Treenode* q = x->right->left;
            rotateLL(T, sub, x);
            rotateRR(T, x, p);
            *T = q;
            return;
        }
        if (p->right == x) { check = false; }

        rotateLL(T, sub, x);
        rotateRR(T, x, p);
    }
};

int main() {
#ifndef ONLINE_JUDGE
    freopen("in", "r", stdin);
    freopen("out", "w", stdout);
#endif
    cin.tie(nullptr);
    cout.tie(nullptr);
    ios::sync_with_stdio(false);

    int arr[20] = {40, 11, 77, 33, 20, 90, 99, 70, 88, 80, 66, 10, 22, 30, 44, 55, 50, 60, 25, 49};
    AVL AVLTree;
    //??????
    for (int i = 0; i < 20; i++) {
        AVLTree.insertAVL(AVLTree.root, arr[i]);
        AVLTree.inorderAVL(AVLTree.root);
        cout << "\n";
    }
    //??????
    for (int i = 0; i < 20; i++) {
        AVLTree.deleteAVL(AVLTree.root, arr[i]);
        AVLTree.inorderAVL(AVLTree.root);
        cout << "\n";
    }
    //??????
    for (int i = 0; i < 20; i++) {
        AVLTree.insertAVL(AVLTree.root, arr[i]);
        AVLTree.inorderAVL(AVLTree.root);
        cout << "\n";
    }
    //????????????
    for (int i = 19; i >= 0; i--) {
        AVLTree.deleteAVL(AVLTree.root, arr[i]);
        AVLTree.inorderAVL(AVLTree.root);
        cout << "\n";
    }
}