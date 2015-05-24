
#include <map>
#include <fstream>
#include "pin.h"

#ifndef CELL_H
#define CELL_H

class cell
{
public:
	cell();		//default constructor
	cell(std::string);
	void addpin(pin);		//add pin to the map of pins
	void displayPins(ofstream&);
	pin& getPinReference(string);
	void addTypes();
	float getPinCapacitance(int);
	string getPinFromNumber(int);
	float getOutputTransition(string, float, float);
	float getDelay(string, float, float);
	float getSetupTime(float, float);
	float getHoldTime(float, float);
private:
	std::string type;
	map<string,pin> pins;


};


#endif