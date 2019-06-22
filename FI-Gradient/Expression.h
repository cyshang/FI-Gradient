#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include <iostream>
#include <vector>
#include <string>
#include "Term.h"

class TokenStream;

class Derivative;

class Expression
{
public:
	Expression() :my_id(0) {}
	Expression(int _id) :my_id(_id) {}

	void getExpr(TokenStream & tk);
	Derivative derivate(int id);
	void print(std::ostream & os) const;

	static std::string name;
private:
	int my_id;
	std::vector<Term> term;
};

class Derivative
{
	friend Expression;

public:
	Derivative(int _pid, int _rid) :pid(_pid), rid(_rid) {}
	void print(std::ostream & os);

	static std::string name;
private:
	int pid;
	int rid;
	std::vector<Term> terms;
};

#endif
