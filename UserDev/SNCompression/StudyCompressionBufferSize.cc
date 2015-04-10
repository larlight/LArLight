#ifndef STUDYCOMPRESSIONBUFFERSIZE_CC
#define STUDYCOMPRESSIONBUFFERSIZE_CC

#include "StudyCompressionBufferSize.hh"

namespace larlight {


  StudyCompressionBufferSize::StudyCompressionBufferSize()
    : _base_tree(nullptr)
    , _compress_tree(nullptr)
    , _compress_algo(nullptr)
    , Model(nullptr)  
  {
    _name="StudyCompressionBufferSize";
    _fout=0;
    _compress_algo = 0; 
  };

  bool StudyCompressionBufferSize::initialize() {
    

    if (!_base_tree)
      _base_tree = new TTree("_base_tree","Baseline Tree");
    _base_tree->Branch("_base_pre",&_base_pre,"base_pre/D");
    _base_tree->Branch("_base_post",&_base_post,"base_post/D");
    _base_tree->Branch("_slope_pre",&_slope_pre,"slope_pre/D");
    _base_tree->Branch("_slope_post",&_slope_post,"slope_post/D");
    _base_tree->Branch("_baseChan",&_baseChan,"baseChan/D");
    _base_tree->Branch("_varChan",&_varChan,"varChan/D");
    _base_tree->Branch("_ch",&_ch,"ch/I");
    _base_tree->Branch("_sygType",&_sygType,"sygType/I");
    _base_tree->Branch("_plane",&_plane,"plane/I");


    // Initalize Histogram that tracks compression factor
    if (!_compress_tree)
      _compress_tree = new TTree("_compress_tree","Compression Info Tree");
    _compress_tree->Branch("_evt",&_evt,"evt/I");
    _compress_tree->Branch("_compression",&_compression,"compression/D");

    _evt = 0;

    PrepareLinearModel();

    return true;
  }
  
  bool StudyCompressionBufferSize::analyze(storage_manager* storage) {

    // If no compression algorithm has been defined, skip
    if ( _compress_algo == 0 ){
      print(MSG::ERROR,__FUNCTION__,"Compression Algorithm Not Set! Exiting");
      return false;
    }

    // Otherwise Get RawDigits and execute compression
    larlight::event_tpcfifo *event_wf = (event_tpcfifo*)(storage->get_data(DATA::TPCFIFO));
    // If raw_digits object is empty -> exit
    if(!event_wf) {
      print(MSG::ERROR,__FUNCTION__,"Data storage did not find associated waveforms!");
      return false;
    }

    // clear place-holder for new, compressed, waveforms
    _out_event_wf.clear();

    // reset variables that hold compression factor
    _inTicks  = 0;
    _outTicks = 0;
    
    // Loop over all waveforms
    for (size_t i=0; i<event_wf->size(); i++){
      
      //get tpc_data
      larlight::tpcfifo* tpc_data = (&(event_wf->at(i)));      
      //Check for empty waveforms!
      if(tpc_data->size()<1){
	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Found 0-length waveform: Event %d ... Ch. %d",event_wf->event_number(),tpc_data->channel_number()));
	continue;
      }//if wf size < 1

      // Figure out channel's plane:
      // used because different planes will have different "buffers"
      UInt_t ch = tpc_data->channel_number();
      int pl = larutil::Geometry::GetME()->ChannelToPlane(ch);

      _plane = pl;
      _ch = ch;

      // finally, apply compression:
      // *-------------------------*
      // 1) Convert tpc_data object to just the vector of shorts which make up the ADC ticks
      std::vector<unsigned short> ADCwaveform = getADCs(tpc_data);
      // Calculate total baseline for channel
      _baseChan = 0;
      _varChan = 0;
      for (size_t b=0; b < 200; b++)
	_baseChan += ADCwaveform[b];
      _baseChan/=200.;
      for (size_t c=0; c < 200; c++)
	_varChan += (ADCwaveform[c]-_baseChan)*(ADCwaveform[c]-_baseChan);
      _varChan = sqrt(_varChan/200.);

      // 2) Now apply the compression algorithm. _compress_algo is an instance of CompressionAlgoBase
      _compress_algo->ApplyCompression(ADCwaveform,pl,ch);
      // 3) Retrieve the output waveforms (vectors of vectors of shorts) produced during the compression
      std::vector<std::vector<unsigned short> > compressOutput = _compress_algo->GetOutputWFs();
      // Now, for each output waveform, calcualte pre & post baseline and fill TTree with info
      for (auto &wf : compressOutput)
	StudyBaseline(wf);
      // 4) Retrieve the time-ticks at which each output waveform saved starts
      std::vector<int> outTimes = _compress_algo->GetOutputWFTimes();
      // 5) Calculate compression factor [ for now Ticks After / Ticks Before ]
      CalculateCompression(ADCwaveform, compressOutput);
      // 6) clear _InWF and _OutWF from compression algo object -> resetting algorithm for next time it is called
      _compress_algo->Reset();
      // 7) Replace .root data file *event_wf* with new waveforms
      //SwapData(tpc_data, compressOutput, outTimes);
	
    }//for all waveforms

    // store compression factor for this event
    _compression = _outTicks/_inTicks;
    _compress_tree->Fill();
    _evt += 1;
    
    //now take new WFs and place in event_wf vector
    event_wf->clear();
    for (size_t m=0; m < _out_event_wf.size(); m++)
      event_wf->push_back(_out_event_wf.at(m));
    return true;
  }

