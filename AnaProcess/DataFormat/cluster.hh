/**
 * \file cluster.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for cluster data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef CLUSTER_HH
#define CLUSTER_HH

#include "data_base.hh"
#include "hit.hh"
/**
   \class cluster
   LArSoft Cluster class equivalent data container
*/
namespace larlight{

  class cluster : public data_base {

  public:
    
    /// Default constructor
    cluster() : data_base() {clear_data();};
    
    /// Default destructor
    virtual ~cluster(){};
    
    void set_charge   (Double_t q)      { fTotalCharge = q; };
    void set_dtdw     (Double_t v)      { fdTdW=v; };
    void set_dqdw     (Double_t v)      { fdQdW=v; };
    void set_dtdw_err (Double_t v)      { fSigmadTdW=v; };
    void set_dqdw_err (Double_t v)      { fSigmadQdW=v; };
    void set_id       (Int_t i)         { fID = i; };
    void set_view     (GEO::View_t v) { fView=v; };
    void set_start_vtx     (const std::vector<Double_t> vtx) { fStartPos=vtx;      };
    void set_end_vtx       (const std::vector<Double_t> vtx) { fEndPos=vtx;        };
    void set_start_vtx_err (const std::vector<Double_t> vtx) { fSigmaStartPos=vtx; };
    void set_end_vtx_err   (const std::vector<Double_t> vtx) { fSigmaEndPos=vtx;   };
    void add_hit           (const hit h)                   { fHitVector.push_back(h); }
    
    inline Double_t       Charge()    const { return fTotalCharge;   }
    inline Double_t       dTdW()      const { return fdTdW;          }
    inline Double_t       dQdW()      const { return fdQdW;          }
    inline Double_t       SigmadTdW() const { return fSigmadTdW;     }
    inline Double_t       SigmadQdW() const { return fSigmadQdW;     }
    inline Int_t          ID()        const { return fID;            }
    inline GEO::View_t  View()      const { return fView;          }
    
    inline const std::vector<Double_t>& StartPos()      const { return fStartPos;      }
    inline const std::vector<Double_t>& EndPos()        const { return fEndPos;        }
    inline const std::vector<Double_t>& SigmaStartPos() const { return fSigmaStartPos; }
    inline const std::vector<Double_t>& SigmaEndPos()   const { return fSigmaEndPos;   }
    inline const std::vector<hit>&    Hits()          const { return fHitVector;     }
    virtual void clear_data(){
      data_base::clear_data();
      fTotalCharge = -1;
      fdTdW = fdQdW = fSigmadTdW = fSigmadQdW = -1;
      fStartPos.clear();
      fEndPos.clear();
      fSigmaStartPos.clear();
      fSigmaEndPos.clear();
      fHitVector.clear();
      fID = -1;
      fView = GEO::kUnknown;
    }
    
    /**
       A utility function to obtain maximum & minimum of clusters' hits' vertex
       NOTE: the initial max/min values matter. It only modifies max/min if it finds values that is 
       above/below those initial values.
    */
    void get_axis_range (std::vector<Double_t> &chmax,   std::vector<Double_t> &chmin,
			 std::vector<Double_t> &wiremax, std::vector<Double_t> &wiremin,
			 std::vector<Double_t> &timemax, std::vector<Double_t> &timemin) const;


  private:
    
    Double_t              fTotalCharge;    ///< total charge in cluster
    Double_t              fdTdW;           ///< slope of cluster in tdc vs wire
    Double_t              fdQdW;           ///< slope of cluster in charge vs wire
    Double_t              fSigmadTdW;      ///< slope of cluster in tdc vs wire
    Double_t              fSigmadQdW;      ///< slope of cluster in charge vs wire
    std::vector<Double_t> fStartPos;       ///< start of cluster in (wire, tdc) plane
    std::vector<Double_t> fEndPos;         ///< start of cluster in (wire, tdc) plane
    std::vector<Double_t> fSigmaStartPos;  ///< start of cluster in (wire, tdc) plane
    std::vector<Double_t> fSigmaEndPos;    ///< start of cluster in (wire, tdc) plane
    std::vector<hit>      fHitVector;      ///< Hit vector
    Int_t                 fID;             ///< cluster's ID
    GEO::View_t           fView;           ///< View associated w/ this cluster
    
    ////////////////////////
    ClassDef(cluster,2)
    ////////////////////////
      
      };
  
  /**
     \class cluster_collection 
     A collection storage class of multiple clusters.
  */
  class event_cluster : public std::vector<larlight::cluster>,
			public data_base {
    
  public:
    
    /// Default constructor
    event_cluster() : std::vector<larlight::cluster>(), data_base() {clear_data();};
    
    /// Default copy constructor
    event_cluster(const event_cluster& original)
      : std::vector<larlight::cluster>(original), data_base(original)
    {}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){data_base::clear_data(); clear();}
    
    /// Default destructor
    ~event_cluster(){}

    /**
       A utility function to obtain maximum & minimum of clusters' hits' vertex
       NOTE: the initial max/min values matter. It only modifies max/min if it finds values that is
       above/below those initial values.
    */
    void get_axis_range (std::vector<Double_t> &chmax,   std::vector<Double_t> &chmin,
			 std::vector<Double_t> &wiremax, std::vector<Double_t> &wiremin,
			 std::vector<Double_t> &timemax, std::vector<Double_t> &timemin) const;
    
  private:
    
    ////////////////////////
    ClassDef(event_cluster,2)
    ////////////////////////
      };
};
#endif
  
  /** @} */ // end of doxygen group 
  
