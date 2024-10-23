#pragma once
#include <exception>
#include <string>
#include "TrenchCoat.h"

class CoatException : public std::exception
{
protected:
	std::string message;
public:
	CoatException(const std::string m);
	virtual const char* what();
};

class CoatValidator {
public:
	bool validate(const TrenchCoat& tc);
	int validate_color(const std::string color);
	int validate_size(const std::string size);
	int validate_price(const std::string price);
	int validate_quantity(const std::string q);
	int validate_link(const std::string link);
	int validate_file(const std::string file);
	int validate_path(const std::string path);
};

