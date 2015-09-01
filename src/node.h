#ifndef _NODE_H_
#define _NODE_H_

#include <iostream>
#include <vector>

extern std::string token;

/* expression types */
typedef enum { VarT, NumT, IdnT, AppT, AbsT } TermType;
typedef enum { NoOp, LtOp, GtOp, EqOp, NeOp, LeOp, GeOp, PlusOp,
	MinusOp, MulOp, DivOp, ExpOp, CommaOp } OpType;

/* tree nodes */
typedef struct treeNode {
    TermType type;

    // NOTE: cannot use Union here because name is checked to free strings
    std::string *name;    // only for VarT
    int value;      // only for integers
    struct treeNode* left;
    struct treeNode* right;
} TreeNode;

/* allocates a memory space for tree node. */
TreeNode * newTreeNode(TermType type);

/* reallocates the memory space for a tree. */
void deleteTree(TreeNode *tree);

/*
 * Deletes a single tree node. Doesn't delete children nodes recursively.
 */
void deleteTreeNode(TreeNode *node);

/* duplicates the tree by allocating a new memory space. */
TreeNode * duplicateTree(TreeNode *tree);

TreeNode * dduplicateTree(TreeNode *tree);

/* Create dentifier. */
TreeNode * createOperator(std::string *op, TreeNode *left, TreeNode *right);

/* prints the syntax tree. */
void printTree(TreeNode * tree);

/* allocates a tree node representing an encoded number. */
TreeNode * createNum(int num);

/* check if a tree node represents a encoded number. */
int checkForNumber(TreeNode *tree);

#endif
