#include "processor.h"
#include <vector>
#include <string>
#include "linux_parser.h"
// #include <iostream>

using std::string;
using std::stol;


void Processor::updateCurrentValues(){

    std::vector<string> cpu_val=LinuxParser::CpuUtilization();

    user_ = stol(cpu_val[0]);
    nice_ = stol(cpu_val[1]);
    system_ = stol(cpu_val[2]);
    idle_ = stol(cpu_val[3]);
    iowait_ = stol(cpu_val[4]);
    irq_ = stol(cpu_val[5]);
    softirq_ = stol(cpu_val[6]);
    steal_ = stol(cpu_val[7]);
    guest_ = stol(cpu_val[8]);
    guest_nice_ = stol(cpu_val[9]);

}

void Processor::updatePrevValues(){
    
    prevuser_ = user_;
    prevnice_ = nice_;
    prevsystem_ = system_;
    previdle_ = idle_;
    previowait_ = iowait_;
    previrq_ = irq_;
    prevsoftirq_ = softirq_;
    prevsteal_ = steal_;
    prevguest_ = guest_;
    prevguest_nice_ = guest_nice_;

}


long Processor::PrevIdle(){

    return previdle_ + previowait_;

}


long Processor::Idle(){

    return idle_ + iowait_;

}

long Processor::PrevNonIdle(){

    return prevuser_ + prevnice_ + prevsystem_ + previrq_ + prevsoftirq_ + prevsteal_;

}

long Processor::NonIdle(){

    return user_ + nice_ + system_ + irq_ + softirq_ + steal_;
}

long Processor::PrevTotal(){

    return PrevIdle() + PrevNonIdle();
}

long Processor::Total(){

    return Idle() + NonIdle();
}


float Processor::Utilization() {

    //Get current CPU values
    updateCurrentValues();

    totald = Total() - PrevTotal();
    idled = Idle() - PrevIdle();

    // Update the previous values
    updatePrevValues();

    // std::cout<<(totald-idled)/totald<<std::endl;

    return (totald-idled)/totald;

 }