//
//  gate.h
//  CSCE337-Project2
//
//  Created by Mac on 5/9/15.
//  Copyright (c) 2015 Mac. All rights reserved.
//

#ifndef gate_h
#define gate_h

#include <string>
#include <vector>

using namespace std;

class gate
{
public:
    /*
    string type; 
    string name;
    int num_inputs;
    vector <string> inputs;
    vector<double> inputTrans;
    string output;
    int level;
    double delay;
    bool flag; 
     */
    
    string name;
    int num_inputs;
    //double cap[2];
    vector<double> cap;
    
    //double delay [2];
    vector<double> delay;
    
    //double transition [2];
    vector<double> trans;
};

#endif
