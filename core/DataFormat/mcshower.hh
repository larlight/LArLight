/**
 * \file mcshower.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for mcshower data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat
    
@{*/

#ifndef MCSHOWER_HH
#define MCSHOWER_HH

#include "data_base.hh"

namespace larlight {
  /**
     \class mcshower
     LArSoft utility class (not data container) MCShowerProfile 
     implemented as a data product in LArLight.
  */
  class mcshower : public data_base {
    
  public:

    /// Default constructor
     mcshower(DATA::DATA_TYPE type=DATA::MCShower);

    /// Default destructor
    virtual ~mcshower(){}

    /// Copy constructor
    mcshower(const mcshower& origin) : data_base(origin),
				       fMotherPDGID(origin.fMotherPDGID),
				       fMotherTrackID(origin.fMotherTrackID),
				       fMotherVtx(origin.fMotherVtx),
				       fMotherMomentum(origin.fMotherMomentum),
				       fMotherPhi(origin.fMotherPhi),
				       fMotherTheta(origin.fMotherTheta),
				       fMotherAngleU(origin.fMotherAngleU),
				       fMotherAngleV(origin.fMotherAngleV),
				       fMotherAngleW(origin.fMotherAngleW),
				       fDaughterTrackID(origin.fDaughterTrackID),
				       fDaughterVtx(origin.fDaughterVtx),
				       fDaughterMomentum(origin.fDaughterMomentum),
                                       fDaughterPhi(origin.fDaughterPhi),
                                       fDaughterTheta(origin.fDaughterTheta),
				       fDaughterAngleU(origin.fDaughterAngleU),
				       fDaughterAngleV(origin.fDaughterAngleV),
                                       fDaughterAngleW(origin.fDaughterAngleW),
                                       fChargeU(origin.fChargeU),
                                       fChargeV(origin.fChargeV),
                                       fChargeW(origin.fChargeW),
                                       fEdepVtx(origin.fEdepVtx)
    {}

    /// Clear method
    virtual void clear_data();

    //--- Getters ---//

    /// Shower mother's PDGID
    Int_t  MotherPDGID()   const { return fMotherPDGID;   }

    /// Shower mother's G4 track ID 
    UInt_t MotherTrackID() const { return fMotherTrackID; }

    /// Shower mother's start point position
    const std::vector<Double_t>& MotherPosition() const { return fMotherVtx;      }

    /// Shower mother's start point momentum
    const std::vector<Double_t>& MotherMomentum() const { return fMotherMomentum; }

    /// Shower mother's 3D angle in conventional definition (not ordinary 3D coord. system)
    void MotherAngle3D (Double_t &phi, Double_t &theta) const
    { phi = fMotherPhi; theta = fMotherTheta;}

    /// Shower mother's 2D angle
    Double_t MotherAngle2D(const GEO::View_t view) const;

    /// Array of daughters' track ID
    const std::vector<UInt_t>&  DaughterTrackID() const
    { return fDaughterTrackID; }

    /// (x,y,z,dE) vector as a weighted average of daughters' energy deposition points
    const std::vector<Float_t>& DaughterPosition() const
    { return fDaughterVtx; }

    /// (x,y,z,dE) vector as a weighted average of daughters' energy deposition points
    const std::vector<Float_t>& DaughterMomentum() const
    { return fDaughterMomentum; }

    /// Daughters' 3D angle
    void DaughterAngle3D(Float_t &phi, Float_t &theta) const
    { phi = fDaughterPhi; theta = fDaughterTheta; }

    /// Daughter's 2D angle
    Float_t DaughterAngle2D(const GEO::View_t view) const;

    /// Charge deposited by daughters per plane
    Float_t Charge(const GEO::View_t view) const;

    /// Dauighter Charge Deposition Points
    const std::vector<std::vector<Float_t> >& DaughterPoints() const
    { return fEdepVtx; }
    

    //--- Setters ---//

    void SetMotherID(Int_t pdgid, UInt_t trackid)
    { fMotherPDGID = pdgid; fMotherTrackID = trackid; }

