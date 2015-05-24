//
//  STA.cpp
//  CSCE337-Project2
//
//  Created by Mac on 5/19/15.
//  Copyright (c) 2015 Mac. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <fstream>
#include <iomanip>
#include <regex>
#include <map>

#include "Liberty.h"
#include "cell.h"
#include "STA.h"
#include "parser.h"
#include "DAG.h"
#include "report_element.h"

using namespace std;

STA :: STA (const string& GLN, const string& Net, const string& TC, const string& Lib)
{
    DAG d(GLN, Net, TC, Lib);
    
    dag = &d;

}
void STA::findPaths(const string& GLN, const string& Net, const string& TC, const string& Lib)
{
    /*
     methodology:
     *This function retrieves the adjacency matrix from the DAG class after parsing the gate level netlist.
     In this graph, the primary i/p, gates, and primary o/p are represented as vertices (nodes).
     If one of the nodes is connected to the other, an edge is drawn between them. For example, if node i
     is connected to node j, cell[i][j] in the adjacent matrix will have a '1'. 
     *After filling the adjacency list, the matrix is copied from the DAG class to the current class. 
     *DFS is then applied to get the different paths in this DAG. Each path is saved in terms of the 
     nodes index in a vector with the name "allPaths".
     */
    
    DAG d(GLN, Net, TC, Lib);
    
    dag = &d;
    
    copyMatrix(GLN, Net, TC, Lib);

    nodesCount = d.getNodesCount();
	//d.displayNodes();

    // for each node
    for (int i = 0; i < nodesCount; i++)
    {
        // if it is an input or if it is a gate and its name contains FF (it is a FF)
		if (d.parse.nodes[i].type == 1 || (d.parse.nodes[i].type == 0 && d.parse.nodes[i].name.find("FF") >= 0 && d.parse.nodes[i].name.find("FF") < d.parse.nodes[i].name.length()))
        {
            // perform DFS to get the different paths from the i/p to an o/p or from i/p to FF or from FF to FF or from FF to o/p
            DFS(i);
            path_stack.pop();
        }
    }
}

void STA :: DFS (int start)
{
	bool IsFF;
	if (dag->parse.nodes[start].name.find("FF") >= 0 && dag->parse.nodes[start].name.find("FF") < dag->parse.nodes[start].name.length())
		IsFF = true;
	else
		IsFF = false;
	vector<bool> unvisited(nodesCount, true);	//V is the number of nodes, initialize the vector with all true
    DFSAux(start, unvisited, IsFF);
}

void STA :: DFSAux(int start, vector<bool>& unvisited, bool IsFF)
{
    cout<< start <<endl;
    
    path_stack.push(start);
    
    unvisited[start] = false;               //mark the node as visited
	if (dag->parse.nodes[start].name.find("FF") >= 0 && dag->parse.nodes[start].name.find("FF") < dag->parse.nodes[start].name.length() && !IsFF)
		return;		//enter only if node met is a FF and it is not the first node
    for(int j=0;j<nodesCount;j++)
    {
        if(adjMatrix[start][j]>0)           //if there is an edge between node start and node j
        {
			if (unvisited[j])                //if the node j is not yet visited
				DFSAux(j, unvisited, false);
            
            string signal = dag->parse.nodes[j].name;
            
            if (dag->parse.find(&(dag->parse.outputs), signal)!=-1 &&
				!dag->parse.isWire(signal) || (dag->parse.nodes[j].name.find("FF") >= 0 && dag->parse.nodes[j].name.find("FF") < dag->parse.nodes[j].name.length()))
            {
                vector<int> aux;
                stack<int> stack_aux (path_stack);
                
                while (!stack_aux.empty()) {
                    
                    int x = stack_aux.top();
                    stack_aux.pop();
                    aux.push_back(x);
                }
                
                allPaths.push_back(aux);
                aux.clear();
            }
            
            if (path_stack.top() == j)
                path_stack.pop();
        }
    }
}

void STA:: displayPaths()
{
    cout << "Display Paths:" << endl;
    for (int i = 0; i < allPaths.size(); i++)
    {
        cout << "Path " << i << ":";
        
        for (int j = 0; j < allPaths[i].size(); j++)
        {
            cout << allPaths[i][j] << " ";
        }
        
        cout << endl;
    }
}

void STA:: copyMatrix(const string& GLN, const string& net, const string& TC, const string& Lib)
{
    DAG d (GLN, net, TC, Lib);
    
    nodesCount = d.getNodesCount();

    adjMatrix.resize(nodesCount);
    for (int i = 0; i < nodesCount; i++)
        adjMatrix[i].resize(nodesCount);
    
    for (int i = 0; i < nodesCount; i++)
        for (int j = 0; j < nodesCount; j++)
            adjMatrix[i][j] = d.adjMatrix[i][j];
}

