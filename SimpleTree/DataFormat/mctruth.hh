/**
 * \file mctruth.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for mctruth data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat
    
@{*/

#ifndef MCTRUTH_HH
#define MCTRUTH_HH

#include "data_base.hh"

/**
   \class mctruth
   Simplified version of MCParticle data container
*/
class mctruth : public data_base {

public:

  /// Default constructor
  mctruth(DATA::DATA_TYPE type=DATA::Track);

  /// Default destructor
  virtual ~mctruth(){};

    /// Implementation of event-wise track data clear method
  virtual void clear_event();

  /// Implementation of track data address setter
  virtual void set_address(TTree* t);


};

#endif

/** @} */ // end of doxygen group 
