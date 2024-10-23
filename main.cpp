#include <iostream>
#include <cstring>
#include "ui.h"
#include "TrenchCoat.h"
#include "Repository.h"
#include "Utils.h"
#include "RepositoryExceptions.h"
 
//#include "tests.h"
//#include "functions.h"

using namespace std;

int main()
{
	UserInterface ui;
	string option;
	Repository* arr;
	ui.RepoTypes();
	option = ui.getOption(option);
	while (option != "1" && option != "2")
	{
		ui.RepoTypes();
		option = ui.getOption(option);
	}
	if (option == "1")
	{
		arr = new FileRepo("coats.txt", true);
	}
	else
	{
		arr = new DataBaseRepo("database.db", true);
	}
	//ShoppingCartRepo shopCart = ShoppingCartRepo(5);
	//genTen(arr);
	string size_o, color_o, photography_o;
	float price_o;
	int quantity_o;

	while (1)
	{
		ui.MainMenu();
		option = ui.getOption(option);
		if (option == "1")
		{
			while (1)
			{
				ui.AdminMenu();
				option = ui.getOption(option);
				while (option == "")
					option = ui.getOption(option);
				if (option == "1")
				{
					size_o = ui.getSize(size_o);
					color_o = ui.getColor(color_o);
					price_o = ui.getPrice();
					quantity_o = ui.getQuantity();
					photography_o = ui.getPhotograph(photography_o);
					TrenchCoat tc = TrenchCoat(size_o, color_o, price_o, quantity_o, photography_o);
					try {
						arr->AddCoat(tc);
					}
					catch (StatementException& e)
					{
						cout << e.what() << endl;
					}
					catch (FileException& f)
					{
						cout << f.what() << endl;
					}
				}
				else if (option == "2")
				{
					size_o = ui.getSize(size_o);
					color_o = ui.getColor(color_o);
					TrenchCoat tc = TrenchCoat(size_o, color_o, -1, -1, "");
					try {
						arr->deleteCoat(tc);
					}
					catch (FileException& f)
					{
						cout << f.what();
					}
				}
				else if (option == "3")
				{
					size_o = ui.getSize(size_o);
					color_o = ui.getColor(color_o);
					price_o = ui.getPrice();
					TrenchCoat tc = TrenchCoat(size_o, color_o, price_o, -1, "");
					try {
						arr->updateCoat(tc);
					}
					catch (FileException& f)
					{
						cout << f.what();
					}
				}
				else if (option == "4")
				{
					size_o = ui.getSize(size_o);
					color_o = ui.getColor(color_o);
					quantity_o = ui.getQuantity();
					TrenchCoat tc = TrenchCoat(size_o, color_o, -1, quantity_o, "");
					try {
						arr->updateCoat(tc);
					}
					catch (FileException& f)
					{
						cout << f.what();
					}
				}
				else if (option == "5")
				{
					size_o = ui.getSize(size_o);
					color_o = ui.getColor(color_o);
					photography_o = ui.getPhotograph(photography_o);
					TrenchCoat tc = TrenchCoat(size_o, color_o, -1, -1, photography_o);
					try {
						arr->updateCoat(tc);
					}
					catch (FileException& f)
					{
						cout << f.what();
					}
				}
				else if (option == "6")
				{
					ui.printElems(*arr);
				}
				else if (option == "7")
				{
					cout << "Buh-Bye!" << endl;
					break;
				}
				else
				{
					cout << "Option doesn't exist" << endl;
				}

			}
		}
		else if (option == "2")
		{
			// I declare the iterator and shopping cart
			vector<TrenchCoat>& v = arr->getCoats(); // Get the copy of the DynamicVector instance
			string filename;
			filename = ui.getFile(filename);
			ShoppingCartRepo* ShopCart;
			vector<string> tokens = tokenize(filename, '.');
			if (tokens[1] == "csv")
				ShopCart = new ShoppingCartCSV(filename, false);
			else
			{
				ShopCart = new ShoppingCartHTML(filename, false);
			}
			vector<TrenchCoat>::iterator it = v.begin(); // Obtain iterator from the copied vector
			while (1)
			{
				ui.secondMenu();
				option = ui.getOption(option);
				if (option == "1")
				{
					size_o = ui.getSize(size_o);
					int ok = 1;
					int l = 0;
					while ((*it).getSize() != size_o)
					{
						(++it);
						if (it == v.end())
							it = v.begin();
						l++;
						if (l == v.size())
						{
							cout << "No Trench Coats of this size \n";
							ok = 0;
							break;
						}
					}
					if (ok == 0)
						continue;
					while (1) {
						ui.IterateCoat(it);
						option = ui.getOption(option);
						if (option == "1")
						{
							ShopCart->AddCoatShopping(*it);
							ShopCart->update();
							(*it).setQuantity((*it).getQuantity() - 1);
							arr->update();
							if ((*it).getQuantity() == 0)
							{
								size_o = (*it).getSize();
								color_o = (*it).getColor();
								TrenchCoat tc = TrenchCoat(size_o, color_o, 0, 0, "");
								arr->deleteCoat(tc);
								cout << "This Trench Coat has ran out of stock! \n";
								v = arr->getCoats(); // Get the copy of the DynamicVector instance
								it = v.begin(); // Obtain iterator from the copied vector
								l = 1;
								while ((*it).getSize() != size_o)
								{
									(++it);
									if (it == v.end())
										it = v.begin();
									l++;
									if (l == v.size())
									{
										cout << "No Trench Coats of this size \n";
										break;
									}
								}
							}
						}
						else if (option == "2")
						{
							int l = 1;
							(++it);
							if (it == v.end())
								it = v.begin();
							while ((*it).getSize() != size_o)
							{
								(++it);
								if (it == v.end())
									it = v.begin();


								l++;
								if (l == v.size())
								{
									cout << "No other Trench Coats of this size \n";
									break;
								}
							}
						}
						else if (option == "3")
						{
							break;
						}
						else
						{
							cout << "Invalid option!" << endl;
						}
					}
				}
				else if (option == "2")
				{
					ui.printCart(*ShopCart);
				}
				else if (option == "3")
				{
					ShopCart->openApp();
				}
				else if (option == "4")
					break;
				else
				{
					cout << "Invalid option\n";
				}
			}
		}
		else if (option == "3")
		{
			break;
		}
		else
			cout << "Invalid option";
	}

	//TestAll();
	return 0;
	_CrtDumpMemoryLeaks();
}