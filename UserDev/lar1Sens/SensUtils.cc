
// Implementation of SensUtils

#include "SensUtils.hh"

namespace lar1
{
  SensUtils::SensUtils(){

  }

  SensUtils::~SensUtils(){

  }
  
  TH1F* SensUtils::AppendHistograms(TH1F * hist1, TH1F* hist2, TH1F* hist3){
    //First, how many bins do we need?
    int nbins1 = hist1->GetNbinsX();
    int nbins2 = hist2->GetNbinsX();
    int nbins3 = hist3->GetNbinsX();
    //Now set up the combo histogram:
    int nbins = nbins1 + nbins2 + nbins3;
    TH1F dummynL("dummy","Side-by-side histogram",nbins,0,nbins);
  
    //Assumed that we put the histograms in order 1, 2, 3.
    //Loop over the number of bins in the final histogram:
    for (int i = 1; i <= nbins; i++){
      if (i <= nbins1) { //adding things from the first histogram
        dummynL.SetBinContent(i, hist1->GetBinContent(i)); 
      }
      else if (i <= nbins1+nbins2) { //adding from the second histogram
        dummynL.SetBinContent(i, hist2->GetBinContent(i-nbins1));
      }
      else { //adding from the last histogram
        dummynL.SetBinContent(i, hist3->GetBinContent(i-nbins1-nbins2));
      }
    }
    TH1F * ret = (TH1F*) dummynL.Clone();
    return ret;
  }
  
  TH1F* SensUtils::AppendHistograms(TH1F * hist1, TH1F* hist2){
    //First, how many bins do we need?
    int nbins1 = hist1->GetNbinsX();
    int nbins2 = hist2->GetNbinsX();
    //Now set up the combo histogram:
    int nbins = nbins1 + nbins2 ;
    TH1F dummynL("dummynL","Side-by-side histogram",nbins,0,nbins);
    
    //Assumed that we put the histograms in order 1, 2, 3.
    //Loop over the number of bins in the final histogram:
    for (int i = 1; i <= nbins; i++){
      if (i <= nbins1) { //adding things from the first histogram
        dummynL.SetBinContent(i, hist1->GetBinContent(i)); 
      }
      else { //adding from the second histogram
        dummynL.SetBinContent(i, hist2->GetBinContent(i-nbins1));
      }

    }
    TH1F * ret = (TH1F*) dummynL.Clone();
    return ret;
  }




