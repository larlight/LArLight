/// ChannelDataGroup.h
/// William Seligman <seligman@nevis.columbia.edu>
///
/// This a general container class for vectors of ADC counts. It can
/// be used for collections of ADC counts from the PMTS, channels
/// output by the Front End Modules (FEMs), etc.
///

#ifndef optdata_ChannelDataGroup_h
#define optdata_ChannelDataGroup_h

// LArSoft includes
#include "OpticalTypes.h"
#include "ChannelData.h"

// C++ includes
#include <vector>

namespace optdata {

  // Collection of all the channels associated with a certain time frame
  // (e.g., an event, a trigger).
  class ChannelDataGroup : public std::vector< optdata::ChannelData >
  {
  public:

    // Simple constructor/destructor.
    ChannelDataGroup ( Optical_Category_t category = kUndefined, 
		       TimeSlice_t time = 0,
		       Frame_t frame = 0 ) 
      : fm_category(category)
      , fm_timeSlice(time)
      , fm_frame(frame)
    {};

    ~ChannelDataGroup() {};

    // Here we have getters and setters for the time information.

#ifndef __GCCXML__

    Optical_Category_t Category() const { return fm_category; }

    // A time slice associated with the first bin in the channel
    // data. For example, the first bin of the ADC channels may refer
    // to clock value 8595824 (in some arbitrary units).
    TimeSlice_t TimeSlice() const { return fm_timeSlice; }
    void SetTimeSlice( TimeSlice_t t ) { fm_timeSlice = t; }

    // The frame number associated with the first frame in the channels.
    Frame_t Frame() const { return fm_frame; }
    void SetFrame( Frame_t f ) { fm_frame = f; }

#endif

  private:
    Optical_Category_t fm_category;
    TimeSlice_t fm_timeSlice;
    Frame_t fm_frame;
  };

} // namespace optdata

#endif // optdata_ChannelDataGroup_h
