#include "TableEditor.h"
#include "Cell.h"
#include "CellManaging.h"
#include <string>
#include <iostream>
#include<stdlib.h>

using namespace std;

TableEditor::TableEditor() {
	this->height = 0;
	this->width = 0;
	this->cells = nullptr;
	this->row_selector = -2;
	this->col_selector = "";
	this->flag = false;
}

TableEditor::~TableEditor() {
	for (int i = 0; i < this->height; i++) {
		for (int j = 0; j < this->width; j++) {
			delete this->cells[i][j];
		}
		delete[] this->cells[i]; 
	}
	delete[] this->cells;
}

int TableEditor::getHeight() {
	return this->height;
}

int TableEditor::getWidth() {
	return this->width;
}

void TableEditor::increaseHeight() {
	this->height++;
}

void TableEditor::increaseWidth() {
	this->width++;
}

void TableEditor::decreaseHeight() {
	this->height--;
}

void TableEditor::decreaseWidth() {
	this->width--;
}

int TableEditor::getColNum(string s) {
	CellManaging* CM = new CellManaging;
	int res =  CM->findColumnNum(this, s);
	delete CM;
	return res;
}

int TableEditor::findNumOfRows(string table) {
	int res = 0;
	int pos = 0;
	while (pos < table.length()) {
		if (table[pos++] == '\n') res++; 
	}
	return res;
}

int TableEditor::findNumOfCols(string table) {
	int res = 1;
	int pos = 0;
	while (table[pos] != '\n' && pos < table.length()) {
		if (table[pos++] == ',') res++;
	}
	return res;
}

int TableEditor::failedNum(string table, int h, int w) {
	int pos = 0; 
	int i = 0;
	int j = 0; 
	while (pos < table.length()) {
		while (table[pos] != '\n' && table[pos] != ',') {
			if (table[pos] == '"') {
				pos++; 
				while (table[pos] != '"') pos++; 
			}
			pos++; 
		}
		if (table[pos] == ',') j++;
		else if (table[pos] == '\n') {
			if (j != w - 1) return i;
			j = 0; 
			i++;
		}
		pos++; 
	}
	return -3;
}

