#ifndef FANNPARAMSALG_CC
#define FANNPARAMSALG_CC

#include "FANNParamsAlg.hh"
#include "math.h"

#include "TString.h"

#include <map>

larreco::FANNParamsAlg::FANNParamsAlg(){
  // Initialize the Principal Component Analysis object
  principal = new TPrincipal(2);
}

larreco::FANNParamsAlg::~FANNParamsAlg(){
  //empty destructor
  // Not sure if I need to delete the root object.  Cause root 
  // sucks like that.  I'll assume yes:
  delete principal;
}

/**
 * takes a cluster and its associated hit list and 
 * calculates everything needed for the feature vector
 */

void larreco::FANNParamsAlg::FillParams(larlight::cluster & cluster,
                                        std::vector<larlight::hit> & hitlist){

  // Here is the method:
  // First, make sure the axis of the cluster is correct!  Or if it hasn't 
  // yet been calc'd, find it.  Once we have the axis, make sure the start
  // and end points are OK.
  
  // Don't forget to reset all the parameters:
  ResetParams();
  
  
  N_Hits = hitlist.size();

  // -> Going to strip off the extra information fromt the hits and just make
  //    two arrays of floats to make this lightweight.
  // -> During this loop I also add the point to the PCA object
  // -> FURTHER, during this loop I accumulate a map of wire number and hits
  //    This can tell me the total number of wires used, and how many are 
  //    multihit wires
  // -> Also, accumulate the charge per hit and find out the mean charge
  // -> Want to know the center of charge as well as the center of hits
  //    So, during this loop accumulate charge* position to find the
  //    center of charge in both wire and time space
  std::map<unsigned int, unsigned int> wireHitMap;
  std::vector<float> x_points, y_points, charge;
  double mean_charge = 0;
  x_points.resize(N_Hits);
  y_points.resize(N_Hits);
  charge.resize(N_Hits);
  // std::cout << "N Hits is " << N_Hits << std::endl;
  for (unsigned int i = 0; i < N_Hits; i++){
    // std::cout << "i is " << i << std::endl;
    // std::cout << "This hit is " << (*hitlist.at(i)) << std::endl;
    // Convert the wire and time to cm, cm space
    x_points[i] = hitlist.at(i).Wire() * fWireToCM;
    y_points[i] = hitlist.at(i).PeakTime() * fTimeToCM;
    // get the charge too:
    charge[i]  = hitlist.at(i).Charge(); 
    mean_charge += charge[i];
    mean_x_charge_wgt += x_points[i]*charge[i];
    mean_y_charge_wgt += y_points[i]*charge[i];
    // The map will index/sort automatically, so just index it with the wire
    wireHitMap[hitlist.at(i).Wire()] ++;
    // This adds the point to the PCA object
    double point[2];
    point[0] = x_points[i];
    point[1] = y_points[i];
    principal->AddRow(point);
  }


  // Calculate the number of multihit wires:
  for (auto & it : wireHitMap)
  {
    if (it.second > 1) multi_hit_wires ++;
  }

  // Finish the calculation of mean x,y charge weighted:
  mean_x_charge_wgt /= mean_charge;  // At the momement, mean_charge 
  mean_y_charge_wgt /= mean_charge;  // is total charge!
  mean_charge /= N_Hits;

  // Actually do the computation for PCA
  principal->MakePrincipals();
  mean_x = (*principal -> GetMeanValues())[0];
  mean_y = (*principal -> GetMeanValues())[1];
  //std::vector<double> principal_Axis = 
  double principal_Axis[2] = 
    {(*principal -> GetEigenVectors())[0][0],
     (*principal -> GetEigenVectors())[0][1]};
  //std::vector<double> secondary_Axis = 
  double secondary_Axis[2] = 
    {(*principal -> GetEigenVectors())[1][0],
     (*principal -> GetEigenVectors())[1][1]};


  // Next need to determine the start and end points
  // In principal, it should be hits closest to the principal
  // axis that are farthest along the principal axis.
  // In reality, it might not be this.  So maybe I'll use
  // the closest hits with high charge that are within some
  // distance to the axis.
  // 
  // For now, I'm punting and grabbing the variables stored in the cluster.
  start_x_real = cluster.StartPos()[0] * fWireToCM;
  start_y_real = cluster.StartPos()[1] * fTimeToCM;
  end_x_real   = cluster.EndPos()[0]   * fWireToCM;
  end_y_real   = cluster.EndPos()[1]   * fTimeToCM;

  // Next we want to fill in the angle variables
  // Basically, start by looping over hits and calculating the angle between
  // that hit and the start/end points, compared to the Principal Axis.
  
  // Compute a few values that get reused a lot in this loop:
  float front_x_axis = (mean_x - start_x_real);
  float back_x_axis  = (end_x_real - mean_x);
  float front_y_axis = (mean_y - start_y_real);
  float back_y_axis  = (end_y_real - mean_y);
  float front_norm  = sqrt(front_x_axis*front_x_axis + front_y_axis*front_y_axis);
  float back_norm   = sqrt(back_x_axis*back_x_axis   + back_y_axis*back_y_axis);

  // Need to accumulate the values of cos_th found
  // could use a histogram, but going to skip that to avoid overhead.
  // instead, use a vector, and take care of binning on the fly
  std::vector<float> front_angle, back_angle; //bins cos_th
  std::vector<float> front_angle_HH, back_angle_HH; //bins cos_th with High charge hits

  // Binning the angles in 1 deg increments, but at 90+ deg just one overflow bin
  int nbins = 91;
  front_angle.resize(nbins);
  back_angle.resize(nbins);
  front_angle_HH.resize(nbins);
  back_angle_HH.resize(nbins);

  // Also during this loop, going to find the hits that are farthest along
  // the perpendicular direction and axis direction.
  // store the greatest extents in these directions (from mean)
  float width_max(0.0);
  float width_min(0.0);
  float length_max(0.0);
  float length_min(0.0);

  double increment = 1.0/N_Hits;

  // std::cout << "start points are x: " << start_x_real << ", y: "<< start_y_real << std::endl;


  for (int i = 0; i < N_Hits; i++){
    // std::cout << "This point is at x: " <<x_points[i] << ", y: " << y_points[i] << std::endl;
    // First, find the angle between this hit and the start point
    // This is a formula follows from the dot product
    float front_cos_th = front_x_axis*(x_points[i] - start_x_real);
    front_cos_th += front_y_axis*(y_points[i] - start_y_real);
    front_cos_th /= front_norm;
    // Need to catch the case that this point is the start point
    if (start_x_real == x_points[i] && start_y_real == y_points[i]){
      front_cos_th = 1;
    }
    else{
      front_cos_th /= sqrt(pow((start_x_real - x_points[i]),2) 
                     + pow((start_y_real - y_points[i]),2));
    }
    float back_cos_th = back_x_axis*(end_x_real - x_points[i]);
    back_cos_th += back_y_axis*(end_y_real - y_points[i]);
    back_cos_th /= back_norm;
    // similarly, catch the case that this point is the end point
    if (end_x_real == x_points[i] && end_y_real == y_points[i]){
      back_cos_th = 1;
    }
    else{
      back_cos_th /= sqrt(pow((end_x_real - x_points[i]),2) 
                    + pow((end_y_real - y_points[i]),2));
    }
    float front_th;
    float back_th;
    if (front_cos_th >= 0.999999) front_th = 0.0;
    else
      front_th = acos(front_cos_th)*(180/3.14159);
    if (back_cos_th >= 0.999999) back_th = 0.0;
    else
      back_th= acos(back_cos_th )*(180/3.14159);

    // std::cout << "\tfront_cos_th is " <<  front_cos_th  << std::endl;
    // std::cout << "\tfront_th is " << front_th << std::endl;
    // std::cout << "\tback_cos_th is " << back_cos_th   << std::endl;
    // std::cout << "\tback_th is "  <<  back_th << std::endl;

    // The cos_th angles should now be between 0 and 1.
    int front_index = front_th; 
    int back_index  = back_th; 
    // std::cout << "\tgoing to access at (front) " << front_index << std::endl;
    // std::cout << "\tgoing to access at (back)  "  << back_index << std::endl;
    if (front_index < nbins - 1) 
      front_angle[front_index] += increment;
    else 
      front_angle.back() += increment;
    if (back_index < nbins - 1)
      back_angle [ back_index ] += increment;
    else back_angle.back() += increment;     

    if (charge[i] > mean_charge) {
      if (front_index < nbins - 1) 
        front_angle_HH[front_index] += increment;
      else 
        front_angle_HH.back() += increment;
      if (back_index < nbins - 1)
        back_angle_HH [ back_index ] += increment;
      else back_angle_HH.back() += increment;  
    }

    // Do the computation for width, length min and max
    float this_width  = (mean_x - x_points[i])*secondary_Axis[0] 
                       +(mean_y - y_points[i])*secondary_Axis[1];
    float this_length = (x_points[i] - mean_x)*principal_Axis[0] 
                       +(y_points[i] - mean_y)*principal_Axis[1];
    // Replace the bounds, if needed:
    if (this_width  < width_min){
        // std::cout << "Replacing current width (" << width_min << ") with "
        //           << this_width << std::endl;
        width_min  = this_width;
    }
    if (this_width  > width_max){
        // std::cout << "Replacing current width (" << width_max << ") with "
                  // << this_width << std::endl;
        width_max  = this_width;
    }
    if (this_length < length_min) length_min = this_length;
    if (this_length > length_max) length_max = this_length;
  }
  // std::cout << "width max and min are: (" << width_max << ", " 
            // << width_min << ")" << std::endl;
  // std::cout << "length max and min are: (" << length_max << ", " 
            // << length_min << ")" << std::endl;

  // Lastly, go through and find the spot where 90% of the hits 
  // are contained within that cos_th for front and back:
  float front_accum    = front_angle[nbins-1]; bool found_front_angle    = false;
  float front_accum_HH = front_angle_HH[nbins-1]; bool found_front_angle_HH = false;
  float back_accum     = back_angle[nbins-1]; bool found_back_angle     = false;
  float back_accum_HH  = back_angle_HH[nbins-1]; bool found_back_angle_HH  = false;
  float threshold = 0.1;

  for ( int i = nbins-2; i != 0; i-- )
  {
    front_accum    += front_angle[i];
    front_accum_HH += front_angle_HH[i];
    back_accum     += back_angle[i];
    back_accum_HH  += back_angle_HH[i];
    if (!found_front_angle && front_accum >= threshold){
      opening_angle = i;
      found_front_angle = true;
    }
    if (!found_front_angle_HH && front_accum_HH >= threshold){
      opening_angle_HH = i;
      found_front_angle_HH = true;
    }
    if (!found_back_angle && back_accum >= threshold){
      closing_angle = i;
      found_back_angle = true;
    }
    if (!found_back_angle_HH && back_accum_HH >= threshold){
      closing_angle_HH = i;
      found_back_angle_HH = true;
    }

    if (found_front_angle && found_back_angle && 
        found_front_angle_HH && found_back_angle_HH)
    {
      break;
    }
  }



  // Now start filling in the variables:
  // N_Hits - already filled
  axis_dist = fabs(length_max - length_min);
  width = fabs(width_max - width_min);
  // opening_angle; - all filled above
  // closing_angle;
  // opening_angle_HH;
  // closing_angle_HH;
  hit_density_1D = 1.0*N_Hits/axis_dist;
  hit_density_2D = 1.0*N_Hits/(axis_dist*width);
  end_x = end_x_real - start_x_real;
  end_y = end_y_real - start_y_real;
  mean_x -= start_x_real;
  mean_y -= start_y_real;
  mean_x_charge_wgt -= start_x_real;
  mean_y_charge_wgt -= start_y_real;
  // multi_hit_wires - computed above
  N_Wires = wireHitMap.size();
  eigenvalue_principal = (*principal->GetEigenValues())[0];
  eigenvalue_secondary = (*principal->GetEigenValues())[1];

  return;
}

