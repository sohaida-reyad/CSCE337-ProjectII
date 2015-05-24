#include "Liberty.h"
#include <fstream>
#include <iostream>
#include <sstream>
Liberty::Liberty() {}

bool Liberty::exists(string sub, string main)
{
	if (main.find(sub) >= 0 && main.find(sub) < main.length())
		return true;
	return false;
}

void Liberty::addPinTypes()
{
	for (map<string, cell>::iterator it = Cells.begin(); it != Cells.end(); it++)
	{
		it->second.addTypes();
	}
}


void Liberty::parsePin(string line, pin& aPin, ifstream& libertyFile)
{
	for (int i = 0; i < 4; i++)
	{
		getline(libertyFile, line);
		if (exists("input", line))
			aPin.setDirection(0);
		else if (exists("output", line))
			aPin.setDirection(1);
		else if (exists("inout", line))
			aPin.setDirection(2);

		if (exists("capacitance :", line) && !exists("_capacitance", line))
		{
			int pos = line.find(":");
			line.erase(0, pos + 1);
			line.erase(line.length() - 1, 1);
			aPin.setCapacitance(stod(line));
		}
	}
}

string Liberty::getrelatedPin(ifstream &libertyFile)
{
	string line;
	string pin = "";
	getline(libertyFile, line);
	if (exists("related_pin", line))
	{
		regex QuotationsRegex(".*\"(.*)\".*");
		smatch match;
		if (regex_search(line, match, QuotationsRegex))
			pin = match[1];
	}
	return pin;
}

void Liberty::addElements(float* A, string line)
{
	stringstream ss(line);
	float i;
	int k = 0;
	while (ss >> i)
	{
		A[k++] = i;
		if (ss.peek() == ',')
			ss.ignore();
	}

}

void Liberty::parseTable2(pin& apin, ifstream &libertyFile, float** table, int N)
{
	string line;

	for (int i = 0; i < N; i++)
	{
		getline(libertyFile, line);
		//parse each line of values
		line.erase(0, line.find("\"") + 1);
		line.erase(line.length() - 3, 3);
		addElements(table[i], line);
	}
	
	
}

void Liberty::parseTable(pin& apin, ifstream& libertyFile)
{
	string line;
	getline(libertyFile, line);
	float* rows;
	float* columns;
	rows = apin.getIndexRow();
	columns = apin.getIndexCol();
	line.erase(0, line.find("\"") + 1);
	line.erase(line.length() - 3, 3);
	addElements(rows, line);
	getline(libertyFile, line);
	line.erase(0, line.find("\"") + 1);
	line.erase(line.length() - 3, 3);
	addElements(columns, line);
	getline(libertyFile, line);			//skip this line
}

void Liberty::parseCLKTable(pin& apin, ifstream &libertyFile)
{
	string line;
	getline(libertyFile, line);
	float* rows;
	float* columns;
	rows = apin.getCLKIndexRow();
	columns = apin.getCLKIndexCol();
	line.erase(0, line.find("\"") + 1);
	line.erase(line.length() - 3, 3);
	addElements(rows, line);
	getline(libertyFile, line);
	line.erase(0, line.find("\"") + 1);
	line.erase(line.length() - 3, 3);
	addElements(columns, line);
	getline(libertyFile, line);			//skip this line

}

