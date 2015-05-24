//
//  node.h
//  CSCE337-Project2
//
//  Created by Mac on 5/9/15.
//  Copyright (c) 2015 Mac. All rights reserved.
//

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>

class node
{
public:
    int type;               // 0= gate; 1= i/p; 2= o/p; 3=wire
    string cell;            // and/nand/nor/...
    string instance;        // cell number
    string name;            // ex. inv2_1_ (cell+instance)
    
    bool flag;
    //float delay;
    int level;
    
    int num_inputs;
    
    string output;
    vector<string> inputs;
    //vector<string> outputs;
    
    double gateDelay;
    double outputTrans;
    
    int pin; 
    
};

#endif


/*
 types:
 - 0 = gate
 - 1 = input
 - 2 = output
 - 3 = wire
 */