#include <iostream>
#include <stack>

using namespace std;

struct Treenode {
    int key;
    int height;

    Treenode* left;
    Treenode* right;
    Treenode(int _key = 0, int _height = 0, Treenode* _left = nullptr, Treenode* _right = nullptr) : key(_key), height(_height), left(_left), right(_right) {}
};
struct BST {
    Treenode* root = nullptr;
    Treenode* getBSTNode() {
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

    void inorderBST(Treenode* T) {
        if (T == nullptr) return;
        inorderBST(T->left);
        cout << T->key << ' ';
        inorderBST(T->right);
    }
    void insertBST(Treenode* T, int newkey) {
        Treenode* p;
        Treenode* q;

        p = T;
        q = nullptr;
        while (p != nullptr) {
            if (newkey == p->key) return;
            q = p;
            if (newkey < p->key) p = p->left;
            else
                p = p->right;
        }

        Treenode* newNode = getBSTNode();
        newNode->key = newkey;

        if (T == nullptr) root = newNode;
        else if (newkey < q->key)
            q->left = newNode;
        else
            q->right = newNode;
    }

    void deleteBST(Treenode* T, int deletekey) {
        Treenode* p;
        Treenode* q;

        p = T;
        q = nullptr;

        while (p != nullptr && deletekey != p->key) {
            q = p;
            if (deletekey < p->key) p = p->left;
            else
                p = p->right;
        }
        if (p == nullptr) return;

        // 삭제할 node 차수 2
        if (p->left != nullptr && p->right != nullptr) {
            Treenode* tempnode;
            tempnode = p;
            //case1.삭제 노드 높이 오른쪽이 더 높을때
            if (height(p->left) < height(p->right)) {
                p = minnode(p->right);
            }
            //case2.삭제 노드 높이 왼쪽이 더 높을때
            else if (height(p->left) > height(p->right)) {
                p = maxnode(p->left);
            }
            //case3.삭제 노드 높이 같을때
            else {
                //case3_1.subtree 왼쪽노드가 더 많거나 같을 때
                if (nonode(p->left) >= nonode(p->right)) {
                    p = maxnode(p->left);
                }
                //case3_2. subtree 오른쪽 노드가 더 많을때
                else {
                    p = minnode(p->right);
                }
            }
            q = searchParentBST(p->key);
            tempnode->key = p->key;
        }

        //삭제할 node 차수 0
        if (p->left == nullptr && p->right == nullptr) {
            if (q == nullptr) root = nullptr;
            else if (q->left == p)
                q->left = nullptr;
            else
                q->right = nullptr;
        }

        // 삭제할 node 차수 1
        else {
            if (p->left != nullptr) { //p 오른쪽이 nullptr
                if (q == nullptr) root = root->left;
                else if (q->left == p)
                    q->left = p->left;
                else
                    q->right = p->left;
            } else { // p 왼쪽이 nullptr
                if (q == nullptr) root = root->right;
                else if (q->left == p)
                    q->left = p->right;
                else
                    q->right = p->right;
            }
        }
        delete p;
    }
    //최대 노드 구하는 메서드
    Treenode* maxnode(Treenode* T) {
        if (T->right == nullptr) {
            return T;
        } else {
            return maxnode(T->right);
        }
    }
    //최소 노드 구하는 메서드
    Treenode* minnode(Treenode* T) {
        if (T->left == nullptr) {
            return T;
        } else {
            return minnode(T->left);
        }
    }
    //노드 높이 구하는 메서드
    int height(Treenode* T) {
        if (T == nullptr) return 0;
        int leftsub = height(T->left);
        int rightsub = height(T->right);

        return 1 + max(leftsub, rightsub);
    }

    //하위 노드 개수 세는 메소드
    int nonode(Treenode* T) {
        if (T == nullptr) return 0;
        int leftsub = nonode(T->left);
        int rightsub = nonode(T->right);
        return 1 + leftsub + rightsub;
    }
};

int main() {
#ifndef ONLINE_JUDGE
    freopen("in", "r", stdin);
    freopen("out", "w", stdout);
#endif
    int arr[20] {25, 500, 33, 49, 17, 403, 29, 105, 39, 66, 305, 44, 19, 441, 390, 12, 81, 50, 100, 999};
    BST BSTTree;
    //삽입
    for (int i = 0; i < 20; i++) {
        BSTTree.insertBST(BSTTree.root, arr[i]);
        BSTTree.inorderBST(BSTTree.root);
        cout << "\n";
    }
    //삭제
    for (int i = 0; i < 20; i++) {

        BSTTree.deleteBST(BSTTree.root, arr[i]);
        BSTTree.inorderBST(BSTTree.root);
        cout << "\n";
    }
    //삽입
    for (int i = 0; i < 20; i++) {
        BSTTree.insertBST(BSTTree.root, arr[i]);
        BSTTree.inorderBST(BSTTree.root);
        cout << "\n";
    }
    //역순삭제
    for (int i = 19; i >= 0; i--) {
        BSTTree.deleteBST(BSTTree.root, arr[i]);
        BSTTree.inorderBST(BSTTree.root);
        cout << "\n";
    }
}