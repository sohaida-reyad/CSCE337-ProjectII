//
//  DAG.cpp
//  CSCE337-Project2
//
//  Created by Mac on 5/10/15.
//  Copyright (c) 2015 Mac. All rights reserved.
//

#include <iostream>
#include <string>

#include "DAG.h"
#include "node.h"

//DAG:: DAG(const string& GLN, const string& Lib, const string& TC)
DAG:: DAG(const string& GLN, const string& net, const string& TC, const string& Lib)
{
    parse.parse(GLN, net, TC, Lib);
    
    numberGates = parse.getGatesCounter();
    numberInputs = parse.getInputsCounter();
    numberOutputs = parse.getOutputsCounter();
    numberWires = parse.getWiresCounter();
    
    //nodesCount = numberGates + numberInputs + numberOutputs;
    //nodesCount = numberGates + numberInputs + numberOutputs;
    
    nodesCount = parse.nodes.size();
    
    //gate * g, * g1;
    node * n;
    
    //unsigned int numberGates;
    
    
    //if (netlist.readFile(VerilogFileName))
    //{
    
    /*
     vector<string> nodes;
     nodes.resize(nodesCount);
     for (int i = 0; i < numberInputs; i++)
     {
     nodes.push_back(parse.inputs[i]);
     }
     
     for (int i = 0; i < numberOutputs; i++)
     {
     nodes.push_back(parse.outputs[i]);
     }
     */
    
    adjMatrix.resize(nodesCount);
    for (int i = 0; i < nodesCount; i++)
        adjMatrix[i].resize(nodesCount);
    /*
     for (int i = 0; i < nodesCount; i++)
     {
     //string nodeName =
     g = &(parse.gates[i]);
     
     for (int j = 0; j < g->inputs.size(); j++)          // for each input
     {
     // check if the each input of the gate is an output of another gate
     int outin = parse.find(&(parse.outputs),g->inputs[j]);
     
     for (int k = 0; k < numberGates; k++)   // for each output
     {
     g1 = &(parse.gates[k]);
     if (outin!= -1)
     if (g1->output == parse.outputs[outin])
     adjMatrix[i][k] = 1;
     }
     }
     
     // check if gate output is input to other gates
     }
     
     */
    for (int i = 0; i < nodesCount; i++)	//for each node
    {
        // g = &(parse.gates[i]);
        n = &(parse.nodes[i]);		// n is a reference to a node
        
        // if node is an input find the gates/nodes connected to it
        if (n->type == 1)
        {
            n->pin = 0;
            
            for (int j = 0; j < nodesCount; j++)
            {
                if (parse.nodes[j].type == 0)   // if node j is a gate
                {
                    int in = parse.find(&(parse.nodes[j].inputs),n->name);// search for n in j's inputs
                    if (in!=-1)
                        adjMatrix[i][j] = 1;
                }
            }
            
        }
        // if the node is a gate check if it is connected to other gates
        else if (n->type == 0)
        {
            n->pin = 0;

            // get gate output
            string s = n->output;

            // for each gate connected to the current gate
            for (int j = 0; j < nodesCount; j++)
            {
                // for each input of the other gate
                for (int k = 0; k < parse.nodes[j].inputs.size(); k++)
                {
                    // if the other gate's input is the first gate's output
                    // fill corresponding edge cell with 1
                    if (parse.nodes[j].inputs[k] == s)
                        adjMatrix[i][j] = 1;
                }
                
                // if the gate's output is a primary output
                // fill corresponding edge cell with 1
                if (s == parse.nodes[j].name && i!=j)
                    adjMatrix[i][j] = 1;
            }
        }
        
        // if node is an output find if it is output of some gate
        else if (n->type == 2)
        {
            n->pin = 0;

            for (int j = 0; j < nodesCount; j++)
            {
                if (parse.nodes[j].type == 0)                   // if it is a gate
                    if(parse.nodes[j].output == n->name)        // if the gate's output is n
                        adjMatrix[i][j] = 1;                    // fill corresponding edge cell with 1
            }
        }
    }
    
    
    // int k = 0;
    
    /*
     adjMatrix.resize(nodesCount);
     for (int i = 0; i < nodesCount; i++)
     adjMatrix[i].resize(nodesCount);
     
     // insert inputs to vector of nodes
     for (int i = 0; i < numberInputs; i++)
     {
     //for (int j = 0; j < parse.gates[i].inputs.size(); j++)
     //{
     node * n = new node;
     string element = parse.inputs[i];
     
     //if (parse.find(&(parse.inputs), element) != -1)
     //{
     n->name = element;
     n->type = "input";
     
     float delay = parse.getDelay(element);
     if (delay !=-1)
     n->delay = delay;
     
     else
     {
     cerr << "Cannot find delay for input " << element << ".\n";
     return;
     }
     
     n->flag = 1;
     
     nodes.push_back(*n);
     //}
     //}
     }
     
     
     // insert gates to vector of nodes
     for (int i = 0; i < numberGates; i++)
     {
     //for (int j = 0; j < parse.gates[i].inputs.size(); j++)
     //{
     node * n = new node;
     string element = parse.gates[i].name;
     
     //if (parse.find(&(parse.inputs), element) != -1)
     //{
     n->name = element;
     n->type = "gate";
     
     float delay = parse.getDelay(element); /// check this again
     if (delay !=-1)
     n->delay = delay;
     
     else
     {
     cerr << "Cannot find delay for input " << element << ".\n";
     return;
     }
     
     n->flag = 0;
     
     nodes.push_back(*n);
     //}
     //}
     }
     
     // insert wires to vector of nodes
     for (int i = 0; i < numberWires; i++)
     {
     //for (int j = 0; j < parse.gates[i].inputs.size(); j++)
     //{
     node * n = new node;
     string element = parse.Wires[i];
     
     //if (parse.find(&(parse.inputs), element) != -1)
     //{
     n->name = element;
     n->type = "wire";
     
     float delay = parse.getDelay(element);
     if (delay !=-1)
     n->delay = delay;
     
     else
     {
     cerr << "Cannot find delay for input " << element << ".\n";
     return;
     }
     
     n->flag = 0;
     
     nodes.push_back(*n);
     //}
     //}
     }
     
     // insert outputs to vector of nodes
     for (int i = 0; i < numberGates; i++)
     {
     //for (int j = 0; j < parse.gates[i].output.size(); j++)
     //{
     node * n = new node;
     string element = parse.gates[i].output;
     
     //if (parse.find(&(parse.inputs), element) != -1)
     //{
     n->name = element;
     n->type = "output";
     
     // NOT SURE OF DELAY AND FLAG
     float delay = parse.getDelay(element);
     if (delay !=-1)
     n->delay = delay;
     
     else
     {
     cerr << "Cannot find delay for input " << element << ".\n";
     return;
     }
     
     n->flag = 1;
     
     nodes.push_back(*n);
     //}
     //}
     }
     
     // fill matrix
     for (int i = 0; i < nodes.size(); i++)
     {
     //gate * g, * g1;
     node * n;
     
     //g = &(parse.gates[i]);
     n = &(nodes[i]);
     int delay = n->delay;
     adjMatrix[i][i] = delay;
     
     if (n->type == "input" || n->type == "wire")
     {
     // find all gates connected to it
     // check if each input of the gate is an output of another gate
     int outin = parse.find(&(parse.outputs),g->inputs[j]);
     
     
     for (int j = 0; j < nodes.size(); j++)
     {
     if (nodes[j].type == "gate")
     {
     
     }
     
     }
     
     
     }
     else if (n->type == "gate")
     {
     }
     else if (n->type == "output")
     {
     }
     
     for (int j = 0; j < g->inputs.size(); j++)          // for each input
     {
     // check if each input of the gate is an output of another gate
     int outin = parse.find(&(parse.outputs),g->inputs[j]);
     
     for (int k = 0; k < numberGates; k++)   // for each output
     {
     g1 = &(parse.gates[k]);
     if (outin!= -1)
     if (g1->output == parse.outputs[outin])
     {
     // adjMatrix[i][k] = 1;
     
     // find if there is an interconnect delay
     // if there is, then edge in graph = delay
     // else edge = 0
     
     
     
     }
     
     }
     }
     
     // check if gate output is input to other gates
     }
     */
    
}




unsigned int DAG :: getNumberGates()
{
    return numberGates;
}

unsigned int DAG :: getNodesCount()
{
    return nodesCount;
}

void DAG:: displayNodes ()
{
    cout << "Display Nodes: " << endl;
    
    for (int i = 0; i < nodesCount; i++)
    {
        cout << parse.nodes[i].name << endl;
        
    }
    
}

double DAG:: getDelay(const string & cellType,
                      const double & inputTransition,
                      const double & outputCapacitance)
{
    /*
     input: input transition time (output transition time for input) +
     output capactive load (capacitive load of wire) +
     type of gate (cell type)
     
     
     output:delay retrieved from liberty file
     */
    
    // return function (cellType, inputTransition, outputCapacitance);
    
    return 1;
    
}





