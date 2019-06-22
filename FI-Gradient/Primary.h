#ifndef PRIMARY_H_
#define PRIMARY_H_

#include <iostream>
#include <string>

class TokenStream;

class Primary
{
public:
	void getPrim(TokenStream & ts);
	void print(std::ostream & os) const;
	inline void derivate() { --exp; }

	static std::string name;
	inline int & getId() { return my_id; }
	inline int & getExp() { return exp; }
private:
	int my_id;
	int exp;
};

#endif
