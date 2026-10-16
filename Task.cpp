#include <iostream>
#include <limits>
using namespace std;

//  NODE STRUCTURE 

struct Node {
    int data;
    Node* left;
    Node* right;
    int height;

    Node(int value) {
        data = value;
        left = NULL;
        right = NULL;
        height = 1;
    }
};

//  AVL TREE CLASS 

class AVLTree {
private:
    Node* root;

    // Return height of a node
    int getHeight(Node* node) {
        if (node == NULL) {
            return 0;
        }
        return node->height;
    }

    // Return maximum of two values
    int maxValue(int a, int b) {
        if (a > b) {
            return a;
        }
        return b;
    }

    // Calculate balance factor
    int getBalance(Node* node) {
        if (node == NULL) {
            return 0;
        }
        return getHeight(node->left) - getHeight(node->right);
    }

    // Update node height
    void updateHeight(Node* node) {
        if (node != NULL) {
            node->height = 1 + maxValue(
                getHeight(node->left),
                getHeight(node->right)
            );
        }
    }

    // Right rotation for LL imbalance
    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* temp = x->right;

        x->right = y;
        y->left = temp;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    // Left rotation for RR imbalance
    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* temp = y->left;

        y->left = x;
        x->right = temp;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    // Insert a new node
    Node* insertNode(Node* node, int value) {
        if (node == NULL) {
            return new Node(value);
        }

        if (value < node->data) {
            node->left = insertNode(node->left, value);
        }
        else if (value > node->data) {
            node->right = insertNode(node->right, value);
        }
        else {
            cout << "Duplicate values are not allowed. " << endl;
            return node;
        }

        updateHeight(node);

        int balance = getBalance(node);

        // LL Case
        if (balance > 1 && value < node->left->data) {
            return rightRotate(node);
        }

        // RR Case
        if (balance < -1 && value > node->right->data) {
            return leftRotate(node);
        }

        // LR Case
        if (balance > 1 && value > node->left->data) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // RL Case
        if (balance < -1 && value < node->right->data) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // Search for a value
    Node* searchNode(Node* node, int value) {
        if (node == NULL || node->data == value) {
            return node;
        }

        if (value < node->data) {
            return searchNode(node->left, value);
        }

        return searchNode(node->right, value);
    }

    // Find the smallest node
    Node* findMin(Node* node) {
        if (node == NULL) {
            return NULL;
        }

        while (node->left != NULL) {
            node = node->left;
        }

        return node;
    }

    // Delete a node
    Node* deleteNode(Node* node, int value) {
        if (node == NULL) {
            return NULL;
        }

        if (value < node->data) {
            node->left = deleteNode(node->left, value);
        }
        else if (value > node->data) {
            node->right = deleteNode(node->right, value);
        }
        else {
            // Node has no left child
            if (node->left == NULL) {
                Node* temp = node->right;
                delete node;
                return temp;
            }

            // Node has no right child
            if (node->right == NULL) {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            // Node has two children
            Node* temp = findMin(node->right);
            node->data = temp->data;
            node->right = deleteNode(node->right, temp->data);
        }

        updateHeight(node);

        int balance = getBalance(node);

        // LL Case
        if (balance > 1 && getBalance(node->left) >= 0) {
            return rightRotate(node);
        }

        // LR Case
        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // RR Case
        if (balance < -1 && getBalance(node->right) <= 0) {
            return leftRotate(node);
        }

        // RL Case
        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // Inorder traversal
    void inorder(Node* node) {
        if (node != NULL) {
            inorder(node->left);
            cout << node->data << " ";
            inorder(node->right);
        }
    }

    // Preorder traversal
    void preorder(Node* node) {
        if (node != NULL) {
            cout << node->data << " ";
            preorder(node->left);
            preorder(node->right);
        }
    }

    // Postorder traversal
    void postorder(Node* node) {
        if (node != NULL) {
            postorder(node->left);
            postorder(node->right);
            cout << node->data << " ";
        }
    }

    // Count total nodes
    int countNodes(Node* node) {
        if (node == NULL) {
            return 0;
        }
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    // Find maximum value
    Node* findMax(Node* node) {
        if (node == NULL) {
            return NULL;
        }

        while (node->right != NULL) {
            node = node->right;
        }

        return node;
    }

    // Display tree sideways
    void displayTree(Node* node, string prefix, bool isLeft) {
        if (node == NULL) {
            return;
        }

        cout << prefix;

        if (isLeft) {
            cout << "|-- ";
        }
        else {
            cout << "-- ";
        }

        cout << node->data << endl;

        displayTree(
            node->left,
            prefix + (isLeft ? "|   " : "    "),
            true
        );

        displayTree(
            node->right,
            prefix + (isLeft ? "|   " : "    "),
            false
        );
    }
    // Delete all nodes using postorder cleanup
    void destroyTree(Node* node) {
        if (node != NULL) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    // Constructor
    AVLTree() {
        root = NULL;
    }

    // Destructor
    ~AVLTree() {
        destroyTree(root);
        root = NULL;
    }

    // Insert value
    void insert(int value) {
        if (searchNode(root, value) != NULL) {
            cout << "Value already exists in the tree.\n";
            return;
        }

        root = insertNode(root, value);
        cout << "Value inserted successfully.\n";
    }

    // Search value
    void search(int value) {
        Node* result = searchNode(root, value);

        if (result != NULL) {
            cout << "Value " << value << " found in the tree.\n";
        }
        else {
            cout << "Value not found.\n";
        }
    }

    // Delete value
    void remove(int value) {
        if (searchNode(root, value) == NULL) {
            cout << "Value not found. Nothing deleted.\n";
            return;
        }

        root = deleteNode(root, value);
        cout << "Value deleted successfully.\n";
    }

    // Display inorder traversal
    void showInorder() {
        if (root == NULL) {
            cout << "Tree is empty.\n";
            return;
        }

        cout << "Inorder: ";
        inorder(root);
        cout << endl;
    }

    // Display preorder traversal
    void showPreorder() {
        if (root == NULL) {
            cout << "Tree is empty.\n";
            return;
        }

        cout << "Preorder: ";
        preorder(root);
        cout << endl;
    }

    // Display postorder traversal
    void showPostorder() {
        if (root == NULL) {
            cout << "Tree is empty.\n";
            return;
        }

        cout << "Postorder: ";
        postorder(root);
        cout << endl;
    }

    // Display minimum value
    void showMinimum() {
        Node* result = findMin(root);

        if (result == NULL) {
            cout << "Tree is empty.\n";
        }
        else {
            cout << "Minimum value: " << result->data << endl;
        }
    }

    // Display maximum value
    void showMaximum() {
        Node* result = findMax(root);

        if (result == NULL) {
            cout << "Tree is empty.\n";
        }
        else {
            cout << "Maximum value: " << result->data << endl;
        }
    }

    // Display tree height
    void showHeight() {
        cout << "Tree height: " << getHeight(root) << endl;
    }

    // Display node count
    void showNodeCount() {
        cout << "Total nodes: " << countNodes(root) << endl;
    }

    // Display balance factor of root
    void showBalance() {
        if (root == NULL) {
            cout << "Tree is empty.\n";
        }
        else {
            cout << "Root value: " << root->data << endl;
            cout << "Root balance factor: " << getBalance(root) << endl;
        }
    }

    // Display complete tree
    void showTree() {
        if (root == NULL) {
            cout << "Tree is empty." << endl;
            return;
        }

        cout << "\nTree structure:" << endl;
        cout << root->data << endl;

        displayTree(root->left, "", true);
        displayTree(root->right, "", false);

        cout << endl;
    }

    // Check if tree is empty
    bool isEmpty() {
        return root == NULL;
    }
};

//  INPUT VALIDATION 

int getInteger() {
    int value;

    while (!(cin >> value)) {
        cout << "Invalid input. Enter an integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return value;
}

//  MAIN FUNCTION 

int main() {
    AVLTree tree;
    int choice;
    int value;

    do {
        cout << "|------------------------------------------------------------------------| " << endl;
        cout << "                       DYNAMIC AVL TREE SYSTEM\n";
        cout << "|------------------------------------------------------------------------| " << endl;
        cout << "1. Insert a value " << endl;
        cout << "2. Search a value " << endl;
        cout << "3. Delete a value " << endl;
        cout << "4. Inorder traversal " << endl;
        cout << "5. Preorder traversal " << endl;
        cout << "6. Postorder traversal " << endl;
        cout << "7. Display minimum value " << endl;
        cout << "8. Display maximum value " << endl;
        cout << "9. Display tree height " << endl;
        cout << "10. Count total nodes " << endl;
        cout << "11. Display root balance factor " << endl;
        cout << "12. Display tree structure " << endl;
        cout << "13. Check if tree is empty " << endl;
        cout << "0. Exit " << endl;
        cout << "|--------------------------------------------| " << endl;
        cout << "Enter your choice: ";

        choice = getInteger();

        switch (choice) {
        case 1: {
            int size;

            cout << "Enter number of values to insert: ";
            size = getInteger();

            if (size <= 0) {
                cout << "Size must be greater than 0." << endl;
                break;
            }

            for (int i = 0; i < size; i++) {
                cout << "Enter value " << i + 1 << ": ";
                value = getInteger();
                tree.insert(value);
            }

            break;
        }
        case 2:
            cout << "Enter value to search: ";
            value = getInteger();
            tree.search(value);
            break;
        case 3:
            cout << "Enter value to delete: ";
            value = getInteger();
            tree.remove(value);
            break;
        case 4:
            tree.showInorder();
            break;
        case 5:
            tree.showPreorder();
            break;
        case 6:
            tree.showPostorder();
            break;
        case 7:
            tree.showMinimum();
            break;
        case 8:
            tree.showMaximum();
            break;
        case 9:
            tree.showHeight();
            break;
        case 10:
            tree.showNodeCount();
            break;
        case 11:
            tree.showBalance();
            break;
        case 12:
            tree.showTree();
            break;
        case 13:
            if (tree.isEmpty()) {
                cout << "Tree is empty.\n";
            }
            else {
                cout << "Tree contains data.\n";
            }
            break;
        case 0:
            cout << "Program ended. Memory cleanup will be performed.\n";
            break;

        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}