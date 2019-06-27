#include "NewLiner.h"
#include <string>

using namespace std;

int maxlen(95);

void newLiner(istream & fin, ostream & fout)
{
	char   ch;
	string term;
	size_t count(0);

	while (fin.get(ch)) {

		switch (ch) {
		case '\n':
			if (count + term.size() > maxlen) {
				fout << " &\n" << term << endl;
			}
			else {
				fout << term << endl;
			}
			term.clear();
			count = 0;
			break;
		case '+':
			if (count + term.size() > maxlen) {
				fout << " &\n" << term;
				count = term.size();
			}
			else {
				fout << term;
				count += term.size();
			}
			term = "+";
			break;
		case '=':
			fout << term;
			count = term.size();
			term = "=";
			break;
		default:
			term += ch;
		}
	}

	if (count + term.size() > maxlen) {
		fout << " &\n" << term;
	}
	else {
		fout << term;
	}

}
