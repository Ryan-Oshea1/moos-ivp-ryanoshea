#include "PrimeEntry.h"
#include <list>
#include <string>
#include <cstdint>
#include <vector>
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <time.h>

using namespace std;

//---------------------------------------------------------                                                                                                                        
// Constructor                                                                                                                                                                     

PrimeEntry::PrimeEntry()
{
  m_start_index = 2;
  /*uint64_t      m_orig;
  bool          m_done;
  unsigned int  m_received_index;
  unsigned int  m_calculated_index;
  std::vector<uint64_t> m_factors;
  time_t m_start_time;
  time_t m_stop_time; */
}

//---------------------------------------------------------                                                                                                                        
// Destructor                                                                                                                                                                      

PrimeEntry::~PrimeEntry()
{
}


//sets original prime number
void PrimeEntry::setOriginalVal(unsigned long int v)
{
  m_orig = v;
  // m_start_index = 2;
  //  m_start_time = time (NULL);
  m_start_time = MOOSTime();
}

//factor for some amount of steps
bool   PrimeEntry::factor(unsigned long int max_steps)
{
  //set a place holder variable for m_orig
  uint64_t m_current_value = m_orig;
    
    //bool for wether or not the factoring is done 
    bool done_state = false;
  //divide m_orig by the list of factors, to get current state
    for(int p=0;p!=m_factors.size();p++) {
      m_current_value = m_current_value/m_factors[p];
    }

  //start at the start index

  //if the current value is divisible, then add it to the list of primes (m_factors), else continue until max steps is reached, if a prime is found, set m_start_index to 1
  for(int p=0;p<=max_steps;p++)
 { 
   //if the current index is less than the square root of the current value then we set done to be true and stop the search set start index to 0          
   //cout << "1" <<endl;
   //cout << "SI: " << m_start_index << " sqrtOG: " << sqrt(m_orig) <<endl;
   if(m_start_index <= sqrt(m_orig)) 
     {
       //  cout << "2" <<endl;
       if((m_current_value % m_start_index) != 0)
	 {
	   //  cout << "3" <<endl;
	   m_start_index = m_start_index+1;
	 }
       else //if the number evenly divides into it
	 {
	   //cout << "4" <<endl;
	   //update the current value
	   m_current_value = m_current_value/m_start_index;

	   //add the current start index to the list of factors
	   m_factors.push_back(m_start_index);

	   //reset start index to start looking at 2 again
	   m_start_index = 2;
	 }
     }
   else //if greater than the sqrt of the current value, then we set done to true and break out of the for loop
     {
       //cout << "5" <<endl;
       done_state =true;
       if(m_current_value != 1)
	 {
	   m_factors.push_back(m_current_value);
	 }
       m_stop_time = MOOSTime();
       //       m_stop_time = time (NULL);
       //       setDone(true);
       break;
     }
 }

  return(done_state);
}


//generate report
string  PrimeEntry::getReport()
{
  //generate result for original number
  string result = "orig=";
  stringstream ss;
  ss << m_orig;
  string s_orig = ss.str();
  result = (result + s_orig+",");
  
  //now update for received numbers
  stringstream ssr;
  ssr << m_received_index;
  string s_received_index = ssr.str();
  result = result + "received=" + s_received_index + ",";

  //now update for calculated number 
  stringstream ssc;  
  ssc << m_calculated_index;
  string s_calculated_index = ssc.str();
  result = result + "calculated=" + s_calculated_index + ",";

  //now update for solve time
  time_t solve_time = m_stop_time - m_start_time; 
  stringstream sst;
  sst << solve_time;
  string s_time = sst.str();
  result = result + "solve_time=" + s_time + ",";
  

  //now update for primes
  result = result + "primes=";
  for(int p=0;p!=m_factors.size();p++) {
    // cout << "," << m_factors[p] << endl;
  stringstream ssp;
  ssp << m_factors[p];
  string s_prime = ssp.str();
  result = result + s_prime + ":";
  }

  // now update for username
  result = result + ",username=ryanoshea";

  cout << "result: " << result << endl; 
 
  //for now just iterate through the current primes
  return(result);
}
