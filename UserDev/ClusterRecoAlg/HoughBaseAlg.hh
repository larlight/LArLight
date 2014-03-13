////////////////////////////////////////////////////////////////////////
// HoughBaseAlg.h
//
// HoughBaseAlg class
//
// Ben Carls (bcarls@fnal.gov)
//
////////////////////////////////////////////////////////////////////////
#ifndef HOUGHBASEALG_H
#define HOUGHBASEALG_H

#include "TMath.h"
#include <vector>
//#include <unordered_map>

#include "Base-TypeDef.hh"
#include "LArUtil-TypeDef.hh"
#include "DataFormat-TypeDef.hh"

//#include "RecoBase/Hit.h"

namespace larreco {

  struct houghCorner
  {
    Double_t strength=0;
    Double_t p0=0;
    Double_t p1=0;
    houghCorner(Double_t strengthTemp=0,
		Double_t p0Temp=0,
		Double_t p1Temp=0)
    {
      strength=strengthTemp;
      p0=p0Temp;
      p1=p1Temp;
    }
    
    bool operator < (const houghCorner& houghCornerComp) const
    {
      return (strength < houghCornerComp.strength);
    }
  };
  
  
  // This stores information about merged lines
  struct mergedLines
  {
    Double_t totalQ=0;
    Double_t pMin0=0;
    Double_t pMin1=0;
    Double_t pMax0=0;
    Double_t pMax1=0;
    Int_t clusterNumber=-999999;
    Double_t showerLikeness=0;
    mergedLines (Double_t totalQTemp=0,
		 Double_t pMin0Temp=0,
		 Double_t pMin1Temp=0,
		 Double_t pMax0Temp=0,
		 Double_t pMax1Temp=0,
		 Double_t clusterNumberTemp=-999999,
		 Double_t showerLikenessTemp=0)
    {
      totalQ=totalQTemp;
      pMin0=pMin0Temp;
      pMin1=pMin1Temp;
      pMax0=pMax0Temp;
      pMax1=pMax1Temp;
      clusterNumber=clusterNumberTemp;
      showerLikeness=showerLikenessTemp;
    }
  };
  
  struct protoTrack
  {
    Int_t clusterNumber=999999;
    Int_t oldClusterNumber=999999;
    Float_t clusterSlope=999999;
    Float_t clusterIntercept=999999;
    Float_t totalQ=-999999;
    Float_t pMin0=999999;
    Float_t pMin1=999999;
    Float_t pMax0=-999999;
    Float_t pMax1=-999999;
    Float_t iMinWire=999999;
    Float_t iMaxWire=-999999;
    Float_t minWire=999999;
    Float_t maxWire=-999999;
    Float_t isolation=-999999;
    Float_t showerLikeness=-999999;
    bool merged=false;
    bool showerMerged=false;
    bool mergedLeft=false;
    bool mergedRight=false;
    std::vector<larlight::hit> hits;
    protoTrack(){
    }
    
    void Init(UInt_t num=999999, 
	      Float_t slope=999999, 
	      Float_t intercept=999999,
	      Float_t totalQTemp=-999999,
	      Float_t Min0=999999, 
	      Float_t Min1=999999, 
	      Float_t Max0=-999999, 
	      Float_t Max1=-999999,
	      Int_t    iMinWireTemp=999999,
	      Int_t    iMaxWireTemp=-999999,
	      Int_t    minWireTemp=999999,
	      Int_t    maxWireTemp=-999999,
	      std::vector<larlight::hit> hitsTemp=std::vector<larlight::hit>())
    {
      clusterNumber = num;
      oldClusterNumber = num;
      clusterSlope = slope;
      clusterIntercept = intercept;
      totalQ=totalQTemp;
      pMin0 = Min0;
      pMin1 = Min1;
      pMax0 = Max0;
      pMax1 = Max1;
      iMinWire = iMinWireTemp;
      iMaxWire = iMaxWireTemp;
      minWire = minWireTemp;
      maxWire = maxWireTemp;
      merged = false;
      showerMerged = false;
      showerLikeness = 0;
      hits.swap(hitsTemp);
    }
  };
  
  class HoughTransform {
  public:
    
    HoughTransform();
    ~HoughTransform();
    
    void Init(Int_t dx, Int_t dy, Int_t rhoresfact, Int_t numACells);
    std::vector<Int_t>  AddPointReturnMax(Int_t x, Int_t y);
    bool SubtractPoint(Int_t x, Int_t y);
    inline Int_t  GetCell(Int_t row, Int_t col)     { return m_accum[row][col];  }
    void SetCell(Int_t row, Int_t col, Int_t value) { m_accum[row][col] = value; }
    void IncrementCell(Int_t row, Int_t col)      { m_accum[row][col]++;       }
    void GetAccumSize(Int_t &numRows, Int_t &numCols) 
    { 
      numRows = m_accum.size();
      numCols  = m_rowLength;
    }
    Int_t NumAccumulated()                      { return m_numAccumulated; }
    void GetEquation( Float_t row, Float_t col, Float_t &rho, Float_t &theta)
    {
      theta = (TMath::Pi()*row)/m_numAngleCells;
      rho   = (col - (m_rowLength/2.))/m_rhoResolutionFactor;
    }
    Int_t GetMax(Int_t & xmax, Int_t & ymax);
    
