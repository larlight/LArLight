#include <vector>
#include <utility>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>

class Polygon{

private:
  std::vector< std::pair<float,float> > vertices;
  
 public:
  Polygon(std::vector< std::pair<float,float> > points) { vertices = points; }
  unsigned int Size() { return vertices.size(); }
  std::pair<float,float> Point(unsigned int p) { return vertices.at(p); }
  Polygon Translate(std::pair<float,float>);
  Polygon Rotate(float);
};



Polygon Polygon::Translate(std::pair<float,float> p){
  std::vector< std::pair<float,float> > newvertices;
  std::pair<float,float> ptmp;
  for (unsigned int i=0; i<vertices.size(); i++){
    ptmp = std::make_pair(   (vertices.at(i)).first - p.first  ,   (vertices.at(i)).second - p.second   );
    newvertices.push_back(ptmp);
  }
  return Polygon(newvertices);
}

//possibly cut time by looking only at x value after this since that is all that matters
Polygon Polygon::Rotate(float theta){
  std::vector< std::pair<float,float> > newvertices;
  std::pair<float,float> ptmp;
  for (unsigned int i=0; i<vertices.size(); i++){
    ptmp = std::make_pair(   ((vertices.at(i)).first)*cos(theta) + ((vertices.at(i)).second)*sin(theta)  , -((vertices.at(i)).first)*sin(theta) + ((vertices.at(i)).second)*cos(theta)   );
    newvertices.push_back(ptmp);
  }
  return Polygon(newvertices);
}


//*********************************************
float FindSlope( std::pair<float,float> p1, std::pair<float,float> p2 ){
  float slope = (p2.second-p1.second)/(p2.first-p1.first);
  return slope;
}


std::pair<float,float> Range(Polygon poly){

  std::pair<float,float> range(10000,-10000);
  for (unsigned int i=0; i<poly.Size(); i++){
    if ( (poly.Point(i)).first < range.first )
      range.first = (poly.Point(i)).first;
    if ( (poly.Point(i)).first > range.second )
      range.second = (poly.Point(i)).first;
  }
  return range;
}
    
bool Overlap(float slope, Polygon poly1, Polygon poly2, std::pair<float,float> origin){
  //translate and rotate both polygons
  float theta = tan(slope);
  Polygon poly1new = (poly1.Translate(origin)).Rotate(theta);
  Polygon poly2new = (poly2.Translate(origin)).Rotate(theta);
  //now x-coordinates of all points in polygono are with
  //respect to slope in consideration. Find range for 
  //each polygon and see if they intersect!
  std::pair<float,float> range1 = Range(poly1new);
  std::pair<float,float> range2 = Range(poly2new);
  //if the two projections don't overlap --> no overlap!
  if ( ( ((range1.first < range2.second) and ( range1.first > range2.first )) or ((range1.second < range2.second) and ( range1.second > range2.first )) ) or ( ((range2.first < range1.second) and ( range2.first > range1.first )) or ((range2.second < range1.second) and ( range2.second > range1.first )) ) )
    return true;     //yes...they overlap
  else
    return false;    //no....they do not overlap
}


//********************************

bool PolyOverlap(Polygon poly1, Polygon poly2){
  //start from first pair in vector then check all edges
  //edges are (0,1), (1,2), ..., (N-1,N) each pair a pair
  //of vertexes
  for (unsigned int i=0; i<poly1.Size(); i++){//loop over first polygon's vertices
    //find projection line
    //line: y=ax+b --- the line variable 
    float slope;
    if (i==(poly1.Size()-1))
      slope = FindSlope( poly1.Point(i) , poly1.Point(0) );
    else
      slope = FindSlope( poly1.Point(i) , poly1.Point(i+1) );
    //if there is even one no-overlap 
    //need to exit and return no overlap!
    if (!Overlap( slope, poly1, poly2, poly1.Point(i) ) ){
      return false;
    }
  }//loop over first polygon vertices
  //do the exact same thing but reversing polygon role
  for (unsigned int i=0; i<poly2.Size(); i++){//loop over first polygon
    //find projection line
    //line: y=ax+b --- the line variable 
    float slope;
    if (i==(poly2.Size()-1))
      slope = FindSlope( poly2.Point(i), poly2.Point(0) );
    else
      slope = FindSlope( poly2.Point(i) , poly2.Point(i+1) );
    //if there is even one no-overlap 
    //need to exit and return no overlap!
    if (!Overlap( slope, poly2, poly1, poly2.Point(i) ) ){
      return false;
    }
  }//loop over first polygon vertices
  return true;
} 

