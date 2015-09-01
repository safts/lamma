#ifndef __REDUCTION__
#define __REDUCTION__

#include <iostream>
#include <list>

using namespace std;

struct redInfo{

	int betas;
	int alphas;
	int etas;

	redInfo(){
		betas=0;
		alphas=0;
		etas=0;
	}

	void print(){

		cerr << alphas+betas+etas << " reduction" <<
			(((alphas+betas+etas) == 1)? "" : "s");
		if (alphas > 0)
			cerr <<", "<< alphas << " alpha" ;
		if (betas > 0)
			cerr <<", "<< betas << " beta" ;
		if (etas >0)
			cerr <<", " <<  etas << " eta";
	}
	void reset(){
		alphas=0;
		betas=0;
		etas=0;
	}
};


std::list<std::string> FV(TreeNode* expr);

TreeNode * alphaConversion(TreeNode *expr);

static TreeNode *substitute(TreeNode *expr, TreeNode *var, TreeNode *sub);

TreeNode * betaReduction(TreeNode *expr, bool *);

TreeNode * etaReduction(TreeNode *expr, bool *);

TreeNode * evaluate(TreeNode *expr, bool *);

#endif