/**
 * returns a string of the feature vector, enumerated below
 * purely a "getter"
 * @return [string of features, deliminated by spaces ]
 */
const std::string larreco::FANNParamsAlg::FeatureVector() const{

  TString featureVector;
  featureVector += N_Hits;
  featureVector += " ";
  featureVector += axis_dist;
  featureVector += " ";
  featureVector += width;
  featureVector += " ";
  featureVector += opening_angle;
  featureVector += " ";
  featureVector += closing_angle;
  featureVector += " ";
  featureVector += opening_angle_HH;
  featureVector += " ";
  featureVector += closing_angle_HH;
  featureVector += " ";
  featureVector += hit_density_1D;
  featureVector += " ";
  featureVector += hit_density_2D;
  featureVector += " ";
  featureVector += end_x;
  featureVector += " ";
  featureVector += end_y;
  featureVector += " ";
  featureVector += mean_x;
  featureVector += " ";
  featureVector += mean_y;
  featureVector += " ";
  featureVector += mean_x_charge_wgt;
  featureVector += " ";
  featureVector += mean_y_charge_wgt;
  featureVector += " ";
  featureVector += multi_hit_wires;
  featureVector += " ";
  featureVector += N_Wires;
  featureVector += " ";
  featureVector += eigenvalue_principal;
  featureVector += " ";
  featureVector += eigenvalue_secondary;
  featureVector += " ";
  featureVector += " \n";

  // std::cout << featureVector.Data() << std::endl; 
  std::string s(featureVector.Data());
  return s;

}
const std::string larreco::FANNParamsAlg::TruthInformation() const{
  std::string s = "Wrong!";
  return s;
}