void Liberty::ParseTiming(pin& apin, ifstream &libertyFile, bool clk)
{
	string line;
	getline(libertyFile, line);
	int tables = 0;
	regex BracketsRegex(".*\\((.*)\\).*");
	regex templateRegex("delay_template_(.*?)x(.*?)");
	regex templateRegex2("delay_template_5x(.*)");
	regex templateRegex3("delay_template_6x(.*)");
	//regex holdRegex("hold_tempate_(.*?)x(.*?)");
	regex holdRegex("hold_template_3x(.*)");
	//regex setupRegex("setup_template_3x(.*)");
	
	smatch match;

	if (exists("hold", line) || exists("setup", line))
	{
		getline(libertyFile, line);
		
		while (!exists("hold", line) && !exists("setup", line))
			getline(libertyFile, line);
		if (exists("hold", line))
		{
			string table_template;
			if (regex_search(line, match, BracketsRegex))
				table_template = match[1];
			if (regex_search(table_template, match, holdRegex))
				apin.setClkTablesDimensions(3, stoi(match[1]));
				//cout << match[1];
			parseCLKTable(apin, libertyFile);
			float** table = apin.getHoldTime_rise();
			parseTable2(apin, libertyFile, table, 3);

			//parse holdTime fall
			while (!exists("fall", line))
				getline(libertyFile, line);
			getline(libertyFile, line);
			getline(libertyFile, line);
			getline(libertyFile, line);
			table = apin.getHoldTime_fall();
			parseTable2(apin, libertyFile, table, 3);

		}
		else if (exists("setup", line))
		{
			for (int i = 0; i < 3;i++)
				getline(libertyFile, line);
			float** table = apin.getSetupTime_rise();
			parseTable2(apin, libertyFile, table, 3);
			while (!exists("fall", line))
				getline(libertyFile, line);
			getline(libertyFile, line);
			getline(libertyFile, line);
			getline(libertyFile, line);
			//getline(libertyFile, line);
			table = apin.getSetupTime_fall();
			parseTable2(apin, libertyFile, table, 3);
		}
	}
	else
	{

		bool index = false;
		while (tables < 4)
		{
			if (exists("cell_rise", line))
			{
				if (!index)
				{
					string table_template;
					if (regex_search(line, match, BracketsRegex))
						table_template = match[1];
					if (regex_search(table_template, match, templateRegex2) || regex_search(table_template, match, templateRegex3))
					{
						//cout << match[1] << endl;
						if (match[1] == "1")
							return;
					}
					if ((regex_search(table_template, match, templateRegex)))
						apin.setTableDimension(stoi(match[1]), stoi(match[1]));
					parseTable(apin, libertyFile);
					index = true;
				}
				else
				{
					getline(libertyFile, line);
					getline(libertyFile, line);
					getline(libertyFile, line);
				}
				tables++;
				float** table = apin.getCell_rise();
				parseTable2(apin, libertyFile, table, apin.getNRows());

			}
			else if (exists("cell_fall", line))
			{
				if (!index)
				{
					string table_template;
					if (regex_search(line, match, BracketsRegex))
						table_template = match[1];
					if ((regex_search(table_template, match, templateRegex)))
						apin.setTableDimension(stoi(match[1]), stoi(match[1]));
					parseTable(apin, libertyFile);
					index = true;
				}
				else
				{
					getline(libertyFile, line);
					getline(libertyFile, line);
					getline(libertyFile, line);
				}
				tables++;
				//parseTable(apin, libertyFile);
				float** table = apin.getCell_fall();
				parseTable2(apin, libertyFile, table, apin.getNRows());
			}
			else if (exists("rise_transition", line))
			{
				if (!index)
				{
					string table_template;
					if (regex_search(line, match, BracketsRegex))
						table_template = match[1];
					if ((regex_search(table_template, match, templateRegex)))
						apin.setTableDimension(stoi(match[1]), stoi(match[1]));
					parseTable(apin, libertyFile);
					index = true;
				}
				else
				{
					getline(libertyFile, line);
					getline(libertyFile, line);
					getline(libertyFile, line);
				}
				tables++;
				//parseTable(apin, libertyFile);
				float** table = apin.getRise_transition();
				parseTable2(apin, libertyFile, table, apin.getNRows());
			}
			else if (exists("fall_transition", line))
			{
				if (!index)
				{
					string table_template;
					if (regex_search(line, match, BracketsRegex))
						table_template = match[1];
					if ((regex_search(table_template, match, templateRegex)))
						apin.setTableDimension(stoi(match[1]), stoi(match[1]));
					parseTable(apin, libertyFile);
					index = true;
				}
				else
				{
					getline(libertyFile, line);
					getline(libertyFile, line);
					getline(libertyFile, line);
				}
				tables++;
				//parseTable(apin, libertyFile);
				float** table = apin.getFall_transition();
				parseTable2(apin, libertyFile, table, apin.getNRows());
			}
			getline(libertyFile, line);
		}
	}
}

