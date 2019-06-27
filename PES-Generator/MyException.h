#ifndef EDITOREXCEPTION_H_
#define EDITOREXCEPTION_H_

#include <iostream>
#include <fstream>
#include <string>

void openCheck(std::ifstream & fin, const std::string & file);

class MyException : public std::exception {
public:
	explicit MyException(const std::string& message) : message(message) {
	}
	~MyException() throw() {
	}
	const char* what() const throw() {
		return message.c_str();
	}
private:
	std::string message;
};

#endif
