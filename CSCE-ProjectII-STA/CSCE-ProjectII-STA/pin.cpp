#include "pin.h"
#include <iostream>
#include <fstream>
#include <algorithm>
pin::pin() {}

pin::pin(string s) : type(s), nRows(0), nColumns(0) {}


pin::~pin()
{
	if (nRows > 0 && nColumns < 0)
	{

		for (int i = 0; i < nRows; i++)
		{
			delete cell_rise[i];
			delete rise_transition[i];
			delete cell_fall[i];
			delete fall_transition[i];
			if (setupTime_rise[i])
				delete setupTime_rise[i];
			if (setupTime_fall[i])
				delete setupTime_fall[i];
			if (holdTime_rise[i])
				delete holdTime_rise[i];
			if (holdTime_fall[i])
				delete holdTime_fall[i];
		}
		if (cell_rise)
			delete [] cell_rise;
		if (cell_fall)
			delete [] cell_fall;
		if (rise_transition)
			delete [] rise_transition;
		if (fall_transition)
			delete [] fall_transition;
		if (indexRow)
			delete [] indexRow;
		if (indexCol)
			delete [] indexCol;
		if (setupTime_rise)
			delete [] setupTime_rise;
		if (setupTime_fall)
			delete [] setupTime_fall;
		if (holdTime_rise)
			delete [] holdTime_rise;
		if (holdTime_fall)
			delete [] holdTime_fall;
		if (clkIndexCol)
			delete [] clkIndexCol;
		if (clkIndexRows)
			delete [] clkIndexRows;
	}
}
void pin::setDirection(int dir)
{
	direction = dir;
}
void pin::setCapacitance(float c)
{
	capacitance = c;
}
void pin::setTableDimension(int r, int c)
{
	nRows = r;
	nColumns = c;
	indexRow = new float[r];
	indexCol = new float[c];

	cell_rise = new float*[r];
	cell_fall = new float*[r];
	rise_transition = new float*[r];
	fall_transition = new float*[r];
	for (int i = 0; i < r; i++)
	{
		cell_rise[i] = new float[c];
		cell_fall[i] = new float[c];
		rise_transition[i] = new float[c];
		fall_transition[i] = new float[c];
	}
}

float** pin::getSetupTime_rise()
{
	return setupTime_rise;
}
float** pin::getSetupTime_fall()
{
	return setupTime_fall;
}
float** pin::getHoldTime_rise()
{
	return holdTime_rise;
}
float** pin::getHoldTime_fall()
{
	return holdTime_fall;
}
float** pin::getCell_rise()
{
	return cell_rise;
}
float** pin::getCell_fall()
{
	return cell_fall;
}
float** pin::getRise_transition()
{
	return rise_transition;
}
float** pin::getFall_transition()
{
	return fall_transition;
}

string pin::getType()
{
	return type;
}
float pin::getCapacitance()
{
	return capacitance;
}
int pin::getDirection()
{
	return direction;
}
float* pin::getIndexRow()
{
	return indexRow;
}

float* pin::getCLKIndexRow()
{
	return clkIndexRows;
}

float* pin::getCLKIndexCol()
{
	return clkIndexCol;
}
float* pin::getIndexCol()
{
	return indexCol;
}

int pin::getNRows()
{
	return nRows;
}

void pin::displayTable(int k, ofstream& out)
{
	if (k > 4)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				 if (k == 5)
					out << holdTime_rise[i][j] << '\t';
				else if (k == 6)
					out << holdTime_fall[i][j] << '\t';
				else if (k == 7)
					out << setupTime_rise[i][j] << '\t';
				else if (k == 8)
					out << holdTime_fall[i][j] << '\t';
			}
			out << endl;
		}
		return;
	}
	for (int i = 0; i < getNRows(); i++)
	{

		for (int j = 0; j < getNRows(); j++)
		{
			if (k == 1)
				out << cell_rise[i][j] << '\t';
			else if (k==2)
				out << rise_transition[i][j] << '\t';
			else if (k==3)
				out << cell_fall[i][j] << '\t';
			else if (k == 4)
				out << fall_transition[i][j] << '\t';
			
		}
		out << endl;
	}
	out << "-----------------------------------------------------" << endl;
}

void pin::setType(string s)
{
	type = s;
}

void pin::setClkTablesDimensions(int r, int c)
{
	nRowsSetup = r;
	nColumnsSetup = c;
	clkIndexRows = new float[r];
	clkIndexCol = new float[c];

	setupTime_rise = new float*[r];
	setupTime_fall = new float*[r];
	holdTime_rise = new float*[r];
	holdTime_fall = new float*[r];
	for (int i = 0; i < r; i++)
	{
		setupTime_rise[i] = new float[c];
		setupTime_fall[i] = new float[c];
		holdTime_rise[i] = new float[c];
		holdTime_fall[i] = new float[c];
	}
}

float pin::intrapolate(float x0, float y0, float x1, float x2, float y1, float y2, float T11, float T12, float T21, float T22)
{
	float T00, x01, x20, y01, y20;
	x01 = (x0 - x1) / (x2 - x1);
	x20 = (x2 - x0) / (x2 - x1);
	y01 = (y0 - y1) / (y2 - y1);
	y20 = (y2 - y0) / (y2 - y1);

	T00 = x20 * y20 * T11 + x20 * y01 * T12 + x01 * y20 *T21 + x01 * y01 * T22;
	return T00;
}

