//
//  DAG.h
//  CSCE337-Project2
//
//  Created by Mac on 5/10/15.
//  Copyright (c) 2015 Mac. All rights reserved.
//

#ifndef DAG_H
#define DAG_H

#include <iostream>
#include "parser.h"

class DAG
{
public:
    //DAG (const string&, const string&, const string&);
    DAG (const string&, const string&, const string&, const string&);

    void files (const string&, const string&, const string&, const string&);
    
    parser parse;

    vector <vector<double> > adjMatrix;
    unsigned int getNumberGates();
    unsigned int getNodesCount();
 
    void displayNodes (); 

    double getDelay (const string &, const double &, const double &);

private:
    
    //vector <node> nodes;
    
    unsigned int numberGates;
    unsigned int numberInputs;
    unsigned int numberOutputs;
    unsigned int numberWires; 
    unsigned int nodesCount;
    
    int findGate (const string &);
    double getNetCap (const string &);
    double getInputDelay (const string &); 
    double getOutputCapacitance (const string &);
    bool isPrimaryInput(const string &); 
    bool isPrimaryOutput (const string &);
    double getCapacitance (const string&);
    double getDelay(const string &, double, double, int);
    double getTransition(const string &, double, double, int);

    
    
    vector<double> delays;
    vector<double> transition;

    double max (vector<double>& );
    
    
};

#endif