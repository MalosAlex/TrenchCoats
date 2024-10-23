#include "repository.h"
#include "RepositoryExceptions.h"
#include "TrenchCoat.h"
#include "ui.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <Windows.h>
#include <ios>
//#include <winsqlite/winsqlite3.h>
#include "sqlite3.h"


using namespace std;

Repository::Repository(const string& filename, bool read)
{
	this->filename = filename;
	if(read == true)
		this->ReadFromFile();
}

void Repository::AddCoat(const TrenchCoat& t)
{
	auto count = count_if(this->coats.begin(), this->coats.end(), [&](TrenchCoat x) { return x == t; });
	if(count > 0)
		throw DuplicateCoatException();
	this->coats.push_back(t);
	this->WriteToFile();
}

void Repository::deleteCoat(const TrenchCoat& t)
{
	auto it = find(this->coats.begin(), this->coats.end(), t);
	if (it != this->coats.end())
	{
		this->coats.erase(it);
	}
	this->WriteToFile();
}

void Repository::updateCoat(const TrenchCoat& t)
{
	//photography might be "" quantity -1 price -1
	// We check which one of the 3 isn't that
	auto it = find(this->coats.begin(), this->coats.end(), t);
	if (it != this->coats.end())
	{
		if (t.getPhotography() != "")
		{
			(*it).setPhoto(t.getPhotography());
		}
		else if (t.getPrice() != -1)
		{
			(*it).setPrice(t.getPrice());
		}
		else if (t.getQuantity())
		{
			(*it).setQuantity(t.getQuantity());
		}
	}
	this->WriteToFile();
}

TrenchCoat Repository::findCoat(const string size, const string color)
{
	for (auto tc : this->coats)
	{
		if (tc.getSize() == size and tc.getColor() == color)
			return tc;
	}

	return TrenchCoat{};
}

void Repository::update()
{
	WriteToFile();
}

void ShoppingCartRepo::AddCoatShopping(const TrenchCoat& t)
{
	vector<TrenchCoat>& repoCoats = getCoats();
	auto it = find(repoCoats.begin(), repoCoats.end(), t);
	if (it != repoCoats.end())
	{	
		int q = (*it).getQuantity();
		float price = (*it).getPrice() / q;
		q++;
		(*it).setPrice(price*q);
		(*it).setQuantity(q);
		return;
	}
	TrenchCoat tc = t;
	tc.setQuantity(1);
	this->AddCoat(tc);
	return;
}

FileRepo::FileRepo(std::string filename, bool read)
{
	this->filename = filename;
	if (read)
	{
		ReadFromFile();
	}
}

void FileRepo::WriteToFile()
{
	ofstream file(this->filename);
	if (!file.is_open())
		throw FileException("This file could not be opened!");

	for (auto tc : this->coats)
	{
		file << tc;
	}

	file.close();

}

void FileRepo::ReadFromFile()
{
	ifstream file(this->filename);
	if (!file.is_open())
	{
		throw FileException("This file could not be opened!");
	}

	this->coats.clear();
	TrenchCoat T;
	while (file >> T)
	{
		this->coats.push_back(T);
	}
	file.close();
}

void DataBaseRepo::ClearCoatsTable() {
	const char* sql = "DELETE FROM Coats";
	int result = sqlite3_exec(db, sql, 0, 0, 0);
	if (result != SQLITE_OK) {
		throw FileException("Error clearing Coats table");
	}
}

void DataBaseRepo::WriteToFile()
{
	sqlite3 *db;
	int rc = sqlite3_open("database.db", &db);
	if (rc != SQLITE_OK)
	{
		throw FileException("The database could not be opened!");
		sqlite3_close(db);
		return;
	}
	ClearCoatsTable();
	createTable();
	for (auto tc : this->coats)
	{
		addToDataBase(tc);
	}
	sqlite3_close(db);
}

