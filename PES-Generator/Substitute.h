#ifndef SUBSTITUTE_H_
#define SUBSTITUTE_H_

#include <iostream>
#include <string>

void substitute(std::istream & fin, std::ostream & fout);
bool replaceRn(std::string & str);
bool replaceRab(std::string & str);

#endif
