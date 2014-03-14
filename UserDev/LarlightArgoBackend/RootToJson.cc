#include "RootToJson.h"
#include <TH1.h>

JsonObject TH1ToHistogram( TH1* hist )
{
  JsonObject h;
  if(!hist) return h;
  h.add("classname",hist->ClassName());
  h.add("name",hist->GetName());
  h.add("title",hist->GetTitle());
  h.add("xlabel",hist->GetXaxis()->GetTitle());
  h.add("ylabel",hist->GetYaxis()->GetTitle());
  h.add("n",hist->GetNbinsX());
  h.add("min",JsonElement(hist->GetXaxis()->GetXmin(),9));
  h.add("max",JsonElement(hist->GetXaxis()->GetXmax(),9));
  h.add("underflow",hist->GetBinContent(0));
  h.add("overflow",hist->GetBinContent(hist->GetNbinsX()+1));
  double tot = hist->GetSumOfWeights();
  h.add("total",JsonElement(tot,5));
  h.add("sum_x",JsonElement(tot*hist->GetMean(),5));
  h.add("max_content",hist->GetMaximum());
  h.add("min_content",hist->GetMinimum());
  h.add("time_on_x",hist->GetXaxis()->GetTimeDisplay());
  JsonArray data;
  for(int i=1; i <= hist->GetNbinsX();i++) {
    data.add(JsonSigFig(hist->GetBinContent(i),3));
  }
  h.add("data",data);
  return h;
}