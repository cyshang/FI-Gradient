#include "Substitute.h"
#include <iostream>
#include <sstream>

using namespace std;

extern int maxExp;
extern int dimR;

void substitute(std::istream & fin, std::ostream & fout)
{
	char   ch;
	string term;

	while (fin.get(ch)) {
		switch (ch)
		{
		case '+':
		case '\n':
			while (replaceRn(term));
			while (replaceRab(term));
			fout << term << ch;
			term.clear();
			break;
		case '=':
			fout << term << '=';
			term.clear();
			break;
		default:
			term += ch;
			break;
		}
	}

	if (!term.empty()) {
		while (replaceRn(term));
		while (replaceRab(term));
		fout << term << endl;
	}
}

bool replaceRn(string & str)
{
	if (str.empty()) return false;

	size_t pos;
	for (pos = 1; pos < str.size(); ++pos) {
		if (str[pos] == '*' && str[pos - 1] == '*') {
			pos--;
			break;
		}
	}

	if (pos >= str.size())	return false;

	size_t begin = pos;
	while (str[--begin] != 'r');

	size_t end = pos + 1;
	while (isdigit(str[++end]));

	size_t len = end - begin;

	string strRn = str.substr(begin, len);

	int id;
	int exp;
	istringstream sin(strRn);

	while (sin.get() != '(');
	sin >> id;
	while (sin.get() != '*');
	sin.get();
	sin >> exp;

	maxExp = (exp > maxExp) ? exp : maxExp;

	if (id > dimR) {
		cerr << "Error: id > dimR" << endl;
		exit(127);
	}

	ostringstream sout;
	sout << 'r' << exp << '(' << id << ')';

	str.replace(begin, len, sout.str());

	return true;
}

bool replaceRab(string & str)
{
	if (str.empty()) return false;

	size_t begin1;
	for (begin1 = 1; begin1 < str.size(); ++begin1) {
		if (str[begin1] == '(' && str[begin1 - 1] == 'r') {
			begin1--;
			break;
		}
	}

	if (begin1 >= str.size()) return false;

	size_t begin2;
	for (begin2 = begin1 + 2; begin2 < str.size(); ++begin2) {
		if (str[begin2] == '(' && str[begin2 - 1] == 'r') {
			begin2--;
			break;
		}
	}

	if (begin2 >= str.size()) return false;

	size_t end;
	for (end = begin2 + 2; end < str.size(); ++end) {
		if (str[end] == ')')
			break;
	}

	if (end == str.size()) {
		cerr << "Error in term: \"" << str << "\"!" << endl;
		exit(127);
	}

	size_t len = end - begin1 + 1;

	//	cout << str << ".substr(" << begin1 << ", " << len << ")" << endl;
	string strRab = str.substr(begin1, len);

	int aid, bid;
	istringstream sin(strRab);

	while (sin.get() != '(');
	sin >> aid;
	while (sin.get() != '(');
	sin >> bid;

	if (bid > dimR) {
		cerr << "Error: id > dimR" << endl;
		exit(127);
	}

	int id = (2 * dimR - aid) * (aid - 1) / 2 + (bid - aid);

	ostringstream sout;
	sout << "rab(" << id << ")";

	str.replace(begin1, len, sout.str());

	return true;
}
