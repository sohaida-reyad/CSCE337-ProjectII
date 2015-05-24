//
//  report_element.h
//  CSCE337-Project2
//
//  Created by Mac on 5/20/15.
//  Copyright (c) 2015 Mac. All rights reserved.
//

#ifndef report_element_h
#define report_element_h

#include <string>
#include <iostream>

using namespace std;

class report_element
{
public:
    string pin;
    string type;
    double inc;
    double path_delay;
    char rise_fall;
};
#endif
