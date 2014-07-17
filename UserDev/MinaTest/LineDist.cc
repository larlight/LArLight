#ifndef LINEDIST_CC
#define LINEDIST_CC

#include "LineDist.hh"
#include <cmath>

double LineDist::Calc(const MinaTestParticle& part1, const MinaTestParticle& part2)
{

  //Calculate the distance between the two MCParticle objects read in. (Efficiency unknown)                                                                                            
  std::vector<double> x0_1 = {part1.fStartXYZT()[0], part1.fStartXYZT()[1], part1.fStartXYZT()[2]};
  std::vector<double> x0_2 = {part2.fStartXYZT()[0], part2.fStartXYZT()[1], part2.fStartXYZT()[2]};
  std::vector<double> xf_1 = {part1.fEndXYZT()[0], part1.fEndXYZT()[1], part1.fEndXYZT()[2]};
  std::vector<double> xf_2 = {part2.fEndXYZT()[0], part2.fEndXYZT()[1], part2.fEndXYZT()[2]};

  std::vector<double> c_a = {x0_2[0] - x0_1[0], x0_2[1] - x0_1[1], x0_2[2] - x0_1[2]};
  std::vector<double> b = {xf_1[0] - x0_1[0], xf_1[1] - x0_1[1], xf_1[2] - x0_1[2]};
  std::vector<double> d = {xf_2[0] - x0_2[0], xf_2[1] - x0_2[1], xf_2[2] - x0_2[2]};

  //Compute the necessary cross products, dot products, etc.                                                                                                                           
  std::vector<double> cross = {(b[1]*d[2] - d[1]*b[2]), -(b[0]*d[1] - b[1]*d[0]), (b[0]*d[2] - b[2]*d[0])};
  double dist = ((cross[0]*c_a[0] + cross[1]*c_a[1] + cross[2]*c_a[2]) / std::sqrt(cross[0]*cross[0] + cross[1]*cross[1] + cross[2]*cross[2]));

  return dist;
}


#endif
