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

    void cut(Node* x, Node* y) {
        if (x->right == x) {
            y->child = nullptr;
        } else {
            x->right->left = x->left;
            x->left->right = x->right;
            if (y->child == x) {
                y->child = x->right;
            }
        }
        y->degree--;

        x->right = minNode->right;
        x->left = minNode;
        minNode->right->left = x;
        minNode->right = x;

        x->p = nullptr;
        x->mark = false;
    }

    void cascadingCut(Node* y) {
        Node* z = y->p;
        if (z != nullptr) {
            if (y->mark == false) {
                y->mark = true;
            } else {
                cut(y, z);
                cascadingCut(z);
            }
        }
    }

    void printTree(Node* node, string prefix, bool isTail) {
        if (node == nullptr) return;

        cout << prefix;
        cout << (isTail ? "+-- " : "|-- ");

        cout << "[Key: " << node->key << "]";
        if (node->mark) cout << " (M)";
        if (node == minNode) cout << " <--- MIN";
        cout << endl;

        vector<Node*> children;
        if (node->child != nullptr) {
            Node* current = node->child;
            do {
                children.push_back(current);
                current = current->right;
            } while (current != node->child);
        }

        for (size_t i = 0; i < children.size(); ++i) {
            printTree(children[i], prefix + (isTail ? "    " : "|   "), i == children.size() - 1);
        }
    }

public:
    FibonacciHeap() {
        minNode = nullptr;
        n = 0;
    }

    Node* insert(int key) {
        Node* node = new Node(key);
        if (minNode == nullptr) {
            minNode = node;
        } else {
            node->left = minNode;
            node->right = minNode->right;
            minNode->right->left = node;
            minNode->right = node;
            if (key < minNode->key) {
                minNode = node;
            }
        }
        n++;
        return node;
    }

    int findMin() {
        if (minNode) return minNode->key;
        return -1;
    }

    int extractMin() {
        if (minNode == nullptr) {
            cout << "El heap está vacío." << endl;
            return -1;
        }

        Node* z = minNode;
        if (z->child != nullptr) {
            Node* child = z->child;
            vector<Node*> children;
            do {
                children.push_back(child);
                child = child->right;
            } while (child != z->child);

            for (Node* x : children) {
                x->right = minNode->right;
                x->left = minNode;
                minNode->right->left = x;
                minNode->right = x;
                x->p = nullptr;
            }
        }

        z->left->right = z->right;
        z->right->left = z->left;

        if (z == z->right) {
            minNode = nullptr;
        } else {
            minNode = z->right;
            consolidate();
        }
        n--;
        int minKey = z->key;
        delete z;
        return minKey;
    }

    void decreaseKey(Node* x, int newKey) {
        if (newKey > x->key) {
            cout << "Error: Nueva clave es mayor que la actual." << endl;
            return;
        }
        x->key = newKey;
        Node* y = x->p;
        if (y != nullptr && x->key < y->key) {
            cut(x, y);
            cascadingCut(y);
        }
        if (x->key < minNode->key) {
            minNode = x;
        }
    }

    void deleteNode(Node* x) {
        decreaseKey(x, -2147483648);
        extractMin();
    }

    void traverseAndCollect(Node* node, vector<string>& nodesJson, vector<string>& edgesJson, int level) {

        stringstream ss;
        ss << "{ \"id\": \"" << (long long)node << "\", \"label\": \"" << node->key << "\", \"group\": \"" << (node == minNode ? "min" : (node->mark ? "marked" : "node")) << "\"";
        ss << ", \"title\": \"Degree: " << node->degree << "\", \"level\": " << level;
        ss << " }";
        nodesJson.push_back(ss.str());

        if (node->right) {
            stringstream ssEdge;
            float roundnessR = (node->right == node) ? 0.2 : 0.2;
            string fontR = (node->right == node) ? ", \"font\": { \"align\": \"bottom\", \"background\": \"#252526\", \"color\": \"#00FF00\" }" : "";

            ssEdge << "{ \"from\": \"" << (long long)node << "\", \"to\": \"" << (long long)node->right << "\", \"label\": \"R\", \"arrows\": \"to\", \"color\": { \"color\": \"#00FF00\" }, \"smooth\": { \"type\": \"curvedCW\", \"roundness\": " << roundnessR << " }" << fontR << " }";
            edgesJson.push_back(ssEdge.str());
        }

        if (node->left) {
            stringstream ssEdge;
            string typeL = (node->left == node) ? "curvedCW" : "curvedCCW";
            float roundnessL = (node->left == node) ? 0.6 : 0.5;
            string colorL = (node->left == node) ? "#FF00FF" : "#00FF00";
            string fontL = (node->left == node) ? ", \"font\": { \"align\": \"top\", \"background\": \"#252526\", \"color\": \"#FF00FF\" }" : "";

            ssEdge << "{ \"from\": \"" << (long long)node << "\", \"to\": \"" << (long long)node->left << "\", \"label\": \"L\", \"arrows\": \"to\", \"color\": { \"color\": \"" << colorL << "\" }, \"smooth\": { \"type\": \"" << typeL << "\", \"roundness\": " << roundnessL << " }" << fontL << " }";
            edgesJson.push_back(ssEdge.str());
        }

        if (node->p) {
            stringstream ssEdge;
            ssEdge << "{ \"from\": \"" << (long long)node << "\", \"to\": \"" << (long long)node->p << "\", \"label\": \"P\", \"arrows\": \"to\", \"dashes\": true, \"color\": { \"color\": \"#888888\" }, \"smooth\": { \"type\": \"curvedCW\", \"roundness\": 0.15 } }";
            edgesJson.push_back(ssEdge.str());
        }

        if (node->child) {
            stringstream ssEdge;
            ssEdge << "{ \"from\": \"" << (long long)node << "\", \"to\": \"" << (long long)node->child << "\", \"label\": \"Child\", \"arrows\": \"to\", \"width\": 2, \"color\": { \"color\": \"#FFFFFF\" }, \"smooth\": { \"type\": \"curvedCW\", \"roundness\": 0.15 } }";
            edgesJson.push_back(ssEdge.str());

            Node* startNode = node->child;
            Node* curr = startNode;
            do {
                traverseAndCollect(curr, nodesJson, edgesJson, level + 1);
                curr = curr->right;
            } while (curr != startNode);
        }
    }

    void getGraphData(vector<string>& nodesJson, vector<string>& edgesJson) {
        if (minNode == nullptr) return;

        Node* startNode = minNode;
        Node* curr = startNode;
        if (curr) {
            do {
                traverseAndCollect(curr, nodesJson, edgesJson, 0);
                curr = curr->right;
            } while (curr != startNode);
        }
    }

    void display() {
        if (minNode == nullptr) {
            cout << "(Heap Vacio)" << endl;
            return;
        }
        cout << "Estructura del Heap (Raices):" << endl;
        cout << "-----------------------------" << endl;

        vector<Node*> roots;
        Node* curr = minNode;
        if (curr) {
            do {
                roots.push_back(curr);
                curr = curr->right;
            } while (curr != minNode);
        }

        for (size_t i = 0; i < roots.size(); ++i) {
            printTree(roots[i], "", true);
            cout << "   (Siguiente raiz -->)" << endl;
        }
        cout << "-----------------------------" << endl;
        cout << "Total Nodos: " << n << endl << endl;
    }

    void unionHeap(FibonacciHeap* h2) {
        if (h2->minNode == nullptr) return;
        if (minNode == nullptr) {
            minNode = h2->minNode;
            n = h2->n;
        } else {
            Node* min1 = minNode;
            Node* min2 = h2->minNode;
            Node* right1 = min1->right;
            Node* left2 = min2->left;

            min1->right = min2;
            min2->left = min1;
            left2->right = right1;
            right1->left = left2;

            if (h2->minNode->key < minNode->key) {
                minNode = h2->minNode;
            }
            n += h2->n;
        }
        h2->minNode = nullptr;
        h2->n = 0;
    }
};

