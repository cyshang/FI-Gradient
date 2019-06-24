#include <iostream>
#include <string>

using namespace std;

constexpr size_t maxlen = 130;

int main(void)
{
	char   ch;
	string term;
	size_t count(0);

	while (cin.get(ch)) {

		switch (ch) {
		case '\n':
			if (count + term.size() > maxlen) {
				cout << " &\n" << term << endl;
			}
			else {
				cout << term << endl;
			}
			term.clear();
			count = 0;
			break;
		case '+':
			if (count + term.size() > maxlen) {
				cout << " &\n" << term;
				count = term.size();
			}
			else {
				cout << term;
				count += term.size();
			}
			term = "+";
			break;
		case '=':
			cout << term;
			count = term.size();
			term = "=";
			break;
		default:
			term += ch;
		}
	}

	if (count + term.size() > maxlen) {
		cout << " &\n" << term;
	}
	else {
		cout << term;
	}

	return 0;
}
