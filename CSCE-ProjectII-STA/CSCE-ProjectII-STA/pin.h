#include <string>

using namespace std;

#ifndef PIN_H
#define PIN_H

class pin
{
public:
	pin();		//default constructor
	~pin();
	pin(string);
	void setDirection(int);
	void setCapacitance(float);
	void setTableDimension(int, int);
	void displayTable(int, ofstream&);
	float** getCell_rise();
	float** getCell_fall();
	float** getRise_transition();
	float** getFall_transition();
	float** getSetupTime_rise();
	float** getSetupTime_fall();
	float** getHoldTime_rise();
	float** getHoldTime_fall();
	string getType();
	float getCapacitance();
	int getDirection();
	float* getIndexRow();
	float* getIndexCol();
	float* getCLKIndexRow();
	float* getCLKIndexCol();
	int getNRows();
	void setType(string);
	void setClkTablesDimensions(int, int);
	float getOutputTransition(float, float);
	float getDelay(float, float);
	float getSetupTime(float, float);
	float getHoldTime(float, float);
	
private:
	string type;
	int direction;			//0 for input, 1 for output
	float capacitance;
	float** cell_rise;
	float** rise_transition;
	float** cell_fall;
	float** fall_transition;
	int nRows, nColumns, nRowsSetup, nColumnsSetup;
	float* indexRow;
	float* indexCol;
	float** setupTime_rise;
	float** setupTime_fall;
	float** holdTime_rise;
	float** holdTime_fall;
	float* clkIndexRows;
	float* clkIndexCol;
	float intrapolate(float, float, float, float, float, float, float, float, float, float);
	void getValues(float*, float&, float&, float, int, int&, int&);

};


#endif