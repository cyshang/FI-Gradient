#include <algorithm>
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

Term Term::derivate(int id)
{
	int len(static_cast<int>(prim.size()));

	int pos;
	for (pos = 0; pos < len; ++pos) {
		if (prim[pos].getId() == id) break;
	}

	if (pos == len) return Term(0);

	Term terms;
	for (int i = 0; i < len; ++i) {		
		terms.prim.push_back(prim[i]);

		if (i == pos) {
			terms.coef *= terms.prim[i].getExp();
			terms.prim[i].derivate();
		}
	}

	sort(terms.prim.begin(), terms.prim.end());

	return terms;
}

void Term::print(ostream & os) const
{
	if (coef == 0) return;

	if (coef > 1)
		os << coef << '*';

	for (size_t i = 0; i < prim.size(); ++i) {
		prim[i].print(os);

		if (i != prim.size() - 1)
			os << '*';
	}
}
