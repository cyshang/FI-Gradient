#include "Primary.h"
#include "Token.h"

using namespace std;

extern int dimR;

string Primary::name;

void Primary::getPrim(TokenStream & ts)
{
	ts.get();

	if (ts.current().kind != Kind::r) { throw "Error"; }

	my_id  = ts.getId();
	dimR   = (my_id > dimR) ? my_id : dimR;
	exp    = 1;

	if (ts.current().kind == Kind::exp) {
		ts.get();
		if (ts.current().kind != Kind::number) { throw "Error"; }
		exp = ts.current().number_value;
		ts.get();
	}
}

void Primary::print(ostream & os) const
{
	if (exp == 0) {
		os << "1.d0";
		return;
	}

	os << name << '(' << my_id << ')';
	if (exp > 1)
		os << "**" << exp;
}