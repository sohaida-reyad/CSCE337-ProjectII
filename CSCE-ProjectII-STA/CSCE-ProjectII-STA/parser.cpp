//
//  parser.cpp
//  CSCE337-Project2
//
//  Created by Mac on 5/10/15.
//  Copyright (c) 2015 Mac. All rights reserved.
//

#include "parser.h"

using namespace std;

/*
parser :: parser(const string& GLN, const string& Lib, const string& TC)
{
    parseGLN(GLN);
    //parseLib(Lib);
    //parseTimeConst(TC);
}
*/

/*void parser:: parse(const string& GLN, const string& Lib, const string& TC)
{
    parseGLN(GLN);
    //parseLib(Lib);
    //parseTimeConst(TC);
}
 */

void parser:: parse(const string& GLN, const string& Net, const string& TC, const string& Lib)
{
    gateCount = 0;
    
    parseGLN(GLN);
    parseNet(Net);
    parseTimeConst(TC);
   // parseLib(Lib);
}

int parser :: getGatesCounter()
{
    //return (gates.size());
    return gateCount; 
}

int parser :: getInputsCounter()
{
    return (inputs.size());
}

int parser :: getOutputsCounter()
{
    return (outputs.size());
}

int parser :: getWiresCounter()
{
    return (Wires.size());
}

void parser :: parseGLN (const string& file)
{
    // open gate-level netlist file
    if (openFile(file))
    {
        // read gate-level netlist file
        readGLNFile();
    }
    // close gate-level netlist file
    in.close();
}

void parser:: parseNet (const string & file)
{
    string name, cap;
    double capVal;
    
    net * n = new net;
    
    if (openFile(file))
    {
        in >> name;             // name
        while (!in.eof())
        {
            in >> cap ;
            capVal = atof(cap.c_str());
            
            n->name = name;
            n->capacitance = capVal;
        
            nets.push_back(*n);
            
            in >> name;
        }
    }
    in.close(); 
}

void parser :: parseLib(const string & file)
{
    // open liberty file
    if (openFile(file))
    {
        // read liberty file
        // readFile(&lib);
        
        string s;
        gate * g = new gate;
        
        in >> g->name;
        
        
        while (!in.eof()) {
            //g->name = "test";
            
            in >> s;
            g->num_inputs = atoi(s.c_str());
            
            for (int i = 0 ; i < g->num_inputs; i++)
            {
                in >> s;
                g->cap.push_back(atof(s.c_str()));
            }
            
            for (int i = 0 ; i < 2; i++)
            {
                in >> s;
                g->delay.push_back(atof(s.c_str()));
            }

            for (int i = 0 ; i < 2; i++)
            {
                in >> s;
                g->trans.push_back(atof(s.c_str()));
            }

            gates.push_back(*g);
            
            in >> s;
            
            g->name = "";
            g->num_inputs = 0;
            g->cap.clear();
            g->delay.clear();
            g->trans.clear(); 
        }
    }
    // close liberty file
    in.close();
}

void parser :: parseTimeConst (const string & file)
{
    // open time constraints file
    if (openFile(file))
    {
        // read time constraints file
        //readFile(&timeConst);
        string count, name, delay, cap;
        constraint * c = new constraint;
        
        in >> count;
        for (int i = 0; i < atoi(count.c_str()); i++)
        {
            c->type = 0;
            in >> c->name >> c->delay;
            constraints.push_back(*c);
        }
        
        in >> count;
        for (int i = 0; i < atoi(count.c_str()); i++)
        {
            c->type = 1;
            in >> c->name >> c->cap >> c->delay ;
            constraints.push_back(*c);
        }
        
        in >> count;
        clkPeriod= atof(count.c_str()); 
    }
    // close time constraints file
    in.close();
}

bool parser :: openFile(const string& file)
{
    in.open(file);
    
    if (in.fail())
        return 0;
    else
        return 1;
}

void parser :: readGLNFile()
{
    //gate * g = new gate;
    node * n = new node;
    
    string s, s1, numInputs, gateName, gateNumber;
    
    // inputs
    in >> s ;
    inCount = atoi(s.c_str());
    
    for (int i = 0; i < inCount; i++)
    {
        in >> s >> s1;
        In.push_back(s+s1);
    }
    
    // outputs
    in >> s ;
    outCount = atoi(s.c_str());
    for (int i = 0; i < outCount; i++)
    {
        in >> s >> s1;
        Out.push_back(s+s1);
    }
    
    // inout
    in >> s ;
    inoutCount = atoi(s.c_str());
    for (int i = 0; i < inoutCount; i++)
    {
        in >> s >> s1;
        InOut.push_back(s+s1);
    }
    
    // wires
    in >> s ;
    wiresCount = atoi(s.c_str());
    for (int i = 0; i < wiresCount; i++)
    {
        in >> s;
        Wires.push_back(s);
    }
    
    // assign
    in >> s ;
    assignCount = atoi(s.c_str());
    for (int i = 0; i < assignCount; i++)
    {
        in >> s >> s1;
        pair<string, string> p (s, s1);
        Assign.push_back(p);
    }
    
    // gates
    in >> gateName;
    while (!in.eof()) {
        
        gateCount++;
        
        in >> gateNumber;
        n->type = 0;
        n->cell = gateName;
        n->instance = gateNumber;
        n->name = gateName + gateNumber;
        
        in >> numInputs;
        n->num_inputs = atoi(numInputs.c_str());
        n->pin = 0;
        
        for (int i = 0 ; i < n->num_inputs; i++)
        {
            node * n1 = new node;
            
            in >> s;
            n1->name = s;
            n1->type = 1;
            
            n->inputs.push_back(s);
            
            if (find(&inputs, s) == -1)
                inputs.push_back(s);
            
            // SHOULD BE INPUT ONLY (check that it is not a wire)
            if (!findNode(s) && !isWire(s))
                nodes.push_back(*n1);
            n1->inputs.clear();
        }
        
        in >> n->output;
        if (find(&outputs, n->output) == -1)
            outputs.push_back(n->output);
        
        // SHOULD BE OUTPUT ONLY (check that it is not a wire)
        if (!findNode(n->output) && !isWire(n->output))
        {
            node * n1 = new node;
            
            n1->name = n->output;
            n1->type = 2;
            
            nodes.push_back(*n1);

        }
        
        nodes.push_back(*n);
        
        n->inputs.clear();
        
        in >> gateName;
    }
    
    delete n;
    
    in.close();
}

void parser :: readFile(vector<pair<string, float> > * v)
{
    string name, delay, cap;
    
    in >> name;
    while (!in.eof()) {
        
        in >> delay;
        
        pair<string, float> p (name, atof(delay.c_str()));
        v->push_back(p);
        
        in >> name;
        
    }
}

int parser :: find(vector<string>* vec, const string & st)
{
    // find string st in vector vec and send position of st in vec if found and -1 otherwise
    
    for (int i=0; i< vec->size(); i++)
        if ( vec->at(i)== st)
            return i;
    
    return -1;      // not found
}
/*
float parser:: getDelay(const string & node)
{
    for (int i = 0; i < timeConst.size(); i++)
        if (timeConst[i].first == node)
            return timeConst[i].second;
    
    return -1;
}*/

bool parser:: findNode (const string & s)
{
    for (int i = 0; i < nodes.size(); i++)
        if (nodes[i].name == s)
            return 1;
    return 0;
}

bool parser:: isWire (const string & s)
{
    for (int i = 0; i < Wires.size(); i++)
        if (Wires[i] == s)
            return 1;
    return 0;
    
}