//
//  constraint.h
//  CSCE337-Project2
//
//  Created by Mac on 5/20/15.
//  Copyright (c) 2015 Mac. All rights reserved.
//

#ifndef constraint_h
#define constraint_h

#include <string>

class constraint
{
public:
    bool type;      // 0=input; 1=output
    string name;
    float cap;
    float delay; 
};

#endif
