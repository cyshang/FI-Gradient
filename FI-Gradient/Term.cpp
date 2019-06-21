#include "Term.h"
#include "Token.h"

using namespace std;

void Term::getTerm(TokenStream & ts)
{
	coef = 1;	

	do {
		prim.push_back(Primary());
		prim.back().getPrim(ts);
	} while (ts.current().kind == Kind::mul);

}