bool TableEditor::checkFirstRow() {
	for (int i = 0; i < this->width; i++) {
		if (this->cells[0][i]->text[1] != ':') return false;
		for (int j = 2; j < this->cells[0][i]->text.length(); j++) {
			char c = this->cells[0][i]->text[j];
			if (!((c >= '0' && c <= '9') || (c == '_') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) return false;
		}
		// za text[0] je provereno u importTable
	}
	return true;
}

bool TableEditor::checkCell(Cell* C) {
	if (C->t == Type::INT) {
		for (int i = 0; i < C->text.length(); i++) {
			if (C->text[i] > '9' || C->text[i] < '0') return false;
		}
	}
	if (C->t == Type::DECIMAL) {
		int dotCounter = 0; 
		for (int i = 0; i < C->text.length(); i++) {
			if (C->text[i] == '.') {
				dotCounter++;
				continue;
			}
			if (C->text[i] > '9' || C->text[i] < '0') return false;
		}
		if (dotCounter != 1) return false;
	}
	if (C->t == Type::TEXT) {

	}
	return true;
}

int TableEditor::checkOthers() {
	// samo sadrzaj celija, forma tabele je dobra ako smo vec dovde stigli
	for (int i = 1; i < this->height; i++) {
		for (int j = 0; j < this->width; j++) {
			if (!checkCell(this->cells[i][j])) return i;
		}
	}
	return -3;
}

int TableEditor::importTable(string table) {
	// pravimo zeljeni format od stringa table
	// ----------- ne zaboravi izuzetke tj. greske
	if (this->flag == true) delete this;
	this->flag = true;
	int pos = 0;

	// i broj reda 
	// j broj kolone

	int i = 0;
	int j = 0;
	//cout << table.length();
	int numRows = this->findNumOfRows(table);
	int numCols = this->findNumOfCols(table); 

	CellManaging* CM = new CellManaging;

	CM->makeMatrix(this, numRows, numCols);
	
	delete CM;

	int failed_row = failedNum(table, numRows, numCols); 
	if (failed_row != -3) {
		//cout << failed_row - 1;
		return failed_row - 1; 
	}
	// Ok, imamo dobar broj elemenata 
	// sad proveravamo njihov sadrzaj
	
	for (; pos < table.length();) {
		while (table[pos] != ',' && table[pos] != '\n') {

			if (table[pos] == '"') {
				this->cells[i][j]->add('"');
				pos++;
				while (table[pos] != '"') this->cells[i][j]->add(table[pos++]);
				this->cells[i][j]->add('"');
			}
			else this->cells[i][j]->add(table[pos]);
			pos++;
		}

		this->cells[i][j]->row = i;
		this->cells[i][j]->column_num = j;
		this->cells[i][j]->column = this->cells[0][j]->text;


		if (this->cells[0][j]->text[0] == 'i') this->cells[i][j]->t = Type::INT;
		if (this->cells[0][j]->text[0] == 'd') this->cells[i][j]->t = Type::DECIMAL;
		if (this->cells[0][j]->text[0] == 't') this->cells[i][j]->t = Type::TEXT;
		if (this->cells[0][j]->text[0] != 'i' && this->cells[0][j]->text[0] != 'd' && this->cells[0][j]->text[0] != 't') return -1;


		if (table[pos] == '\n') {
			if (i == 0) this->width = j + 1;
			i++;
			j = 0;
		}
		else if (table[pos] == ',') j++;
		pos++;
	}
	this->height = i;

	// posto imamo sigurno dobar broj elemenata, ovde mozemo proveriti njihov sadrzaj

	if (!this->checkFirstRow()) return -1;

	// prvi red sigurno dobar

	failed_row = this->checkOthers();
	if (failed_row != -3) {
		return failed_row - 1;
	}

	return -2;
}

string TableEditor::exportTable() {
	string table=""; 
	for (int i = 0; i < this->height; i++) {
		for (int j = 0; j < this->width; j++) {
			if (this->cells[i][j]->t == Type::DECIMAL && i > 0) {
				//string str = this->cells[i][j]->text; 
				if (this->cells[i][j]->text == "") {
					if (j != this->width - 1) {
						table.append(","); //cout << ",";
					}
					continue;
				}
				double tmp = this->cells[i][j]->why();
				tmp *= 10;
				if (abs(tmp - (int)tmp - 0.5) < 1e-4) {
					// pravilo parne cifre i ako je ostatak blizu 0.5 priblizi ga sledecem broju
					if ((int)tmp % 2 == 1) tmp += 0.5;
					else tmp -= 0.5;
				}
				tmp = (double)round(tmp) / 10;
				string str = to_string(tmp);
				str.erase(str.find_last_not_of('0') + 1, std::string::npos);
				//cout << "---A" << str << "A---";

				for (int k = 0; k < str.length(); k++) { 
					table.append(1, str[k]);  
				}
				if (str[str.length() - 1] == '.') table.append(1, '0');
				//cout << str;
				if (j != this->width - 1) {
					table.append(","); //cout << ",";
				}
				// moram uraditi pravilno zaokruzivanje
				continue; 
			}
			//cout << this->cells[i][j]->text; 
				for (int k = 0; k < this->cells[i][j]->text.length(); k++) {
					table.append(1, this->cells[i][j]->text[k]);
			}
			if (j != this->width - 1) {
				table.append(","); //cout << ",";
			}
		}

		table.append("\n");
		//cout << "\n";
	}
	//cout << table;
	//cout << "----------------------" << endl;
	return table;
}

void TableEditor::deselect() {
	this->row_selector = -2;
	this->col_selector = "";
}

void TableEditor::selectRow(int row_position) {
	if (this->row_selector != -2) {
		deselect();
	}
	this->row_selector = row_position;
}

void TableEditor::selectColumn(string col_name) {
	if (this->col_selector != "") {
		deselect();
	}
	this->col_selector = col_name;
}

void TableEditor::selectCell(int row_position, string col_name) {
	this->row_selector = row_position;
	this->col_selector = col_name;
}

void TableEditor::insertRow() {
	int od = this->height - 1;
	//cout << this->row_selector;
	if (this->row_selector != -2) od = this->row_selector; 

	string nova;
	getline(cin, nova);
	//cout << nova;

	CellManaging* CM = new CellManaging;
	//cout << this->cells[0][0]->text;
	CM->addRoomForColumns(this);
	//cout << this->cells[0][0]->text;
	//cout << "-" << this->cells[5][0]->text << "-";
	delete CM;
	//string nova;
	//cin >> nova;
	// na od+1
	//cout << this->height - 1 << " " << od + 1 << endl; 
	for (int i = this->height - 1; i > od + 1; i--) {
		//this->cells[i] = this->cells[i - 1];
		for (int j = 0; j < this->width; j++) {
			this->cells[i][j]->text = this->cells[i - 1][j]->text;
			this->cells[i][j]->row++;
		}
		//cout << "a";
	}
	for (int j = 0; j < this->width; j++) this->cells[od + 1][j]->text = "";

	int pos = 0;
	int k = 0;
	for (; pos < nova.length();) {
		this->cells[od + 1][k]->text = ""; 
		while (nova[pos] != ',' && pos<nova.length()) {
			if (nova[pos] == '"') {
				this->cells[od + 1][k]->add('"');
				pos++;
				while (nova[pos] != '"') this->cells[od + 1][k]->add(nova[pos++]);
				this->cells[od + 1][k]->add('"');
			}
			else this->cells[od + 1][k]->add(nova[pos]);
			pos++; 
		} 
		this->cells[od + 1][k]->t = this->cells[0][k]->t;
		pos++;
		k++; 
	}
	if (k != this->width) cout << "Doslo je do greske pri unosu" << endl;
	for (int i = 0; i < this->width; i++) {
		if (!checkCell(this->cells[od + 1][i])) cout << "Doslo je do greske pri unosu" << endl;
	}
}

void TableEditor::insertColumn(string col_name, Type type) {
	//cout << this->col_selector;
	int od = this->getColNum(this->col_selector); 
	if (od != this->width - 1) od--; 
	//cout << od;
	CellManaging* CM = new CellManaging;
	CM->addRoomForRows(this);
	//cout << this->width;
	delete CM;
	for (int i = 0; i < this->height; i++) {
		for (int j = this->width - 1; j > od + 1; j--) {
			this->cells[i][j]->text = this->cells[i][j - 1]->text; 
			this->cells[i][j]->column = this->cells[i][j - 1]->column;
			this->cells[i][j]->t = this->cells[i][j - 1]->t;
		}
	}
	for (int i = 0; i < this->height; i++) this->cells[i][od + 1]->text = ""; 
	// sada za ubacenu, na mesto od+1

	//char dodaj;
	switch (type) {
	case INT:
		this->cells[0][od + 1]->add('i');
		//dodaj = 'i';
		break;
	case DECIMAL:
		this->cells[0][od + 1]->add('d');
		//dodaj = 'd';
		break;
	case TEXT:
		this->cells[0][od + 1]->add('t');
		//dodaj = 't';
		break;
	}
	
	//CM->print(this); 

	this->cells[0][od + 1]->add(':');
	//cout << od;

	int pos = 0;
	for (; pos < col_name.length(); pos++) this->cells[0][od + 1]->add(col_name[pos]);

	this->cells[0][od + 1]->column = this->cells[0][od + 1]->text;

	this->cells[0][od + 1]->t = type;

	for (int i = 1; i < this->height; i++) {
		this->cells[i][od + 1]->t = type;
		this->cells[i][od + 1]->column = this->cells[0][od + 1]->text;

		this->cells[i][od + 1]->init();

	}
	// gotovo
}

void TableEditor::deleteContent() {

	// brisanje sadrzaja selektovane celije
	if (this->row_selector != -2 && this->col_selector != "") {
		//cout << "usao"; 
		int kol = this->getColNum(this->col_selector);
		this->cells[row_selector + 1][kol]->destroy();
		return;
	}

	// brisanje selektovanog reda
	if (this->row_selector != -2) {
		//cout << this->row_selector; 
		for (int j = 0; j < this->width; j++) this->cells[this->row_selector + 1][j]->text = ""; 
		for (int i = this->row_selector + 1; i < this->height - 1; i++) {
			for (int j = 0; j < this->width; j++) {
				this->cells[i][j]->text = this->cells[i + 1][j]->text;

			}
		}
		for (int i = 0; i < this->width; i++) delete this->cells[this->height - 1][i];
		delete[] this->cells[this->height - 1];
		this->decreaseHeight();
		this->row_selector = -2;
		return;
	}

	// brisanje selektovane kolone
	if (this->col_selector != "") {
		//cout << this->col_selector << " ABC";
		int kol = this->getColNum(this->col_selector);
		//cout << kol;

		for (int i = 0; i < this->height; i++) this->cells[i][kol]->text = ""; 

		for (int i = 0; i < this->height; i++) {
			for (int j = kol; j < this->width - 1; j++) {
				this->cells[i][j]->text = this->cells[i][j + 1]->text;
				this->cells[i][j]->t = this->cells[i][j + 1]->t;
			}
			delete this->cells[i][this->width - 1];
		}
		this->decreaseWidth();
		this->col_selector = ""; 
		return;
	}
	// nista nije selektovano
	return;
}

void TableEditor::editCellValue() {
	if (this->col_selector != "" && this->row_selector != -2) {
		string nova;
		cin >> nova;
		int kol = this->getColNum(this->col_selector);
		this->cells[this->row_selector + 1][kol]->text = "";
		for (int pos = 0; pos < nova.length(); pos++) this->cells[this->row_selector + 1][kol]->add(nova[pos]);
		return;
	}
	return;
}

void TableEditor::add(double value) {
	if (this->col_selector == "") return;

	int kol = this->getColNum(this->col_selector);
	if (this->cells[0][kol]->t == Type::TEXT) return;

	if (this->row_selector == -2) {
		// znaci cela kolona
		// dodaje se val 

		for (int i = 1; i < this->height; i++) {
			if (this->cells[i][kol]->t == Type::DECIMAL) {
				this->cells[i][kol]->addDouble(value);
			}
			else {
				this->cells[i][kol]->addInt(floor(value));
			}
		}
		return;
	}
	else {
		if (this->cells[row_selector + 1][kol]->t == Type::DECIMAL) {
			this->cells[row_selector + 1][kol]->addDouble(value);
		}
		else {
			this->cells[row_selector + 1][kol]->addInt(floor(value));
		}
		return;
	}
}

void TableEditor::mul(double value) {
	if (this->col_selector == "") return;

	int kol = this->getColNum(this->col_selector);
	if (this->cells[0][kol]->t == Type::TEXT) return;

	if (this->row_selector == -2) {
		// znaci cela kolona
		// dodaje se val 
		//cout << "ovde sam"; 
		for (int i = 1; i < this->height; i++) {
			//cout << "ovde sam"; 
			//if (this->cells[i][kol]->text == "") continue; 
			if (this->cells[i][kol]->t == Type::DECIMAL) {
				this->cells[i][kol]->mulDouble(value);
			}
			else {
				this->cells[i][kol]->mulInt(floor(value));
			}
		}
		return;
	}
	else {
		if (this->cells[row_selector + 1][kol]->t == Type::DECIMAL) {
			//cout << value; 
			this->cells[row_selector + 1][kol]->mulDouble(value);
		}
		else {
			this->cells[row_selector + 1][kol]->mulInt(floor(value));
		}
		return;
	}
}

void TableEditor::sortByValue(string col_name, bool asc) {
	CellManaging* CM = new CellManaging;
	if (asc == true) CM->sortItAsc(this, col_name);
	else CM->sortItDesc(this, col_name);
	delete CM;
	return;
}

int TableEditor::findFirstOf(string value, string col_name) {
	int kol = this->getColNum(col_name);
	for (int i = 1; i < this->height; i++) {
		if (this->cells[i][kol]->text == value) return i-1;
	}
	return -2;
}

int TableEditor::findLastOf(string value, string col_name) {
	int kol = this->getColNum(col_name);
	for (int i = this->height - 1; i >= 1; i--) {
		if (this->cells[i][kol]->text == value) return i-1;
	}
	return -2;
}

int TableEditor::countValues(string value, string col_name) {
	int kol = this->getColNum(col_name);
	int res = 0;
	for (int i = 1; i < this->height; i++) {
		if (this->cells[i][kol]->text == value) res++;
	}
	return res;
}

int TableEditor::countDistinctValues(string col_name) {
	int kol = this->getColNum(col_name);
	int count = 1;
	int i, j;
	for (i = 2; i < this->height; i++) {
		for (j = 1; j < i; j++) {
			if (this->cells[i][kol]->text == this->cells[j][kol]->text) {
				break;
			}
		}
		if (i == j) count++;
	}
	return count;
}