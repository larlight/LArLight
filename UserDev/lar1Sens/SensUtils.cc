
// Implementation of SensUtils

#include "SensUtils.hh"

namespace lar1
{
  SensUtils::SensUtils(){
    // there's really no better way to do this than as such:
    miniboone_bins.push_back(0.200);
    miniboone_bins.push_back(0.300);
    miniboone_bins.push_back(0.375);
    miniboone_bins.push_back(0.475);
    miniboone_bins.push_back(0.550);
    miniboone_bins.push_back(0.675);
    miniboone_bins.push_back(0.800);
    miniboone_bins.push_back(0.950);
    miniboone_bins.push_back(1.100);
    miniboone_bins.push_back(1.300);
    miniboone_bins.push_back(1.500);
    miniboone_bins.push_back(3.000);

    numu_bins.push_back(.200);
    numu_bins.push_back(.300);
    numu_bins.push_back(.400);
    numu_bins.push_back(.450);
    numu_bins.push_back(.500);
    numu_bins.push_back(.550);
    numu_bins.push_back(.600);
    numu_bins.push_back(.650);
    numu_bins.push_back(.700);
    numu_bins.push_back(.750);
    numu_bins.push_back(.800);
    numu_bins.push_back(.850);
    numu_bins.push_back(.900);
    numu_bins.push_back(.950);
    numu_bins.push_back(1.000);
    numu_bins.push_back(1.250);
    numu_bins.push_back(1.500);
    numu_bins.push_back(2.000);
    numu_bins.push_back(2.500);
    numu_bins.push_back(3.000);

    double emin = 0.0;
    double emax = 3.0;
    double nbins = 120.0;
    double binSize = (emax - emin)/nbins;
    default_bins.resize(nbins+1);
    for (double i = 0; i <= nbins; i++) {
      default_bins.at(i) = emin + i*binSize;
    }

    emin = 0.2;
    nbins = 14;
    binSize = (emax - emin)/nbins;
    nue_bins.resize(nbins+1);
    for (double i = 0; i <= nbins; i++) {
      nue_bins.at(i) = emin + i*binSize;
    }
  }

  SensUtils::~SensUtils(){

  }


  std::vector<float> SensUtils::rebinVector(std::vector<float> inputVector,
                                            std::vector<float> bins){

    if (bins.size() < 2 || inputVector.size()+1 != default_bins.size()){
      std::cerr << "Error in the input bins for rebinVector!" << std::endl;
      exit(-9);
    }

    // the input vector is meant to have the default bins which are defined above
    // So we loop over the input vector and pack the data into the appropriate
    // spots in the output vector

    std::vector<float> result;
    result.resize(bins.size()-1);

    // loop over the input vector of data (inputVector) and know that
    // the bin boundaries for the data at inputVector[i] are 
    // default_bins[i] and default_bins[i+1].
    // For each bin, we have to find out what spot in the result vector
    // the data gets added to.  Again, result[i] has bin boundaries given
    // by bins[i] to bins[i+1]
    // 
    // I'm not going to check for pathological cases where bins is more fine than
    // the default bins, sorry.  Just don't do that.
    for(unsigned int inputBin = 0; inputBin < inputVector.size(); inputBin++){
      // So, we want to find out where to put this input data
      // into the output data.  That means seeing where this bin's boundaries
      // lie wrt the input bins.
      // std::cout << "This is bin " << inputBin << "(E = " << default_bins[inputBin] << ")"
                // << " with content " << inputVector[inputBin] << std::endl;
      // 
      // first catch the case that the first bin is not at zero:
      if (default_bins[inputBin] < bins.front()) continue;
      // and catch the case that we're being more restrictive than the 
      // default bins at the high end:
      if (default_bins[inputBin] > bins.back()) continue;
      // Next, look at each bin and find where this input bin [lower] 
      // is greater than or equal to a final bin:
      unsigned int tempLowerBin = 0;
      for (tempLowerBin = 0; tempLowerBin < bins.size(); ++tempLowerBin)
      {
        if (default_bins[inputBin] == bins[tempLowerBin]){
          // std::cout << "  Found a match at " << tempLowerBin <<": "
                    // << default_bins[inputBin] << "==" << bins[tempLowerBin]
                    // << std::endl;
          break;
        }
        if (default_bins[inputBin] < bins[tempLowerBin]){
          // std::cout << "  bin is greater at " << tempLowerBin <<": "
                    // << default_bins[inputBin] << "< " << bins[tempLowerBin]
                    // << std::endl;
          tempLowerBin --;
          break;
        }
      }
      // Now do the rebinning:
      // std::cout << "Adding content " << result[tempLowerBin] << " to the final bin spot: " << tempLowerBin<<"\n";
      result[tempLowerBin] += inputVector[inputBin];
    }

    return result;

  }
  

