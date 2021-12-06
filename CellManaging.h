#include <string>
#include "TableEditor.h"
#include "Cell.h"

class CellManaging {
public:
	void addRoomForOne(TableEditor*, int id, int len);
	void addRoomForArray(TableEditor* T, int len);
	void addRoomForRows(TableEditor* T);
	void addRoomForColumns(TableEditor* T);

	void makeMatrix(TableEditor* T, int h, int w);
	void deleteMatrix(Cell*** C, int h, int w);

	void print(TableEditor* T); 

	int findColumnNum(TableEditor* T, string s);

	void sortItAsc(TableEditor* T, string s);
	void sortItDesc(TableEditor* T, string s);

	friend class TableEditor;
	friend class Cell;
};
