#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>


using namespace std;

struct Node {
    int key;
    int degree;
    bool mark;
    Node* p;
    Node* child;
    Node* left;
    Node* right;

    Node(int k) {
        key = k;
        degree = 0;
        mark = false;
        p = nullptr;
        child = nullptr;
        left = this;
        right = this;
    }

class FibonacciHeap {
    private:
        Node* minNode;
        int n;

        void link(Node* y, Node* x) {
            y->left->right = y->right;
            y->right->left = y->left;

            y->p = x;
            if (x->child == nullptr) {
                x->child = y;
                y->right = y;
                y->left = y;
            } else {
                y->left = x->child;
                y->right = x->child->right;
                x->child->right->left = y;
                x->child->right = y;
            }
            x->degree++;
            y->mark = false;
        }

        void consolidate() {
            if (minNode == nullptr) return;
    
            int max_degree = (int)(log(n) / log(1.618)) + 5;
            vector<Node*> A(max_degree, nullptr);
    
            vector<Node*> rootList;
            Node* x = minNode;
            if (x) {
                do {
                    rootList.push_back(x);
                    x = x->right;
                } while (x != minNode);
            }
    
            for (Node* w : rootList) {
                Node* x = w;
                int d = x->degree;
                while (A[d] != nullptr) {
                    Node* y = A[d];
                    if (x->key > y->key) {
                        swap(x, y);
                    }
                    link(y, x);
                    A[d] = nullptr;
                    d++;
                }
                A[d] = x;
            }
    
            minNode = nullptr;
            for (int i = 0; i < max_degree; i++) {
                if (A[i] != nullptr) {
                    if (minNode == nullptr) {
                        minNode = A[i];
                        minNode->left = minNode;
                        minNode->right = minNode;
                    } else {
                        A[i]->left = minNode;
                        A[i]->right = minNode->right;
                        minNode->right->left = A[i];
                        minNode->right = A[i];
                        if (A[i]->key < minNode->key) {
                            minNode = A[i];
                        }
                    }
                }
            }
        }

    public:
        FibonacciHeap() {
            minNode = nullptr;
            n = 0;
        }
};


int main(){

    return 0;
}