  // std::vector<float> SensUtils::rebinVector(std::vector<float> & inputVector,
  //                                  std::string binSchemeName){
  //   if (binSchemeName == "miniboone"){
  //     return rebinVector(inputVector, miniboone_bins);
  //   }
  //   else if (binSchemeName == "nue"){
  //     return rebinVector(inputVector,nue_bins);
  //   }
  //   else if (binSchemeName == "numu"){
  //     return rebinVector(inputVector,numu_bins);
  //   }
  //   else{
  //     std::cerr << "Unknown bin scheme name, bailing because I don't know what to do." << std::endl;
  //     exit(-9);
  //   }

  // }
  
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

  std::vector<std::vector<float> > SensUtils::CollapseMatrix(
                                    std::vector<std::vector<float> > entries,
                                    int nbins_nue, int nbins_numu, int nL)
  {

    int nbins = 2*nbins_nue + nbins_numu;
    int nbins_collapsed = nbins - nbins_nue;

    // float collapsedMatrix[nL*nbins_collapsed][nL*nbins_collapsed];
    std::vector<std::vector<float> > collapsedMatrix;
    collapsedMatrix.resize(nL*nbins_collapsed, std::vector<float>(nL*nbins_collapsed,0));
    // Collapse the fullosc onto the nue sample.  This is tricky...

    for (int ibin = 0; ibin < nbins*nL; ++ibin)
    {
      for (int jbin = 0; jbin < nbins*nL; ++jbin)
      {
        // So, this is looping over the uncollapsed matrix.
        // Each point in the uncollapsed matrix has a single destination
        // in the collapsed matrix.
        int i_destination;
        int j_destination;

        // If the i or j bins are beyond nbins, then start by making the 
        // destinations the start of the correct block of the final matrix
        // Relying on the truncation here to make (ibin/nbins) = 0, 1, 2
        // Then scaling by the final block size
        i_destination = (ibin/nbins)*nbins_collapsed;
        j_destination = (jbin/nbins)*nbins_collapsed;

        // Now the block offset is known, but is this an osc bin that needs
        // to be collapsed or a normal bin?

        // this if is true if it's an osc bin
        if (ibin % nbins < nbins_nue)
          i_destination += (ibin % nbins);
        else //not an osc bin, so the destination is shorter by nbins_nue
          i_destination += (ibin % nbins)-nbins_nue;
        
        // this if is true if it's an osc bin
        if (jbin % nbins < nbins_nue)
          j_destination += (jbin % nbins);
        else //not an osc bin, so the destination is shorter by nbins_nue
          j_destination += (jbin % nbins)-nbins_nue;


        // Now the destination is set, just fill the matrix up:
        collapsedMatrix[i_destination][j_destination] += entries[ibin][jbin];
      }
    }

    return collapsedMatrix;

  }

