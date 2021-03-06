/************************************************************/
/*    NAME: Ryan Oshea                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactor.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef PrimeFactor_HEADER
#define PrimeFactor_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include <list>
#include <string>
#include "PrimeEntry.h"
//#include <vector>
using namespace std;

class PrimeFactor : public CMOOSApp
{
 public:
   PrimeFactor();
   ~PrimeFactor();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();
   string m_input_number;
   list<string> m_input_number_str_list;
   list<PrimeEntry> m_prime_entries;
   int m_received_index_pp;
   unsigned int m_calc_index;
 private: // Configuration variables

 private: // State variables
};

#endif 