void larreco::FANNParamsAlg::SetConversion(float wireToCM, float timeToCM){
  fWireToCM = wireToCM;
  fTimeToCM = timeToCM;
}
void larreco::FANNParamsAlg::GetData(float * data) const{

  int i = 0;
  data[i] = N_Hits;  i++;
  data[i] = axis_dist;  i++;
  data[i] = width;  i++;
  data[i] = opening_angle;  i++;
  data[i] = closing_angle;  i++;
  data[i] = opening_angle_HH;  i++;
  data[i] = closing_angle_HH;  i++;
  data[i] = hit_density_1D;  i++;
  data[i] = hit_density_2D;  i++;
  data[i] = end_x;  i++;
  data[i] = end_y;  i++;
  data[i] = mean_x;  i++;
  data[i] = mean_y;  i++;
  data[i] = mean_x_charge_wgt;  i++;
  data[i] = mean_y_charge_wgt;  i++;
  data[i] = multi_hit_wires;  i++;
  data[i] = N_Wires;  i++;
  data[i] = eigenvalue_principal;  i++;
  data[i] = eigenvalue_secondary;  //i++;

  return;
}

void larreco::FANNParamsAlg::GetData(
                float& _N_Hits, float& _axis_dist, float& _width, 
                float& _opening_angle, float& _closing_angle, 
                float& _opening_angle_HH, float& _closing_angle_HH, 
                float& _hit_density_1D, float& _hit_density_2D, 
                float& _end_x, float& _end_y, float& _mean_x, float& _mean_y, 
                float& _mean_x_charge_wgt, float& _mean_y_charge_wgt, 
                float& _multi_hit_wires, float& _N_Wires, 
                float& _eigenvalue_principal, float& _eigenvalue_secondary)
{
  _N_Hits = N_Hits;
  _axis_dist = axis_dist;
  _width = width;
  _opening_angle = opening_angle;
  _closing_angle = closing_angle;
  _opening_angle_HH = opening_angle_HH;
  _closing_angle_HH = closing_angle_HH;
  _hit_density_1D = hit_density_1D;
  _hit_density_2D = hit_density_2D;
  _end_x = end_x;
  _end_y = end_y;
  _mean_x = mean_x;
  _mean_y = mean_y;
  _mean_x_charge_wgt = mean_x_charge_wgt;
  _mean_y_charge_wgt = mean_y_charge_wgt;
  _multi_hit_wires = multi_hit_wires;
  _N_Wires = N_Wires;
  _eigenvalue_principal = eigenvalue_principal;
  _eigenvalue_secondary = eigenvalue_secondary;
  return;
}

