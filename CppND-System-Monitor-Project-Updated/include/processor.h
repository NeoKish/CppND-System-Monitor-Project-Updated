#ifndef PROCESSOR_H
#define PROCESSOR_H



class Processor {

 public:

// Member function returning aggregate CPU utilization
  float Utilization();  

// Member functions for updating the current and previous CPU values
  void updateCurrentValues();  
  void updatePrevValues();  

// Member functions for calculating the CPU utilization 
  long PrevIdle();
  long Idle();
  long PrevNonIdle();
  long NonIdle();
  long PrevTotal();
  long Total();


 private:

//  member variables for storing current CPU values
    long user_;
    long nice_;
    long system_;
    long idle_;
    long iowait_;
    long irq_;
    long softirq_;
    long steal_;
    long guest_;
    long guest_nice_;
   
//  member variables for storing previous CPU values  
    long prevuser_ = 0;
    long prevnice_ = 0;
    long prevsystem_ = 0;
    long previdle_ = 0;
    long previowait_ = 0;
    long previrq_ = 0;
    long prevsoftirq_ = 0;
    long prevsteal_ = 0;
    long prevguest_ = 0;
    long prevguest_nice_ = 0;

    float totald;
    float idled;
    

};

#endif