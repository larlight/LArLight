/**
 * \file TrackSubtractorAlg.hh
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class TrackSubtractorAlg
 *
 * @author cadams
 *
 * How to use this algorithm...
 *
 * 1) Load all of the hits from a plane into the alg by calling 
 *    LoadAllHits(hits, plane).  It's on you to get the hits and 
 *    make sure there aren't multiple sets (like from multiple 
 *    hit finders)
 * 2) Externally, loop over the cluster and get their associated 
 *    hits and add them via AddCluster(cluster, hits). It's expected
 *    that these are the same hits as above in (1) - this method only
 *    grabs the hits associated with the cluster you passed it.
 *    2a) For each cluster added, it decides if this cluster is clearly a
 *        track and if so adds its hits to track_hits
 * 3) Lastly, loop over all hits and compare to track_hits.  Any hits that
 *    are in all_hits but not in track_hits go into remaining_hits
 * 4) You can extract the remaining hits via GetRemainingHits().  Since 
 *    this class doesn't own the hits, presumably you are getting back 
 *    pointers to hits that you already own.  Fair warning.
 *
 * This class is meant to run on the output of something like cluster
 * crawler, because that is a track finder.  Main criteria for deciding
 * something is obviously a track are:
 *   Number of hits > 30
 *   ChargeRMS / Mean Charge < 0.5...?
 */


#ifndef TRACKSUBTRACTORALG
#define TRACKSUBTRACTORALG value

#include "ana_base.hh"
#include "Geometry.hh"
#include "RecoUtilException.hh"
#include <TH2D.h>
#include "LArUtilManager.hh"
#include "ClusterParamsAlgNew.hh"


namespace clusteralg {

  class TrackSubtractorAlg
  {
  public:
    // TrackSubtractorAlg();

    // virtual ~TrackSubtractorAlg();
 
    void Initialize(); 

    int LoadAllHits(const larlight::event_hit *hits, const UChar_t plane_id);

    int AddCluster (const UChar_t plane_id,
                    const larlight::cluster &i_cluster,
                    const larlight::event_hit *hits);

    int ResolveHits();
    std::vector<const larlight::hit*> GetRemainingHits(){
      return remaining_hits;
    }

    TH2D * GetAllHitsHist(){return all_hitsHIST;}
    TH2D * GetTrackHitsHist(){return track_hitsHIST;}
    TH2D * GetClustHitsHist(){return cluster_hitsHIST;}

    int GetRemainingHitsNumber();
    double GetPercentTracks();

    bool FailsCuts();

    void LoopClusters(const UChar_t plane_id,
                      const std::vector<larlight::cluster> & cluster_v,
                      const larlight::event_hit *hits);


    void FillHists();

  protected:
    
    bool isTrackLike(const larlight::cluster &i_cluster,
                     std::vector<const larlight::hit*> & hits);

    // Used for drawing help
    larutil::GeometryUtilities  *fGSer;

    cluster::ClusterParamsAlgNew  fCPAN;

    // As far as I know, this class does not take ownership of any hits.
    // It simply takes pointers to hits and returns the ones that either
    // are or are not associated with good tracks.

    // Containers for the hits.  Using pointers and not afraid to duplicate
    // addressed, because I think it's useful.
    std::vector<const larlight::hit*> all_hits;
    // The hits that I deem are associated with tracks and should be subtracted:
    std::vector<const larlight::hit*> track_hits;
    // The hits that are remaining (not tracks)
    std::vector<const larlight::hit*> remaining_hits;

    // hits associated with clusters are stored here
    std::vector< std::vector <const larlight::hit*> > cluster_hits;
    // this is where the clusters are stored
    std::vector<larlight::cluster> cluster_vec;


    TH2D * all_hitsHIST;
    TH2D * track_hitsHIST;
    TH2D * cluster_hitsHIST;

    /* data */
  };


}





#endif