    //void reconfigure(fhicl::ParameterSet const& pset);
    void reconfigure();
    
  private:
    
    Int_t m_dx;
    Int_t m_dy;
    // Note, m_accum is a vector of associative containers, the vector elements are called by rho, theta is the container key, the number of hits is the value corresponding to the key
    std::vector<std::map<Int_t,Int_t> > m_accum;  // column=rho, row=theta
    //std::vector< std::vector<Int_t> > m_accum;  // column=rho, row=theta
    Int_t m_rowLength;
    Int_t m_numAccumulated;
    Int_t m_rhoResolutionFactor;
    Int_t m_numAngleCells;
    //std::vector<float> m_cosTable;
    //std::vector<float> m_sinTable;
    std::vector<Int_t>  DoAddPointReturnMax(Int_t x, Int_t y);
    bool DoSubtractPoint(Int_t x, Int_t y);
    
    
  }; // class HoughTransform  
  
  class HoughBaseAlg : public larlight::larlight_base {
    
  public:
    
    //HoughBaseAlg(fhicl::ParameterSet const& pset); 
    HoughBaseAlg();
    virtual ~HoughBaseAlg();
    
    size_t FastTransform(std::vector<larlight::cluster>            const& clusIn,
			 std::vector<larlight::hit>                const& hitIn,
			 std::vector<larlight::cluster>                 & ccol,  
			 std::vector<std::vector<UShort_t> >            & clusHitsOut);
    
    size_t Transform(std::vector<larlight::hit> const& hits,
                     std::vector<UInt_t>     *fpointId_to_clusterId,
                     UInt_t clusterId, // The id of the cluster we are examining
                     Int_t *nClusters,
                     std::vector<larreco::protoTrack> *protoTracks);
    
    
    // interface to look for lines only on a set of hits,without slope and totalQ arrays
    size_t FastTransform(const std::vector<larlight::hit>       & clusIn,
			 std::vector<std::vector<UShort_t> >    & clusHitsOut
			 );
    
    // interface to look for lines only on a set of hits
    size_t FastTransform(const std::vector <larlight::hit>      & clusIn,
			 std::vector<std::vector<UShort_t> >    & clusHitsOut,
			 std::vector<Double_t> &slope, 
			 std::vector<Double_t> &totalQ
			 );
    
    
    size_t Transform(std::vector<larlight::hit> const& hits);
    
    size_t Transform(std::vector<larlight::hit> const& hits,
		     Double_t                          & slope,
		     Double_t                          & intercept);
    
    //virtual void reconfigure(fhicl::ParameterSet const& pset);
    virtual void reconfigure();
    
  protected:
    
    void HLSSaveBMPFile(Char_t const*, UChar_t*, Int_t, Int_t);
    
  private:

    Int_t    fMaxLines;                      ///< Max number of lines that can be found 
    Int_t    fMinHits;                       ///< Min number of hits in the accumulator to consider 
                                           ///< (number of hits required to be considered a line).
    Int_t    fSaveAccumulator;               ///< Save bitmap image of accumulator for debugging?
    Int_t    fNumAngleCells;                 ///< Number of angle cells in the accumulator 
                                           ///< (a measure of the angular resolution of the line finder). 
                                           ///< If this number is too large than the number of votes 
                                           ///< that fall into the "correct" bin will be small and consistent with noise.
    Float_t  fMaxDistance;                   ///< Max distance that a hit can be from a line to be considered part of that line
    Float_t  fMaxSlope;                      ///< Max slope a line can have
    Int_t    fRhoZeroOutRange;               ///< Range in rho over which to zero out area around previously found lines in the accumulator
    Int_t    fThetaZeroOutRange;             ///< Range in theta over which to zero out area around previously found lines in the accumulator
    Float_t  fRhoResolutionFactor;           ///< Factor determining the resolution in rho
    Int_t    fPerCluster;                    ///< Tells the original Hough algorithm to look at clusters individually, or all hits
                                           ///< at once
    Int_t    fMissedHits;                    ///< Number of wires that are allowed to be missed before a line is broken up into
                                           ///< segments
    Float_t  fMissedHitsDistance;            ///< Distance between hits in a hough line before a hit is considered missed
    Float_t  fMissedHitsToLineSize;          ///< Ratio of missed hits to line size for a line to be considered a fake

  protected:
    
    friend class HoughTransformClus;
  };
  
  
}// namespace

#endif // HOUGHBASEALG_H
