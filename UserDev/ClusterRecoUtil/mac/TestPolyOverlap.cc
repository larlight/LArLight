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


//****************************************************
float FindSlope( std::pair<float,float> p1, std::pair<float,float> p2 ){
  float slope = (p2.second-p1.second)/(p2.first-p1.first);
  return slope;
}

bool Clockwise(double Ax,double Ay,double Bx,double By,double Cx,double Cy){
  return (Cy-Ay)*(Bx-Ax) > (By-Ay)*(Cx-Ax);
}

bool SegmentOverlap(double Ax,double Ay,double Bx,double By,double Cx,double Cy, double Dx, double Dy){

  bool overlap = ( (Clockwise(Ax,Ay,Cx,Cy,Dx,Dy) != Clockwise(Bx,By,Cx,Cy,Dx,Dy))
		   and (Clockwise(Ax,Ay,Bx,By,Cx,Cy) != Clockwise(Ax,Ay,Bx,By,Dx,Dy)) );
  return overlap;
}


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


//*****************************************************
bool Polygon::Overlap(float slope, Polygon poly2, std::pair<float,float> origin){
  //translate and rotate both polygons
  float theta = tan(slope);
  //here we translate origin, rotate and find x-coordinates and find range of projection on edge line
  std::pair<float,float> range1 = this->Project(origin,theta);
  std::pair<float,float> range2 = poly2.Project(origin,theta);
  //std::cout << "range 1: " << range1.first << " " << range1.second << std::endl;
  //std::cout << "range 2: " << range2.first << " " << range2.second << std::endl;
  //if the two projections don't overlap --> no overlap!
  if ( ( ((range1.first <= range2.second) and ( range1.first >= range2.first )) or ((range1.second <= range2.second) and ( range1.second >= range2.first )) ) or ( ((range2.first <= range1.second) and ( range2.first >= range1.first )) or ((range2.second <= range1.second) and ( range2.second >= range1.first )) ) )
    return true;     //yes...they overlap
  else
    return false;    //no....they do not overlap
}



//*****************************************************
bool Polygon::PolyOverlapSegments(Polygon poly2){

  //loop over the two polygons checking wehther
  //two segments ever intersect
  for (unsigned int i=0; i<this->Size(); i++){
    for (unsigned int j=0; j<poly2.Size(); j++){
      if (SegmentOverlap( this->Point(i).first, this->Point(i).second,
			 this->Point(i+1).first, this->Point(i+1).second,
			 poly2.Point(j).first, poly2.Point(j).second,
			  poly2.Point(j+1).first, poly2.Point(j+1).second) )
	return true;
    }
  }
  return false;
}



//*****************************************************
bool Polygon::PointInside(std::pair<float,float> point){

  //any ray originating at point will cross polygon
  //even number of times if point outside
  //odd number of times if point inside
  int intersections = 0;
  for (unsigned int i=0; i<this->Size(); i++){
    if ( SegmentOverlap( this->Point(i).first, this->Point(i).second,
			 this->Point(i+1).first, this->Point(i+1).second,
			 10000.0, 10000.0,
			 point.first, point.second) )
      intersections += 1;
  }
  if ( (intersections%2) == 0 )
    return false;
  else 
    return true;
  
}


//*****************************************************
bool Polygon::PolyOverlap(Polygon poly2){

  //start from first pair in vector then check all edges.
  //edges are (0,1), (1,2), ..., (N,N-1) each pair a pair
  //of vertexes
  for (unsigned int i=0; i<this->Size(); i++){//loop over first polygon's vertices
    //find projection line's slope
    //line: y=ax+b --- slope is a variable 
    float slope;
    slope = FindSlope( this->Point(i) , this->Point(i+1) );
    //if there is even one no-overlap 
    //need to exit and return no overlap!
    if (! (this->Overlap( slope, poly2, this->Point(i) )) )
      return false;
  }//loop over first polygon vertices

  //do the exact same thing but reversing polygon role
  for (unsigned int i=0; i<poly2.Size(); i++){//loop over first polygon
    float slope;
    slope = FindSlope( poly2.Point(i) , poly2.Point(i+1) );
    if (!(poly2.Overlap( slope, *this, poly2.Point(i) )) )
      return false;
  }//loop over second polygon vertices
  return true;
} 




//example function to run algorithm...can choose from several
//polygons...each pair overlaps which means algorithm will be 
//maximally slow...good for calculating algorithm speed
int main() {

  //set random number seed
  srand(time(NULL));

  //file where to write points
  std::ofstream myout;
  myout.open("polytmp.txt");

  int times;
  int sides;
  std::cout << "How many sides for the polygon? " << std::endl;
  std::cin  >> sides;

  std::cout << "How many times do you want to do this?" << std::endl;
  std::cin  >> times;
  std::vector< std::pair<float,float> > P1;
  std::vector< std::pair<float,float> > P2;
  std::pair<float,float> pairtmp;

  //center of polygons
  float c1 = 10+rand()%10;
  float c2 = 10+rand()%10;
  //radius is 5
  float r1 = 5.0;
  float r2 = 5.0;
  //random angle
  float angle1 = 2*3.14*((rand()%1000)/1000.0);
  float angle2 = 2*3.14*((rand()%1000)/1000.0);
  float x1, x2, y1, y2;
  std::pair<float,float> tmppair;
  for (int i=0; i<sides; i++){
    x1 = c1+r1*sin(angle1);
    y1 = c1+r1*cos(angle1);
    x2 = c2+r2*sin(angle2);
    y2 = c2+r2*cos(angle2);
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

  if (poly1.PolyOverlapSegments(poly2))
    std::cout << "Overlap!" << std::endl;
  else{
    std::cout << "No Overlap!" << std::endl;

    if (poly1.PointInside(poly2.Point(1)))
      std::cout << "2 Inside 1!" << std::endl;
    else
      std::cout << "Outside!" << std::endl;
    
    if (poly2.PointInside(poly1.Point(1)))
      std::cout << "1 Inside 2!" << std::endl;
    else
      std::cout << "Outside!" << std::endl;
  }
  const long double timebegin = time(0);
  for (int t=0; t<times; t++){

    //print area of both polygons:
    //    float area1 = poly1.Area();
    //    float area2 = poly2.Area();
    //    std::cout << "Poly 1 Area: " << poly1.Area() << std::endl;
    //    std::cout << "Poly 2 Area: " << poly2.Area() << std::endl;

    if (!poly1.PolyOverlapSegments(poly2)){
      poly1.PointInside(poly2.Point(1));
      poly2.PointInside(poly1.Point(1));
    }
    
  }
  const long double timeend = time(0);
  long double msec_elapsed = timeend-timebegin;
  std::cout << "Time elapsed for " << times << " runs is: " << msec_elapsed << " seconds" << std::endl;
}
