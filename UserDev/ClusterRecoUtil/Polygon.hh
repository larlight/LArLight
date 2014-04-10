#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <utility>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctime>

//a polygon is a vector of std::pairs with first = x coordinate
//and second = y coordinate of that vertex
//access vertices with Point function. Points are:
//0   = first ordered vertex
//n-1 = last ordered vertex (n=size of polygon)
//n   = first vertex again
//>n  = invalid...return error message

//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//                BEGIN POLYGON CLASS               //
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
class Polygon{
  
private:
  std::vector< std::pair<float,float> > vertices;
  
 public:
  Polygon(std::vector< std::pair<float,float> > points) { vertices = points; }
  unsigned int Size() { return vertices.size(); } 
  std::pair<float,float> Point(unsigned int p); 
  std::pair<float,float> Project(std::pair<float,float>,float);
  float Area();
  bool Overlap(float slope, Polygon poly2, std::pair<float,float> origin);
  bool PolyOverlap(Polygon poly2);
  bool PolyOverlapSegments(Polygon poly2);
  bool PointInside(std::pair<float,float> point);
};


#endif
