#include "CoatValidator.h"
#include <string.h>
#include <vector>
#include "Utils.h"

using namespace std;

CoatException::CoatException(const std::string m) : message(m)
{
}

const char* CoatException::what()
{
	return this->message.c_str();
}

bool CoatValidator::validate(const TrenchCoat& tc)
{
	int v1 = validate_size(tc.getSize());
	int v2 = validate_color(tc.getColor());
	int v3 = validate_price(std::to_string(tc.getPrice()));
	int v4 = validate_quantity(std::to_string(tc.getQuantity()));
	int v5 = validate_link(tc.getPhotography());
	return (v1 == 0 && v2 == 0 && v3 == 0 && v4 == 0 && v5 == 0);
}

int CoatValidator::validate_color(const std::string color)
{
	if (color.size() == 0)
		return 1;
	for (int i = 0; i < color.size(); i++)
	{
		if (isdigit(color[i]))
			return 2;
	}
	return 0;
}

int CoatValidator::validate_size(const std::string size)
{
	if (!(size == "XS" or size == "S" or size == "M" or size == "L" or size == "XL" or size == "XXL"))
		return 1;
	return 0;
}

int CoatValidator::validate_price(const std::string price)
{
	if (price.length() == 0)
		return 1;
	for (int i = 0; i < price.size(); i++)
	{
		if (isdigit(price[i]) == 0 and price[i]!='.')
			return 2;
	}
	return 0;
}

int CoatValidator::validate_quantity(const std::string q)
{
	if (q.length() == 0)
		return 1;
	for (int i = 0; i < q.size(); i++)
	{
		if (isdigit(q[i]) == 0)
			return 2;
	}
	return 0;
}

int CoatValidator::validate_link(const std::string link)
{
	if (link.length() == 0)
		return 1;
	size_t posWww = link.find("www");
	size_t posHttp = link.find("http");
	if (posWww != 0 && posHttp != 0)
		return 2;
	return 0;
}

int CoatValidator::validate_file(const std::string file)
{
	vector<string> tokens = tokenize(file, '.');
	if (tokens.size() != 2)
		return 1;
	if (tokens[1] != "csv" and tokens[1] != "html")
		return 2;
	return 0;
}

int CoatValidator::validate_path(const std::string path)
{
	if (path.length() == 0)
		return 1;
	return 0;
}
