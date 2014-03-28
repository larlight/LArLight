//*******************************************************//
//                                                       //
//Algorithm that checks and sees if two polygons overlap //
//                                                       //
//See main() function for example                        //
//PolyOverlap takes 2 Polygon type-objects as input each //
//polygon object is just a vector of std::pair where     //
//each pair represents the x,y coordinates of a vertex   //
//                                                       //
//Idea behind algorithm:                                 //
//To see if two polygons overlap in a 2D plane think     //
//about walking around them from far away on that plane. //
//If you ever notice a sliver of "empty space" between   //
//them then it means that the two polygons cannot overlap//
//To write an algorithm that determines whether the two  //
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
  float Area();
};

//****************************************************
float Polygon::Area(){
  //how? here:
  //http://www.mathsisfun.com/geometry/area-irregular-polygons.html
  float area = 0;
  for (unsigned int i=0; i<vertices.size(); i++){
    if ( i < (vertices.size()-1) )
      area += (((vertices.at(i)).second)+((vertices.at(i+1)).second))*(((vertices.at(i)).first)-((vertices.at(i+1)).first))*0.5;
    if ( i == (vertices.size()-1) )
      area += (((vertices.at(i)).second)+((vertices.at(0)).second))*(((vertices.at(i)).first)-((vertices.at(0)).first))*0.5;
  }
  if (area<0.0)
    area = -area;
  return area;
}
//****************************************************

//****************************************************
std::pair<float,float> Polygon::Point(unsigned int p){
  //This function returns the vertex under consideration
  //as a std::pair<float,float> Returns vertex for argument
  //from 0 to N-1. For input N = number of sides then
  //the first vertex is returned
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
    //finally calculate range of projection on x-axis: look at every x position and compare it to range
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
  if ( ( ((range1.first < range2.second) and ( range1.first > range2.first )) or ((range1.second < range2.second) and ( range1.second > range2.first )) ) or ( ((range2.first < range1.second) and ( range2.first > range1.first )) or ((range2.second < range1.second) and ( range2.second > range1.first )) ) )
    return true;     //yes...they overlap
  else
    return false;    //no....they do not overlap
}
//*****************************************************

//*****************************************************
bool PolyOverlap(Polygon poly1, Polygon poly2){

  //start from first pair in vector then check all edges.
  //edges are (0,1), (1,2), ..., (N,N-1) each pair a pair
  //of vertexes
  for (unsigned int i=0; i<poly1.Size(); i++){//loop over first polygon's vertices
    //find projection line's slope
    //line: y=ax+b --- slope is a variable 
    float slope;
    slope = FindSlope( poly1.Point(i) , poly1.Point(i+1) );
    //if there is even one no-overlap 
    //need to exit and return no overlap!
    if (!Overlap( slope, poly1, poly2, poly1.Point(i) ) )
      return false;
  }//loop over first polygon vertices

  //do the exact same thing but reversing polygon role
  for (unsigned int i=0; i<poly2.Size(); i++){//loop over first polygon
    float slope;
    slope = FindSlope( poly2.Point(i) , poly2.Point(i+1) );
    if (!Overlap( slope, poly2, poly1, poly2.Point(i) ) )
      return false;
  }//loop over second polygon vertices
  return true;
} 
//*****************************************************



//example function to run algorithm...can choose from several
//polygons...each pair overlaps which means algorithm will be 
//maximally slow...good for calculating algorithm speed
int main() {

  //set random number seed
  srand(time(NULL));

  //file where to write points
  std::ofstream myout;
  myout.open("polytmp.txt");


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
  /*
  if (sides==4){
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
  */

  //center of polygons
  float c1 = 10+rand()%10;
  float c2 = 10+rand()%10;
  //radius is 5
  float r = 5.0;
  //random angle
  float angle1 = 2*3.14*((rand()%1000)/1000.0);
  float angle2 = 2*3.14*((rand()%1000)/1000.0);
  float x1, x2, y1, y2;
  std::pair<float,float> tmppair;
  for (int i=0; i<sides; i++){
    x1 = c1+r*sin(angle1);
    y1 = c1+r*cos(angle1);
    x2 = c2+r*sin(angle2);
    y2 = c2+r*cos(angle2);
    tmppair = std::make_pair(x1,y1);
    P1.push_back(tmppair);
    angle1 += (2*3.14)/sides;
    tmppair = std::make_pair(x2,y2);
    P2.push_back(tmppair);
    angle2 += (2*3.14)/sides;
    myout << x1 << "\t" << y1 << "\n" << x2 << "\t" << y2 << "\n";
  }
  //add first points again to draw closed shapes
  myout << (P1.at(0)).first << "\t" << (P1.at(0)).second << "\n" << (P2.at(0)).first << "\t" << (P2.at(0)).second << "\n";

  myout.close();

  Polygon poly1(P1);
  Polygon poly2(P2);
  
  const long double timebegin = time(0);
  for (int t=0; t<times; t++){

    //print area of both polygons:
    float area1 = poly1.Area();
    float area2 = poly2.Area();
    std::cout << "Poly 1 Area: " << poly1.Area() << std::endl;
    std::cout << "Poly 2 Area: " << poly2.Area() << std::endl;

    //PolyOverlap(poly1,poly2);
    
    if ( PolyOverlap(poly1,poly2) )
      std::cout << "Overlap!" << std::endl;
    else
      std::cout << "No Overlap!" << std::endl;
    
  }
  const long double timeend = time(0);
  long double msec_elapsed = timeend-timebegin;
  std::cout << "Time elapsed for " << times << " runs is: " << msec_elapsed << " seconds" << std::endl;
}
