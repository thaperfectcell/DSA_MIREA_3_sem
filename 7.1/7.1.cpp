#include <iostream>
#include <string>
#include <algorithm>
#include <limits>

using namespace std;

enum Color { RED, BLACK };

struct Node {
    string city;
    Color color;
    Node *left, *right, *parent;

    Node(const string& city = "", Color c = RED)
        : city(city), color(c), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RedBlackTree {
private:
    Node* root;
    Node* NIL;

    void rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != NIL)
            y->left->parent = x;

        y->parent = x->parent;
        if (x->parent == NIL)
            root = y;
        else if (x == x->parent->left)
            x->parent->left  = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    void rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        if (x->right != NIL)
            x->right->parent = y;

        x->parent = y->parent;
        if (y->parent == NIL)
            root = x;
        else if (y == y->parent->left)
        y->parent->left  = x;
        else
            y->parent->right = x;

        x->right = y;
        y->parent = x;
    }

    void insertFixup(Node* z) {
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right; // y дядя
                if (y->color == RED) {               // дядя красный
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {     // дядя черный, z внешний
                        z = z->parent;
                        rotateLeft(z);
                    }
                    // дядя черный, z внутренний
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotateRight(z->parent->parent);
                }
            } else { // Зеркально!
                Node* y = z->parent->parent->left;   // дядя
                if (y->color == RED) {               // дядя красный
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {      // дядя черный, ребенок внутренний
                        z = z->parent;
                        rotateRight(z);
                    }
                    // дядя черный, ребенок внешний
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotateLeft(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    // Поиск узла, поиск минимального узла, замена
    Node* findNode(const string& city) const {
        Node* cur = root;
        while (cur != NIL) {
            if (city < cur->city) cur = cur->left;
            else if (city > cur->city) cur = cur->right;
            else return cur;
        }
        return NIL;
    }

    static Node* minimum(Node* x, Node* NIL) {
        while (x->left != NIL) x = x->left;
        return x;
    }

    void transplant(Node* u, Node* v) {
        if (u->parent == NIL) root = v;
        else if (u == u->parent->left)  u->parent->left  = v;
        else                            u->parent->right = v;
        v->parent = u->parent;
    }

    // Балансировка после удаления
    void deleteFixup(Node* x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;          // брат
                if (w->color == RED) {               // брат красный
                    w->color = BLACK;
                    x->parent->color = RED;          // перекрашиваем узел и брата
                    rotateLeft(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) { // брат черный, оба ребенка черные
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right->color == BLACK) {  // брат черный, дальний ребенок черный
                        w->left->color = BLACK;
                        w->color = RED;
                        rotateRight(w);
                        w = x->parent->right;
                    }
                    // брат черный, дальний ребенок красный
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    rotateLeft(x->parent);
                    x = root;
                }
            } else {
                Node* w = x->parent->left;
                if (w->color == RED) {               // Case 1
                    w->color = BLACK;
                    x->parent->color = RED;
                    rotateRight(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK) { // Case 2
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left->color == BLACK) {   // Case 3
                        w->right->color = BLACK;
                        w->color = RED;
                        rotateLeft(w);
                        w = x->parent->left;
                    }
                    // Case 4
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    // Обходы
    void preorder(Node* node) const {
        if (node == NIL) return;
        cout << node->city << " (" << (node->color == RED ? "R" : "B") << ") ";
        preorder(node->left);
        preorder(node->right);
    }

    void inorder(Node* node) const {
        if (node == NIL) return;
        inorder(node->left);
        cout << node->city << " (" << (node->color == RED ? "R" : "B") << ") ";
        inorder(node->right);
    }

    int height(Node* node) const {
        if (node == NIL) return 0;
        return 1 + max(height(node->left), height(node->right));
    }

    int pathLength(Node* node, const string& city, int depth = 0) const {
        if (node == NIL) return -1;
        if (city == node->city) return depth;
        else if (city < node->city) return pathLength(node->left, city, depth + 1);
        else return pathLength(node->right, city, depth + 1);
    }

    // Удаление узла
    void erase(Node* z) {
        Node* y = z;
        Color yOriginal = y->color;
        Node* x;

        if (z->left == NIL) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == NIL) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right, NIL);
            yOriginal = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;

        if (yOriginal == BLACK) deleteFixup(x);
    }

    // рекурсивное удаление всех узлов
    void destroy(Node* n) {
        if (n == NIL) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

public:
    RedBlackTree() {
        NIL = new Node("", BLACK);
        NIL->left = NIL->right = NIL->parent = NIL;
        root = NIL;
    }

    ~RedBlackTree() {
        destroy(root);
        delete NIL;
    }

    void insert(const string& city) {
        Node* z = new Node(city, RED);
        z->left = z->right = z->parent = NIL;

        Node* y = NIL;
        Node* x = root;

        while (x != NIL) {
            y = x;
            if (z->city < x->city)      x = x->left;
            else if (z->city > x->city) x = x->right;
            else {
                cout << "Node with such value already exists.\n";
                delete z;
                return;
            }
        }

        z->parent = y;
        if (y == NIL)          root = z;
        else if (z->city < y->city) y->left  = z;
        else                        y->right = z;

        insertFixup(z);
    }

    bool remove(const string& city) {
        Node* z = findNode(city);
        if (z == NIL) return false;
        erase(z);
        return true;
    }

    void showPreorder() const {
        cout << "Preorder: ";
        preorder(root);
        cout << endl;
    }

    void showInorder() const {
        cout << "Inorder: ";
        inorder(root);
        cout << endl;
    }

    void showHeight() const {
        cout << "Tree heght(not black height): " << height(root) << endl;
    }

    void showPathLength(const string& city) const {
        int len = pathLength(root, city);
        if (len == -1)
            cout << "Can't find such city.\n";
        else
            cout << "Path length from root to'" << city << "': " << len << endl;
    }
};

void menu() {
    RedBlackTree tree;
    int choice;
    string city;

    do {
        cout << "\nRed-Black Tree Menu (with NIL sentinel)\n";
        cout << "1. Insert city\n";
        cout << "2. Preorder traversal\n";
        cout << "3. Inorder traversal\n";
        cout << "4. Find path length to city\n";
        cout << "5. Find height of the tree\n";
        cout << "6. Delete city\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        if (!(cin >> choice)) return;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                cout << "Enter city name: ";
                getline(cin, city);
                tree.insert(city);
                break;
            case 2:
                tree.showPreorder();
                break;
            case 3:
                tree.showInorder();
                break;
            case 4:
                cout << "Enter city name: ";
                getline(cin, city);
                tree.showPathLength(city);
                break;
            case 5:
                tree.showHeight();
                break;
            case 6:
                cout << "Enter city name: ";
                getline(cin, city);
                if (tree.remove(city)) cout << "Deleted.\n";
                else cout << "There's no such city.\n";
                break;
            case 0:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

// ТЕСТ НА СЛЕДУЮЩИХ ГОРОДАХ N=10 A, B, C, D, E, F, G, H, I, J
int main() {
    menu();
    return 0;
}
