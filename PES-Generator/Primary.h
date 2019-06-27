#ifndef PRIMARY_H_
#define PRIMARY_H_

#include <iostream>
#include <string>

class TokenStream;

class Primary
{
public:
	Primary() :my_id(0), exp(1) {}

	void getPrim(TokenStream & ts);
	void print(std::ostream & os) const;
	inline void derivate() { --exp; }

	static std::string name;
	inline int & getId() { return my_id; }
	inline int & getExp() { return exp; }

	friend inline bool operator<(const Primary & a, const Primary & b) {
		return (a.exp != b.exp) ? (a.exp > b.exp) : (a.my_id < b.my_id);
	}
private:

	int my_id;
	int exp;
};

#endif