  TMatrix SensUtils::CollapseMatrix(TMatrix entries,
                         int nbins_nue, int nbins_numu, int nL)
  {

    int nbins = 2*nbins_nue + nbins_numu;
    int nbins_collapsed = nbins - nbins_nue;

    // float collapsedMatrix[nL*nbins_collapsed][nL*nbins_collapsed];
    TMatrix collapsedMatrix;
    collapsedMatrix.ResizeTo(nL*nbins_collapsed,nL*nbins_collapsed);
    // Collapse the fullosc onto the nue sample.  This is tricky...

    for (int ibin = 0; ibin < nbins*nL; ++ibin)
    {
      for (int jbin = 0; jbin < nbins*nL; ++jbin)
      {
        // So, this is looping over the uncollapsed matrix.
        // Each point in the uncollapsed matrix has a single destination
        // in the collapsed matrix.
        int i_destination;
        int j_destination;

        // If the i or j bins are beyond nbins, then start by making the 
        // destinations the start of the correct block of the final matrix
        // Relying on the truncation here to make (ibin/nbins) = 0, 1, 2
        // Then scaling by the final block size
        i_destination = (ibin/nbins)*nbins_collapsed;
        j_destination = (jbin/nbins)*nbins_collapsed;

        // Now the block offset is known, but is this an osc bin that needs
        // to be collapsed or a normal bin?

        // this if is true if it's an osc bin
        if (ibin % nbins < nbins_nue)
          i_destination += (ibin % nbins);
        else //not an osc bin, so the destination is shorter by nbins_nue
          i_destination += (ibin % nbins)-nbins_nue;
        
        // this if is true if it's an osc bin
        if (jbin % nbins < nbins_nue)
          j_destination += (jbin % nbins);
        else //not an osc bin, so the destination is shorter by nbins_nue
          j_destination += (jbin % nbins)-nbins_nue;

        // if (jbin == 0)
          // std::cout << "ibin is " << ibin << ", destination is " 
                    // << i_destination << std::endl;

        // Now the destination is set, just fill the matrix up:
        collapsedMatrix[i_destination][j_destination] += entries[ibin][jbin];
      }
    }

    return collapsedMatrix;

  }

/*
  Float_t * SensUtils::CollapseMatrix(TMatrix entries,
                           int nbinsE, int nL)
  {
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

  Float_t * SensUtils::CollapseMatrix(std::vector<std::vector<float> > entries,
                           int nbinsE, int nL)
  {
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
*/

  bool SensUtils::fileExists(const char *filename)
  {
    ifstream ifile(filename);
    return ifile.is_open();
  }

  //make sure fracentries is initialized as [nbins][nbins] or this will
  // definitely fall over
  int SensUtils::buildCovarianceMatrix(std::vector<std::vector<float> > & fracentries, 
                                  std::vector<std::string> & cov_max_name, int nbins){    
    for(std::vector<std::string>::iterator it = cov_max_name.begin();
        it != cov_max_name.end(); it++)
    {
        const char * name = (*it).c_str();
        double readInArray[nbins][nbins];
        std::cout << "\nReading in covariance matrix from " << name << std::endl;
        if (fileExists(name) ){
            ifstream indata(name);
	    std::string line;
	    std::getline(indata, line);
	    std::stringstream ss(line);
            for (int i = 0; i < nbins; i ++ ){
	      //                std::string line;
	      //                std::getline(indata, line);
	      //                std::stringstream ss(line);
                for (int j = 0; j < nbins; j ++ ){
                    ss >> readInArray[i][j];
                }    
            }
            //Quick check on corner entries:
            std::cout << "Covariance Matrix " << name << "is read, check entries...?" << std::endl;
            std::cout << "Top left: " << readInArray[0][0] << std::endl;
            std::cout << "Bottom left: " << readInArray[0][nbins-1] << std::endl;
            std::cout << "Top right: " << readInArray[nbins-1][0] << std::endl;
            std::cout << "Bottom right: " << readInArray[nbins-1][nbins-1] << std::endl;    
        }
        else{
            std::cout << "Error while building covariance matrix: can't find file: \n" ;
            std::cout << "\t" << name << std::endl;
            return 1;
        }
        //At this point, got the file!  Add it to fracentries and set readInArray back to zero:
        for (int i = 0; i < nbins; i ++ ){
            for (int j = 0; j < nbins; j++){
                fracentries[i][j] += readInArray[i][j];
            } //loop over j
        } //loop over i
    } //loop over cov_max_name
    return 0;
  }

  std::vector<float> SensUtils::appendVectors(std::vector<float> vec1,
                                              std::vector<float> vec2){        
    std::vector<float> result;
    unsigned int size1 = vec1.size();
    unsigned int size2 = vec2.size();
    result.resize(size1+size2);
    for (unsigned int i = 0; i < result.size(); i++){
        if (i < size1) result[i] = vec1[i];
        else result[i] = vec2[i-size1];
    }
    return result;
  }

