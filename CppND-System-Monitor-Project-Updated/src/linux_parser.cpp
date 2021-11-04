#include <dirent.h>
#include <unistd.h>
// #include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"


using std::stoi;
using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Read data about the operating system
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// Storing process ids in a vector
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{
  string line;
  string mem_name;
  float mem_total;
  float mem_free;
  
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream>>mem_name>>mem_total;
    std::getline(stream, line);
    linestream.str(line);
    linestream>>mem_name>>mem_free;
    
  }

  return (mem_total-mem_free)/mem_total;
   
   
}

// Read and return the system uptime
long LinuxParser::UpTime() 
{ 
  string line;
  long sys_uptime;
  long time_idle;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> sys_uptime >> time_idle;
  }
  
  return sys_uptime; 
  
  }

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{ 
  string cpu;
  string val;
  string line;
  vector<string> cpu_ints;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream>>val) cpu_ints.push_back(val);
    cpu_ints.erase(cpu_ints.begin());
  }
  return cpu_ints;

  
}

int LinuxParser::ProcInfo(string proc_name){
  
    string line;
    string key;
    string value;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
          if (key == proc_name) {
            return stoi(value);
          }
        }
      }
    }
  return stoi(value);

}

//  Read and return the total number of processes
int LinuxParser::TotalProcesses() {

  return ProcInfo("processes");
   
}

//  Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  
  return ProcInfo("procs_running"); 
  
}


