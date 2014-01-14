/**
 * \file encoder_manager.hh
 *
 * \ingroup Encoder
 * 
 * \brief Class def header for a class encoder_manager class
 *
 * @author kazuhiro
 */

/** \addtogroup Encoder

    @{*/

#ifndef ENCODER_MANAGER_HH
#define ENCODER_MANAGER_HH

#include "decoder_base.hh"

/**
   \class encoder_manager
   A driver class for encoding ROOT file output back into the electornics
   readout system output binary (or ASCII) file.
 */
class encoder_manager : public decoder_base {

public:

  /// Default constructor
  encoder_manager(){};

  /// Default destructor
  ~encoder_manager();

};

#endif
/** @} */ // end of doxygen group
