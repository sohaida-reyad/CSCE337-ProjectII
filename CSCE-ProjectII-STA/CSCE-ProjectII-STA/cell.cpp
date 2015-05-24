#include "cell.h"
#include <iostream>
#include <fstream>
using namespace std;

cell::cell() {}		//default constructor
cell::cell(string s) : type(s) {}
void cell::addpin(pin p)
{
	pins[p.getType()] = p;
	//pins.push_back(p);
}

void cell::displayPins(ofstream& out)
{
	
	//cout << pins.size() << endl;
	out << type << endl;
	for (map<string, pin>::iterator it = pins.begin(); it != pins.end(); ++it)
	{
		out << it->first << '\t' << it->second.getDirection() << '\t' << it->second.getCapacitance() << endl;
		if (it->second.getDirection() != 1)
		{
			it->second.displayTable(1, out);
			it->second.displayTable(2, out);
			it->second.displayTable(3, out);
			it->second.displayTable(4, out);
			if (it->first == "CLK" && type.find("DFF") >= 0 && type.find("DFF") < type.length())
			{
				it->second.displayTable(5, out);
				it->second.displayTable(6, out);
				it->second.displayTable(7, out);
				it->second.displayTable(8, out);
			}
		}
	}

	
}

pin& cell::getPinReference(string s)
{
	return (pins[s]);
}

void cell::addTypes()
{
	for (map<string, pin>::iterator it = pins.begin(); it != pins.end(); ++it)
	{
		it->second.setType(it->first);
	}
}

string cell::getPinFromNumber(int pinNumber)
{
	int counter = 0;
	for (map<string, pin>::iterator it = pins.begin(); it != pins.end(); it++)
	{
		if (counter == pinNumber)
			return it->first;
		counter++;
	}
}

float cell::getPinCapacitance(int pinNumber)
{
	float value;
	int counter = 0;
	for (map<string, pin>::iterator it = pins.begin(); it != pins.end(); it++)
	{
		if (counter == pinNumber)
		{
			return it->second.getCapacitance();
			
		}
		counter++;
	}
}



float cell::getOutputTransition(string pinName, float inputTransition, float capacitance)
{
	return pins[pinName].getOutputTransition(inputTransition, capacitance);
}
float cell::getDelay(string pinName, float inputTransition, float capacitance)
{
	return pins[pinName].getDelay(inputTransition, capacitance);
}

float cell::getSetupTime(float CLKtransition, float pinTransition)
{
	return pins["CLK"].getSetupTime(CLKtransition, pinTransition);
}
float cell::getHoldTime(float CLKtransition, float pinTransition)
{
	return pins["CLK"].getHoldTime(CLKtransition, pinTransition);
}
