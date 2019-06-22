#ifndef TERM_H_
#define TERM_H_

#include <iostream>
#include <vector>
#include "Primary.h"

class TokenStream;

class Term
{
public:
	Term() :coef(1) {}
	Term(int _coef) :coef(_coef) {}

	void getTerm(TokenStream & ts);
	Term derivate(int id);
	void print(std::ostream & os) const;	

	inline int getCoef() const { return coef; }
private:
	int coef;
	std::vector<Primary> prim;
};

#endif
