// File: PrimeEntry.h
 #include <list>
 #include <iostream>
 #include <string> 
 #include <vector> 
 #include <cstdint>
 #include <math.h>
using namespace std; 
 class PrimeEntry
{
 public:
  PrimeEntry();
  ~PrimeEntry();

  void setOriginalVal(unsigned long int v);
  void setReceivedIndex(unsigned int v)    {m_received_index=v;};
  void setCalculatedIndex(unsigned int v)  {m_calculated_index=v;};
  void setDone(bool v)                     {m_done=v;};

  bool   done() {return(m_done);};

  bool   factor(unsigned long int max_steps);

  string  getReport();

 protected:
  uint64_t      m_start_index;
  uint64_t      m_orig;
  bool          m_done;
  unsigned int  m_received_index;
  unsigned int  m_calculated_index;

  std::vector<uint64_t> m_factors;
};
//#endif 
