//
//  STA.h
//  CSCE337-Project2
//
//  Created by Mac on 5/19/15.
//  Copyright (c) 2015 Mac. All rights reserved.
//

#ifndef STA_H
#define STA_H

#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <queue>

#include "parser.h"
#include "DAG.h"
#include "report_element.h"

using namespace std;

class STA
{
public:
    STA(const string&, const string&, const string&, const string&);
    
    void findPaths(const string&, const string&, const string&, const string&);
    void displayPaths();
    void DFS(int);
    void DFSAux(int, vector<bool>&, bool);
    void copyMatrix(const string&, const string&, const string&, const string&);
    void performSTA(const string&, const string&, const string&, const string&);

    void writeReport(const string&);
    
    vector<vector<int> > allPaths;
    vector<vector<int> > orderedPaths;
	Liberty libertyFile;
    
private:
    
    //double getGateCapacitance (const string &, const int &);
    double getWireCap (const string &);
    //double getGateDelay (const string &, const double &, const double &);
    //double getGateOutTrans(const string &, const double &, const double &);
    //double getFromTimingConst (const string &);
    float getDelay(const string &);
    float getOutputCap (const string &);
    
    float getRequiredTime(vector<int>*);
    
    int nodesCount;
    
    stack<int> path_stack;
    
    DAG * dag;
    
    vector <vector<double> > adjMatrix;
    
    vector<report_element> reportElements;
    vector<vector<report_element> > report;
    
    float pathDelay;
    float requiredTime;
    float slack;
    
    vector<float> pathDelays;
    vector<float> requiredTimes; 
    vector<float> slacks;
    

};

#endif
