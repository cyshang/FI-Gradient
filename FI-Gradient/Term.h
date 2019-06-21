#ifndef TERM_H_
#define TERM_H_

#include <iostream>
#include <vector>
#include "Primary.h"

class TokenStream;

class Term
{
public:
	Term() :coef(0) {}

	void getTerm(TokenStream & ts);
	Term derivate(int id);
	friend std::ostream & operator << (std::ostream os, const Term & term);

private:
	int coef;
	std::vector<Primary> prim;
};

#endif
