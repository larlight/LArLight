#ifndef CLUSTER_HH
#define CLUSTER_HH
/**
 * \file cluster.hh
 *
 * \ingroup SimpleTreeData
 * 
 * @author Kazu - Nevis 2013
 */

/** \addtogroup SimpleTreeData

    @{*/

#include "data_base.hh"

/**
   \class cluster
   LArSoft Cluster class equivalent data container. It contains C-array of cluster information (1 entry per cluster).
*/
class cluster : public data_base {

public:

  /// Default constructor
  cluster(DATA::DATA_TYPE type=DATA::Cluster);

  /// Default destructor
  virtual ~cluster(){};

  /**
     Implementation data_base::clear_event. 
     When the argument boolean "all" is set to true, clear ALL C-array data elements to the default value.
     When "all" is set to false (default), only filled entries are cleared.
     This way we save time to loop over array elements.
  */
  virtual void clear_event(bool all=false);

  /// Implementation of track data address setter
  virtual void set_address(TTree* t);

  /// Setter method to append a cluster
  void add_cluster(UShort_t clusterID, UChar_t view,
		   Double_t charge,
		   Double_t dtdw,   Double_t dtdw_sigma,
		   Double_t dqdw,   Double_t dqdw_sigma,
		   Float_t  startx, Float_t starty, Float_t startz,
		   Float_t  startx_sigma, Float_t starty_sigma, Float_t startz_sigma,
		   Float_t  endx,   Float_t endy,   Float_t endz,
		   Float_t  endx_sigma,   Float_t endy_sigma,   Float_t endz_sigma);

  //--- Getter methods ---//
  UShort_t        num_clusters()  const { return _num_clusters;  };
  const UShort_t* clusterID()    const { return _clusterID;    };
  const UChar_t*  view()         const { return _view;         };
  const Double_t* charge()       const { return _charge;       };
  const Double_t* dtdw()         const { return _dtdw;         };
  const Double_t* dtdw_sigma()   const { return _dtdw_sigma;   };
  const Double_t* dqdw()         const { return _dqdw;         };
  const Double_t* dqdw_sigma()   const { return _dqdw_sigma;   };
  const Float_t*  startx()       const { return _startx;       };
  const Float_t*  starty()       const { return _starty;       };
  const Float_t*  startz()       const { return _startz;       };
  const Float_t*  startx_sigma() const { return _startx_sigma; };
  const Float_t*  starty_sigma() const { return _starty_sigma; };
  const Float_t*  startz_sigma() const { return _startz_sigma; };
  const Float_t*  endx()         const { return _endx;         };
  const Float_t*  endy()         const { return _endy;         };
  const Float_t*  endz()         const { return _endz;         };
  const Float_t*  endx_sigma()   const { return _endx_sigma;   };
  const Float_t*  endy_sigma()   const { return _endy_sigma;   };
  const Float_t*  endz_sigma()   const { return _endz_sigma;   };  

protected:

  UShort_t _num_clusters;                      ///< Number of clusters
  UShort_t _clusterID[DATA::kMaxClusters];    ///< Cluster ID 
  UChar_t  _view[DATA::kMaxClusters];         ///< View_t key in LArSoft
  Double_t _charge[DATA::kMaxClusters];       ///< Charge sum
  Double_t _dtdw[DATA::kMaxClusters];         ///< dt/dw ... d(time)/d(wire)
  Double_t _dtdw_sigma[DATA::kMaxClusters];   ///< dt/dw sigma
  Double_t _dqdw[DATA::kMaxClusters];         ///< dq/dw ... d(charge)/d(wire)
  Double_t _dqdw_sigma[DATA::kMaxClusters];   ///< dq/dw sigma
  Float_t  _startx[DATA::kMaxClusters];       ///< Starting vtx X coordinate
  Float_t  _starty[DATA::kMaxClusters];       ///< Starting vtx Y coordinate
  Float_t  _startz[DATA::kMaxClusters];       ///< Starting vtx Z coordinate
  Float_t  _startx_sigma[DATA::kMaxClusters]; ///< Starting vtx X coordinate sigma
  Float_t  _starty_sigma[DATA::kMaxClusters]; ///< Starting vtx Y coordinate sigma
  Float_t  _startz_sigma[DATA::kMaxClusters]; ///< Starting vtx Z coordinate sigma
  Float_t  _endx[DATA::kMaxClusters];         ///< Ending vtx X coordinate
  Float_t  _endy[DATA::kMaxClusters];         ///< Ending vtx Y coordinate
  Float_t  _endz[DATA::kMaxClusters];         ///< Ending vtx Z coordinate
  Float_t  _endx_sigma[DATA::kMaxClusters];   ///< Ending vtx X coordinate sigma
  Float_t  _endy_sigma[DATA::kMaxClusters];   ///< Ending vtx Y coordinate sigma
  Float_t  _endz_sigma[DATA::kMaxClusters];   ///< Ending vtx Z coordinate sigma

};

#endif

/** @} */ // end of doxygen group 
