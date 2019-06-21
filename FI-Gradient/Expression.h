#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include <iostream>
#include <vector>
#include "Term.h"

class TokenStream;

class Expression
{
	void getExpr(TokenStream & tk);

private:
	int pid;
	std::vector<Term> term;
};

#endif
