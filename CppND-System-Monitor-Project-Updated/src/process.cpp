#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"


using std::string;
using std::to_string;
using std::vector;
using std::stol;



// Return this process's ID

int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() { 

    string cpu;
    string val;
    string line;
    int count=0;
    std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid_) + LinuxParser::kStatFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        while(linestream>>val && count<22) {
            process_val.push_back(val);
            count++;
        }
        
    }

    long utime = stol(process_val[13]);
    long stime = stol(process_val[14]);
    long cutime = stol(process_val[15]);
    long cstime = stol(process_val[16]);
    long starttime = stol(process_val[21]);
    
    long total_time = utime + stime;
    total_time = total_time + cutime+ cstime;
    
    float seconds = LinuxParser::UpTime() - (starttime/sysconf(_SC_CLK_TCK));

    return (total_time/sysconf(_SC_CLK_TCK))/seconds;
    
  }

// Return the command that generated this process
string Process::Command() { 

    string line;

    std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid_)+ LinuxParser::kCmdlineFilename);

    if (stream.is_open()) {
        std::getline(stream, line);
      
    }
    
    return line.substr(0,39);
    }

// Return this process's memory utilization
string Process::Ram() {

    string line;
    string key;
    long value;
    long mem_size;

    // Get the uid 
    std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid_) + LinuxParser::kStatusFilename);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {

          // Used VmRSS instead of VmSize mentioned by Udacity. As per the manpages link, VmSize is for virtual memory size while VmRss gives 
          // exact physical memory used as part of physical RAM
          if (key == "VmRSS:") {
             mem_size = value;
             
          }
        }
      }
    }

    mem_size=mem_size/1024;


    return to_string(mem_size);
 }

// Return the user (name) that generated this process
string Process::User() { 

    string line;
    string key;
    string value;
    string uid;
    string username;
    string dummy;
    string uid_name;

    // Get the uid 
    std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid_) + LinuxParser::kStatusFilename);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
          if (key == "Uid:") {
             uid = value;
          }
        }
      }
    }
   
    // Get the username matching with the uid
    std::ifstream stream(LinuxParser::kPasswordPath);
    if (stream.is_open()) {
      while (std::getline(stream, line)) {
        std::replace(line.begin(), line.end(), '/', '_'); 
        std::replace(line.begin(), line.end(), ':', ' ');  
        std::istringstream linestream(line);
        while (linestream >> username >> dummy >> uid_name) {
          if (uid_name==uid) {
            return username;
          }
        }
      }
    }
    return username;
}

// Return the age of this process (in seconds)
long int Process::UpTime() { 
  
    return LinuxParser::UpTime() - stol(process_val[21])/sysconf(_SC_CLK_TCK);

 }

// Overload the "less than" comparison operator for Process objects used in sorting the processes
bool Process::operator<(Process& a)  { 

    return a.CpuUtilization() < CpuUtilization();
  
  }