void pin::getValues(float* index, float& value1, float& value2, float target, int N, int& i1, int& i2)
{
	if (target < index[0])
	{
		value1 = index[0];
		value2 = index[1];
		i1 = 0;
		i2 = 1;
		
	}
	else if (target > index[N - 1])
	{
		value2 = indexRow[N - 1];
		value1 = indexRow[N - 2];
		i1 = N - 2;
		i2 = N - 1;
	}


	else
	{
		for (int i = 0; i < N - 1; i++)
		{
			if (index[i] <= target && index[i + 1] >= target)
			{
				value1 = index[i];
				value2 = index[i + 1];
				i1 = i;
				i2 = i + 1;
			}
		}
	}
}
float pin::getOutputTransition(float inputTransition, float capac)
{
	int row = -1, col = -1;
	int x1, x2, y1, y2;
	float row_value1, col_value1, row_value2, col_value2;
	for (int i = 0; i < nRows; i++)
		if (indexRow[i] == capac)
			row = i;
	for (int i = 0; i < nColumns;i++)
	if (indexCol[i] == inputTransition)
		col = i;
	if (row >= 0 && col >= 0)
		return max(rise_transition[row][col], fall_transition[row][col]);

	getValues(indexRow, row_value1, row_value2, capac, nRows, x1, x2);
	getValues(indexCol, col_value1, col_value2, inputTransition, nColumns, y1, y2);

	return max(
		intrapolate(capac, inputTransition, row_value1, row_value2, col_value1, col_value2, rise_transition[x1][y1], rise_transition[x1][y2], rise_transition[x2][y1], rise_transition[x2][y2]),
		intrapolate(capac, inputTransition, row_value1, row_value2, col_value1, col_value2, fall_transition[x1][y1], fall_transition[x1][y2], fall_transition[x2][y1], fall_transition[x2][y2])
		);
}
float pin::getDelay(float inputTransition, float capac)
{
	int row = -1, col = -1;
	int x1, x2, y1, y2;
	float row_value1, col_value1, row_value2, col_value2;
	for (int i = 0; i < nRows; i++)
	{

		if (indexRow[i] == capac)
		{
			row = i;
			break;
		}
	}
	for (int i = 0; i < nColumns; i++)
	{
		if (indexCol[i] == inputTransition)
		{
			col = i;
			break;
		}
	}
	if (row >= 0 && col >= 0)
		return max(cell_rise[row][col], cell_fall[row][col]);

	getValues(indexRow, row_value1, row_value2, capac, nRows, x1, x2);
	getValues(indexCol, col_value1, col_value2, inputTransition, nColumns, y1, y2);

	return max(
		intrapolate(capac, inputTransition, row_value1, row_value2, col_value1, col_value2, cell_rise[x1][y1], cell_rise[x1][y2], cell_rise[x2][y1], cell_rise[x2][y2]),
		intrapolate(capac, inputTransition, row_value1, row_value2, col_value1, col_value2, cell_fall[x1][y1], cell_fall[x1][y2], cell_fall[x2][y1], cell_fall[x2][y2])
		);
}

float pin::getSetupTime(float CLKTransition, float pinTransition)
{
	int row = -1, col = -1;
	int x1, x2, y1, y2;
	float row_value1, col_value1, row_value2, col_value2;
	for (int i = 0; i < nRowsSetup; i++)
	{

		if (indexRow[i] == CLKTransition)
		{
			row = i;
			break;
		}
	}
	for (int i = 0; i < nColumnsSetup; i++)
	{
		if (indexCol[i] == pinTransition)
		{
			col = i;
			break;
		}
	}
	if (row >= 0 && col >= 0)
		return max(setupTime_rise[row][col], setupTime_fall[row][col]);

	getValues(clkIndexRows, row_value1, row_value2, CLKTransition, nRowsSetup, x1, x2);
	getValues(clkIndexCol, col_value1, col_value2, pinTransition, nColumnsSetup, y1, y2);

	return max(
		intrapolate(CLKTransition, pinTransition, row_value1, row_value2, col_value1, col_value2, setupTime_rise[x1][y1], setupTime_rise[x1][y2], setupTime_rise[x2][y1], setupTime_rise[x2][y2]),
		intrapolate(CLKTransition, pinTransition, row_value1, row_value2, col_value1, col_value2, setupTime_fall[x1][y1], setupTime_fall[x1][y2], setupTime_fall[x2][y1], setupTime_fall[x2][y2])
		);
}
float pin::getHoldTime(float CLKTransition, float pinTransition)
{
	int row = -1, col = -1;
	int x1, x2, y1, y2;
	float row_value1, col_value1, row_value2, col_value2;
	for (int i = 0; i < nRowsSetup; i++)
	{

		if (indexRow[i] == CLKTransition)
		{
			row = i;
			break;
		}
	}
	for (int i = 0; i < nColumnsSetup; i++)
	{
		if (indexCol[i] == pinTransition)
		{
			col = i;
			break;
		}
	}
	if (row >= 0 && col >= 0)
		return max(holdTime_rise[row][col], holdTime_fall[row][col]);

	getValues(clkIndexRows, row_value1, row_value2, CLKTransition, nRowsSetup, x1, x2);
	getValues(clkIndexCol, col_value1, col_value2, pinTransition, nColumnsSetup, y1, y2);

	return max(
		intrapolate(CLKTransition, pinTransition, row_value1, row_value2, col_value1, col_value2, holdTime_rise[x1][y1], holdTime_rise[x1][y2], holdTime_rise[x2][y1], holdTime_rise[x2][y2]),
		intrapolate(CLKTransition, pinTransition, row_value1, row_value2, col_value1, col_value2, holdTime_fall[x1][y1], holdTime_fall[x1][y2], holdTime_fall[x2][y1], holdTime_fall[x2][y2])
		);
}