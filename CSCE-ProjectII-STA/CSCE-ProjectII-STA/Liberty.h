#include <string>
#include <map>
#include "pin.h"
#include "cell.h"
#include <regex>
using namespace std;
#ifndef LIBERTY_H
#define LIBERTY_H

class Liberty
{
public:
	void parse(string);
	Liberty();
	void displayCells();
	float getCapacitance(string, int);
	float getOutputTransition(string, float, float, int);
	float getDelay(string, float, float, int);
	float getSetupTime(string,  float, float);
	float getHoldTime(string, float, float);
private:
	bool exists(string, string);
	void parsePin(string, pin&, ifstream&);
	string getrelatedPin(ifstream &);
	void addElements(float*, string);
	void parseTable2(pin& , ifstream &, float**, int );
	void parseTable(pin& , ifstream &);
	void parseCLKTable(pin&, ifstream&);
	void ParseTiming(pin& , ifstream &, bool);
	void addPinTypes();

	map<string, cell> Cells;
};

#endif