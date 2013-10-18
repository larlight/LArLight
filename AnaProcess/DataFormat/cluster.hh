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

/**
   \class cluster
   LArSoft Cluster class equivalent data container
*/
class cluster : public data_base {

public:

  /// Default constructor
  cluster() : data_base() {};

  /// Default destructor
  virtual ~cluster(){};

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

  virtual void clear_data(){
    fTotalCharge = -1;
    fdTdW = fdQdW = fSigmadTdW = fSigmadQdW = -1;
    fStartPos.clear();
    fEndPos.clear();
    fSigmaStartPos.clear();
    fSigmaEndPos.clear();
    fID = -1;
    fView = GEO::kUnknown;
  }

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
  int                        fID;             ///< cluster's ID
  GEO::View_t                fView;           ///< View associated w/ this cluster

  ////////////////////////
  ClassDef(cluster,1)
  ////////////////////////

};

/**
 \class cluster_collection 
 A collection storage class of multiple clusters.
*/
class cluster_collection : public std::vector<cluster>,
			   public data_base {
  
public:

  /// Default constructor
  cluster_collection() : data_base() {clear_data();};

  /// Default copy constructor
  cluster_collection(const cluster_collection& original)
    : std::vector<cluster>(original),
      data_base(original)
  {};

  /// Method to clear currently held data contents in the buffer
  virtual void clear_data(){clear();};

  /// Default destructor
  ~cluster_collection(){};


private:

  ////////////////////////
  ClassDef(cluster_collection,1)
  ////////////////////////
};

#endif

/** @} */ // end of doxygen group 
