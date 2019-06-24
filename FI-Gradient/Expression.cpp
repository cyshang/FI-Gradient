#include <algorithm>
#include "Expression.h"
#include "Token.h"

using namespace std;

extern int pTotal;

string Expression::name;
string Derivative::name;

void Expression::getExpr(TokenStream & tk)
{
	string errStr = string("Error in") + __func__ + ": ";

	term.clear();
	if (tk.current().kind != Kind::fi) { throw "Error"; }

	my_id = tk.getId();

	pTotal = (my_id > pTotal) ? my_id : pTotal;

	if (tk.current().kind != Kind::assign) { throw "Error"; }

	do {
		term.push_back(Term());
		term.back().getTerm(tk);
	} while (tk.current().kind == Kind::plus);

}

Derivative Expression::derivate(int id)
{
	Derivative deriv(my_id, id);

	Term tmp;
	for (size_t i = 0; i < term.size(); ++i) {
		tmp = term[i].derivate(id);
		if (tmp.getCoef())
			deriv.terms.push_back(tmp);
	}

	sort(deriv.terms.begin(), deriv.terms.end());

	return deriv;
}

void Expression::print(ostream & os) const
{
	os << name << '(' << my_id << ")=";

	for (size_t i = 0; i < term.size(); ++i) {
		term[i].print(os);

		if (i != term.size() - 1)
			os << '+';
	}

	os << endl;
}

void Derivative::print(ostream & os)
{
	if (terms.size() == 0) return;

	os << name << '(' << pid << ',' << rid << ")=";
	for (size_t i = 0; i < terms.size(); ++i) {
		terms[i].print(os);
		if (i != terms.size() - 1)
			os << '+';
	}

	os << endl;
}