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

using namespace std;

//---------------------------------------------------------
// Constructor

PrimeFactor::PrimeFactor()
{
  //input value
  //  string m_input_number = 0;
  //list<string> m_input_number_str_list;
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


    Notify("NUM_RESULT", "ODD");

    //if the message is from the NAV_X file then we iterate x current                                                                                                                
    /*if (key == "NUM_VALUE")
      {
	//	Notify("NUM_RESULT", "ODD");

	m_input_number = sval;
	m_input_number_str_list.push_back((m_input_number));
      }
    */

    //    Notify("NUM_RESULT", "ODD");    

 //integer values for below modulo
    // int zero = 0;
    // int two = 0;

    //even / odd assignment using modulo
    //if (m_input_number % 2 == 0)
       //{
	//EVEN
       //	Notify("NUM_RESULT", "EVEN");
	// }
     // else
      //{
       //ODD
       // Notify("NUM_RESULT", "ODD");
       //}

    //Notify when some mail is received
     // Notify("NUM_RESULT",m_input_number);


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
  //    Notify("NUM_RESULT",102);

  Notify("NUM_RESULT", "ODD"); 
  /*
  //iterate through and assign value to NUM_Result
  list<string>::iterator p;
  for(p=m_input_number_str_list.begin(); p!=m_input_number_str_list.end(); ) {
    string str = *p;
    //   stringstream ss_str(str);

    uint64_t f_int_input_number = 0;
    f_int_input_number = strtoul(str.c_str(),NULL,0);;
    uint64_t zero = 0;
    uint64_t two = 2;
    //even / odd assignment using modulo                                                                                               
    if (f_int_input_number % two == zero)                                                                                                     
    {                                                                                                                             
    //EVEN                                                                                                                         
     Notify("NUM_RESULT", "EVEN");                                                                                          
     }                                                                                                                           
     else                                                                                                                           
    {                                                                                                                              
    //ODD                                                                                                                           
     Notify("NUM_RESULT", "ODD");                                                                                                 
    }                                                                                                                            

    p = m_input_number_str_list.erase(p);
  }
  */

  Notify("NUM_RESULT", "ODD");



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
