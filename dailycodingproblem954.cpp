#include<bits/stdc++.h>
using namespace std;

/*
PROBLEM STATEMENT:
A unival tree (universal value tree) is a tree where ALL nodes under it have the same value.
Given the root of a binary tree, count the number of unival subtrees.

Example:
   0
  / \
 1   0
    / \
   1   0
  / \
 1   1

Unival subtrees: 5
1. Leaf node with value 1 (left child of root)
2. Leaf node with value 0 (right child of right child)
3. Leaf node with value 1 (left child of left child of right child)
4. Leaf node with value 1 (right child of left child of right child)
5. Subtree rooted at left child of right child (all nodes have value 1)

INTUITION:
- A subtree is unival if ALL nodes in that subtree have the same value
- We need to check both: 
  1. If current node's children have same value as current node
  2. If the children themselves are roots of unival subtrees
- Use bottom-up approach: check children first, then current node
- Return boolean to indicate if current subtree is unival
- Increment count only when entire subtree is unival
*/

int count = 0;

class node {
    public:
    int data;
    node* left;
    node* right;
    node(int val) {
        data = val;
        left = NULL;
        right = NULL;
    }
};

/*
CORRECTED FUNCTION:
- Returns true if the subtree rooted at 'root' is unival
- Uses bottom-up approach: check children first, then current node
- Only increments count when entire subtree is confirmed unival

YOUR ORIGINAL MISTAKE:
1. You were only checking if immediate children equal parent value
2. You weren't verifying that children themselves are roots of unival subtrees
3. This caused incorrect counting in cases like:
   1
  / \
 1   1
    / \
   0   1
   Your code would count root as unival (wrong!) because immediate children are 1,
   but the subtree contains a 0, making it not unival.
*/
bool findunival(node* root) {
    // Base case: empty tree is considered unival
    if (root == NULL) return true;

    // Step 1: Check if left and right subtrees are unival (bottom-up approach)
    bool left = findunival(root->left);
    bool right = findunival(root->right);
    
    // Step 2: If either child subtree is not unival, current subtree cannot be unival
    if (!left || !right) return false;
    
    // Step 3: Check if current node's value matches its children's values
    if (root->left != NULL && root->left->data != root->data) return false;
    if (root->right != NULL && root->right->data != root->data) return false;
    
    // Step 4: If we reach here, current subtree is unival
    count++;
    return true;
}

int main() {
    // Create the example tree:
    //   0
    //  / \
    // 1   0
    //    / \
    //   1   0
    //  / \
    // 1   1
    node* root = new node(0);
    root->left = new node(1);
    root->right = new node(0);
    root->right->left = new node(1);
    root->right->right = new node(0);
    root->right->left->left = new node(1);
    root->right->left->right = new node(1);
    
    count = 0; // Reset count
    findunival(root);
    cout << "Number of unival subtrees: " << count << endl;
    
    return 0;
}