#include "PrimeEntry.h"
#include <list>
#include <string>
#include <cstdint>
#include <vector>
#include <iostream>
#include <math.h>

using namespace std;

//---------------------------------------------------------                                                                                                                        
// Constructor                                                                                                                                                                     

PrimeEntry::PrimeEntry()
{
  uint64_t      m_start_index = 2;
  uint64_t      m_orig;
  bool          m_done;
  unsigned int  m_received_index;
  unsigned int  m_calculated_index;
  std::vector<uint64_t> m_factors;
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
  m_start_index = 2;
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
  for(int p=0;p!=max_steps;p++)
 { 
   //if the current index is less than the square root of the current value then we set done to be true and stop the search set start index to 0          
   cout << "1" <<endl;
   cout << "SI: " << m_start_index << " sqrtOG: " << sqrt(m_orig) <<endl;
   if(m_start_index < sqrt(m_orig)) 
     {
       cout << "2" <<endl;
       if((m_current_value % m_start_index) != 0)
	 {
	   cout << "3" <<endl;
	   m_start_index = m_start_index+1;
	 }
       else //if the number evenly divides into it
	 {
	   cout << "4" <<endl;
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
       cout << "5" <<endl;
       done_state =true;
       //       setDone(true);
       break;
     }
 }

  return(done_state);
}


//generate report
string  PrimeEntry::getReport()
{
  cout << "list of primes: " << endl;

  //divide m_orig by the list of factors, to get current state                                                                                     
   for(int p=0;p!=m_factors.size();p++) {
   cout << "," << m_factors[p] << endl;
  }


  //for now just iterate through the current primes
  return("report");
}
