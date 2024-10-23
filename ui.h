#pragma once
#include "repository.h"
#include "CoatValidator.h"
class UserInterface {
private:
	CoatValidator c_validator;

public:
	void MainMenu();
	void AdminMenu();
	void RepoTypes();
	std::string getOption(std::string option);
	std::string getFile(std::string file);
	std::string getSize(std::string size);
	std::string getColor(std::string color);
	float getPrice();
	int getQuantity();
	std::string getPhotograph(std::string photo);
	void printElems(Repository& arr);
	void secondMenu();
	void IterateCoat(vector<TrenchCoat>::iterator it);
	std::string getAppPath(std::string app);
	void printCart(Repository& arr);
};

