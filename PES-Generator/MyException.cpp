#include "MyException.h"

using namespace std;

void openCheck(ifstream & fin, const string & file)
{
	if (!fin.is_open()) {
		throw MyException(string("Error: open file ") + file + " failed!");
	}
}