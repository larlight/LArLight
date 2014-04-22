#include "Polygon.hh"




//****************************************************
float FindSlope( const std::pair<float,float> &p1, 
		 const std::pair<float,float> &p2 )
{
  float slope = (p2.second-p1.second)/(p2.first-p1.first);
  return slope;
}


bool Clockwise(double Ax,double Ay,double Bx,double By,double Cx,double Cy)
{
  return (Cy-Ay)*(Bx-Ax) > (By-Ay)*(Cx-Ax);
}

bool SegmentOverlap(double Ax,double Ay,double Bx,double By,double Cx,double Cy, double Dx, double Dy)
{

  bool overlap = ( (Clockwise(Ax,Ay,Cx,Cy,Dx,Dy) != Clockwise(Bx,By,Cx,Cy,Dx,Dy))
		   and (Clockwise(Ax,Ay,Bx,By,Cx,Cy) != Clockwise(Ax,Ay,Bx,By,Dx,Dy)) );
  return overlap;
}


//****************************************************
float Polygon::Area() const
{
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
const std::pair<float,float>& Polygon::Point(unsigned int p) const
{
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
std::pair<float,float> Polygon::Project(const std::pair<float,float> &p, 
					float theta) const
{

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
bool Polygon::Overlap(float slope, 
		      const Polygon &poly2, 
		      const std::pair<float,float> &origin) const
{
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
bool Polygon::PolyOverlap(const Polygon &poly2) const
{

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



//*****************************************************
bool Polygon::PolyOverlapSegments(const Polygon &poly2) const
{

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
bool Polygon::PointInside(const std::pair<float,float> &point) const
{

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

