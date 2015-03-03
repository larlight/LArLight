/**
 * \file FEMConstants.hh
 *
 * \ingroup Base
 * 
 * \brief defines FEM electronics related constants
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Base

    @{*/

#ifndef FEMCONSTANTS_HH
#define FEMCONSTANTS_HH
#include "DataFormatConstants.hh"

/// Namespace of everything in this framework
namespace larlight{
  
  /// Defines constants for PMT related electronics
  namespace FEM{
    
    const UShort_t INVALID_CH=DATA::INVALID_USHORT; ///< Invalid (or initialized) channel number
    const UInt_t   INVALID_WORD=DATA::INVALID_UINT; ///< Invalid (or initialized) event word
    const UInt_t   EMPTY_WORD=0x0;                  ///< Empty word
    
    const UInt_t   BEAMGATE_NSAMPLE=1500;  ///< # samples in the beamgate
    const UShort_t NUM_PMT_CHANNEL=48;     ///< # of PMT readout channels
    const UShort_t MAX_ADC=4095;           ///< Maximum ADC value
    
    /// PMT discriminator type
    enum DISCRIMINATOR {
      BASE_DISC = 0, ///< lowest disc. threshold so-called disc. 0
      COSMIC_DISC,   ///< discriminator for cosmics
      MICHEL_DISC,   ///< discriminator for michel electrons (not used, just a place hlder)
      BEAM_DISC,     ///< discriminator for signal within the beam window
      BEAM_WIN,      ///< discriminator for beam window (forced N samples read-out)
      DISC_MAX
    };

    /// Defines PMT word-type
    enum FEM_WORD{
      UNDEFINED_WORD = 0, ///< Word type ... undefined
      EVENT_HEADER,       ///< A signal word (no data) to flag the start of headers 
      FEM_HEADER,         ///< Event header word
      FEM_FIRST_WORD,     ///< First word in the event (after the event header, before channel data )
      CHANNEL_HEADER,     ///< Channel header word
      CHANNEL_WORD,       ///< Channel word such as ADC sample
      CHANNEL_LAST_WORD,  ///< Last word per channel-wise readout
      FEM_LAST_WORD,      ///< Last word per fem
      EVENT_LAST_WORD,    ///< Last word in the event
      CHANNEL_TIME,       ///< Channel time word
      CHANNEL_PACKET_LAST_WORD  ///< Channel: last word of the packet
    };

  }

}
#endif
/** @} */ // end of doxygen group