void DataBaseRepo::ReadFromFile()
{
	sqlite3* db;
	sqlite3_stmt* stmt;
	int rc = sqlite3_open("database.db", &db);
	if (rc != SQLITE_OK)
	{
		throw FileException("The database could not be opened!");
		sqlite3_close(db);
		return;
	}
	int r = sqlite3_prepare_v2(db, "SELECT * FROM Coats", -1, &stmt, NULL);
	if (r != SQLITE_OK)
	{
		throw FileException("Error preparing statement");
	}

	while ((r = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		string size = (const char*)sqlite3_column_text(stmt, 0);
		string color = (const char*)sqlite3_column_text(stmt, 1);
		float price = stof((const char*)sqlite3_column_text(stmt, 2));
		int quantity = stoi((const char*)sqlite3_column_text(stmt, 3));
		string photo = (const char*)sqlite3_column_text(stmt, 4);
		TrenchCoat tc = TrenchCoat(size, color, price, quantity, photo);
		this->coats.push_back(tc);
	}
}

sqlite3* DataBaseRepo::createTable()
{
	const char* sql = "CREATE TABLE IF NOT EXISTS Coats("
		"CoatSize TEXT NOT NULL, "
		"CoatColor TEXT NOT NULL, "
		"Price REAL NOT NULL, "
		"Quantity INTEGER NOT NULL, "
		"PhotoLink TEXT NOT NULL, "
		"CONSTRAINT pk_coatID PRIMARY KEY (CoatSize, CoatColor)"
		")";
	int result = sqlite3_exec(this->db, sql, 0, 0, 0);
	if(result != SQLITE_OK)
		throw FileException("Error creating database in file");
	return this->db;
}

void DataBaseRepo::addToDataBase(TrenchCoat tc)
{
	const char* sql = "INSERT INTO Coats (CoatSize, CoatColor, Price, Quantity, PhotoLink) VALUES (?,?,?,?,?)";
	sqlite3_stmt* stmt;
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
		throw FileException("Error preparing statement");
	//Binding parameters
	if (sqlite3_bind_text(stmt, 1, tc.getSize().c_str(), tc.getSize().length(), SQLITE_TRANSIENT) != SQLITE_OK)
		throw StatementException();
	if (sqlite3_bind_text(stmt, 2, tc.getColor().c_str(), tc.getColor().length(), SQLITE_TRANSIENT) != SQLITE_OK)
		throw StatementException();
	if(sqlite3_bind_double(stmt, 3, tc.getPrice()) != SQLITE_OK)
		throw StatementException();
	if (sqlite3_bind_int(stmt, 4, tc.getQuantity()) != SQLITE_OK)
		throw StatementException();
	if (sqlite3_bind_text(stmt, 5, tc.getPhotography().c_str(), tc.getPhotography().length(), SQLITE_TRANSIENT) != SQLITE_OK)
		throw StatementException();

	//Executing the statement
	int result = sqlite3_step(stmt);
	if (result != SQLITE_DONE)
	{
		const char* errorMsg = sqlite3_errmsg(db);
		throw FileException(string("Error inserting new Coat in the database ") + errorMsg + tc.getSize().c_str() + tc.getColor().c_str());
	}
	sqlite3_finalize(stmt);
}

void ShoppingCartCSV::openApp()
{
	/*
	C:\Program Files\Microsoft Office\root\Office16\EXCEL.EXE
	C:\Program Files\Notepad++\notepad++.exe
	*/
	UserInterface ui;
	this->app = ui.getAppPath(this->app);
	string params = "\"" + this->filename + "\"";
	ShellExecuteA(NULL, "open", this->app.c_str(), params.c_str(), NULL, SW_SHOWNORMAL);
	//ShellExecuteA(NULL, "open", "C:\Program Files\Microsoft Office\root\Office16\EXCEL.EXE", params.c_str(), NULL, SW_SHOWNORMAL);
}

void ShoppingCartCSV::WriteToFile()
{
	ofstream file(this->filename);
	if (!file.is_open())
		throw FileException("This file could not be opened!");
	for (auto tc : this->coats)
	{
		file << tc.getSize() << ',' << tc.getColor() << ',' <<
			tc.getPrice() << ',' << tc.getQuantity() << ',' << tc.getPhotography() << endl;
	}
}

void ShoppingCartHTML::openApp()
{
	ShellExecuteA(NULL, NULL, this->filename.c_str(), NULL, NULL, SW_SHOWMAXIMIZED);
}

void ShoppingCartHTML::WriteToFile()
{
	ofstream file(this->filename);
	if (!file.is_open())
		throw FileException("This file could not be opened!");
	file << "<!DOCTYPE html>" << endl;
	file << "<html>" << endl;
	file << "<head>" << endl;
	file << "<title>ShoppingCart</title>" << endl;
	file << "</head>" << endl;
	file << "<body>" << endl;
	file << "<table border=\"1\">" << endl;
	file << "<tr>" << endl;
	file << "<td>Size</td>" << endl;
	file << "<td>Color</td>" << endl;
	file << "<td>Amount</td>" << endl;
	file << "<td>Quantity</td>" << endl;
	file << "<td>Photo Link</td>" << endl;
	file << "</tr>" << endl;
	for (auto tc : this->coats)
	{
		file << "<tr>" << endl;
		file << "<td>" << tc.getSize() << "</td>";
		file << "<td>" << tc.getColor() << "</td>";
		file << "<td>" << tc.getPrice() << "</td>";
		file << "<td>" << tc.getQuantity() << "</td>";
		file << "<td><a href=\"" << tc.getPhotography() << "\">Photo</a></td>";
		file << "</tr>" << endl;
	}
	file << "</table>" << endl;
	file << "</body>" << endl;
	file << "</html>" << endl;
}
