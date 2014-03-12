// This class is meant to pull together all of the parameters 
// that are fed into an Artifical Neural Network used for track/shower
// discrimination.
// 
// The input is designed to be a cluster and it's associated hitlist
// 
// The class also handles the acrobatics of formatting the data
// to feed to the ANN, which in the end is going to be a string 
// that contains the numerical inputs, separated by spaces
// 
// The string format is useful because that is the easiest way to
// train the ANN, though perhaps for running it won't be necessary.
// 
// 
// 

#ifndef FANNPARAMSALG_H
#define FANNPARAMSALG_H 

// // Larsoft includes:
// #include "RecoBase/Hit.h"
// #include "RecoBase/Cluster.h"

// LArLight includes:
#include "cluster.hh"
#include "hit.hh"

// #include "art/Persistency/Common/Ptr.h" 

// Root includes:
#include "TPrincipal.h"

// Standard type includes:
#include <vector>
#include <ostream>
#include <iostream>

namespace cluster{

  class FANNParamsAlg
  {
  public:
    // takes a cluster and its associated hitlist
    FANNParamsAlg();
    ~FANNParamsAlg();

    /**
     * takes a cluster and its associated hit list and 
     * calculates everything needed for the feature vector
     */
    void FillParams(larlight::cluster &, std::vector<larlight::hit> &);
    void ResetParams();

    //friend std::ostream & operator << (std::ostream &, const FANNParamsAlg &);

    /**
     * returns a string of the feature vector, enumerated below
     * purely a "getter"
     * @return [string of features, deliminated by spaces ]
     */
    const std::string FeatureVector() const;
    const std::string TruthInformation() const;
    void GetData(float *) const;
    void Report() const;


    void SetConversion(float wireToCM, float timeToCM);

  private:

    // Most certainly need an axis for this.
    // also need a start point and an endpoint
    // can probably pull all of that together from some PCA?
    // Here is the root object that does PCA
    TPrincipal * principal;

    // This list enumerates the items in the feature vector, in order.
    // They must all be floats!
    float N_Hits;                // Number of hits in this cluster
    float axis_dist;             // Length along the assumed axis of the cluster
    float width;                 // Width of cluster, perp to the axis above
    float opening_angle;         // angle which contains some (high) percent of the hits
                                 //     from the start point of the cluster
    float closing_angle;         // same as above, but from the end point of the shower
    float opening_angle_HH;      // as above, but only counting hits with charge 
                                 //     above a threshold.
    float closing_angle_HH;      // As above, but only hits with charge above thresh
    float hit_density_1D;        // Number of hits per length, collapsing into 1D
    float hit_density_2D;        // Number of hits divided by (length*width)
    float end_x;                 // All showers are shifted to put the "start" at (0,0)
    float end_y;                 // this and previous are the x, y points in cm, cm space
    float mean_x;                // the location of the center point, simply using hit peaks
    float mean_y;                // the same
    float mean_x_charge_wgt;     // the center, but weighing each point by charge
    float mean_y_charge_wgt;     // the center, but weighing each point by charge
    float multi_hit_wires;       // the number of wires with more than one hit
    float N_Wires;               // the number of wires with hits in this cluster
    float eigenvalue_principal;  // the principal eigenvalue from PCA
    float eigenvalue_secondary;  // the secondary eigenvalue from PCA


    // Need to get the truth information to train the alg, so have a spot for it:
    bool isShower;
    bool isBackwards;

    // Some things about this cluster that get picked up:
    float start_x_real;
    float start_y_real;
    float end_x_real;
    float end_y_real;

    // The start points above are the real cm-cm space points, whereas the
    // points passed to the FANN are such that the startx,y is zero:
    // end_x = real_end_x - start_x; for example

    // Need the conversions to get from time and wire to cm-cm space:
    float fWireToCM;
    float fTimeToCM;

  public:
    // One last, long winded function to help get the params:
    void GetData(float& _N_Hits, float& _axis_dist, float& _width, 
                 float& _opening_angle, float& _closing_angle, 
                 float& _opening_angle_HH, float& _closing_angle_HH, 
                 float& _hit_density_1D, float& _hit_density_2D, 
                 float& _end_x, float& _end_y, float& _mean_x, float& _mean_y, 
                 float& _mean_x_charge_wgt, float& _mean_y_charge_wgt, 
                 float& _multi_hit_wires, float& _N_Wires, 
                 float& _eigenvalue_principal, float& _eigenvalue_secondary);

  };
}


#endif