    void SetMotherAngles(Double_t phi, Double_t theta, Double_t u, Double_t v, Double_t w)
    { fMotherPhi = phi; fMotherTheta = theta; fMotherAngleU = u; fMotherAngleV = v; fMotherAngleW = w; }

    void SetMotherPoint(const std::vector<Double_t> &vtx);

    void SetMotherMomentum(const std::vector<Double_t> &mom);

    void SetDaughterPosition(const std::vector<Float_t> &vtx);

    void SetDaughterMomentum(const std::vector<Float_t> &mom);
    
    void SetDaughterTrackList(const std::vector<UInt_t> &list)
    { fDaughterTrackID = list; }

    void SetDaughterAngles(Double_t phi, Double_t theta, Double_t u, Double_t v, Double_t w)
    { fDaughterPhi = phi; fDaughterTheta = theta; fDaughterAngleU = u; fDaughterAngleV = v; fDaughterAngleW = w; }

    void SetPlaneCharge(Float_t qU, Float_t qV, Float_t qW)
    { fChargeU = qU; fChargeV = qV; fChargeW = qW; }

    void AddEdepVtx(const std::vector<Float_t>& vtx);

    void SetEdepVtx(const std::vector<std::vector<Float_t> >& vtx);

  private:

    //---- Mother info ----//
    Int_t  fMotherPDGID;                   ///< mother PDG code
    UInt_t fMotherTrackID;                 ///< mother G4 Track ID
    std::vector<Double_t> fMotherVtx;      ///< mother position 4-vector @ generation
    std::vector<Double_t> fMotherMomentum; ///< mother momentum 4-vector @ generation
    /// mother 3D angle phi (along shower angle definition, not ordinary coord. system)
    Double_t fMotherPhi;
    /// mother 3D angle theta (along shower angle definition, not ordinary coord. system)
    Double_t fMotherTheta;
    /// mother 2D angle on U-plane
    Double_t fMotherAngleU;
    /// mother 2D angle on V-plane
    Double_t fMotherAngleV;
    /// mother 2D angle on W-plane
    Double_t fMotherAngleW;

    //---- Daughter info ----//
    std::vector<UInt_t>  fDaughterTrackID;  ///< Daughters' track ID
    std::vector<Float_t> fDaughterVtx;      ///< Daughters' deposit point closest to the mother vtx
    std::vector<Float_t> fDaughterMomentum; ///< Daughters' deposit sum momentum 4-vector
    /// daughter 3D angle phi (along shower angle definition, not ordinary coord. system)
    Float_t fDaughterPhi;
    /// daughter 3D angle theta (along shower angle definition, not ordinary coord. system)
    Float_t fDaughterTheta;
    /// daughter 2D angle on U-plane
    Float_t fDaughterAngleU;
    /// daughter 2D angle on V-plane
    Float_t fDaughterAngleV;
    /// daughter 2D angle on W-plane
    Float_t fDaughterAngleW;

    //---- Charge per plane ----//
    Float_t fChargeU; ///< Charge deposit on U plane
    Float_t fChargeV; ///< Charge deposit on V plane
    Float_t fChargeW; ///< Charge deposit on W plane

    /// Charge deposition points
    std::vector<std::vector<Float_t> > fEdepVtx;

    ////////////////////////
    ClassDef(mcshower,2)
    ////////////////////////
      
  };

  /**
     \class event_mcshower
     A collection storage class of multiple mcshower
  */
  class event_mcshower : public std::vector<larlight::mcshower>,
			 public event_base {
    
  public:
    
    /// Default constructor
    event_mcshower(DATA::DATA_TYPE type=DATA::MCShower);
    
    /// Default copy constructor
    event_mcshower(const event_mcshower& original) : std::vector<larlight::mcshower>(original),
						     event_base(original)
    {}
    
    /// Default destructor
    ~event_mcshower(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
  private:
    
    //////////////////////////
    ClassDef(event_mcshower,1)
    //////////////////////////
  };
  
}
#endif

/** @} */ // end of doxygen group 
