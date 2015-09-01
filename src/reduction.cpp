#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <typeinfo>
#include <list>

#include "node.h"
#include "reduction.h"
#include "utils.h"

#include <algorithm>

extern redInfo reductions;
extern TreeNode * treeRoot;

using namespace std;

void print2str(TreeNode * tree, bool mostRight, bool par) {
    if (tree == NULL) return;
    int num;
    switch (tree->type) {
        case VarT:
            cout << *tree->name;
            break;
        case NumT:
            cout << tree->value;
            break;
        case IdnT:
            cout << *tree->name;
            break;
        case AbsT:
            if ((num = checkForNumber(tree)) >= 0) {
                cout << num;
                break;
            }
            if (par || !mostRight)
                cout << "(";
            cout << "\\";
            print2str(tree->left, false, false);
            cout << ".";
            print2str(tree->right, true, par);
            if (par || !mostRight)
                cout << ")";
            break;
        case AppT:
            if (par)
                cout << "(";
            print2str(tree->left, false, par);
            cout << " ";
            if (!par && tree->right->type == AppT) 
                cout << "(";
            print2str(tree->right, mostRight, par);
            if (!par && tree->right->type == AppT) 
                cout << ")";
            if (par)
                cout << ")";
            break;
        default:
            printf("Unknown term type.\n");
    }
}

list<string> FV(TreeNode* expr){

	list<string> set;
	switch (expr->type) {
        case VarT:
        	set.push_front(*(expr->name));
            break;
        case AbsT:
        	set = FV(expr->right);
        	set.remove(*(expr->left->name));
            break;
        case AppT:
        	list <string> temp1 = FV(expr->left);
            list <string> temp2 = FV(expr->right);
            temp1.insert(temp1.end(), temp2.begin(), temp2.end());
            set = temp1;
            break;
    }
    return set;
}


TreeNode * alphaConversion(TreeNode *expr) {

    list<string> set = FV(expr->right);
    string name = *(expr->left->name);

    list<string>::iterator it;
    it = find(set.begin(), set.end(), name);

    int i = 0;
	stringstream tempN;

    while ( it != set.end()) {
		tempN.str("");
		tempN << name;
		tempN << "_" << i;	

    	it = find(set.begin(), set.end(), tempN.str());
    	i++;
    }
    name = tempN.str();
	reductions.alphas++;

    TreeNode *var = newTreeNode(VarT);
    var->name = new string(name);
    TreeNode *result = substitute(expr->right, expr->left, var);

    expr->right = result;
    expr->left = var;
    return expr;
}

TreeNode* etaReduction(TreeNode* expr, bool* reduced){
    string * varName = expr->left->name;
    *reduced = false;
    if (expr == NULL)
        return expr;
    if (expr->right->type == AppT &&
        expr->right->right->type == VarT &&
        expr->right->right->name->compare(*varName) == 0) {
        list<string> set = FV(expr->right->left);
        list<string>::iterator it;
        it = find(set.begin(), set.end(), *varName);
        if (it == set.end()) {
            deleteTree(expr->left);
            deleteTree(expr->right->right);
            *reduced = true;
            reductions.etas++;
            return expr->right->left;
        }
    }
    return expr;
}

static TreeNode *substitute(TreeNode *expr, TreeNode *var, TreeNode *sub) {
    string parname;
    TreeNode * result = NULL;
    switch (expr->type) {
        case VarT:
        	if (var != NULL && (*(expr->name)).compare(*(var->name)) == 0) {
                deleteTree(expr);
                return duplicateTree(sub);
            } else {
                return expr;
            }
        case AbsT:
            parname = *(expr->left->name);
            if (parname.compare(*var->name) != 0) {
                
                list<string> set = FV(sub);

                list<string>::iterator it;
                it = find(set.begin(), set.end(), parname);

                while (it != set.end()) {  // do alpha conversion
                    alphaConversion(expr);
                    parname = *(expr->left->name);
                    it = find(set.begin(), set.end(), parname);
                }

               	expr->right = substitute(expr->right, var, sub);
            }

            return expr;
        case AppT:
            expr->left = substitute(expr->left, var, sub);
            expr->right = substitute(expr->right, var, sub);
            return expr;
    }
   	return expr;
}

TreeNode * betaReduction(TreeNode *expr, bool* reduced) {
    TreeNode* left = expr->left;
    if (left->type != AbsT) {
        *reduced = false;
    	return expr;
    } else {
        TreeNode* result = 
        	substitute(left->right,left->left,expr->right);
        reductions.betas++;
        *reduced = true;
        return result;
    }
    return expr;
}


TreeNode * evaluate(TreeNode *expr, bool* reduced) {
    bool redu;
    if (expr != NULL) {
		TreeNode* term;
        switch (expr->type) {
        	case NumT:
        		term = createNum(expr->value);
                *reduced = false;
                return term;
        	case IdnT:
        		term = findIdentifier(*(expr->name));
                *reduced = true;
                return term;
            case VarT:
            	*reduced = false;
                return expr;
            case AbsT:
                expr = etaReduction(expr, &redu);
                if (redu) {
                    *reduced = redu;
                    return expr;
                }
                expr->right = evaluate(expr->right, &redu);
                *reduced = redu;
                return expr;
            case AppT:
                while (expr->left->type == IdnT ||
                        expr->left->type == NumT) {
                    expr->left = evaluate(expr->left, &redu);
                }
                if (expr->left->type != AbsT) {
                    expr->left = evaluate(expr->left, &redu);
                    *reduced = redu;
                    if (redu)
                        return expr;
                    expr->right = evaluate(expr->right, &redu);
                    *reduced = redu;
                    return expr;
                }
                expr = betaReduction(expr, &redu);
                *reduced = redu;
                return expr;
        }
    }
    return expr;
}
