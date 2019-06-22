#include "Token.h"
#include "Expression.h"
#include "Primary.h"

using namespace std;

const string fi_name(Expression::name);
const string r_name(Primary::name);

int TokenStream::getId()
{
	get();
	if (ct.kind != Kind::lp) throw "Error";

	get();
	if (ct.kind != Kind::number) throw "Error";
	int id = ct.number_value;

	get();
	if (ct.kind != Kind::rp) throw "Error";

	get();
	return id;
}

Token TokenStream::get()
{
	char ch;

	do {
		if (!ip->get(ch)) return ct = { Kind::end };
	} while (isspace(ch) || ch == '\n' || ch == '&');

	switch (ch) {
	case '*':
		if (ip->peek() == '*') {
			ip->get(ch);
			ct.kind = Kind::exp;
			break;
		}
	case '+':
	case '(':
	case ')':
	case '=':
		ct = { static_cast<Kind>(ch) }; break;	
	case '0':case'1':case'2':case '3':case'4':case'5':case '6':case'7':case'8':case'9':
		ip->putback(ch);
		*ip >> ct.number_value;
		ct.kind = Kind::number;
		break;
	default:
		if (isalpha(ch)) {
			string string_value;
			string_value = ch;
			while (ip->get(ch) && isalpha(ch))
				string_value += ch;

			if (ch == '(' && (string_value == fi_name || string_value == r_name)) {
				ip->putback(ch);

				ct.string_value = string_value;
				ct.kind = (string_value == fi_name) ? Kind::fi : Kind::r;

				break;
			}
		
			throw "Error";
		}		
	}

	return ct;
}