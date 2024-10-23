#pragma once
#include <iostream>

class TrenchCoat {
public:
	TrenchCoat();
	TrenchCoat(std::string size, std::string color, float price, int quantity, std::string photography);

	// Getters for member variables
	std::string getSize() const;
	std::string getColor() const;
	float getPrice() const;
	int getQuantity() const;
	std::string getPhotography() const;
	friend std::istream& operator>>(std::istream& is, TrenchCoat & t);
	friend std::ostream& operator<<(std::ostream& os, const TrenchCoat& t);

	//Setters
	void setPrice(float p);
	void setQuantity(int q);
	void setPhoto(std::string ph);

	bool operator==(const TrenchCoat& other) const;

	TrenchCoat& operator=(const TrenchCoat& other);

private:
	std::string size;
	std::string color;
	float price;
	int quantity;
	std::string photography;
};