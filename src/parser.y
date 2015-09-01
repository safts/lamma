%{
	#include <iostream>
	#include <cstdio>
	#include <cstdlib>
	#include "node.h"
	#include "utils.h"

	using namespace std;

	TreeNode * treeRoot;

	bool checkIdent = false;

	// stuff from flex that bison needs to know about:
	extern int yylex();
	extern int yyparse();
	extern int yy_scan_string(const char *str);
	extern FILE *yyin;
	 
	void yyerror(const char *s) { printf("Error: %s\n", s); }
%}

// Bison fundamentally works by asking flex to get the next token, which it
// returns as an object of type "yystype".  But tokens could be of any
// arbitrary data type!  So we deal with that in Bison by defining a C union
// holding each of the types of tokens that Flex could return, and have Bison
// use that union instead of "int" for the definition of "yystype":
%union {
	TreeNode * treeNode;
	int nval;
	std::string *vval;
	std::string *idval;
	std::string *opval;
}

// define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:
%token <nval> NUM
%token <vval> VAR
%token <idval> IDENT
%token <opval> OPAND OPOR
%token <opval> OPLT OPGT OPEQ OPNE OPLE OPGE
%token <opval> OPCOMMA
%token <opval> OPPLUS OPMINUS OPMUL OPDIV OPEXP
%token ERROR

/* Define the type of node our nonterminal symbols represent.
   The types refer to the %union declaration above. Ex: when
   we call an ident (defined by union type ident) we are really
   calling an (NIdentifier*). It makes the compiler happy.
 */
%type <treeNode> top_term term_list term_lo term_cmp term_pr term_sm term_ml term_xp term

/* Operator precedence for mathematical operators */
%left OPAND OPOR
%left OPLT OPGT OPEQ OPNE OPLE OPGE
%left OPPLUS OPMINUS
%left OPEXP OPMUL OPDIV

%start top_term

%%

// this is the actual grammar that bison will parse, but for right now it's just
// something silly to echo to the screen what bison gets from flex.  We'll
// make a real one shortly:

top_term: term_list { treeRoot = $1; }
		;

term_list: term_list term { $$ = newTreeNode(AppT);
							$$->left = $1;
							$$->right = $2;}
		| term_lo  { $$ = $1;}
		;

term_lo: term_lo OPAND term_cmp { $$ = createOperator($2, $1, $3); }
		| term_lo OPOR term_cmp { $$ = createOperator($2, $1, $3); }
		| term_cmp { $$ = $1; }
		;

term_cmp: term_cmp OPLT term_pr { $$ = createOperator($2, $1, $3); }
		| term_cmp OPGT term_pr { $$ = createOperator($2, $1, $3); }
		| term_cmp OPEQ term_pr { $$ = createOperator($2, $1, $3); }
		| term_cmp OPNE term_pr { $$ = createOperator($2, $1, $3); }
		| term_cmp OPLE term_pr { $$ = createOperator($2, $1, $3); }
		| term_cmp OPGE term_pr { $$ = createOperator($2, $1, $3); }
		| term_pr { $$ = $1; }
		;

term_pr: term_pr OPCOMMA term_sm { $$ = createOperator($2, $1, $3); }
		| term_sm { $$ = $1; }
		;

term_sm: term_sm OPPLUS term_ml { $$ = createOperator($2, $1, $3); }
		| term_sm OPMINUS term_ml { $$ = createOperator($2, $1, $3); }
		| term_ml { $$ = $1; }
		;

term_ml: term_ml OPMUL term_xp { $$ = createOperator($2, $1, $3); }
		| term_ml OPDIV term_xp { $$ = createOperator($2, $1, $3); }
		| term_xp { $$ = $1; }
		;

term_xp: term_xp OPEXP term { $$ = createOperator($2, $1, $3); }
		| term { $$ = $1; }
		;

term: ERROR { YYERROR; }
	| VAR { $$ = newTreeNode(VarT);
			$$->name = $1; }
	| NUM { $$ = newTreeNode(NumT);
			$$->value = $1; }
	| IDENT { if (!checkIdentifier($1,checkIdent)) {
					cout<<"Error: Identifier "<< *$1 <<
					" does not exist!!!\n";YYERROR;}
				$$ = newTreeNode(IdnT);
				$$->name = $1; }
	| '\\' VAR '.' term_list { $$ = newTreeNode(AbsT);
								$$->left = newTreeNode(VarT);
								$$->left->name = $2;
								$$->right = $4; }
	| '(' term_list ')' { $$ = $2; }
	;
%%