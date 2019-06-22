#include <cstdlib>
#include <iostream>
#include <fstream>
#include "Expression.h"
#include "Token.h"

using namespace std;

int main(int argc, char **argv)
{
	int r_num(0);

	cout << "Please input r nums: ";
	cin >> r_num;

	if (argc < 2) {
		cerr << "Error: missing input file!" << endl;
		exit(127);
	}
	string   file_name(argv[1]);
	ifstream fin;

	fin.open(file_name.c_str(), ifstream::in);

	TokenStream tk(fin);

	tk.get();
	Expression expr;
		
	while (tk.current().kind != Kind::end) {
		expr.getExpr(tk);
		for (int rid = 1; rid <= r_num; ++rid) {
			expr.derivate(rid).print(cout);
		}
	}

	fin.close();

	return 0;
}

