
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

  std::vector<float> SensUtils::collapseVector(std::vector<float> input,
                                               int nbinsE, int nL){
    //this function takes a vector and collapses the fullosc block onto the nue background
    //make a vector to store the output:
    std::vector<float> output(nbinsE*nL*2,0);
    //Do this a simple but not super efficient way.  That's ok, though.
    //Take the input vector, and split it into nL pieces of 3*nbinsE each:
    std::vector<std::vector<float> > splitBaselines(nL, std::vector<float>(3*nbinsE,0));
    for (unsigned int i = 0; i < splitBaselines.size(); i++){
        std::copy( input.begin()+ (i*3*nbinsE), input.begin() + i*3*nbinsE + 3*nbinsE, splitBaselines[i].begin());
    }
    //Now everything is split, start putting things together in the output:
    for (unsigned int i = 0; i < splitBaselines.size(); i++){
        std::transform( splitBaselines[i].begin(), splitBaselines[i].begin() + nbinsE,
                        splitBaselines[i].begin()+ nbinsE, output.begin() + i*2*nbinsE,
                        std::plus<float>());
        std::copy(splitBaselines[i].begin()+2*nbinsE, splitBaselines[i].end(), output.begin()+ i*2*nbinsE + nbinsE);
    }
    return output;
  }


}
