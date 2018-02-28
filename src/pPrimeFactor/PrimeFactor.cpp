/************************************************************/
/*    NAME: Ryan Oshea                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactor.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "PrimeFactor.h"
#include <list>
#include <string>
#include <cstdint>
//#include "PrimeEntry.h"

using namespace std;

//---------------------------------------------------------
// Constructor

PrimeFactor::PrimeFactor()
{
  //input value
  string m_input_number = "empty";
  list<string> m_input_number_str_list;
  list<PrimeEntry *> m_prime_entries;
  int m_received_index_pp = 1;
}

//---------------------------------------------------------
// Destructor

PrimeFactor::~PrimeFactor()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PrimeFactor::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString();
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();


    //  Notify("NUM_RESULT", "ODD");

    //if the message is from the NAV_X file then we iterate x current                                                                                                                
    if (key == "NUM_VALUE")
      {
	//	Notify("NUM_RESULT", "ODD");
	
	m_input_number = sval;
	m_input_number_str_list.push_back((m_input_number));
      }
    
    //if num_value, then we assign the input to a class and put it in a list
    if (key == "NUM_VALUE")
      {
	//convert string to unsigned long
	m_input_number = sval;
	unsigned long int input_prime_value;
	input_prime_value = strtoul(sval.c_str(),NULL,0);

	//define a pointer to class time prime, to make a list of multiple entries of same class
	PrimeEntry * optimus;
		Notify("NUM_OUT", input_prime_value);
	//initialize this class entry
	optimus->setOriginalVal(input_prime_value);
	optimus->setReceivedIndex(m_received_index_pp);
	
	 m_prime_entries.push_back(optimus);
	m_received_index_pp = m_received_index_pp +1;
      }
    
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PrimeFactor::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PrimeFactor::Iterate()
{
  //iterate through the list of PrimeEntries, doing some amount of work on each one
  // if a prime entry finishes, remove it from the list and report its output using the getreport feature of the class
  




  //iterate through and assign value to NUM_Result
  list<string>::iterator p;
  for(p=m_input_number_str_list.begin(); p!=m_input_number_str_list.end(); ) {
    string str = *p;
    string full_result = str.c_str();
    
    //   stringstream ss_str(str);

    uint64_t f_int_input_number = 0;
    f_int_input_number = strtoul(str.c_str(),NULL,0);;
    uint64_t zero = 0;
    uint64_t two = 2;
    //even / odd assignment using modulo                                                                                               
    if (f_int_input_number % two == zero)                                                                                                     
    {                                                                                                                             
    //EVEN                                                                                                                         
      full_result = full_result+ ",EVEN";
      Notify("NUM_RESULT", full_result);                                                                                          
     }                                                                                                                           
     else                                                                                                                           
    {                                                                                                                              
    //ODD                          
     full_result = full_result+ ",ODD";                                                                                                 
     Notify("NUM_RESULT", full_result);                                                                                                 
    }                                                                                                                            

    p = m_input_number_str_list.erase(p);
  }
  

  // Notify("NUM_RESULT", "ODD");



  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PrimeFactor::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string original_line = *p;
      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);
      
      if(param == "FOO") {
        //handled
      }
      else if(param == "BAR") {
        //handled
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void PrimeFactor::RegisterVariables()
{
  // Register for NUM_Value
  Register("NUM_VALUE", 0);
  
}
