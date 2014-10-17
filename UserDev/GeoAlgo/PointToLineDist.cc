#ifndef POINTTOLINEDIST_CC
#define POINTTOLINEDIST_CC

#include "PointToLineDist.hh"

double PointToLineDist::Distance(){

  if ( (_point.size() == 0) or (_segmentStart.size() == 0) or (_segmentEnd.size() == 0) ){
    std::cout << "Error: point and segment not initialized." << std::endl;
    return 0;
  }

  if ( (_point.size() != 3) or (_segmentStart.size() != 3) or (_segmentEnd.size() != 3) ){
    std::cout << "Error: Did not provide 3D points." << std::endl;
    return 0;
  }

  //Distance Algorithm: see "Real-Time Collision Analysis" book, section 5.1.2.1

  std::vector<float> line; // End-Start
  std::vector<float> pToStart; // point - Start
  std::vector<float> pToEnd;  // point - End
  for (size_t i=0; i < _point.size(); i++){
    line.push_back(_segmentEnd.at(i)-_segmentStart.at(i));
    pToStart.push_back(_point.at(i)-_segmentStart.at(i));
    pToEnd.push_back(_point.at(i)-_segmentEnd.at(i));
  }

  float e = DotProduct(pToStart,pToEnd);
  float segLenSquared = DotProduct(line,line);
  if (e <= 0.) return sqrt(DotProduct(pToStart,pToStart));
  else if (e > segLenSquared) return sqrt(DotProduct(pToEnd,pToEnd));
  else return sqrt( DotProduct(pToStart,pToStart)-e*e/segLenSquared );

}


double PointToLineDist::DotProduct(std::vector<float> A, std::vector<float> B){
    
    if (A.size() != B.size()){
      std::cout << "Error: Did not provide equal-dimension points" << std::endl;
      return 0;
    }

    double dotProd = 0;
    for (size_t i=0; i < A.size(); i++)
      dotProd += A.at(i)*B.at(i);

    return dotProd;
  }

#endif
