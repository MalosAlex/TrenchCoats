#include "ui.h"
#include "Repository.h"
#include "Utils.h"
#include "CoatValidator.h"
#include <Windows.h>
#include <iostream>
#include <ios>
#include <limits>
#include <string>
#include <cstring>

using namespace std;

void UserInterface::MainMenu()
{
	cout << "Hello! Please chose the mode to access the app" << endl;
	cout << "1. Administrator mode" << endl;
	cout << "2. User mode" << endl;
	cout << "3. Exit" << endl;
}

void UserInterface::AdminMenu()
{
	cout << "Administration mode. Choose one of the folllowing:" << endl;
	cout << "1. Add a new Trench Coat" << endl;
	cout << "2. Delete a Trench Coat" << endl;
	cout << "3. Change a Trench Coat's price" << endl;
	cout << "4. Change the quantity for a Trench Coat" << endl;
	cout << "5. Change the photograph for a Trench Coat" << endl;
	cout << "6. Show all Trench Coats" << endl;
	cout << "7. Exit" << endl;
}

void UserInterface::RepoTypes()
{
	cout << "Press 1 for the text file based repo or 2 for the data-base based repo" << endl;
}

std::string UserInterface::getOption(std::string option)
{
	// Clear the input buffer before reading the string
	cout << ">";
	cin >> option;
	cin.ignore();
	if (option.size() != 1 && !isdigit(option[0]))
	{
		cout << "\nInvalid Option\n";
		
		return "";
	}
	return option;
}

std::string UserInterface::getFile(std::string file)
{
	while (1)
	{
	cout << "Please input a CSV or HTML file" << endl;
	cout << ">";
	cin >> file;
	cin.ignore();
	int v = c_validator.validate_file(file);
	if(v == 0)
	{
		return file;
	}
	if (v == 1)
	{
		cout << "File has to be like name.type" << endl;
	}
	if (v == 2)
	{
		cout << "FIle has to be of type csv or html" << endl;
	}
	}
}

std::string UserInterface::getSize(std::string size)
{
	while (1)
	{
		cout << "Please input the size of the Trench Coat" << endl;
		cout << ">";
		cin >> size;
		cin.ignore();
		int v = c_validator.validate_size(size);
		if (v == 1)
		{
			cout << "Size has to be XS/S/M/L/XL/XXL. Try again." << endl;
		}
		return size;
	}
}

float UserInterface::getPrice()
{
	string price_s;
	while (1)
	{
		int ok = 1;
		cout << "Please input the price of the Trench Coat" << endl;
		cout << ">";
		cin >> price_s;
		cin.ignore();
		int v = c_validator.validate_price(price_s);
		if (v == 1) {
			cout << "Input cannot be empty. Please try again." << endl;
			continue;
		}
		if (v == 2)
		{
			cout << "Input must be a number. Please try again" << endl;
			continue;
		}
		if (v == 0)
		{
			float price = stof(price_s);
			return price;
		}

	}
}

std::string UserInterface::getColor(std::string color)
{
	while (1)
	{
		cout << "Please input a color for the Trench Coat" << endl;
		cout << ">";
		cin >> color;
		cin.ignore();
		int v = c_validator.validate_color(color);
		if (v == 1) {
			cout << "Input cannot be empty. Please try again." << endl;
			continue;
		}
		if (v == 2) {
			cout << "Color cannot contain numbers. Please try again." << endl;
		}
		
		return color;
	}
}

int UserInterface::getQuantity()
{
	string quant_o;

	int ok;
	while (1)
	{
		ok = 1;
		cout << "Please input the quantity for the Trench Coat" << endl;
		cout << ">";
		cin >> quant_o;
		cin.ignore();
		int v = c_validator.validate_quantity(quant_o);
		if (v == 1)
		{
			cout << "Input cannot be empty. Please try again." << endl;
			continue;
		}
		if (v == 2)
		{
			cout << "Quantity must be an integer. Please try again." << endl;
			continue;
		}
		if(v==0)
			return stoi(quant_o);
	}
}

std::string UserInterface::getPhotograph(std::string photo)
{
	while (1)
	{
		cout << "Please enter the link for the photo" << endl;
		cout << ">";
		cin >> photo;
		cin.ignore(); 
		int v = c_validator.validate_link(photo);
		if (v==1) {
			cout << "Input cannot be empty. Please try again." << endl;
			continue;
		}
		if (v == 2)
		{
			cout << "The link must start with \"http\" or \"www\". Please try again" << endl;
			continue;
		}
		if(v==0)
			return photo;
	}
}

void UserInterface::printElems(Repository& arr)
{
	vector<TrenchCoat> v = arr.getCoats();
	for (int i = 0; i < v.size(); i++)
	{
		TrenchCoat el = v[i];
		cout << "Trench coat of size " << el.getSize() << ", color " <<
			el.getColor() << ", priced at " << el.getPrice() << ", in a total of " << el.getQuantity() << " coats, with picture: " << el.getPhotography() << endl;
	}
	if (v.size() == 0)
	{
		cout << "There are no Trench Coats in the data base" << endl;
	}
}

void UserInterface::secondMenu()
{
	cout << "Hello choose one of the following" << endl;
	cout << "1. Shop coats of a a certain size" << endl;
	cout << "2. See the shopping cart" << endl;
	cout << "3. Open the shopping cart in another app" << endl;
	cout << "4. Exit" << endl;
}

void UserInterface::IterateCoat(vector<TrenchCoat>::iterator it)
{
	cout << "Current coat: " << endl;
	TrenchCoat coat = *it;
	cout << "Size: " << coat.getSize() << ", Color : " << coat.getColor() << ", Price : $" << coat.getPrice() <<
		", Quantity : " << coat.getQuantity() << ", Photography : " << coat.getPhotography() << endl;
	ShellExecuteA(NULL, "open", coat.getPhotography().c_str(), NULL, NULL, SW_SHOWNORMAL);
	cout << "Press 1 to add one coat to the cart" << endl;
	cout << "Press 2 to go to the next coat" << endl;
	cout << "Press 3 to exit" << endl;
}

string UserInterface::getAppPath(string app)
{
	while (1)
	{
		cout << "Please write the path for the file with which you want to open the csv file" << endl;
		cout << ">";
		getline(cin, app);
		int v = c_validator.validate_path(app);
		if (v==1) {
			cout << "Input cannot be empty. Please try again." << endl;
			continue;
		}
		if(v==0)
			return app;
	}
}

void UserInterface::printCart(Repository& arr)
{
	vector<TrenchCoat> elems = arr.getCoats();
	float total = 0;
	for (int i = 0; i < elems.size(); i++)
	{
		TrenchCoat el = elems[i];
		cout << "Trench coat of size " << el.getSize() << ", color " <<
			el.getColor() << ", in a total of " << el.getQuantity() << ", priced at " << el.getPrice() << " coats, with picture: " << el.getPhotography() << endl;
		total += el.getPrice();
	}
	if (elems.size() == 0)
	{
		cout << "There are no Trench Coats in the shopping cart" << endl;
	}
	else {
		cout << "Total price:" << total << endl;
	}
}