void STA:: performSTA(const string& GLN, const string& Net, const string& TC, const string& Lib)
{
    //Liberty libertyFile;
    string filename = "osu035.lib";
    libertyFile.parse(filename);

    DAG d(GLN, Net, TC, Lib);
    
    dag = &d;
    
    /*
     methodology:
     = For each path in the allPaths vector:
     1) reverse to get proper order
     2) Analyize the path
     */
    
    // reorder reversed paths
    stack<int> aux;
    
    orderedPaths.resize(allPaths.size());

    for (int i = 0; i < allPaths.size(); i++)
    {
        //orderedPaths.resize(allPaths[i].size());
        
        for (int j = 0; j < allPaths[i].size(); j++)
            aux.push(allPaths[i][j]);
        
        while(!aux.empty())
        {
            int x = aux.top();
            orderedPaths[i].push_back(x);
            aux.pop();
        }
    }
    //paths are now re-ordered
    // analyze paths
    report_element * r = new report_element;
    
    node * n;
    node * n1;
    node * n2;
    
    for (int i = 0; i < orderedPaths.size(); i++) {	//for each ordered path
        vector<int> * path = &orderedPaths[i];
		if (dag->parse.nodes[path->at(0)].name == "clk")		//discard the path if it is the clock path
			continue;
        pathDelay = 0;
        
        for (int j = 0; j < path->size(); j++)	//for each node in the path
        {
            n = &(dag->parse.nodes[path->at(j)]);	//get a reference to this node
            
            // if path element is a primary i/p
            if (n->type == 1)
            {
                // get name
                r->pin = n->name;
                
                // get type
                r->type = "INPUT";
                
                // get delay
                //r->inc = getFromTimingConst(n->name);
                r->inc = getDelay(n->name);
                n->outputTrans = r->inc;
                
                // accumulate increment to path delay
                r->path_delay = r->inc;
                pathDelay += r->path_delay;
                
                // assign rise
                r->rise_fall = 'r';
            }
            
            // if element is a gate
			if (n->type == 0 &&  (n->name.find("FF") < 0 || n->name.find("FF") >= n->name.length()))

            {
                // get name + pin under analysis
                r->pin = n->name + "/" + to_string(n->pin);
                
                // get type
                r->type = n->cell;
                
                // get next
                n2 = &(dag->parse.nodes[path->at(j+1)]);
                
                // get previous
                n1 = &(dag->parse.nodes[path->at(j-1)]);
                
                // if next is a gate
                if (n2->type == 0)
                {
                    // get the previous node's o/p transition
                    float prevTrans = n1->outputTrans;
                    
                    // get next gate's pin number
                    int nextPin = (n2->pin++)%2;
                    
                    // get the next node's i/p capacitance
                    /* FROM LIBERTY*/
                    //double nextCap = getGateCapacitance (n2->cell,nextPin);
                    float nextCap = libertyFile.getCapacitance(n2->cell, nextPin);
                    
                    // get interconnect wire capacitance
                    /* FROM TIMING CONSTRAINTS*/
                    float interConnectCap = getWireCap (n->output);
                    
                    // total capacitance
                    float totOutCap = nextCap + interConnectCap;
                    
                    // get gate delay from liberty
                    /* FROM LIBERTY*/
                    //float gateDelay = getGateDelay (n->cell, prevTrans, totOutCap);
                    float gateDelay = libertyFile.getDelay(n->cell, prevTrans, totOutCap, nextPin);
                    
                    // save in node
                    n->gateDelay = gateDelay;
                    
                    // save in report element
                    r->inc = gateDelay;
                    
                    // update path delay (previous + current)
                    pathDelay+= gateDelay;
                    r->path_delay = pathDelay;
                    
                    // get gate o/p tranistion
                    /* FROM LIBERTY*/
                    //double outTrans = getGateOutTrans(n->cell, prevTrans, totOutCap);
                    float outTrans = libertyFile.getOutputTransition(n->cell, prevTrans, totOutCap, nextPin);
                    // save in node
                    n->outputTrans = outTrans;
                    
                }
                
                // if next is a primary o/p or FF 
				if (n2->type == 2 || (n->name.find("FF") >= 0 && n->name.find("FF") < n->name.length()))
                {
                    // get the previous node's o/p transition
                    float prevTrans = n1->outputTrans;
                    
                    // get o/p capacitance
                    // float outCap = getFromTimingConst (n->output);
                    float outCap = getOutputCap(n->output);
                    
                    // get gate delay from liberty
                    /* FROM LIBERTY*/
                    //float gateDelay = getGateDelay (n->cell, prevTrans, outCap);
                    float gateDelay = libertyFile.getDelay(n->cell, prevTrans, outCap, 0);
                    
                    // save in node
                    n->gateDelay = gateDelay;
                    
                    // save in report element
                    r->inc = gateDelay;
                    
                    // update path delay (previous + current)
                    pathDelay+= gateDelay;
                    r->path_delay = pathDelay;
                    
                    // get gate o/p tranistion
                    /* FROM LIBERTY*/
                    //float outTrans = getGateOutTrans(n->cell, prevTrans, outCap);
                    float outTrans = libertyFile.getOutputTransition(n->cell, prevTrans, outCap, 0);

                    // save in node
                    n->outputTrans = outTrans;

                }
            }
            
            // if element is a primary o/p
            if (n->type == 2 && !dag->parse.isWire(n->name))
            {
                // get previous
                n1 = &(dag->parse.nodes[path->at(j-1)]);

                // get name
                r->pin = n->name;
                
                // get type
                r->type = "OUTPUT";
                
                // get delay
               // r->inc = n1->outputTrans;
				r->inc = 0;	
                
                // accumulate increment to path delay
                //r->path_delay = pathDelay;
                //pathDelay+= n1->outputTrans;
                r->path_delay = pathDelay;
                
                // assign rise
                r->rise_fall = 'f';
            }

			//if element is a FF
			if (n->name.find("FF") >=0 && n->name.find("FF") < n->name.length())
			{
				// get previous
				//n1 = &(dag->parse.nodes[path->at(j - 1)]);

				// get name
				r->pin = n->name;

				// get type
				r->type = "FF";

				// get delay
				if (j>0)
					r->inc = 0;
				else
				{
					// if next is a gate
					n2 = &(dag->parse.nodes[path->at(j + 1)]);
					if (n2->type == 0)
					{
						// get the previous node's o/p transition
						float prevTrans = 0;
						// get next gate's pin number
						int nextPin = (n2->pin++) % 2;
						float nextCap = libertyFile.getCapacitance(n2->cell, nextPin);
						float interConnectCap = getWireCap(n->output);
						float totOutCap = nextCap + interConnectCap;
						float gateDelay = libertyFile.getDelay(n->cell, prevTrans, totOutCap, nextPin);
						// save in node
						n->gateDelay = gateDelay;
						// save in report element
						r->inc = gateDelay;
						pathDelay += r->inc;
					}
					else
					{
						float gateDelay = libertyFile.getDelay(n->cell, 0, 0, 0);
						n->gateDelay = gateDelay;
						r->inc = gateDelay;
						pathDelay += gateDelay;

					}
				}

				// accumulate increment to path delay
				//r->path_delay = pathDelay;
				//pathDelay += n1->outputTrans;
				r->path_delay = pathDelay;

				// assign rise
				r->rise_fall = 'f';
			}
            
            reportElements.push_back(*r);
            r->pin = "";
            r->type = "";
            r->inc = 0;
            r->path_delay = 0;
            r->rise_fall = NULL;
        }
        
        pathDelays.push_back(pathDelay);
        
		requiredTime = getRequiredTime(path);
        requiredTimes.push_back(requiredTime);
        
        slack = requiredTime - pathDelay;
        slacks.push_back(slack);
        
        requiredTime=0; 
        pathDelay = 0;
        report.push_back(reportElements);
        reportElements.clear();
    }
    
}

