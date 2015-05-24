//
//  parser.h
//  CSCE337-Project2
//
//  Created by Mac on 5/10/15.
//  Copyright (c) 2015 Mac. All rights reserved.
//

#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <fstream>

#include "gate.h"
#include "node.h"
#include "net.h"
#include "constraint.h"

class parser
{
public:
    
    // public functions
    //parser(const string&, const string&, const string&);
    //void parse (const string&, const string&, const string&);
    void parse (const string&, const string&, const string&, const string&);
    int getGatesCounter();
    int getInputsCounter();
    int getOutputsCounter();
    int getWiresCounter();
    
    float getDelay (const string &);
    
    int find(vector<string>*, const string &);
    bool findNode (const string &);
    bool isWire (const string &); 
    
    //int findTimeConst(string)

    // public data structures
    vector<gate> gates;
    vector<node> nodes;
    vector<net> nets;
    
    vector<pair<string, float> > lib;
    //vector<pair<string, float> > timeConst;
    
    
    vector<string> In;
    vector<string> Out;
    vector<string> InOut;
    vector<string> Wires;
    vector<pair<string, string> > Assign;
    
    vector<string> outputs;
    vector<string> inputs;
    //vector<string> wires;
    
    vector<constraint> constraints; 

    float clkPeriod; 
    
    //
    
    //vector<vector<int> > adjMatrix;
    
private:
    
    // private functions
    
    void parseGLN (const string&);
    void parseLib (const string&);
    void parseTimeConst(const string &);
    void parseNet (const string &);
    
    bool openFile(const string&);

    void readGLNFile();
    void readFile(vector<pair<string, float> > *);
    
    
    // private data structures
    ifstream in;
    
    // used in parseGLN
    int gateCount; 
    int inCount;
    int outCount;
    int inoutCount;
    int wiresCount;
    int assignCount;
    
};

#endif
