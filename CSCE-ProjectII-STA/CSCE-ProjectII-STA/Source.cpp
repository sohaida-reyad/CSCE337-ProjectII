#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include "cell.h"
#include "Liberty.h"
using namespace std;

/*int main()
{
	Liberty libertyFile;
	string filename = "osu035.lib";	//put here the path of the liberty file 
	libertyFile.parse(filename);
	cout << libertyFile.getDelay("NOR2X1", 0.130977824, 0.949999988, 0) << endl;
	cout << libertyFile.getSetupTime("LATCH", 0.3, 0.24) << endl;



	/*
	Public Member functions to use them ya Sohaida
	void parse(string); --> to parse the liberty file and fill the data structures
	void displayCells(); --> to write the content of the cells to make sure I parsed them correctly , mainly for testing purposes
	float getCapacitance(string gatename, int pinNumber); pins are zero-indexed, so 1st pin is pin 0 
	float getOutputTransition(string gatename, float inputTransition, float capacitance, int pinNumber);
	float getDelay(string gatename, float inputTransition, float capacitance, int pinNumber);
	float getSetupTime(string gatename,  float CLKtransition, float D_pin_Transition);
	float getHoldTime(string gatename, float CLKtransition, float D_pin_Transition);
	*/
	//************************************************************************
	//for testing purposes, don't use them ya Sohaida
/*	libertyFile.displayCells();
	//cout << libertyFile.getDelay("AND2X1", 0.05, 0.02, 0);

	
	return 0;
}
	 	*/
	 