void STA :: writeReport(const string& reportFile)
{
    
    ofstream out;
    
    out.open(reportFile.c_str());
    
    if (out.fail())
    {
        cout << "Failed to open - " << reportFile << endl;
        return;
    }
    else
    {
        out << endl << endl;
        
        for (int i = 0 ; i < report.size(); i++)
        {
            out <<"\t\t\t\tREPORT # " << i << endl;
            out << "------------------------------------------------------------------------------------------" << endl;
            out << left << setw(20) << "Pin\t" << "Type\t\t" << "Incr\t\t" << "Path Delay\t\t"<< endl;
            out << "------------------------------------------------------------------------------------------" << endl;
            
            report_element * r;
            
            for (int j = 0; j < report[i].size(); j++)
            {
                r = & (report[i][j]);
                
                out << left << setw(20) << r->pin << "\t" ;
                out << left << setw(10) << r->type << "\t";
                out << left << setw(10) << r->inc << "\t";
                out << left << setw(10) << r->path_delay << "\t";
                out << left << setw(10) << r->rise_fall << endl;
            }
            
            out << "------------------------------------------------------------------------------------------" << endl;

            //out << "Data Arrival Time \t\t\t\t\t"  << report[i][report[i].size()-1].path_delay << endl;
            out << "Data Arrival Time \t\t\t\t\t"  << pathDelays[i] << endl;
            out << "Data Required Time \t\t\t\t\t" << requiredTimes[i] << endl;
            
            out << "------------------------------------------------------------------------------------------" << endl;

            out << "Slack\t\t\t\t\t\t\t" << slacks[i] << endl << endl;
            
            out << "******************************************************************************************" << endl;

            out << "******************************************************************************************" << endl << endl;
        }
    }
}