  Float_t * CollapseMatrix(std::vector<std::vector<float> > entries,
                           int nbinsE, int nL){
    //taking in a matrix that should be (nbinsE*3*nL) by (nbinsE*3*nL)
    //Goal is to collapse the fosc signal into the nue background.
    //This is a complicated process for more than one baseline!
    
    Int_t nbins = 3*nbinsE*nL;
    
    Float_t entriescollapsed1[nbins-nbinsE][nbins-nbinsE];
      Float_t entriescollapsed2[nbins-2*nbinsE][nbins-2*nbinsE];
      Float_t entriescollapsed3[nbins-nL*nbinsE][nbins-nL*nbinsE];
      Float_t *entriescollapsed = new float[(nbins-nL*nbinsE)*(nbins-nL*nbinsE)];
    
    for (int ibin=0; ibin<(nbins-nbinsE); ibin++)
    {
      for (int jbin=0; jbin<(nbins-nbinsE); jbin++)
      {

        //fullosc block of any baseline
        //checked, think OK:
        if (ibin<nbinsE && jbin<nbinsE)
        {
          entriescollapsed1[ibin][jbin]=
            entries[ibin][jbin]+
            entries[ibin+nbinsE][jbin]+
            entries[ibin][jbin+nbinsE]+
            entries[ibin+nbinsE][jbin+nbinsE];
        }
        //checked, think OK:
        else if (ibin<nbinsE && jbin>=nbinsE)
        {
          entriescollapsed1[ibin][jbin]=
            entries[ibin][jbin+nbinsE]+
            entries[ibin+nbinsE][jbin+nbinsE];
        }
        //checked, think OK:
        else if (ibin>=nbinsE && jbin<nbinsE)
        {
          entriescollapsed1[ibin][jbin]=
            entries[ibin+nbinsE][jbin]+
            entries[ibin+nbinsE][jbin+nbinsE];
        }
        //checked, think OK:
        else if (ibin>=nbinsE && jbin>=nbinsE)
        {
          entriescollapsed1[ibin][jbin]=
            entries[ibin+nbinsE][jbin+nbinsE];
        }

      }
    }

    if (nL>1)
    {
      for (int ibin=0; ibin<(nbins-nbinsE-nbinsE); ibin++)
      {
        for (int jbin=0; jbin<(nbins-nbinsE-nbinsE); jbin++)
        {
          if (ibin<(nbinsE+nbinsE) && jbin<(nbinsE+nbinsE))
          {
            entriescollapsed2[ibin][jbin] = entriescollapsed1[ibin][jbin];
          }
          //checked, think OK:
          else if ( ibin>=(nbinsE+nbinsE) && ibin<(nbinsE+nbinsE+nbinsE) && jbin>=(nbinsE+nbinsE) && jbin<(nbinsE+nbinsE+nbinsE) )
          {
            entriescollapsed2[ibin][jbin] = entriescollapsed1[ibin][jbin]+
              entriescollapsed1[ibin+nbinsE][jbin+nbinsE]+
              entriescollapsed1[ibin+nbinsE][jbin]+
              entriescollapsed1[ibin][jbin+nbinsE];
          }
          //checked, think OK:
          else if ( ibin>=(nbinsE+nbinsE) && ibin<(nbinsE+nbinsE+nbinsE) && jbin<(nbinsE+nbinsE))
          {
            entriescollapsed2[ibin][jbin] = entriescollapsed1[ibin][jbin]+
              entriescollapsed1[ibin+nbinsE][jbin];
          }
          //checked, think OK:
          else if ( ibin>=(nbinsE+nbinsE+nbinsE) && jbin<(nbinsE+nbinsE))
          {
            entriescollapsed2[ibin][jbin] = entriescollapsed1[ibin+nbinsE][jbin];
          }
          //checked, think OK:
          else if ( jbin>=(nbinsE+nbinsE) && jbin<(nbinsE+nbinsE+nbinsE) && ibin<(nbinsE+nbinsE))
          {
            entriescollapsed2[ibin][jbin] = entriescollapsed1[ibin][jbin]+
              entriescollapsed1[ibin][jbin+nbinsE];
          }
          //checked, think OK:
          else if ( jbin>=(nbinsE+nbinsE+nbinsE) && ibin<(nbinsE+nbinsE))
          {
            entriescollapsed2[ibin][jbin] = entriescollapsed1[ibin][jbin+nbinsE];
          }
          //checked, think OK:
          else if ( ibin>=(nbinsE+nbinsE) && ibin<(nbinsE+nbinsE+nbinsE) && jbin>=(nbinsE+nbinsE+nbinsE))
          {
            entriescollapsed2[ibin][jbin] = entriescollapsed1[ibin][jbin+nbinsE]+
              entriescollapsed1[ibin+nbinsE][jbin+nbinsE];
          }
          //checked, think OK:
          else if ( jbin>=(nbinsE+nbinsE) && jbin<(nbinsE+nbinsE+nbinsE) && ibin>=(nbinsE+nbinsE+nbinsE))
          {
            entriescollapsed2[ibin][jbin] = entriescollapsed1[ibin+nbinsE][jbin]+
              entriescollapsed1[ibin+nbinsE][jbin+nbinsE];
          }
          //checked, think OK:
          else if ( ibin>=(nbinsE+nbinsE+nbinsE) && jbin>=(nbinsE+nbinsE+nbinsE) )
          {
            entriescollapsed2[ibin][jbin] = entriescollapsed1[ibin+nbinsE][jbin+nbinsE];
          }
        }
      }

    }

    if (nL>2)
    {
      for (int ibin=0; ibin<(nbins-nbinsE-nbinsE-nbinsE); ibin++)
      {
        for (int jbin=0; jbin<(nbins-nbinsE-nbinsE-nbinsE); jbin++)
        {
          if (ibin<(2*(nbinsE+nbinsE)) && jbin<(2*(nbinsE+nbinsE)))
          {
            entriescollapsed3[ibin][jbin] = entriescollapsed2[ibin][jbin];
          }
          else if ( ibin>=(2*(nbinsE+nbinsE)) && ibin<(2*(nbinsE+nbinsE)+nbinsE) && jbin>=(2*(nbinsE+nbinsE)) && jbin<(2*(nbinsE+nbinsE)+nbinsE) )
          {
            entriescollapsed3[ibin][jbin] = entriescollapsed2[ibin][jbin]+
              entriescollapsed2[ibin+nbinsE][jbin+nbinsE]+
              entriescollapsed2[ibin+nbinsE][jbin]+
              entriescollapsed2[ibin][jbin+nbinsE];
          }
          else if ( ibin>=(2*(nbinsE+nbinsE)) && ibin<(2*(nbinsE+nbinsE)+nbinsE) && jbin<(2*(nbinsE+nbinsE)) )
          {
            entriescollapsed3[ibin][jbin] = entriescollapsed2[ibin][jbin]+
              entriescollapsed2[ibin+nbinsE][jbin];
          }
          else if ( ibin>=(2*(nbinsE+nbinsE)+nbinsE) && jbin<2*(nbinsE+nbinsE) )
          {
            entriescollapsed3[ibin][jbin] = entriescollapsed2[ibin+nbinsE][jbin];
          }
          else if ( jbin>=2*(nbinsE+nbinsE) && jbin<(2*(nbinsE+nbinsE)+nbinsE) && ibin<2*(nbinsE+nbinsE) )
          {
            entriescollapsed3[ibin][jbin] = entriescollapsed2[ibin][jbin]+
              entriescollapsed2[ibin][jbin+nbinsE];
          }
          else if ( jbin>=(2*(nbinsE+nbinsE)+nbinsE) && ibin<(2*(nbinsE+nbinsE)) )
          {
            entriescollapsed3[ibin][jbin] = entriescollapsed2[ibin][jbin+nbinsE];
          }
          else if ( ibin>=2*(nbinsE+nbinsE) && ibin<(2*(nbinsE+nbinsE)+nbinsE) && jbin>=(2*(nbinsE+nbinsE)+nbinsE) )
          {
            entriescollapsed3[ibin][jbin] = entriescollapsed2[ibin][jbin+nbinsE]+
              entriescollapsed2[ibin+nbinsE][jbin+nbinsE];
          }
          else if ( jbin>=2*(nbinsE+nbinsE) && jbin<(2*(nbinsE+nbinsE)+nbinsE) && ibin>=(2*(nbinsE+nbinsE)+nbinsE) )
          {
            entriescollapsed3[ibin][jbin] = entriescollapsed2[ibin+nbinsE][jbin]+
              entriescollapsed2[ibin+nbinsE][jbin+nbinsE];
          }
          else if ( ibin>=(2*(nbinsE+nbinsE)+nbinsE) && jbin>=(2*(nbinsE+nbinsE)+nbinsE) )
          {
            entriescollapsed3[ibin][jbin] = entriescollapsed2[ibin+nbinsE][jbin+nbinsE];
          }
        }
      }
    }

    int iterator=0;
    for (int ibin=0; ibin<(nbins-nL*nbinsE); ibin++)
    {
        for (int jbin=0; jbin<(nbins-nL*nbinsE); jbin++)
        {
            if (nL==1)    entriescollapsed[iterator] = entriescollapsed1[ibin][jbin];
            else if (nL==2) entriescollapsed[iterator] = entriescollapsed2[ibin][jbin];
            else if (nL==3) entriescollapsed[iterator] = entriescollapsed3[ibin][jbin];
            iterator++;
        }
    }
    
    return entriescollapsed; 
  } //Collapse Matrix


}
