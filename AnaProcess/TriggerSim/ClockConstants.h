
#ifndef CLOCKCONSTANTS_H
#define CLOCKCONSTANTS_H

#include <climits>

namespace trigger{

  // Invalid frame and sample
  const unsigned int kINVALID_FRAME  = std::numeric_limits<unsigned int>::max();
  const unsigned int kINVALID_SAMPLE = std::numeric_limits<unsigned int>::max();

}

#endif