/*
double STA :: getGateCapacitance (const string & gateName, const int & pinIndex)
{
    int gatesSize = dag->parse.gates.size();
    
    for (int i = 0; i < gatesSize; i++)
    {
        if (dag->parse.gates[i].name == gateName)
            return dag->parse.gates[i].cap[pinIndex];
    }
    return -1;
}
*/

double STA :: getWireCap (const string & netName)
{
    int wiresSize = dag->parse.nets.size();
    
    for (int i = 0; i < wiresSize; i++)
    {
        if (dag->parse.nets[i].name == netName)
            return dag->parse.nets[i].capacitance;
    }
    return 0.013;		//if not found, return 0.01 by default

}

/*
double STA :: getGateDelay (const string & gateType, const double & d1, const double & d2)
{
    int gatesSize = dag->parse.gates.size();
    
    for (int i = 0; i < gatesSize; i++)
    {
        if (dag->parse.gates[i].name == gateType)
            return dag->parse.gates[i].delay[rand()%2];
    }
    return -1;
}
*/

/*
double STA :: getGateOutTrans(const string & gateType, const double & d1, const double & d2)
{
    int gatesSize = dag->parse.gates.size();
    
    for (int i = 0; i < gatesSize; i++)
    {
        if (dag->parse.gates[i].name == gateType)
            return dag->parse.gates[i].trans[rand()%2];
    }
    return -1;
}
 */

/*
double STA :: getFromTimingConst (const string & outName)
{
    int outputs = dag->parse.timeConst.size();
    
    for (int i = 0; i < outputs; i++)
    {
        if (dag->parse.timeConst[i].first == outName)
            return dag->parse.timeConst[i].second;
    }
    return -1;
}
 */

float STA :: getDelay(const string & signal)
{
    int size = dag->parse.constraints.size();
    
    for (int i = 0; i < size; i++)
    {
        if (dag->parse.constraints[i].name == signal)
            return dag->parse.constraints[i].delay;
    }
    return 0;	//if not found, delay is 0 by default
}

float STA :: getOutputCap (const string & output)
{
    int size = dag->parse.constraints.size();
    
    for (int i = 0; i < size; i++)
    {
        if (dag->parse.constraints[i].name == output)
            return dag->parse.constraints[i].cap;
    }
    return 0;
    
}

float STA :: getRequiredTime(vector<int> * v)
{
    /*
    //DAG d(GLN, Net, TC, Lib);
    
    //dag = &d;

    requiredTime = getDelay(dag->parse.nodes[v[v.size()-1]].name);
    
    //requiredTime = getDelay();
    
    for (int i = v.size()-2; i>=0; i--)
    {
        requiredTime = dag->parse.nodes[v.at(i-1)].outputTrans;
    }
    
    return requiredTime;
     */
    
    /*Liberty libertyFile;
    string filename = "osu035.lib";
    libertyFile.parse(filename);*/

    node * n;
    node * n1;
    node * n2;
    
    float Tsetup = 0;
    
    //for (int i =0; i < orderedPaths.size(); i++)
    //{
        //vector<int> * path = &orderedPaths[i];
	n = &(dag->parse.nodes[v->at(v->size() - 1)]);

    //I need to calculate the required time only if the path ends with FF 
	int pos = dag->parse.nodes[v->at(v->size() - 1)].name.find("FF");
	if (pos < 0 || pos > dag->parse.nodes[v->at(v->size() - 1)].name.length())
		return getDelay(n->name);	//return output delay
		//return 0.5;		//always assume output delay is 0.5
	
	//Now I need to calculate Setup time of the capture FF
	

	float CLKtransition = 0; //assume clk transition is 0
	float pinTransition;
	float clkSkew = 0.5;

	//get pinTransition = outputTransition of previous stage
	n1 = &(dag->parse.nodes[v->at(v->size() - 2)]);		//the node before the last node , the node connected to D
	if (n1->type == 1)	//if it is an input
		pinTransition = getDelay(n->name);	//get input delay
	else
		pinTransition = n1->outputTrans;
	Tsetup = libertyFile.getSetupTime(n->cell, CLKtransition, pinTransition);
	float requiredTime = dag->parse.clkPeriod + clkSkew - Tsetup;
	return requiredTime;


}


