#ifndef __UTILS__
#define __UTILS__

#include <map>
#include <string>
#include "node.h"

typedef std::map<std::string, TreeNode*> IdentifierList;

/* insert a new Identifier into the IdentifierList */
void newIdentifier(std::string Ident, TreeNode* value);

/* find the value of an Identifier from IdentifierList */
TreeNode* findIdentifier(std::string Ident);

bool checkIdentifier(std::string *ident, bool check);

int readAliases(std::string fileName);

#endif