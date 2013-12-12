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
    cluster() : data_base() {};
    
    /// Default destructor
    virtual ~cluster(){};
    
    void set_charge   (double q)      { fTotalCharge = q; };
    void set_dtdw     (double v)      { fdTdW=v; };
    void set_dqdw     (double v)      { fdQdW=v; };
    void set_dtdw_err (double v)      { fSigmadTdW=v; };
    void set_dqdw_err (double v)      { fSigmadQdW=v; };
    void set_id       (int i)         { fID = i; };
    void set_view     (GEO::View_t v) { fView=v; };
    void set_start_vtx     (const std::vector<double> vtx) { fStartPos=vtx;      };
    void set_end_vtx       (const std::vector<double> vtx) { fEndPos=vtx;        };
    void set_start_vtx_err (const std::vector<double> vtx) { fSigmaStartPos=vtx; };
    void set_end_vtx_err   (const std::vector<double> vtx) { fSigmaEndPos=vtx;   };
    void add_hit           (const hit h)                   { fHitVector.push_back(h); }
    
    inline double       Charge()    const { return fTotalCharge;   }
    inline double       dTdW()      const { return fdTdW;          }
    inline double       dQdW()      const { return fdQdW;          }
    inline double       SigmadTdW() const { return fSigmadTdW;     }
    inline double       SigmadQdW() const { return fSigmadQdW;     }
    inline int          ID()        const { return fID;            }
    inline GEO::View_t  View()      const { return fView;          }
    
    inline const std::vector<double>& StartPos()      const { return fStartPos;      }
    inline const std::vector<double>& EndPos()        const { return fEndPos;        }
    inline const std::vector<double>& SigmaStartPos() const { return fSigmaStartPos; }
    inline const std::vector<double>& SigmaEndPos()   const { return fSigmaEndPos;   }
    inline const std::vector<hit>&    Hits()          const { return fHitVector;     }
    virtual void clear_data(){
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
    void get_axis_range (double &xmax, double &xmin,
			 double &ymax, double &ymin,
			 double &zmax, double &zmin) const;  

  private:
    
    double                     fTotalCharge;    ///< total charge in cluster
    double                     fdTdW;           ///< slope of cluster in tdc vs wire
    double                     fdQdW;           ///< slope of cluster in charge vs wire
    double                     fSigmadTdW;      ///< slope of cluster in tdc vs wire
    double                     fSigmadQdW;      ///< slope of cluster in charge vs wire
    std::vector<double>        fStartPos;       ///< start of cluster in (wire, tdc) plane
    std::vector<double>        fEndPos;         ///< start of cluster in (wire, tdc) plane
    std::vector<double>        fSigmaStartPos;  ///< start of cluster in (wire, tdc) plane
    std::vector<double>        fSigmaEndPos;    ///< start of cluster in (wire, tdc) plane
    std::vector<hit>           fHitVector;      ///< Hit vector
    int                        fID;             ///< cluster's ID
    GEO::View_t                fView;           ///< View associated w/ this cluster
    
    ////////////////////////
    ClassDef(cluster,2)
    ////////////////////////
      
      };
  
  /**
     \class cluster_collection 
     A collection storage class of multiple clusters.
  */
  class event_cluster : public data_base {
    
  public:
    
    /// Default constructor
    event_cluster() : data_base(),fCluster_v() {clear_data();};
    
    /// Default copy constructor
    event_cluster(const event_cluster& original)
      : data_base(original), fCluster_v(original.fCluster_v)
    {};
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){data_base::clear_data(); fCluster_v.clear();};
    
    /// Method to add a cluster
    void add_cluster(const cluster c){fCluster_v.push_back(c);};
    
    /// Getter
    const std::vector<larlight::cluster>& GetClusterCollection() const {return fCluster_v;};
    
    /// Default destructor
    ~event_cluster(){};

    /**
       A utility function to obtain maximum & minimum of clusters' hits' vertex
       NOTE: the initial max/min values matter. It only modifies max/min if it finds values that is
       above/below those initial values.
    */
    void get_axis_range (double &xmax, double &xmin,
			 double &ymax, double &ymin,
			 double &zmax, double &zmin) const;
    
  private:
    
    std::vector<cluster> fCluster_v;
    
    ////////////////////////
    ClassDef(event_cluster,1)
    ////////////////////////
      };
};
#endif
  
  /** @} */ // end of doxygen group 
  
