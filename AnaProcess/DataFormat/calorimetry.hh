/**
 * \file calorimetry.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for calorimetry data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef CALORIMETRY_HH
#define CALORIMETRY_HH

#include "data_base.hh"

namespace larlight{
  /**
     \class calorimetry
     LArSoft Calorimetry class equivalent data container
  */
  class calorimetry : public data_base {
    
  public:
    
    /// Default constructor
    calorimetry(DATA::DATA_TYPE type=DATA::Calorimetry);
    
    /// Copy constructor
    calorimetry(const calorimetry& original) : data_base(original),
					       fKineticEnergy(original.fKineticEnergy),
					       fdEdx(original.fdEdx),
					       fdQdx(original.fdQdx),
					       fResidualRange(original.fResidualRange),
					       fDeadWireResR(original.fDeadWireResR),
					       fRange(original.fRange),
					       fTrkPitch(original.fTrkPitch)
    {}
    
    /// Default destructor
    virtual ~calorimetry(){}
    
    virtual void clear_data();
    
    // Set methods
    void set_dedx(const std::vector<double> &v)           { fdEdx = v;          }
    void set_dqdx(const std::vector<double> &v)           { fdQdx = v;          }
    void set_residual_range(const std::vector<double> &v) { fResidualRange = v; }
    void set_deadwire_range(const std::vector<double> &v) { fDeadWireResR  = v; }
    void set_kinetic_energy(const double v)               { fKineticEnergy = v; }
    void set_range(const double v)                        { fRange = v;         }
    void set_track_pitch(const std::vector<double> &v)    { fTrkPitch = v;      }

    // Get Methods 
    inline const std::vector<double>& dEdx()          const { return fdEdx; }
    inline const std::vector<double>& dQdx()          const { return fdQdx; }
    inline const std::vector<double>& ResidualRange() const { return fResidualRange; }
    inline const std::vector<double>& DeadWireResRC() const { return fDeadWireResR; }
    inline double                     KineticEnergy() const { return fKineticEnergy; }
    inline double                     Range()         const { return fRange; }
    inline const std::vector<double>& TrkPitchVec()   const { return fTrkPitch; }
    inline double                     TrkPitchC()     const
    {
      if (fTrkPitch.size())
	return fTrkPitch[0];
      else return 0;
    }

  protected:

    double              fKineticEnergy;   ///< determined kinetic energy
    std::vector<double> fdEdx;            ///< dE/dx, should be same size as fResidualRange
    std::vector<double> fdQdx;            ///< dQ/dx
    std::vector<double> fResidualRange;   ///< range from end of track
    std::vector<double> fDeadWireResR;    ///< dead wire residual range, collection plane
    double              fRange;           ///< total range of track
    std::vector<double> fTrkPitch;        ///< track pitch on collection plane
    
  private:
    
    ////////////////////////
    ClassDef(calorimetry,1)
    ////////////////////////
      
  };
  
  /**
     \class event_calorimetry
     A collection storage class of multiple calorimetrys.
  */
  class event_calorimetry : public std::vector<larlight::calorimetry>,
			    public event_base {
    
  public:
    
    /// Default constructor
    event_calorimetry(DATA::DATA_TYPE type=DATA::Calorimetry);
    
    /// Default copy constructor
    event_calorimetry(const event_calorimetry& original) : std::vector<larlight::calorimetry>(original),
							   event_base(original)
    {}
    
    /// Default destructor
    ~event_calorimetry(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

  protected:
    
    std::vector<larlight::calorimetry> fCalorimetry_v;
    
  private:
    
    ////////////////////////
    ClassDef(event_calorimetry,2)
    ////////////////////////
  };
}
#endif

/** @} */ // end of doxygen group 
