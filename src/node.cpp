#include <cstdlib>
#include <cstdio>
#include <string>
#include "node.h"

using namespace std;

static void printSpaces(int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf(" ");
    }
}

TreeNode * newTreeNode(TermType type) {
    TreeNode * node = (TreeNode *) malloc(sizeof(TreeNode));
    if (node == NULL) {
        fprintf(stderr,"Out of memory.\n");
    } else {
        node->type = type;
        node->name = NULL;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

void printTree(TreeNode * tree) {
    static int indentno = 0;
    int i;
    if (tree == NULL) return;

    switch (tree->type) {
        case VarT:
            printf("Variable: %s\n", tree->name->c_str());
            break;
        case NumT:
            printf("Numerical: %d\n", tree->value);
            break;
        case IdnT:
            printf("Identifier: %s\n", tree->name->c_str());
            break;
        case AbsT:
            printf("Abstraction:\n");
            break;
        case AppT:
            printf("Application:\n");
            break;
        default:
            printf("Unknown expression kind.\n");
    }
    indentno += 2;
    // for (i = 0; i < 2; i++) {
        if (tree->left != NULL) {
            printSpaces(indentno);
            printTree(tree->left);
        }
        if (tree->right != NULL) {
            printSpaces(indentno);
            printTree(tree->right);
        }
    // }
    // indentno -= 2;
}

void deleteTree(TreeNode* tree) {
    if (tree != NULL) {
        deleteTree(tree->left);
        tree->left = NULL;
        deleteTree(tree->right);
        tree->right = NULL;
        deleteTreeNode(tree);
        // if (tree->name != NULL){
        //     delete(tree->name);
        //     tree->name=NULL;
        // }
        // free(tree);
    }
}

void deleteTreeNode(TreeNode *node) {
    if (node != NULL) {
        if (node->name != NULL) {
            delete(node->name);
        }
        free(node);
    }
}

TreeNode * duplicateTree(TreeNode* tree) {
    if (tree!=NULL) {
        TreeNode *result = newTreeNode(tree->type);
        if (tree->name != NULL){
            result->name = new std::string(*tree->name);
            // delete tree->name;
        }
        result->value = tree->value;
        result->left = duplicateTree(tree->left);
        result->right = duplicateTree(tree->right);
        // deleteTreeNode(tree);
        // tree=NULL;
        return result;
    }
    return NULL;
}

TreeNode * dduplicateTree(TreeNode* tree) {
    return tree;
    if (tree!=NULL) {
        TreeNode *result = newTreeNode(tree->type);
        if (tree->name != NULL){
            result->name = new std::string(*tree->name);
            // delete tree->name;
        }
        result->value = tree->value;
        result->left = duplicateTree(tree->left);
        result->right = duplicateTree(tree->right);
        // deleteTreeNode(tree);
        // tree=NULL;
        return result;
    }
    return NULL;
}

TreeNode * createOperator(std::string *op, TreeNode *left, TreeNode *right) {
    TreeNode *root;
    root = newTreeNode(AppT);
    root->left = newTreeNode(AppT);
    root->left->left = newTreeNode(IdnT);
    root->left->left->name = op;
    root->left->right = left;
    root->right = right;
    return root;
}

TreeNode * createNum(int num) {
    TreeNode *root;
    TreeNode *Temp;
    root = newTreeNode(AbsT);
    root->left = newTreeNode(VarT);
    root->left->name = new std::string("f");
    root->right = newTreeNode(AbsT);
    Temp = root->right;
    Temp->left = newTreeNode(VarT);
    Temp->left->name = new std::string("x");
    for (int i = 0; i < num; ++i)
    {
        Temp->right = newTreeNode(AppT);
        Temp = Temp->right;
        Temp->left = newTreeNode(VarT);
        Temp->left->name = new std::string("f");
    }
    Temp->right = newTreeNode(VarT);
    Temp->right->name = new std::string("x");
    return root;
}

int checkForNumber(TreeNode *tree) {
    TreeNode *Temp = tree;
    std::string *ff, *xx;
    int value = 0;
    if (tree == NULL) return -1;

    if (Temp->type == AbsT) {
        ff = Temp->left->name;
        Temp = Temp->right;
        if ((Temp->type == AbsT) && (Temp->left->name->compare(*ff) != 0)) {
            xx = Temp->left->name;
            Temp = Temp->right;
            while ((Temp->type == AppT) && (Temp->left->type == VarT)) {
                if (Temp->left->name->compare(*ff) != 0) {
                    return -1;
                }
                value++;
                Temp = Temp->right;
            }
            if ((Temp->type == VarT) && (Temp->name->compare(*xx) == 0)) {
                return value;
            }
        }
    }
    return -1;
}