void larreco::FANNParamsAlg::Report() const{
  std::cout << "N_Hits: " << N_Hits << "\n"
            << "axis_dist: " << axis_dist << "\n"
            << "width: " << width << "\n"
            << "opening_angle: " << opening_angle << "\n"
            << "closing_angle: " << closing_angle << "\n"
            << "opening_angle_HH: " << opening_angle_HH << "\n"
            << "closing_angle_HH: " << closing_angle_HH << "\n"
            << "hit_density_1D: " << hit_density_1D << "\n"
            << "hit_density_2D: " << hit_density_2D << "\n"
            << "end_x: " << end_x << "\n"
            << "end_y: " << end_y << "\n"
            << "mean_x: " << mean_x << "\n"
            << "mean_y: " << mean_y << "\n"
            << "mean_x_charge_wgt: " << mean_x_charge_wgt << "\n"
            << "mean_y_charge_wgt: " << mean_y_charge_wgt << "\n"
            << "multi_hit_wires: " << multi_hit_wires << "\n"
            << "N_Wires: " << N_Wires << "\n"
            << "eigenvalue_principal: " << eigenvalue_principal << "\n"
            << "eigenvalue_secondary: " << eigenvalue_secondary << "\n";
}

void larreco::FANNParamsAlg::ResetParams(){

  principal -> Clear();

  N_Hits = 0.0;
  axis_dist = 0.0;
  width = 0.0;
  opening_angle = 0.0;
  closing_angle = 0.0;
  opening_angle_HH = 0.0;
  closing_angle_HH = 0.0;
  hit_density_1D = 0.0;
  hit_density_2D = 0.0;
  end_x = 0.0;
  end_y = 0.0;
  mean_x = 0.0;
  mean_y = 0.0;
  mean_x_charge_wgt = 0.0;
  mean_y_charge_wgt = 0.0;
  multi_hit_wires = 0.0;
  N_Wires = 0.0;
  eigenvalue_principal = 0.0;
  eigenvalue_secondary = 0.0;
}

