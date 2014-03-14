#ifndef COLORMAP_H_ATUYSOIN
#define COLORMAP_H_ATUYSOIN

#include <vector>

class ColorMap
{
public:
  ColorMap(int type = 0) { load(type); };
  void load(int type = 0);
  
  void get(unsigned char* out, float x); 
  // x is between -1 and 1 as extremes.
  // Saves output values into out, out+1, and out+2 as rgb values.
  
  std::vector< std::vector<float> >  fTable;
  
};


#endif /* end of include guard: COLORMAP_H_ATUYSOIN */