int main() {

  float poly1X_5[] = {3,4,10,13,12};
  float poly1Y_5[] = {3,20,25,17,5};
  float poly2X_5[] = {5,5,10,14,13};
  float poly2Y_5[] = {1,9,9,8,2};

  float poly1X_6[] = {3,2,4,10,13,12};
  float poly1Y_6[] = {3,7,20,25,17,5};
  float poly2X_6[] = {5,4,5,10,14,13};
  float poly2Y_6[] = {1,7,9,9,8,2};

  float poly1X_7[] = {3,2,4,7,10,13,12};
  float poly1Y_7[] = {3,7,20,28,25,17,5};
  float poly2X_7[] = {5,4,5,7,10,14,13};
  float poly2Y_7[] = {1,7,9,13,9,8,2};

  float poly1X_8[] = {3,2,4,7,10,13,12,10};
  float poly1Y_8[] = {3,7,20,28,25,17,5,4};
  float poly2X_8[] = {5,4,5,7,10,14,13,10};
  float poly2Y_8[] = {1,7,9,13,9,8,2,1};

  int times;
  int sides;
  std::cout << "How many sides for the polygon? " << std::endl;
  std::cin  >> sides;

  std::cout << "How many times do you want to do this?" << std::endl;
  std::cin  >> times;
  std::vector< std::pair<float,float> > P1;
  std::vector< std::pair<float,float> > P2;
  std::pair<float,float> pairtmp;

  if (sides==5){
    int counter = 0;
    for (int i=0; i<sides; i++){
      pairtmp = std::make_pair(poly1X_5[i],poly1Y_5[i]);
      P1.push_back(pairtmp);
      pairtmp = std::make_pair(poly2X_5[i],poly2Y_5[i]);
      P2.push_back(pairtmp);
    }
  }  
  else if (sides==6){
    int counter = 0;
    for (int i=0; i<sides; i++){
      pairtmp = std::make_pair(poly1X_6[i],poly1Y_6[i]);
      P1.push_back(pairtmp);
      pairtmp = std::make_pair(poly2X_6[i],poly2Y_6[i]);
      P2.push_back(pairtmp);
    }
  }  
  else if (sides==7){
    int counter = 0;
    for (int i=0; i<sides; i++){
      pairtmp = std::make_pair(poly1X_7[i],poly1Y_7[i]);
      P1.push_back(pairtmp);
      pairtmp = std::make_pair(poly2X_7[i],poly2Y_7[i]);
      P2.push_back(pairtmp);
    }
  }  
  else if (sides==8){
    int counter = 0;
    for (int i=0; i<sides; i++){
      pairtmp = std::make_pair(poly1X_8[i],poly1Y_8[i]);
      P1.push_back(pairtmp);
      pairtmp = std::make_pair(poly2X_8[i],poly2Y_8[i]);
      P2.push_back(pairtmp);
    }
  }  
  else{
    std::cout << "Wrong Number of sides!!!" << std::endl;
    return 1;
  }
  
  Polygon poly1(P1);
  Polygon poly2(P2);
  
  const long double timebegin = time(0);
  for (int t=0; t<times; t++){
    
    PolyOverlap(poly1,poly2);

  }
  const long double timeend = time(0);
  long double msec_elapsed = timeend-timebegin;
  std::cout << "Time elapsed for " << times << " runs is: " << msec_elapsed << " seconds" << std::endl;
}
