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

    public:
        FibonacciHeap() {
            minNode = nullptr;
            n = 0;
        }
};


int main(){

    return 0;
}