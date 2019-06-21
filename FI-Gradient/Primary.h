#ifndef PRIMARY_H_
#define PRIMARY_H_

class TokenStream;

class Primary
{
public:
	void getPrim(TokenStream & ts);

private:
	int rid;
	int rexp;
};

#endif
