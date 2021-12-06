#ifndef CELL_H
#define CELL_H

#include <string>

using namespace std;

enum Type {
	INT, DECIMAL, TEXT
};

class Cell {
public:

	Cell();

	double why();

	friend class CellManaging;
	friend class TableEditor;

	void add(char s);
	void init();

	void addDouble(double val);
	void addInt(int val);

	void mulDouble(double val);
	void mulInt(int val);

	string takeOff(); 

	void destroy(); 

	Cell& operator=(const Cell& C) {
		row = C.row;
		column_num = C.column_num; 
		text = C.text; 
		column = C.column;
		t = C.t;
		return *this;
	}

private:
	int row, column_num;
	string column;
	string text;

	Type t;

};

#endif 