/************************************************************/
/*    NAME: Ryan O'Shea                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_ZigLeg.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cstdlib>
#include "MBUtils.h"
#include "BuildUtils.h"
#include "BHV_ZigLeg.h"
#include "ZAIC_PEAK.h"

using namespace std;

//---------------------------------------------------------------
// Constructor

BHV_ZigLeg::BHV_ZigLeg(IvPDomain domain) :
  IvPBehavior(domain)
{
  // Provide a default behavior name
  IvPBehavior::setParam("name", "defaultname");

  // Declare the behavior decision space
  m_domain = subDomain(m_domain, "course,speed");

  //initialize the two indices
  m_current_wpt_index = 0;
  m_past_wpt_index = 0;
  m_current_angle = 0;
  m_current_angle_plus_45 = 0;
  m_pulsed_bool = false;
  // Add any variables this behavior needs to subscribe for
  addInfoVars("NAV_X, NAV_Y, NAV_HEADING");
  addInfoVars("WPT_INDEX", "no_warning");
}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_ZigLeg::setParam(string param, string val)
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  // Get the numerical value of the param argument for convenience once
  double double_val = atof(val.c_str());
  
  if((param == "zig_angle") && isNumber(val)) {
    m_zig_angle = double_val;
   // Set local member variables here
    return(true);
  }
  else if ((param == "zig_duration") && isNumber(val)) {
    m_zig_duration = double_val;
    // return(setBooleanOnString(m_my_bool, val));
    return(true);
  }

  // If not handled above, then just return false;
  return(false);
}

//---------------------------------------------------------------
// Procedure: onSetParamComplete()
//   Purpose: Invoked once after all parameters have been handled.
//            Good place to ensure all required params have are set.
//            Or any inter-param relationships like a<b.

void BHV_ZigLeg::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_ZigLeg::onHelmStart()
{
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_ZigLeg::onIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_ZigLeg::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_ZigLeg::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_ZigLeg::onIdleToRunState()
{
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_ZigLeg::onRunToIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_ZigLeg::onRunState()
{
  // Part 1: Build the IvP function
  IvPFunction *ipf = 0;

  // Part 1: Get vehicle position from InfoBuffer and post a                                                                                                                         
  // warning if problem is encountered                                                                                                                                               
  bool ok1, ok2, ok3, ok4;
  m_osx = getBufferDoubleVal("NAV_X", ok1);
  m_osy = getBufferDoubleVal("NAV_Y", ok2);
  if(!ok1 || !ok2) {
    postWMessage("No ownship X/Y info in info_buffer.");
    return(0);
  }

  m_current_wpt_index = getBufferDoubleVal("WPT_INDEX", ok3);
  if(!ok3) {
    //postWMessage("No ownship WI info in info_buffer.");
    return(0);
  }

  m_current_angle = getBufferDoubleVal("NAV_HEADING", ok4);
  if(!ok4) {
    postWMessage("No ownship CH info in info_buffer.");                                                                                                                            
    return(0);
  }

  m_curr_time = getBufferCurrTime();


  //if the waypoint is hit set the current time
  if(m_current_wpt_index != m_past_wpt_index)
    {
      m_past_wpt_index = m_current_wpt_index;
      m_index_time = getBufferCurrTime();
      m_index_time_plus_10 = m_index_time + 5+ m_zig_duration;
      m_index_time_plus_5 = m_index_time + 5;
      m_pulsed_bool = true;
    }
  else
    {
    } 
  
 //if 5 seconds after send out a pulse
  if((m_index_time_plus_5 <  m_curr_time) && (m_curr_time < m_index_time_plus_10) && m_pulsed_bool) 
    {
      m_current_angle_plus_45 = m_current_angle + m_zig_angle;
      m_pulsed_bool = false;
    }

  if((m_index_time_plus_5 <  m_curr_time) && (m_curr_time < m_index_time_plus_10))
    {
      ipf = buildFunctionWithZAIC();
    }
  else
    {
      m_current_angle_plus_45 = m_current_angle;
    }


  // Part N: Prior to returning the IvP function, apply the priority wt
  // Actual weight applied may be some value different than the configured
  // m_priority_wt, depending on the behavior author's insite.
  if(ipf)
    ipf->setPWT(m_priority_wt);

  return(ipf);
}


IvPFunction *BHV_ZigLeg::buildFunctionWithZAIC()
{
  ZAIC_PEAK crs_zaic(m_domain, "course");
  crs_zaic.setSummit(m_current_angle_plus_45);
  crs_zaic.setPeakWidth(0);
  crs_zaic.setBaseWidth(180.0);
  crs_zaic.setSummitDelta(0);
  crs_zaic.setValueWrap(true);
  if(crs_zaic.stateOK() == false) {
    string warnings = "Course ZAIC problems " + crs_zaic.getWarnings();
    postWMessage(warnings);
    return(0);
  }

  //  IvPFunction *spd_ipf = spd_zaic.extractIvPFunction();
  IvPFunction *crs_ipf = crs_zaic.extractIvPFunction();

  //OF_Coupler coupler;
  //  IvPFunction *ivp_function = coupler.couple(crs_ipf, spd_ipf, 50, 50);

  return(crs_ipf);
}
