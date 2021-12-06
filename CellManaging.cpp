#include "CellManaging.h"
#include "TableEditor.h"
#include <iostream>
using namespace std;

// NE ZABORAVI RESAVANJE MEMORY LEAK-A

void CellManaging::makeMatrix(TableEditor* T, int h, int w) {
	T->cells = new Cell * *[h];
	for (int i = 0; i < h; i++) {
		T->cells[i] = new Cell * [w];
	}
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			T->cells[i][j] = new Cell; 
		}
	}
}

void CellManaging::deleteMatrix(Cell*** tmp, int h, int w) {
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			delete tmp[i][j];
		}
		delete[] tmp[i];
	}
	delete[] tmp;
}

void CellManaging::addRoomForOne(TableEditor* T, int id, int len) {
	Cell** tmp = T->cells[id]; 
	T->cells[id] = new Cell * [len + 1]; 
	for (int i = 0; i < len + 1; i++) {
		T->cells[id][i] = new Cell; 
	}
	for (int i = 0; i < len; i++) {
		T->cells[id][i]->column = tmp[i]->column;
		T->cells[id][i]->column_num = tmp[i]->column_num;
		T->cells[id][i]->text = tmp[i]->text;
		T->cells[id][i]->t = tmp[i]->t;
		T->cells[id][i]->row = tmp[i]->row;
	}
	for (int i = 0; i < len; i++) {
		delete tmp[i];
	}
	delete[] tmp;
}

void CellManaging::addRoomForArray(TableEditor* T, int len) {

	Cell*** tmp = T->cells;
	makeMatrix(T, len + 1, T->getWidth());
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < T->getWidth(); j++) {
			T->cells[i][j]->column = tmp[i][j]->column;
			T->cells[i][j]->column_num = tmp[i][j]->column_num;
			T->cells[i][j]->text = tmp[i][j]->text;
			T->cells[i][j]->t = tmp[i][j]->t;
			T->cells[i][j]->row = tmp[i][j]->row;

		}
	}
	deleteMatrix(tmp, len, T->getWidth());
}

void CellManaging::print(TableEditor* T) {
	for (int i = 0; i < T->height; i++) {
		for (int j = 0; j < T->width; j++) {
			cout << T->cells[i][j]->text << " ";
		}
		cout << endl;
	}
}

// dodaje prostor za dodatni element na kraju svakog reda da bi dobili prostor za novu kolonu 
void CellManaging::addRoomForRows(TableEditor* T) {
	for (int i = 0; i < T->getHeight(); i++) {
		addRoomForOne(T, i, T->getWidth());
	}
	T->increaseWidth();
}

// dodaje prostor za dodatni element na kraju svake kolone da bi dobili prostor za novi red
void CellManaging::addRoomForColumns(TableEditor* T) {
	addRoomForArray(T, T->getHeight());
	T->increaseHeight();
}

int CellManaging::findColumnNum(TableEditor* T, string s) {
	for (int i = 0; i < T->getWidth(); i++) {
		if (T->cells[0][i]->takeOff() == s) {
			return i;
		}
	}
	// problem se javlja zbog "(tip):"
	return T->getWidth() - 1;
}

void CellManaging::sortItAsc(TableEditor* T, string s) {
	// ascii kod za tacku je manji od bilo koje cifre tako da valjda mogu bez razmatranja tipova da sortiram
	int kol = findColumnNum(T, s);
	for (int i = 1; i < T->getHeight(); i++) {
		for (int j = i + 1; j < T->getHeight(); j++) {
			if (T->cells[i][kol]->text.length() > T->cells[j][kol]->text.length()) {
				Cell** C;
				C = T->cells[i];
				T->cells[i] = T->cells[j];
				T->cells[j] = C;
				continue;
			}
			if (T->cells[i][kol]->text > T->cells[j][kol]->text && T->cells[i][kol]->text.length() == T->cells[j][kol]->text.length()) {
				Cell** C;
				C = T->cells[i];
				T->cells[i] = T->cells[j];
				T->cells[j] = C;
				continue;
			}
		}
	}
	return;
}

void CellManaging::sortItDesc(TableEditor* T, string s) {
	// ascii kod za tacku je manji od bilo koje cifre tako da valjda mogu bez razmatranja tipova da sortiram
	int kol = findColumnNum(T, s);
	for (int i = 1; i < T->getHeight(); i++) {
		for (int j = i + 1; j < T->getHeight(); j++) {
			if (T->cells[i][kol]->text.length() < T->cells[j][kol]->text.length()) {
				Cell** C;
				C = T->cells[i];
				T->cells[i] = T->cells[j];
				T->cells[j] = C;
				continue;
			}
			if (T->cells[i][kol]->text < T->cells[j][kol]->text && T->cells[i][kol]->text.length() == T->cells[j][kol]->text.length()) {
				Cell** C;
				C = T->cells[i];
				T->cells[i] = T->cells[j];
				T->cells[j] = C;
				continue;
			}
		}
	}
	return;
}

