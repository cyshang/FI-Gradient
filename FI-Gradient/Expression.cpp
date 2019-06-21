#include "Expression.h"
#include "Token.h"

void Expression::getExpr(TokenStream & tk)
{
	term.clear();
	if (tk.current().kind != Kind::fi) { throw "Error"; }

	pid = tk.getId();

	if (tk.current().kind != Kind::assign) { throw "Error"; }

	do {
		term.push_back(Term());
		term.back().getTerm(tk);
	} while (tk.current().kind == Kind::plus);

}