  std::vector<float> SensUtils::appendVectors(std::vector<float> vec1,
                                              std::vector<float>  vec2,
                                              std::vector<float>  vec3){
    std::vector<float> result;
    unsigned int size1 = vec1.size();
    unsigned int size2 = vec2.size();
    unsigned int size3 = vec3.size();
    result.resize(size1+size2+size3);
    for (unsigned int i = 0; i < result.size(); i++){
        if (i < size1) result[i] = vec1[i];
        else if (i < size1+size2) result[i] = vec2[i-size1];
        else result[i] = vec3[i-size1-size2];
    }
    return result;
  }

  TH1F* SensUtils::makeHistogram(std::vector<float> & input,
                                 double lowbin, double highbin){
    static int i = 0;
    char name[200];
    sprintf(name, "result_%i", i);
    TH1F * result = new TH1F(name,name,input.size(), lowbin, highbin);
    for (unsigned int j = 0; j < input.size(); j++){
        result ->SetBinContent(j+1, input[j]);
    }
    i++;
    return result;
  }

  TH1F* SensUtils::makeHistogram(std::vector<float> & input,
                                 std::vector<float> & bins){
    if (bins.size() != input.size()+1){
      std::cerr << "Error in make histogram, number of bins doesn't match input size.\n";
      std::cerr << "  bins: " << bins.size() << "\n";
      std::cerr << "  input: " << input.size() << "\n";
      exit(-1);
    }
    static int i = 0;
    char name[200];
    sprintf(name, "result2_%i", i);
    TH1F * result = new TH1F(name,name,input.size(),&(bins[0]));
    for (unsigned int j = 0; j < input.size(); j++){
        result ->SetBinContent(j+1, input[j]);
    }
    i++;
    return result;
  }
  std::vector<float> SensUtils::collapseVector(std::vector<float> input,
                                               int nbins_nue, int nbins_numu, int nL){
    //this function takes a vector and collapses the fullosc block onto the nue background
    //make a vector to store the output:

    int nbins = 2*nbins_nue + nbins_numu;
    int nbins_collapsed = nbins - nbins_nue;
    std::vector<float> output;
    output.resize(nbins_collapsed*nL);

    for (int ibin = 0; ibin < nbins*nL; ++ibin)
    {

        // So, this is looping over the uncollapsed vector.
        // Each point in the uncollapsed vector has a single destination
        // in the collapsed vector.
        int i_destination;

        // If the i bin is beyond nbins, then start by making the 
        // destinations the start of the correct block of the final vector
        // Relying on the truncation here to make (ibin/nbins) = 0, 1, 2
        // Then scaling by the final block size
        i_destination = (ibin/nbins)*nbins_collapsed;

        // Now the block offset is known, but is this an osc bin that needs
        // to be collapsed or a normal bin?

        // this if is true if it's an osc bin
        if (ibin % nbins < nbins_nue)
          i_destination += (ibin % nbins);
        else //not an osc bin, so the destination is shorter by nbins_nue
          i_destination += (ibin % nbins)-nbins_nue;

        // Now the destination is set, just fill the vector up:
        output[i_destination] += input[ibin];
    }


    // //Do this a simple but not super efficient way.  That's ok, though.
    // //Take the input vector, and split it into nL pieces of 3*nbinsE each:
    // std::vector<std::vector<float> > splitBaselines(nL, std::vector<float>(3*nbinsE,0));
    // for (unsigned int i = 0; i < splitBaselines.size(); i++){
    //     std::copy( input.begin()+ (i*3*nbinsE), input.begin() + i*3*nbinsE + 3*nbinsE, splitBaselines[i].begin());
    // }
    // //Now everything is split, start putting things together in the output:
    // for (unsigned int i = 0; i < splitBaselines.size(); i++){
    //     std::transform( splitBaselines[i].begin(), splitBaselines[i].begin() + nbinsE,
    //                     splitBaselines[i].begin()+ nbinsE, output.begin() + i*2*nbinsE,
    //                     std::plus<float>());
    //     std::copy(splitBaselines[i].begin()+2*nbinsE, splitBaselines[i].end(), output.begin()+ i*2*nbinsE + nbinsE);
    // }
    return output;
  }


}
