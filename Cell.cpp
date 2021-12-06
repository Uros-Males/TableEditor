#include "Cell.h"
#include <iostream>
#include<string>
using namespace std;

Cell::Cell() {
	this->text = ""; 
	this->row = 0;
	this->column_num = 0;
	this->t = Type::INT;  
	this->column = ""; 
}

void Cell::destroy() {
	this->text = "";
	this->row = 0;
	this->column_num = 0;
	this->t = Type::INT;
	this->column = "";
}

void Cell::add(char s) {
	this->text.append(1, s);
}

string Cell::takeOff() {
	string res=""; 
	for (int i = 2; i < this->text.length(); i++) res.append(1, this->text[i]); 
	return res;
}

void Cell::init() {
	switch (this->t) {
	case Type::INT: 
		this->text = '0';
		break;
	case Type::DECIMAL:
		this->text = '0.0';
		break;
	case Type::TEXT:
		this->text = "";
		break;
	}
}

void Cell::addDouble(double val) {
	if (this->text == "") return;
	double pretvoren = stod(this->text);
	pretvoren += val;
	this->text = to_string(pretvoren);
}

void Cell::addInt(int val) {
	if (this->text == "") return;
	int pretvoren = stod(this->text);
	pretvoren += val;
	this->text = to_string(pretvoren);
}

void Cell::mulDouble(double val) {
	if (this->text == "") return;
	double pretvoren = stod(this->text);
	pretvoren *= val;
	this->text = to_string(pretvoren);
}

void Cell::mulInt(int val) {
	if (this->text == "") return;
	int pretvoren = stod(this->text);
	pretvoren *= val;
	this->text = to_string(pretvoren);
}

double Cell::why() {
	if (this->text == "") return -1;
	//cout << this->text;
	double res = stod(this->text);
	return res;
}