/*
namespace cluster{
  

  std::ostream& operator<<(std::ostream & os, const FANNParamsAlg & FPA) {

    
    os << "N_Hits: " << FPA.N_Hits << "\n";
    os << "axis_dist: " << FPA.axis_dist << "\n";
    os << "width: " << FPA.width << "\n";
    os << "opening_angle: " << FPA.opening_angle << "\n";
    os << "closing_angle: " << FPA.closing_angle << "\n";
    os << "opening_angle_HH: " << FPA.opening_angle_HH << "\n";
    os << "closing_angle_HH: " << FPA.closing_angle_HH << "\n";
    os << "hit_density_1D: " << FPA.hit_density_1D << "\n";
    os << "hit_density_2D: " << FPA.hit_density_2D << "\n";
    os << "end_x: " << FPA.end_x << "\n";
    os << "end_y: " << FPA.end_y << "\n";
    os << "mean_x: " << FPA.mean_x << "\n";
    os << "mean_y: " << FPA.mean_y << "\n";
    os << "mean_x_charge_wgt: " << FPA.mean_x_charge_wgt << "\n";
    os << "mean_y_charge_wgt: " << FPA.mean_y_charge_wgt << "\n";
    os << "multi_hit_wires: " << FPA.multi_hit_wires << "\n";
    os << "N_Wires: " << FPA.N_Wires << "\n";
    os << "eigenvalue_principal: " << FPA.eigenvalue_principal << "\n";
    os << "eigenvalue_secondary: " << FPA.eigenvalue_secondary << "\n";

    return os;
  }

}
*/
#endif







