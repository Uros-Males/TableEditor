#ifndef TABLE_EDITOR_H
#define TABLE_EDITOR_H

#include "Cell.h"
#include <string>

using namespace std;


class TableEditor {
public:
	TableEditor();
	~TableEditor();


	int failedNum(string table, int h, int w);
	bool checkFirstRow(); 
	int checkOthers();
	bool checkCell(Cell* C);

	int getHeight();
	int getWidth();

	void increaseHeight();
	void increaseWidth();

	void decreaseHeight();
	void decreaseWidth();

	int importTable(string table);
	string exportTable();

	void deselect();
	void selectRow(int row_position);
	void selectColumn(string col_name);
	void selectCell(int row_position, string col_name);

	void insertRow();
	void insertColumn(string col_name, Type type);

	int getColNum(string s);

	void deleteContent();
	void editCellValue();

	void add(double value);
	void mul(double value);

	void sortByValue(string col_name, bool asc);

	int findFirstOf(string value, string col_name);
	int findLastOf(string value, string col_name);

	int countValues(string value, string col_name);
	int countDistinctValues(string col_name);

	int findNumOfRows(string table);
	int findNumOfCols(string table);

	friend class CellManaging;
	friend class Cell;

private:
	Cell*** cells;
	int height, width;

	int row_selector;
	string col_selector;

	bool flag;

};

#endif