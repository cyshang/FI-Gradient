#include "Primary.h"
#include "Token.h"

using namespace std;

void Primary::getPrim(TokenStream & ts)
{
	ts.get();

	if (ts.current().kind != Kind::r) { throw "Error"; }

	rid  = ts.getId();
	rexp = 1;

	ts.get();
	if (ts.current().kind == Kind::exp) {
		ts.get();
		if (ts.current().kind != Kind::number) { throw "Error"; }
		rexp = ts.current().number_value;
		ts.get();
	}
}