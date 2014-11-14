
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
                                            std::vector<float> bins)
  {

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
  


  
  TH1F* SensUtils::AppendHistograms(TH1F * hist1, TH1F* hist2, TH1F* hist3)
  {
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
  
  TH1F* SensUtils::AppendHistograms(TH1F * hist1, TH1F* hist2)
  {
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

  bool SensUtils::fileExists(const char *filename)
  {
    ifstream ifile(filename);
    return ifile.is_open();
  }



  std::vector<float> SensUtils::appendVectors(std::vector<float> vec1,
                                              std::vector<float> vec2)
  {        
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
                                              std::vector<float> vec2,
                                              std::vector<float> vec3)
  {
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
                                 double lowbin, double highbin)
  {
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
                                 std::vector<float> & bins)
  {
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
                                               int nbins_nue, int nbins_numu, int nL)
  {
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


    return output;
  }

  // This function puts together the name of the matrix requested
  TString SensUtils::GetMatrixFileName( TString fileSource,
                                        TString detNamesString,
                                        bool includeNumus,
                                        bool useXSecWeights,
                                        bool useFluxWeights,
                                        int  multiWeightSource,
                                        bool absolute_MWSource){
      TString matrixFileName = fileSource;
      matrixFileName += "matrixFile_nue_";
      if (includeNumus) matrixFileName += "numu_";
      matrixFileName += detNamesString;
      if (useXSecWeights) matrixFileName += "xsec_";
      if (useFluxWeights) matrixFileName += "flux_";
      if (absolute_MWSource) matrixFileName += "abs_";
      matrixFileName += std::to_string(multiWeightSource) + ".root";
      return matrixFileName;
  }

  // This function puts together the name of the matrix requested
  TString SensUtils::GetMatrixFileName( TString fileSource,
                                        TString detNamesString,
                                        TString fileNameRoot,
                                        bool includeNumus,
                                        bool useXSecWeights,
                                        bool useFluxWeights,
                                        int  multiWeightSource,
                                        bool absolute_MWSource){
      TString matrixFileName = fileSource;
      matrixFileName += "matrixFile_nue_";
      if (includeNumus) matrixFileName += "numu_";
      matrixFileName += detNamesString;
      if (useXSecWeights) matrixFileName += "xsec_";
      if (useFluxWeights) matrixFileName += "flux_";
      if (absolute_MWSource) matrixFileName += "abs_";
      matrixFileName += std::to_string(multiWeightSource);

      if (fileNameRoot.BeginsWith("nue_appearance"))
        fileNameRoot.Remove(0,14);
      else{
        std::cerr << "Theres some thing strange about the fileNameRoot that was supplied!";
      }
      // filenameroot has a trailing "_", trim it off:
      fileNameRoot.Remove(fileNameRoot.Sizeof()-2,1);

      // std::cout << "File name root is added as: " << fileNameRoot << std::endl;
      matrixFileName += fileNameRoot;


      matrixFileName += ".root";
      return matrixFileName;
  }



  // This function puts together the name of the matrix requested
  TString SensUtils::GetMatrixFileName( TString fileSource,
                                        TString detNamesString,
                                        TString fileNameRoot,
                                        bool includeNumus,
                                        std::string uncert,
                                        int  multiWeightSource,
                                        bool absolute_MWSource){
      TString matrixFileName = fileSource;
      matrixFileName += "matrixFile_nue_";
      if (includeNumus) matrixFileName += "numu_";
      matrixFileName += detNamesString;
      matrixFileName += uncert;
      matrixFileName += "_";
      if (absolute_MWSource) matrixFileName += "abs_";
      matrixFileName += std::to_string(multiWeightSource);
      
      if (fileNameRoot.BeginsWith("nue_appearance"))
        fileNameRoot.Remove(0,14);
      else{
        std::cerr << "Theres some thing strange about the fileNameRoot that was supplied!";
      }
      // filenameroot has a trailing "_", trim it off:
      fileNameRoot.Remove(fileNameRoot.Sizeof()-2,1);

      // std::cout << "File name root is added as: " << fileNameRoot << std::endl;
      matrixFileName += fileNameRoot;
      matrixFileName += ".root";
      return matrixFileName;
  }


  TString SensUtils::GetChi2FileName(TString fileSource,
                                     TString fileNameRoot,
                                     TString detNamesString,
                                     bool includeNumus,
                                     std::vector<std::string> covMatrixList,
                                     std::vector<int> covMatrixListSource,
                                     bool absolute_MWSource)
  {

    // This block of code sets up where the chi2 data is going and what assumptions went
    // into go into the file name.
    TString chi2FileName = fileSource;

    chi2FileName += "chi2_nue_";
    if (includeNumus)
      chi2FileName += "numu_";

    if (fileNameRoot.BeginsWith("nue_appearance_"))
      fileNameRoot.Remove(0,15);
    else{
      std::cerr << "There is some thing strange about the fileNameRoot that was supplied!";
    }

    for (unsigned int i = 0; i < covMatrixList.size(); i ++){
      chi2FileName += covMatrixList.at(i);
      chi2FileName += "_";
      chi2FileName += covMatrixListSource.at(i);
      chi2FileName += "_";
    }

    if (absolute_MWSource){
      chi2FileName += "abs_";
    }

    chi2FileName += fileNameRoot;
    chi2FileName += detNamesString;
    
    chi2FileName = chi2FileName+"chi2.root";


    return chi2FileName;

  }

  TMatrix * SensUtils::assembleCovarianceMatrix(TString fileSource,
                                                TString detNamesString,
                                                TString fileNameRoot,
                                                bool includeNumus,
                                                const std::vector<float> & nullVector,
                                                std::vector<std::string> covMatrixList,
                                                std::vector<int> covMatrixListSource,
                                                bool absolute_MWSource)
  {
    // This function needs to grab all of the individual covariance matrices 
    // (including being responsible for file I/O) and assemble it into a final 
    // non fractional covariance matrix.
    // Scaling the matrix can be done by something else.
    
    TMatrix * finalCovarianceMatrix = new TMatrix;

    finalCovarianceMatrix->ResizeTo(nullVector.size(), nullVector.size());


    // First, make sure the lists of sources are the same length:
    if (covMatrixList.size() != covMatrixListSource.size()){
      std::cerr << "Error: In assemble Covariance matrix, the list of sources isn't right.\n";
      exit(-1);
    }

    // next, start looping over the lists and getting the fractional uncertainty matrices.
    for (unsigned int item = 0; item < covMatrixList.size(); item ++){
      // Before anything can happen, gotta get the name of the matrix file:
      TString name = GetMatrixFileName(fileSource, detNamesString, fileNameRoot,
                                       includeNumus, covMatrixList.at(item),
                                       covMatrixListSource.at(item),
                                       absolute_MWSource);
      // check that the file exists:
      if (!(fileExists(name))){
        std::cerr <<"Error: Assemble covariance matrix tried to access file "
                  << name << ".\n";
        exit(-1);
      }

      // Next, open the file and get the matrix in fractional form:
      TFile  f(name);
      TMatrix * matrix = (TMatrix*) f.Get("fractionalCovarianceMatrix");

      // Check that the matrix size is OK:
      if ((unsigned int) matrix -> GetNcols() != nullVector.size() ||
          (unsigned int) matrix -> GetNrows() != nullVector.size())
      { 
        std::cerr << "Fractional matrix from file " << name << " doesn't match"
                  << "the size of the null vector provided.\n";
        exit(-1);
      } 

      // std::cout << "Got the matrix from file, first entry is " << (*matrix)[1][1] << std::endl;

      // If it's ok, start scaling it back up to the full non-fractional matrix:
      for (unsigned int i = 0; i < nullVector.size(); i++){
        for (unsigned int j = 0; j < nullVector.size(); j++){
          (*finalCovarianceMatrix)[i][j] += (*matrix)[i][j]*nullVector[i]*nullVector[j];
        }
      }

    }

    return finalCovarianceMatrix;
  }

  float SensUtils::getMaximum (const std::vector<float> & vals){
    float max = vals.front();
    for (auto & val : vals)
      if (max < val) max = val;
    return max;
  }


  std::vector<float> SensUtils::smoothVector(const std::vector<float> & data,
                                             unsigned int range)
  {
  
    std::vector<float> output(data.size(), 0.0);
    for (unsigned int i = 0; i < data.size(); ++i)
    {
      if (i < range){
        for (unsigned int val = 0; val <= 2*i; val ++){
          output[i] += data[val];
        }
        output[i] /= 2*i+1;      
      }
      else if (i >= data.size() - range){
        double temp_range= data.size() - i - 1;
        for (int val = i - temp_range; val <= i + temp_range; val ++){
          output[i] += data[val];
        }
        output[i] /= 2*temp_range+1;     
      }
      else{
        for (unsigned int val = i - range; val <= i + range; val ++){
          output[i] += data[val];
        }
        output[i] /= 2*range+1;
      }
    }
    return output;
  }

}