void Liberty::parse(string filename)
{
	ifstream libertyFile(filename);
	string line;

	regex BracketsRegex(".*\\((.*)\\).*");
	smatch match;
	bool end = false;
	if (libertyFile.is_open())
	{
		while (getline(libertyFile, line))
		{
			if (line.find("cell") == 0 )
			{
				string cellName = "";
				if (exists("dont_touch", line))
				{
					end = true;
					continue;
				}
				if (regex_search(line, match, BracketsRegex))
					cellName = match[1];
				//if (exists("DFF", cellName) || exists("LATCH",cellName))
					//continue;		//TODO
				cell myCell(cellName);
				Cells[cellName] = myCell;
				cell &aCell = Cells[cellName];
				string pinName = "";
				getline(libertyFile, line);
				while (line.find("/*") != 0 && !end)	//iterate over the whole cell till it ends
				{
					if (exists("dont_touch", line))
						end = true;
					if (line.find("pin(") >= 0 && line.find("pin(") < line.length())	//iterate over the pin
					{
						if (regex_search(line, match, BracketsRegex))
							pinName = match[1];
						pin &aPin = aCell.getPinReference(pinName);
						parsePin(line, aPin, libertyFile);
						//aCell.addpin(aPin);
					}
					else if (exists("timing()", line))
					{
						bool clk = false;
						if (exists("DFF", cellName) || exists("LATCH", cellName))
							clk = true;
						
						string s = getrelatedPin(libertyFile);
						pin &myPin = aCell.getPinReference(s);

						ParseTiming(myPin, libertyFile, clk);
					}
					getline(libertyFile, line);
				}
				
			}
		}
		libertyFile.close();
		addPinTypes();
	}
}

void Liberty::displayCells()
{
	ofstream out;
	out.open("ouput.txt");
	for (map<string, cell>::iterator it = Cells.begin(); it != Cells.end(); it++)
	{
		//it->second.addTypes();
		cout << it->first << endl;
		cout << '\t';
		it->second.displayPins(out);
		cout << endl;
	}
	out.close();
}


float Liberty::getCapacitance(string gatename, int pinNumber)
{
	if (Cells.count(gatename) > 0)
	{
		cell &mycell = Cells[gatename];
		return mycell.getPinCapacitance(pinNumber);
	}
	else
		cout << "Gate not found!" << endl;
}
float Liberty::getOutputTransition(string gatename, float inputTransition, float capacitance, int pinNumber)
{
	if (Cells.count(gatename)>0)
	{
		cell &mycell = Cells[gatename];
		if (gatename == "INVX1")
			pinNumber = 0;
		string pinName = mycell.getPinFromNumber(pinNumber);
		return mycell.getOutputTransition(pinName, inputTransition, capacitance);

	}
	else
		cout << "Gate not found!" << endl;

}
float Liberty::getDelay(string gatename, float inputTransition, float capacitance, int pinNumber)
{
	if (Cells.count(gatename)>0)
	{
		cell &mycell = Cells[gatename];
		if (gatename == "INVX1")
			pinNumber = 0;
		string pinName = mycell.getPinFromNumber(pinNumber);
		return mycell.getDelay(pinName, inputTransition, capacitance);

	}
	else
		cout << "Gate not found!" << endl;
}

float Liberty::getSetupTime(string gatename, float CLKtransition, float pinTransition)
{
	if (Cells.count(gatename) > 0)
	{
		cell &mycell = Cells[gatename];
		return mycell.getSetupTime(CLKtransition, pinTransition);
	}
	else
	{
		cout << "Gate not found!!" << endl;
	}
}
float Liberty::getHoldTime(string gatename, float CLKtransition, float pinTransition)
{
	if (Cells.count(gatename) > 0)
	{
		cell &mycell = Cells[gatename];
		return mycell.getHoldTime(CLKtransition, pinTransition);
	}
	else
	{
		cout << "Gate not found!!" << endl;
	}
}