#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <algorithm>

#include "node.h"
#include "reduction.h"
#include "utils.h"
#include "getCPUTime.h"

using namespace std;

extern int yyparse();
extern int yy_scan_string(const char *str);
extern FILE *yyin;
extern TreeNode * treeRoot;
extern IdentifierList identList;
extern bool checkIdent;

bool trace = false;
bool parenthes = false;

redInfo reductions;

void print2str(TreeNode * tree, stringstream& treeStr, bool mostRight, bool par) {
    if (tree == NULL) return;
    int num;
    switch (tree->type) {
        case VarT:
        	treeStr << *tree->name;
            break;
        case NumT:
        	treeStr << tree->value;
            break;
        case IdnT:
        	treeStr << *tree->name;
            break;
        case AbsT:
        	if ((num = checkForNumber(tree)) >= 0) {
        		treeStr << num;
        		break;
        	}
        	if (par || !mostRight)
        		treeStr << "(";
			treeStr << "\\";
			print2str(tree->left, treeStr, false, false);
			treeStr << ".";
			print2str(tree->right, treeStr, true, par);
        	if (par || !mostRight)
				treeStr << ")";
            break;
        case AppT:
        	if (par)
				treeStr << "(";
			print2str(tree->left, treeStr, false, par);
			treeStr << " ";
			if (!par && tree->right->type == AppT) 
				treeStr << "(";
			print2str(tree->right, treeStr, mostRight, par);
			if (!par && tree->right->type == AppT) 
				treeStr << ")";
        	if (par)
				treeStr << ")";
            break;
        default:
            printf("Unknown term type.\n");
    }
}

void promptHandle(istream& is, bool pline) {
	double startTime = 0.0, endTime;
	string help = ":help";
	string store = ":store";
	string quit = ":quit";
	for(std::string line; getline(is, line); ) {
		if (pline)
			cout << line << endl;
		//removing leading spaces
		line.erase(line.begin(), std::find_if(line.begin(), line.end(),
			std::bind1st(std::not_equal_to<char>(), ' ')));
		//removing trailing spaces
		line.erase(std::find_if(line.rbegin(), line.rend(),
			std::bind1st(std::not_equal_to<char>(), ' ')).base(), line.end());
		//removing extra spaces
		//line = regex_replace(line, std::regex("[ ]+"), " ");
		if (!line.compare(0, help.size(), help)) {
			cout << "\nllama - A lambda calculus interpreter\n\
:help             Display this help message.\n\
:trace (on|off)   Set tracing On or Off.\n\
:par (on|off)     Set full parenthesizing On or Off.\n\
:store            Store a New Identifier after pressed Return Key.\n\
                  Write syntax: Ident = (lambda term).\n\
:quit             Quit the program (same as Ctrl-D)\n" << endl;
		}
		else if (!line.compare(0, store.size(), store)) {
			std::size_t pos1, pos2;
			std::string tmp1, tmp2;
			getline(is, line);
			//get the Identifiers name
			pos1 = line.find(' ');
			tmp1 = line.substr(0, pos1);
			pos1 = line.find('=');
			pos2 = line.find(';');
			tmp2 = line.substr(pos1+2, pos2-pos1-2);
			tmp2 += "\n";
		    yy_scan_string(tmp2.c_str());
		    if (yyparse() != 0) {
		    	cout << "Parse error on .store: "
		    		<<": \"" << line << "\"" << endl;
		    	continue;
		    }
			newIdentifier(tmp1, treeRoot);
		}
		else if (!line.compare(":trace on")) {
			trace = true;
		}
		else if (!line.compare(":trace off")) {
			trace = false;
		}
		else if (!line.compare(":par on")) {
			parenthes = true;
		}
		else if (!line.compare(":par off")) {
			parenthes = false;
		}
		else if (!line.compare(0, quit.size(), quit)) {
			break;
		}
		else if (!line.compare("")) {
		}
		else {
			line += "\n";
		    yy_scan_string(line.c_str());
		    if (yyparse() != 0) {
	    		cout << "llama. ";
		    	continue;
		    }

		    stringstream treeStream;
		    string treeStr;
		    string prevTreeStr;
		    bool reduced = false;
		    if (trace) {
			    //printTree(treeRoot);
			    print2str(treeRoot, treeStream, true, parenthes);
			    cout << treeStream.str() << endl;
		    }
		    do {
			    treeStream.str("");
			    reduced = false;
			    treeRoot = evaluate(treeRoot, &reduced);
		    	print2str(treeRoot, treeStream, true, parenthes);
		    	treeStr = treeStream.str();
		    	if (trace) {
				    getchar();
				    cout << "Reduced term: " << treeStr << endl;
		    	}
		    } while(reduced);

		    endTime = getCPUTime();
		    treeStream.str("");
		    print2str(treeRoot, treeStream, true, parenthes);
		    cout << treeStream.str() << endl;
		    cerr << "( ";
		    reductions.print();
		    fprintf( stderr, " | %.2lfs CPU )\n", (endTime - startTime) );
		    reductions.reset();
		    //deleteTree(treeRoot);
		}
	    cout << "llama. ";
	}
}

int main(int argc, char **argv)
{
	int args;
	int i = 0;
	std::ifstream inputFile;
    for (args = 1 ; args != argc ; ++args) {
	    if (!strcmp(argv[args], "-h")) {
	        fprintf(stderr, "\
Usage: %s [OPTION] [INPUT FILE] ...\n\n\
-h, type this help message\n\n\
[INPUT FILE] with lambda terms is optional\n\n", argv[0]);
	        return 0;
	    }
	    else {
	    	i++;
	    	// open a file handle to a particular file:
	    	inputFile.open(argv[args]);
			// make sure it is valid:
			if (!inputFile) {
				cout << argv[0] << ": can't open " << argv[args] << endl;
				return -1;
			}
	    }
	}
	if (readAliases("src/.llama_aliases") == -1)
		if (readAliases(".llama_aliases") == -1)
			if (readAliases("../.llama_aliases") == -1)
				cout << argv[0] << ": can't find .llama_aliases" << endl;
	cout << "llama 3.14\n\
Type \":help\" for more information.\n\n\
llama. ";

	// from Now on, Check for Identifiers existance
	checkIdent = true;

	// handle INPUT FILES first
	if (i) {
		promptHandle(inputFile, true);
	}
	//handle User Input.
	promptHandle(cin, false);

	putchar('\n');
	return 0;
}