string allSnapshotsJSON = "[";
bool firstSnapshot = true;

void saveSnapshot(FibonacciHeap* h1, FibonacciHeap* h2, string description) {
    if (!firstSnapshot) {
        allSnapshotsJSON += ",";
    }
    firstSnapshot = false;

    vector<string> nodes, edges;
    if (h1) h1->getGraphData(nodes, edges);
    if (h2) h2->getGraphData(nodes, edges);

    stringstream ss;
    ss << "{ \"description\": \"" << description << "\", \"nodes\": [";
    for (size_t i = 0; i < nodes.size(); ++i) {
        ss << nodes[i] << (i < nodes.size() - 1 ? "," : "");
    }
    ss << "], \"edges\": [";
    for (size_t i = 0; i < edges.size(); ++i) {
        ss << edges[i] << (i < edges.size() - 1 ? "," : "");
    }
    ss << "] }";

    allSnapshotsJSON += ss.str();
}

void writeJsonFile() {
    allSnapshotsJSON += "]";
    ofstream outFile("output/data.json");
    outFile << allSnapshotsJSON;
    outFile.close();
    cout << "Archivo JSON generado en output/data.json" << endl;
}

void printHeader(string titulo) {
    cout << "\n========================================" << endl;
    cout << " PRUEBA: " << titulo << endl;
    cout << "========================================" << endl;
}

int main(){

    return 0;
}