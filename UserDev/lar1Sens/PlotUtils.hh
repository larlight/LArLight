// A basic class to help with plotting for the nueappearance stuff

#ifndef PLOTUTILS_HH
#define PLOTUTILS_HH 

#include "TCanvas.h"
#include "TLatex.h"

namespace lar1{

  class PlotUtils
  {
  public:
    PlotUtils();
    ~PlotUtils();
    
    // Add the lsnd_data to a plot
    void lsnd_plot (TCanvas* c);

    void add_plot_label(char* label, double x, double y,
                        double size = 0.05, int color = 1,
                        int font = 62, int align = 22 );


  protected:
    

    /* data */

  };
}

#endif
