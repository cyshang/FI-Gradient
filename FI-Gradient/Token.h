#ifndef TOKEN_H_
#define TOKEN_H_

#include <iostream>
#include <string>

enum class Kind :char
{
	fi, r, exp, number, end, plus = '+', mul = '*', assign = '=', lp = '(', rp = ')'
};

struct Token
{
	Kind   kind;
	std::string string_value;
	int number_value;
};

class TokenStream {
public:
	TokenStream(std::istream &s) :ip(&s), owns(false) {}
	TokenStream(std::istream *p) :ip(p), owns(true) {}
	~TokenStream() { close(); }

	int getId();
	Token get();
	inline const Token& current() { return ct; }
	inline void setInput(std::istream &s) { close(); ip = &s; owns = false; }

private:
	inline void close() { if (owns) delete ip; }

	std::istream *ip;                 // pointer to an input stream
	bool     owns;			     // dose the TokenStream own the std::istream?
	Token    ct{ Kind::end };    // current token
};

#endif
