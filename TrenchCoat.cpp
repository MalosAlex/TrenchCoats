#include "TrenchCoat.h"
#include "Utils.h"
#include <cstring>
#include <iostream>
#include <vector>
#include <winsqlite/winsqlite3.h>
#include <string>

using namespace std;

// Constructor 
TrenchCoat::TrenchCoat()
{
	size = "";
	color = "";
	price = 0.0;
	quantity = 0;
	photography = "";
}

TrenchCoat::TrenchCoat(std::string size, std::string color, float price, int quantity, std::string photography)
{
	this->size = size;
	this->color = color;
	this->price = price;
	this->quantity = quantity;
	this->photography = photography;
}


// Getter functions definitions
std::string TrenchCoat::getSize() const {
	return size;
}

std::string TrenchCoat::getColor() const {
	return color;
}

float TrenchCoat::getPrice() const {
	return price;
}

int TrenchCoat::getQuantity() const {
	return quantity;
}

std::string TrenchCoat::getPhotography() const {
	return photography;
}

void TrenchCoat::setPrice(float p)
{
	this->price = p;
}

void TrenchCoat::setQuantity(int q)
{
	this->quantity = q;
}

void TrenchCoat::setPhoto(string ph)
{
	this->photography = ph;
}

bool TrenchCoat::operator==(const TrenchCoat& other) const
{
	return (this->color == other.color && this->size == other.size);
}


TrenchCoat& TrenchCoat::operator=(const TrenchCoat& other)
{
	if (this != &other) { // Check for self-assignment
		this->size = other.size;
		this->color = other.color;
		this->price = other.price;
		this->quantity = other.quantity;
		this->photography = other.photography;
	}
	return *this;
}


istream& operator>>(istream& is, TrenchCoat & t)
{
	string line;
	getline(is, line);

	vector<string> tokens = tokenize(line, ',');
	if (tokens.size() != 5)
		return is;

	t.size = tokens[0];
	t.color = tokens[1];
	t.price = stof(tokens[2]);
	t.quantity = stoi(tokens[3]);
	t.photography = tokens[4];
	return is;
}

std::ostream& operator<<(std::ostream& os, const TrenchCoat& t)
{
	os << t.size << "," << t.color << "," << t.price << "," << t.quantity << "," << t.photography << "\n";
	return os;
}
