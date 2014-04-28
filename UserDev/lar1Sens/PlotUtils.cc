


#include "PlotUtils.hh"

namespace lar1{

  // PlotUtils::PlotUtils(){}

  void PlotUtils::lsnd_plot(TCanvas * c){

  }

  // adds a plot label to the current canvas
  void PlotUtils::add_plot_label(char* label, double x, double y,
                                 double size , int color,
                                 int font, int align){

    TLatex *latex = new TLatex( x, y, label );
    latex->SetNDC();
    latex->SetTextSize(size);
    latex->SetTextColor(color);
    latex->SetTextFont(font);
    latex->SetTextAlign(align);
    latex->Draw();
  }

}