  bool StudyCompressionBufferSize::finalize() {

    _compress_algo->EndProcess(_fout);
    _compress_tree->Write();
    _base_tree->Write();

    return true;
  }

  void StudyCompressionBufferSize::StudyBaseline(const std::vector<unsigned short> &wf){

    size_t l = wf.size();

    float t0 = wf[0];
    float t1 = wf[1];
    float t2 = wf[2];
    float t3 = wf[l-3];
    float t4 = wf[l-2];
    float t5 = wf[l-1];

    _base_pre   = (t0+t1+t2)/3.;
    _base_post  = (t3+t4+t5)/3.;
    _slope_pre  = 0;
    _slope_post = 0;
    
    const int n=3;
    float DataFront[n] = {t0,t1,t2};
    float DataBack[n] = {t3,t4,t5};
    float Tick[n] = {1,2,3};
    float Errors[n] = {0.5,0.5,0.5};
    float TickErr[n] = {0,0,0};

    TGraph *grPre  = new TGraphErrors(n,Tick,DataFront,TickErr,Errors);
    TGraph *grPost = new TGraphErrors(n,Tick,DataBack,TickErr,Errors);

    grPre->Fit("Model","QN");
    _slope_pre = Model->GetParameter(1);    
    grPost->Fit("Model","QN");
    _slope_post = Model->GetParameter(1);    
    
    delete grPre;
    delete grPost;

    _base_tree->Fill();

    return;
  }


  void StudyCompressionBufferSize::PrepareLinearModel(){

    if (Model) delete Model;
    Model = new TF1("Model","[0]+[1]*x",1,3);
    Model->SetParName(0,"const");
    Model->SetParName(1,"slope");
    return;
  }


  std::vector<unsigned short> StudyCompressionBufferSize::getADCs(larlight::tpcfifo* tpc_data){

    std::vector<unsigned short> ADCs;
    ADCs.clear();
    for (size_t i=0; i < tpc_data->size(); i++)
      ADCs.push_back((unsigned short)(tpc_data->at(i)));

    return ADCs;

  }

  void StudyCompressionBufferSize::SwapData(larlight::tpcfifo* tpc_data, std::vector<std::vector<unsigned short> > outputWFs,
				    std::vector<int> outTimes){

    // In this function we are taking the old larlight::tpcfifo object and replacing it with
    // new larlight::tpcfifo objects, one per output waveform coming from compression
    // Most of the information for this object (channel number, wire plane, etc) stays the same
    // What changes is the actual list of ADC counts, and the start time of the vector.

    UInt_t wf_time = tpc_data->readout_sample_number_RAW();
    UInt_t chan = tpc_data->channel_number();


    //loop over new waveforms created
    for (size_t n=0; n < outputWFs.size(); n++){
      larlight::tpcfifo new_tpc_data( chan, tpc_data->readout_frame_number(),
				      wf_time+outTimes.at(n), tpc_data->module_address(),
				      tpc_data->module_id(), larutil::Geometry::GetME()->View(chan),
				      larutil::Geometry::GetME()->SignalType(chan),
				      DATA::TPCFIFO, outputWFs.at(n));

      _out_event_wf.push_back(new_tpc_data);
    }

    return;
  }


  void StudyCompressionBufferSize::CalculateCompression(std::vector<unsigned short> beforeADCs, std::vector<std::vector<unsigned short> > afterADCs){
    
    _inTicks += beforeADCs.size();

    for (size_t n=0; n < afterADCs.size(); n++)
      _outTicks += afterADCs.at(n).size();

    return;
  }

}
#endif
