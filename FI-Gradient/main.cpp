#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Expression.h"
#include "Token.h"

using namespace std;

int rTotal(0);
int pTotal(0);

void printDeclare(ofstream & fout);

int main(int argc, char **argv)
{

	if (argc < 2) {
		cerr << "Error: missing <file> oprand!" << endl;
		return 127;
	}

	string      argStr = argv[1];
	ifstream    fin(argStr.c_str(), ifstream::in);
	TokenStream tk(fin);
	
	ofstream fout;
	fout.open("grad.out", ofstream::out);
	
	vector<Expression> exprs;

	tk.get();	
	while (tk.current().kind != Kind::end) {
		exprs.push_back(Expression());
		exprs.back().getExpr(tk);
	}

	for (size_t i = 0; i < exprs.size(); ++i) {
		for (int rid = 1; rid <= rTotal; ++rid) {
			exprs[i].derivate(rid).print(fout);
		}
	}

	fout.close();
	fin.close();

	fout.open("declare.out", ofstream::out);
	printDeclare(fout);
	fout.close();

	return 0;
}

void printDeclare(ofstream & fout)
{
	fout << "real*8, intent(in)  :: r(" << rTotal << ')' << endl;
	fout << "real*8, intent(out) :: dpdr(" << pTotal << ',' << rTotal << ')' << endl;
}
