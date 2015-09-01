%{
	#include <cstdlib>
	#include <iostream>
	#include <sstream>

	using namespace std;

	#define YY_DECL extern int yylex()

	#include "node.h"
	#include "parser.hpp"    // to get the token types that we return

	int linenum = 0;
	int charNum = 0;
	int tokens  = 1;

	string token;
%}

%option noyywrap

%%

[\n]           			{ ++linenum; token = ""; }  // increment line count, but don't return a token
[ \t]+					{ ++tokens; token += yytext; }
\\           			{ 	
							charNum++; token += yytext;
							return '\\';
						}
"lambda"           		{ 	
							charNum++; token += yytext;
							return '\\';
						}
\.						{
							charNum++; token += yytext;
							return '.';
						}
\(						{
							charNum++; token += yytext;
							return '(';
						}

\)						{	charNum++; token += yytext;
							return ')';
						}
"&&"					{
							yylval.opval = new std::string(yytext, yyleng);
							charNum += yyleng; token += yytext;
							return OPEQ;
						}
"||"					{
							yylval.opval = new std::string(yytext, yyleng);
							charNum += yyleng; token += yytext;
							return OPNE;
						}
"<"						{
							yylval.opval = new std::string(yytext, yyleng);
							charNum += yyleng; token += yytext;
							return OPLT;
						}
"<="					{
							yylval.opval = new std::string(yytext, yyleng);
							charNum += yyleng; token += yytext;
							return OPLE;
						}
">"						{
							yylval.opval = new std::string(yytext, yyleng);
							charNum += yyleng; token += yytext;
							return OPGT;
						}
">="					{
							yylval.opval = new std::string(yytext, yyleng);
							charNum += yyleng; token += yytext;
							return OPGE;
						}
"=="					{
							yylval.opval = new std::string(yytext, yyleng);
							charNum += yyleng; token += yytext;
							return OPEQ;
						}
"!="					{
							yylval.opval = new std::string(yytext, yyleng);
							charNum += yyleng; token += yytext;
							return OPNE;
						}
","						{
							yylval.opval = new std::string(yytext, yyleng);
							charNum += yyleng; token += yytext;
							return OPCOMMA;
						}
"+"						{
							yylval.opval = new std::string(yytext, yyleng);
							charNum += yyleng; token += yytext;
							return OPPLUS;
						}
"-"						{
							yylval.opval = new std::string(yytext, yyleng);
							charNum += yyleng; token += yytext;
							return OPMINUS;
						}
"*"						{
							yylval.opval = new std::string(yytext, yyleng);
							charNum += yyleng; token += yytext;
							return OPMUL;
						}
"/"						{
							yylval.opval = new std::string(yytext, yyleng);
							charNum += yyleng; token += yytext;
							return OPDIV;
						}
"^"						{
							yylval.opval = new std::string(yytext, yyleng);
							charNum += yyleng; token += yytext;
							return OPEXP;
						}
[a-z][a-zA-Z0-9_]*		{
							// we have to copy because we can't rely on yytext not changing underneath us:
							yylval.vval = new std::string(yytext, yyleng);
							charNum += yyleng;
							token += yytext;
							//cout <<  "Var token!" << endl;
							return VAR;
						}
[0-9]+					{
							yylval.nval = atoi(yytext);
							charNum += yyleng;
							token += yytext;
							//cout <<  "Num token!" << endl;
							return NUM;
						}
[A-Z][a-zA-Z0-9_]*		{
							// we have to copy because we can't rely on yytext not changing underneath us:
							yylval.idval = new std::string(yytext, yyleng);
							charNum += yyleng;
							token += yytext;
							//cout <<  "Identifier token!" << endl;
							return IDENT;
						}
.               		{
							stringstream errstr;
							errstr << "lcin: Syntax error: unkown character:\n\t\t" << token << yytext << endl;
							cout << errstr.str() ;
							printf("\t\t%*s^\n", token.size(), "");
							token = "";
							return ERROR;
						}
%%