#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <cstdlib>

using namespace std;

int rTotal(0);
int maxExp(0);

void getRTotal(ifstream & fin);

bool replaceRn(string & str);
bool replaceRab(string & str);
void printSubst(ostream & out);
void printDeclare(ostream & out);

int main(int argc, char **argv)
{

	if (argc < 2) {
		cerr << "Error: missing oprand!" << endl;
		return 127;
	}

	string   argStr = argv[1];
	ifstream fin;
	ofstream fout;

	fin.open(argStr.c_str(), ifstream::in);

	getRTotal(fin);

	fout.open("deriv.out", ofstream::out);

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

	fin.close();
	fout.close();

	fout.open("subst.out", ofstream::out);
	printSubst(fout);
	fout.close();

	fout.open("declare.out", ofstream::app);
	printDeclare(fout);
	fout.close();

	return 0;
}

void getRTotal(ifstream & fin)
{
	int  id(0);
	bool inTerm(false);

	char ch;
	while (fin.get(ch)) {
		switch (ch)
		{
		case '=':
			inTerm = true;
			break;
		case 'r':
			if (inTerm) {
				while (fin.get() != '(');
				fin >> id;
				rTotal = (id > rTotal) ? id : rTotal;
			}
		case '\n':
			inTerm = false;
			break;
		}
	}

	fin.clear(ios::eofbit);
	fin.seekg(ios::beg);
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

	if (pos == str.size())	return false;

	size_t begin = pos;
	while (str[--begin] != 'r');

	size_t end = pos + 1;
	while (isdigit(str[++end]));

	size_t len = end - begin;

	//	cout << str << ".substr(" << begin << ", " << len << ")" << endl;
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

	if (id > rTotal) {
		cerr << "Error: id > rTotal" << endl;
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

	if (begin1 == str.size()) return false;

	size_t begin2;
	for (begin2 = begin1 + 2; begin2 < str.size(); ++begin2) {
		if (str[begin2] == '(' && str[begin2 - 1] == 'r') {
			begin2--;
			break;
		}
	}

	if (begin2 == str.size()) return false;

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

	if (bid > rTotal) {
		cerr << "Error: id > rTotal" << endl;
		exit(127);
	}

	int id = (2 * rTotal - aid) * (aid - 1) / 2 + (bid - aid);

	ostringstream sout;
	sout << "rab(" << id << ")";

	str.replace(begin1, len, sout.str());

	return true;
}

void printDeclare(ostream & out)
{
	for (int iExp = 2; iExp <= maxExp; ++iExp) {
		out << "real*8, intent(in)  :: r" << iExp << '(' << rTotal << ')' << endl;
	}

	int idMax = rTotal * (rTotal - 1) / 2;
	out << "real*8, intent(in)  :: rab(" << idMax << ')' << endl;
}

void printSubst(ostream & out)
{
	for (int iExp = 2; iExp <= maxExp; ++iExp) {
		if (iExp == 2) {
			for (int i = 1; i <= rTotal; ++i) {
				out << 'r' << iExp << '(' << i << ")=r(" << i << ")**" << iExp << endl;
			}
		}
		else {
			for (int i = 1; i <= rTotal; ++i) {
				out << 'r' << iExp << '(' << i << ")=r" << iExp - 1 << '(' << i << ")*r(" << i << ')' << endl;
			}
		}
	}

	int id = 0;
	for (int i = 1; i <= rTotal - 1; ++i) {
		for (int j = i + 1; j <= rTotal; ++j) {
			id++;
			out << "rab(" << id << ")=r(" << i << ")*r(" << j << ")" << endl;
		}
	}
}
