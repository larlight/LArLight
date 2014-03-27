//*****************************************************
//
//Algorithm that checks and sees if two polygons overlap
//
//See main() function for example
//PolyOverlap takes 2 Polygon type-objects as input
//each polygon object is just a vector of std::pair
//where each pair represents the x,y coordinates of a vertex
//
//Idea behind algorithm:
//To see if two polygons overlap in a 2D plane think
//about walking around them from far away on that plane. If you
//ever notice a sliver of "empty space" between them
//then it means that the two polygons cannot overlap
//To write an algorithm that determines whether the two
//polygons overlap then one can look at the projection of
//the polygons on the line traced out by each side of 
//each polygon. If these projections do not overlap
//for any one of these lines, then the polygons
//also do not overlap.
//
//This is accomplished in several steps:
//for each polygon, loop over all vertices
//find line of edge that connects that vertex
//with subsequent one. Calculate the slope for this line
//Now rotate the vertices of the two polygons such that
//the vertex in consideration is the origin of the x-y 
//plane and the edge is on the +x axis. At this point
//x-coordiante of points of the two polygons will give
//projection position on this line. Determine the range
//occupied by each polygon (min x, max x). If the two
//ranges overlap this means the polygons overlap on that
//projection. If for any edge the projections do not overlap
//then the two polygons do not overlap.
//
//**********************************************************


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
class Polygon{
  
private:
  std::vector< std::pair<float,float> > vertices;
  
 public:
  Polygon(std::vector< std::pair<float,float> > points) { vertices = points; }
  unsigned int Size() { return vertices.size(); } 
  std::pair<float,float> Point(unsigned int p); 
  std::pair<float,float> Project(std::pair<float,float>,float);
};



//****************************************************
std::pair<float,float> Polygon::Point(unsigned int p){
  if (p<vertices.size())
    return vertices.at(p);
  else if (p==vertices.size())
    return vertices.at(0);
  else{
    std::cout << "Out of bounds of Polygon!" <<std::endl;
    return vertices.at(0);
  }
}
//****************************************************    

//****************************************************
std::pair<float,float> Polygon::Project(std::pair<float,float> p, float theta){

  std::pair<float,float> range(10000,0);
  std::pair<float,float> ptmp;

  for (unsigned int i=0; i<vertices.size(); i++){
    //Translation
    //translating each vertex so that origin is on first vertex on polygon's edge being considered
    ptmp = std::make_pair(   (vertices.at(i)).first - p.first  ,   (vertices.at(i)).second - p.second   );
    //Rotation
    //instead of rotating each (x,y) edge (slow) just find nex x-position which gives us information
    //on the projection of that vertex on the line we are considering
    // +x direction is from vertex in consideration (vertex 'i' in loop) to next vertex
    //now find the x-coordinate of that vertex after it is rotated such that edge is now + x axis
    float xnew = (ptmp.first)*cos(theta) + (ptmp.second)*sin(theta);
     if ( xnew < range.first )
      range.first = xnew;
    if ( xnew > range.second )
      range.second = xnew;
  }
  return range;
}
//****************************************************

//****************************************************
float FindSlope( std::pair<float,float> p1, std::pair<float,float> p2 ){
  float slope = (p2.second-p1.second)/(p2.first-p1.first);
  return slope;
}
//*****************************************************
    
//*****************************************************
bool Overlap(float slope, Polygon poly1, Polygon poly2, std::pair<float,float> origin){
  //translate and rotate both polygons
  float theta = tan(slope);
  //here we translate origin, rotate and find x-coordinates and find range of projection on edge line
  std::pair<float,float> range1 = poly1.Project(origin,theta);
  std::pair<float,float> range2 = poly2.Project(origin,theta);
  //if the two projections don't overlap --> no overlap!
  if ( ( ((range1.first < range2.second) and ( range1.first > range2.first )) or ((range1.second < range2.second) and ( range1.second > range2.first )) ) or ( ((range2.first < range1.second) and ( range2.first > range1.first )) or ((range2.second < range1.second) and ( range2.second > range1.first )) ) ){
    return true;     //yes...they overlap
  }
  else{
    return false;    //no....they do not overlap
  }
}
//*****************************************************

//*****************************************************
bool PolyOverlap(Polygon poly1, Polygon poly2){
  //start from first pair in vector then check all edges
  //edges are (0,1), (1,2), ..., (N-1,N) each pair a pair
  //of vertexes
  for (unsigned int i=0; i<poly1.Size(); i++){//loop over first polygon's vertices
    //find projection line
    //line: y=ax+b --- the line variable 
    float slope;
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
    slope = FindSlope( poly2.Point(i) , poly2.Point(i+1) );
    //if there is even one no-overlap 
    //need to exit and return no overlap!
    if (!Overlap( slope, poly2, poly1, poly2.Point(i) ) ){
      return false;
    }
  }//loop over second polygon vertices
  return true;
} 
//*****************************************************



//example function to run algorithm...can choose from several
//polygons...each pair overlaps which means algorithm will be 
//maximally slow...good for calculating algorithm speed
int main() {

  float poly1X_4[] = {3,2,10,12};
  float poly1Y_4[] = {3,7,8,5};
  float poly2X_4[] = {6,6,13,15};
  float poly2Y_4[] = {6,9,8,7};

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
  float poly1Y_8[] = {3,7,10,18,15,17,5,4};
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

  if (sides==4){
    int counter = 0;
    for (int i=0; i<sides; i++){
      pairtmp = std::make_pair(poly1X_4[i],poly1Y_4[i]);
      P1.push_back(pairtmp);
      pairtmp = std::make_pair(poly2X_4[i],poly2Y_4[i]);
      P2.push_back(pairtmp);
    }
  }  
  else if (sides==5){
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
