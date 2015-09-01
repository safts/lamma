#include "utils.h"
#include "node.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <map>
#include <string>
#include "node.h"

using namespace std;

extern int yyparse();
extern int yy_scan_string(const char *str);
extern FILE *yyin;
extern TreeNode * treeRoot;

IdentifierList identList;

/* insert a new Identifier into the IdentifierList */
void newIdentifier(std::string ident, TreeNode* node) {
	identList[ident] = node;
}

/* find the value of an Identifier from IdentifierList */
TreeNode* findIdentifier(std::string ident) {
	return duplicateTree(identList[ident]);
}

bool checkIdentifier(std::string *ident, bool check) {
	if (check) {
		if (identList[*ident] != NULL) {
			return true;
		}
		return false;
	}
	return true;
}

int readAliases(std::string fileName) {
	std::ifstream inputFile;
	inputFile.open(fileName.c_str());
	std::size_t pos1, pos2;
	std::string tmp1, tmp2;
	TreeNode* tempNode=treeRoot;
	if (!inputFile) {
		//cout << "I can't open " << fileName << endl;
		return -1;
	}
	int i=0;
	for(std::string line; getline(inputFile, line); i++) {
		if (line[0] == '#' || !line.compare("")) {
			continue;
		}
		else if (line[0] == '?') {
			continue;
		}
		else if (line[0] == '\'') {
			//get the Identifiers name
			pos1 = line.find(' ');
			tmp1 = line.substr(1, pos1-2);
			pos1 = line.find('\'', 1);
			pos2 = line.find(';');
			pos1 += 4;
			tmp2 = line.substr(pos1, pos2-pos1);
			// newIdentifier(tmp1, tmp2);
			//cout << tmp1 << "=" << findIdentifier(tmp1) << endl;
		}
		else {
			//get the Identifiers name
			pos1 = line.find(' ');
			tmp1 = line.substr(0, pos1);
			pos1 = line.find('=');
			pos2 = line.find(';');
			tmp2 = line.substr(pos1+2, pos2-pos1-2);
			

			//cout << tmp1 << "=" << findIdentifier(tmp1) << endl;
		}
		tmp2 += "\n";
	    yy_scan_string(tmp2.c_str());
	    if (yyparse() != 0) {
	    	cout << "Parse error on line "<< i
	    		<<": \"" << line << "\"" << endl;
	    	continue;
	    }

		newIdentifier(tmp1, treeRoot);
	}
	treeRoot = tempNode;
}