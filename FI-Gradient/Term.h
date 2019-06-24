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

	friend inline bool operator<(const Term & a, const Term & b) {
		return (a.prim.size() && b.prim.size()) ? (a.prim[0] < b.prim[0]) : (a.prim.size() > b.prim.size());
	}
private:
	int coef;
	std::vector<Primary> prim;
};

#endif
