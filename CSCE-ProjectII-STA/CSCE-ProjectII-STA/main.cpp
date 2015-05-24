//
//  main.cpp
//  CSCE337-Project2
//
//  Created by Mac on 5/5/15.
//  Copyright (c) 2015 Mac. All rights reserved.
//

#include <iostream>
#include <string>
#include <regex>
#include <map>

#include "Liberty.h"
#include "DAG.h"
#include "parser.h"

#include "STA.h"
#include "gate.h"
#include "cell.h"

int main(int argc, const char * argv[]) {
	int f ;
	cin >> f;
    
    //string GLN = "parsed_booth.g.v.txt";
	string GLN = "gateLevelNetlists//gatelevelnetlist"+to_string(f) + ".txt";
    //string GLN = "//Users//mac//Desktop//testfiles//parsed_2b_adder.g.v.txt";
    string Lib = "osu035.lib";
	string TC = "TimingConstraints//timing_constraints"+to_string(f)+".txt"; 
	string Net = "NetCapacitances//net_cap" + to_string(f) + ".txt";
	string report = "Reports//report" + to_string(f) + ".txt";
    
    /*
    cout << "Run " << endl;
   
    
    DAG d(GLN, Net, TC, Lib);
    
    d.displayNodes();
    
    cout << "Display Matrix: " << endl;
    
    unsigned int size = d.getNodesCount();
    
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
            cout << d.adjMatrix[i][j];
        cout << endl;
    }
    */
    
    STA sta(GLN, Net, TC, Lib);
    
    sta.findPaths(GLN, Net, TC, Lib);
    sta.displayPaths();
    sta.performSTA(GLN, Net, TC, Lib);
    sta.writeReport(report);
    
    return 0;
}
