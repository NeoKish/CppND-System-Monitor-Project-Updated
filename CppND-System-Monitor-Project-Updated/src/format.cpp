#include <string> 
#include <iostream>
#include <sstream>
#include <iomanip>
#include "format.h"

using std::string;
using std::to_string;


// Getting time in format HH:MM:SS
string Format::ElapsedTime(long time) { 

    int hh;
    int mm;
    int ss;
    std::ostringstream stream;
   
    hh = time/3600;
    time = time%3600;
    mm = time/60;  
    time = time%60;
    ss = time;
    
    // Using setw and setfill for proper formatting
    stream<<std::setw(2)<<std::setfill('0')<<hh<<":"
    <<std::setw(2)<<std::setfill('0')<<mm<<":"
    <<std::setw(2)<<std::setfill('0')<<ss;

    return stream.str();
    
    }