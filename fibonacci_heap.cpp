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
};


int main(){

    return 0;
}