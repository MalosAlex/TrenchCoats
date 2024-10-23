#pragma once
#include "TrenchCoat.h"
#include <vector>
//#include <winsqlite/winsqlite3.h>
#include "sqlite3.h"
#include "RepositoryExceptions.h"

using namespace std;

class Repository {
protected:
	vector<TrenchCoat> coats;
	string filename;
	virtual void ReadFromFile() {};
	virtual void WriteToFile() {};

public:
	/*
	Constructor with parameters
	Initializes an object of type repository, by reading data from the given file.
	Throws: FileException - if the file cannot be opened.
	*/
	Repository(const string& filename, bool read);

	Repository(){}

	/*
	Adds a coat to the repository
	Inputs: t - trench coat
	Output: The trench coat is added to the repository
	Throws: FileException - if the file cannot be opened.
	*/
	void AddCoat(const TrenchCoat& t);
	
	/*
	Finds a coat from the repository
	Inputs: size string and color string
	Output: the coat that was found or an empty coat if it wasn't found
	*/
	TrenchCoat findCoat(const std::string size, const std::string color);

	/*
	Deletes a coat from the repository
	Inputs: t - trench coat
	Output: The trench coat has been deleted
	Throws: FileException - if the file cannot be opened.
	*/
	void deleteCoat(const TrenchCoat& t);

	/*
	Updates a coat from the repository
	Inputs: t - trench coat which might have photography "", quantity -1, price -1, only one not being true
	Output: The trench coat has been updated
	Throws: FileException - if the file cannot be opened.
	*/
	void updateCoat(const TrenchCoat& t);

	vector<TrenchCoat>& getCoats() { return this->coats; }
	void update();
};

class FileRepo : public Repository {
public:
	FileRepo(std::string filename, bool read);
private:
	void WriteToFile();
	void ReadFromFile();
};

class DataBaseRepo : public Repository {
public:
	DataBaseRepo(std::string f, bool read){
		this->filename = f;
		int rc = sqlite3_open(this->filename.c_str(), &db);
		if (rc != SQLITE_OK) {
			throw FileException("The database could not be opened!");
		}
		createTable();
		if (read)
		{
			ReadFromFile();
		}
	}
private:
	sqlite3* db;
	string filename = "database.db";
	void ClearCoatsTable();
	void WriteToFile();
	void ReadFromFile();
	sqlite3* createTable();
	void addToDataBase(TrenchCoat tc);
};

class ShoppingCartRepo : public Repository {
public:
	ShoppingCartRepo(std::string filename, bool read) : Repository(filename, read) {}
	ShoppingCartRepo() : Repository() {}
	void AddCoatShopping(const TrenchCoat& t);
	virtual void openApp() { cout << "B"; };
protected:
	string filename;

};

class ShoppingCartCSV : public ShoppingCartRepo {
public:
	ShoppingCartCSV(std::string filename, bool read) : ShoppingCartRepo(filename, read) { this->filename = filename; }
	void openApp();
private:
	void WriteToFile();
	string app;
};

class ShoppingCartHTML : public ShoppingCartRepo {
public:
	ShoppingCartHTML(std::string filename, bool read) : ShoppingCartRepo(filename, read) { this->filename = filename; }
	void openApp();
private:
	void WriteToFile();
};

