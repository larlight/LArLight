#ifndef PULSE_VIEWER_CC
#define PULSE_VIEWER_CC

#include "pulse_viewer.hh"

namespace larlight {

  pulse_viewer::pulse_viewer() { 
    _name="pulse_viewer"; 
    _cWF=0;
    _hWF=0;
    _lStart=0;
    _lEnd=0;
    _lBase=0;
    _lRMSTop=0;
    _lRMSBottom=0;
    _lTop=0;

    clear_viewer();
    reset_cuts();

  }

  void pulse_viewer::reset_cuts() {

    _cut_tstart   = std::make_pair(-1,2000);
    _cut_tend     = std::make_pair(-1,2000);
    _cut_amp      = std::make_pair(-1,4096);
    _cut_charge   = std::make_pair(-1,4096*2000);
    _cut_pedbase  = std::make_pair(-1,4096);
    _cut_pedrms   = std::make_pair(0,4096);
    _cut_channels = std::make_pair(0,FEM::INVALID_CH);
    _cut_event_id = std::make_pair(0,0xffffffff);
    _cut_npulse   = std::make_pair(0,0xffffffff);
    _cut_sum_charge = std::make_pair(0,4096*2000*FEM::NUM_PMT_CHANNEL);
    _cut_sum_peak = std::make_pair(0,4096*FEM::NUM_PMT_CHANNEL);

  }

  void pulse_viewer::clear_viewer() {

    if(_hWF) delete _hWF;
    if(_lStart) delete _lStart;
    if(_lEnd) delete _lEnd;
    if(_lBase) delete _lBase;
    if(_lRMSTop) delete _lRMSTop;
    if(_lRMSBottom) delete _lRMSBottom;
    if(_lTop) delete _lTop;
  }

  bool pulse_viewer::initialize() {

    if(_verbosity_level==MSG::DEBUG)

      Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"called...");

    set_style();

    if(!_cWF){

      _cWF    = new TCanvas("_cWF","",1200,800);
      _cWF->SetLeftMargin(0.02);
      _cWF->SetBottomMargin(0.02);
      _cWF->SetRightMargin(0.02);

      _cWF->cd();
      //_pTitle = new TPad("_pTitle", "Title",      0.05, 0.92, 0.95, 0.99);
      //_pTitle->Draw();

      _cWF->cd();
      _pData  = new TPad("_pWF",    "Data",       0.01, 0.01, 0.99, 0.99);
      _pData->Draw();

      _pData->cd();
      _pNote  = new TPad("_pNote",  "Parameters", 0.80, 0.02, 0.98, 0.99);
      _pWF    = new TPad("_pWF",    "Waveform",   0.02, 0.02, 0.80, 0.99);
      _pWF->SetRightMargin(0.01);
      _pWF->SetLeftMargin(0.12);
      _pNote->Draw();
      _pWF->Draw();

      _fNote = new TPaveText(0.1,0.1,0.99,0.99);

    }

    clear_map();
    clear_viewer();
    return true;
  }

  void pulse_viewer::clear_map(){

    _pulse_frame.clear();
    _pulse_sample.clear();
    _pulse_tstart.clear();
    _pulse_tend.clear();
    _pulse_amp.clear();
    _pulse_charge.clear();
    _pulse_pedbase.clear();
    _pulse_pedrms.clear();
    _waveforms.clear();
    _pulse_count.clear();
    _channels.clear();

  }

  void pulse_viewer::add_channel_entry(UShort_t ch){

    _channels.insert(ch);
    _pulse_frame[ch]=std::vector<UInt_t>();
    _pulse_sample[ch]=std::vector<UInt_t>();
    _pulse_tstart[ch]=std::vector<double>();
    _pulse_tend[ch]=std::vector<double>();
    _pulse_amp[ch]=std::vector<double>();
    _pulse_charge[ch]=std::vector<double>();
    _pulse_pedbase[ch]=std::vector<double>();
    _pulse_pedrms[ch]=std::vector<double>();
    _pulse_count[ch]=std::make_pair(0,0);

    _waveforms[ch]=std::map<UInt_t,std::map<UInt_t,std::vector<UShort_t> > >();

  }

  bool pulse_viewer::analyze(storage_manager* storage) {

    if(_verbosity_level==MSG::DEBUG)

      Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"called...");

    event_pulse *pulses = (event_pulse*)(storage->get_data(DATA::Pulse));
    event_fifo   *wfs    = (event_fifo*)(storage->get_data(DATA::PMTFIFO));
    _ch_iter     = _channels.begin();


    if(!pulses)
      pulses = (event_pulse*)(storage->get_data(DATA::PMTPulse_FixedWin));

    if(!pulses)
      pulses = (event_pulse*)(storage->get_data(DATA::PMTPulse_ThresWin));

    if(pulses->size()==0) {
      Message::send(MSG::ERROR,__FUNCTION__,"No Pulse Found!");
      return false;
    }
    if(wfs->size()==0) {
      Message::send(MSG::ERROR,__FUNCTION__,"No Waveforms Found!");
      return false;
    }

    _event_id    = wfs->event_id();
    _sum_charge  = pulses->sum_charge();
    _sum_peak    = pulses->sum_peak();
    _npulse      = pulses->npulse();

    // Check if this event is in the range of users' interest
    if(_event_id < _cut_event_id.first || _cut_event_id.second < _event_id)
      return true;
    else if(_sum_charge < _cut_sum_charge.first || _cut_sum_charge.second < _sum_charge)
      return true;
    else if(_sum_peak < _cut_sum_peak.first || _cut_sum_peak.second < _sum_peak)
      return true;
    else if(_npulse < _cut_npulse.first || _cut_npulse.second < _npulse)
      return true;

    // Clear map
    clear_map();

    // Fill map for this event
    for(event_pulse::const_iterator iter(pulses->begin());
	iter!=pulses->end();
	++iter){

      UShort_t ch((*iter).channel_number());
      UInt_t   sample   = (*iter).readout_sample_number();
      UInt_t   frame    = (*iter).readout_frame_number();
      double   t_start  = (*iter).start_time();
      double   t_end    = (*iter).end_time();
      double   charge   = (*iter).charge();
      double   amp      = (*iter).pulse_peak();
      double   ped_base = (*iter).ped_mean();
      double   ped_rms  = (*iter).ped_rms();

      // Check if this pulse passes the criteria
      if(ch < _cut_channels.first || _cut_channels.second < ch)
	continue;
      if(t_start < _cut_tstart.first || _cut_tstart.second < t_start)
	continue;
      if(t_end < _cut_tend.first || _cut_tend.second < t_end)
	continue;
      if(charge < _cut_charge.first || _cut_charge.second < charge)
	continue;
      if(amp < _cut_amp.first || _cut_amp.second < amp)
	continue;
      if(ped_base < _cut_pedbase.first || _cut_pedbase.second < ped_base)
	continue;
      if(ped_rms < _cut_pedrms.first || _cut_pedrms.second < ped_rms)
	continue;    

      if(_channels.find(ch)==_channels.end())
	add_channel_entry(ch);

      _pulse_frame[ch].push_back(frame);
      _pulse_sample[ch].push_back(sample);
      _pulse_tstart[ch].push_back(t_start);
      _pulse_tend[ch].push_back(t_end);
      _pulse_amp[ch].push_back(amp);
      _pulse_charge[ch].push_back(charge);
      _pulse_pedbase[ch].push_back(ped_base);
      _pulse_pedrms[ch].push_back(ped_rms);

      if(_waveforms[ch].find(frame)==_waveforms[ch].end())
	_waveforms[ch][frame]=std::map<UInt_t,std::vector<UShort_t> >();

      if(_waveforms[ch][frame].find(sample) == _waveforms[ch][frame].end())
	_waveforms[ch][frame][sample]=std::vector<UShort_t>();

      _pulse_count[ch].first+=1;
    }



    for(event_fifo::const_iterator ch_iter(wfs->begin());
	ch_iter!=wfs->end();
	++ch_iter){
      UShort_t ch        = (*ch_iter).channel_number();
      UInt_t this_sample = (*ch_iter).readout_sample_number_RAW();
      UInt_t this_frame  = (*ch_iter).readout_frame_number();
      
      if(_waveforms.find(ch) == _waveforms.end())
	continue;
      if(_waveforms[ch].find(this_frame)==_waveforms[ch].end())
	continue;
      if(_waveforms[ch][this_frame].find(this_sample) == _waveforms[ch][this_frame].end())
	continue;

      if(_waveforms[ch][this_frame][this_sample].size()) {
	sprintf(_buf,"Found already filled waveform (ch=%d, frame=%d, sample=%d)",
		ch,this_frame,this_sample);
	Message::send(MSG::ERROR,__FUNCTION__,_buf);
	continue;
      }

      // Fill the waveform vector
      for(fifo::const_iterator adc_iter((*ch_iter).begin());
	  adc_iter!=(*ch_iter).end();
	  ++adc_iter){      
	_waveforms[ch][this_frame][this_sample].push_back((*adc_iter));
      }

    } // Finieh processing for this channel.

    _ch_iter=_channels.begin();
    std::cout<<_channels.size()<<std::endl;

    return true;
  }

  short pulse_viewer::get_npulse(UShort_t ch){

    if(_channels.find(ch)==_channels.end())
      return 0;

    return _pulse_count[ch].first;

  }

  UShort_t pulse_viewer::next_channel(){

    UShort_t ch=FEM::INVALID_CH;

    if(_ch_iter==_channels.end())
      return ch;

    else
      ch=(*_ch_iter);

    _ch_iter++;

    return ch;
  }

  TH1D* pulse_viewer::next_pulse(UShort_t ch){

    if(_channels.find(ch)==_channels.end()){
      sprintf(_buf,"No pulse found for channel %d!",ch);
      Message::send(MSG::ERROR,__FUNCTION__,_buf);
      return 0;
    }

    //  short this_index = _pulse_count[ch].second;

    // Check event cut criteria
    if( (_event_id < _cut_event_id.first || _cut_event_id.second < _event_id) ||
	(_sum_charge < _cut_sum_charge.first || _cut_sum_charge.second < _sum_charge) ||
	(_sum_peak < _cut_sum_peak.first || _cut_sum_peak.second < _sum_peak) ||
	(_npulse < _cut_npulse.first || _cut_npulse.second < _npulse) )
      {
	// event cut criteria changed and this event is not qualified anymore!
	_pulse_count[ch].second=_pulse_count[ch].first;
	return 0;
      }

    // Check pulse cut criteria
    short index=0;
    for(index = _pulse_count[ch].second;
	index < _pulse_count[ch].first;
	index++){

      if(index<0) index=0;

      if(ch < _cut_channels.first || _cut_channels.second < ch)
	continue;
      if(_pulse_tstart[ch][index] < _cut_tstart.first || _cut_tstart.second < _pulse_tstart[ch][index])
	continue;
      if(_pulse_tend[ch][index] < _cut_tend.first || _cut_tend.second < _pulse_tend[ch][index])
	continue;
      if(_pulse_charge[ch][index] < _cut_charge.first || _cut_charge.second < _pulse_charge[ch][index])
	continue;
      if(_pulse_amp[ch][index] < _cut_amp.first || _cut_amp.second < _pulse_amp[ch][index])
	continue;
      if(_pulse_pedbase[ch][index] < _cut_pedbase.first || _cut_pedbase.second < _pulse_pedbase[ch][index])
	continue;
      if(_pulse_pedrms[ch][index] < _cut_pedrms.first || _cut_pedrms.second < _pulse_pedrms[ch][index])
	continue;
      break;
    }

    if(index==_pulse_count[ch].first){
      _pulse_count[ch].second=_pulse_count[ch].first;
      return 0;
    }else{
      _pulse_count[ch].second=index+1;
      sprintf(_buf,"Pulse index %d for ch. %d",index,ch);
      Message::send(MSG::NORMAL,__FUNCTION__,_buf);
      return get_waveform(ch,(size_t)index);
    }

  }


  TH1D* pulse_viewer::previous_pulse(UShort_t ch){

    if(_channels.find(ch)==_channels.end()){
      sprintf(_buf,"No pulse found for channel %d!",ch);
      Message::send(MSG::ERROR,__FUNCTION__,_buf);
      return 0;
    }

    short this_index = _pulse_count[ch].second;

    if(this_index < 0){
      sprintf(_buf,"No more pulse found on ch %d... (%d/%d)",ch,this_index,_pulse_count[ch].first);
      Message::send(MSG::NORMAL,__FUNCTION__,_buf);
      return 0;
    }
    else if(this_index >= _pulse_count[ch].first) {
      sprintf(_buf,"Last pulse on Ch: %d",ch);
      Message::send(MSG::INFO,__FUNCTION__,_buf);
      this_index=_pulse_count[ch].first-1;
      _pulse_count[ch].second=this_index-1;
    }else{
      sprintf(_buf,"Pulse index %d for ch. %d",this_index,ch);
      Message::send(MSG::NORMAL,__FUNCTION__,_buf);
      _pulse_count[ch].second-=1;
    }

    return get_waveform(ch,(size_t)this_index);

  }


  TH1D* pulse_viewer::get_waveform(UShort_t ch, size_t index) {

    clear_viewer();
    gStyle->SetOptStat(0);

    bool is_wf_stored=true;
    UInt_t sample=_pulse_sample[ch][index];
    UInt_t frame =_pulse_frame[ch][index];

    if(_waveforms.find(ch)==_waveforms.end())
      is_wf_stored=false;
    else if(_waveforms[ch].find(frame)==_waveforms[ch].end())
      is_wf_stored=false;
    else if(_waveforms[ch][frame].find(sample)==_waveforms[ch][frame].end())
      is_wf_stored=false;

    if(!is_wf_stored){
      sprintf(_buf,"Waveform not stored for ch=%d, frame=%d, sample=%d!",
	      ch,frame,sample);
      Message::send(MSG::ERROR,__FUNCTION__,_buf);
      return 0;
    }

    if(_waveforms[ch][frame][sample].size()<1){
      sprintf(_buf,"Waveform has 0-length! ch=%d, frame=%d, sample=%d!",
	      ch,frame,sample);
      Message::send(MSG::ERROR,__FUNCTION__,_buf);
      return 0;
    }

    _hWF = new TH1D("hWF",
		    Form("Channel %d Reconstructed Pulse; Time-Slice; Amplitude",ch),
		    _waveforms[ch][frame][sample].size(),
		    -0.5,
		    (double)(_waveforms[ch][frame][sample].size())-0.5);

    for(size_t adc_index=0; adc_index<_waveforms[ch][frame][sample].size(); ++adc_index)
      _hWF->SetBinContent(adc_index+1,_waveforms[ch][frame][sample][adc_index]);

    //double x_min=_pulse_tend[ch][index]-20;
    //double x_max=_pulse_tend[ch][index]+20;
    double y_min=_pulse_pedbase[ch][index] - _pulse_pedrms[ch][index]*5 - 20;
    double y_max=_pulse_pedbase[ch][index] + _pulse_amp[ch][index]*1.1;

    _hWF->SetFillColor(kGray);
    _hWF->SetFillStyle(3004);
    _hWF->SetLineWidth(2);
    _hWF->SetMinimum(y_min);
    _hWF->SetMaximum(y_max);
    //_hWF->GetXaxis()->SetRangeUser(x_min,x_max);

    //
    // Canvas title
    //
    //_fTitle->SetText(0.5,0.5,Form("Reconstructed Waveform @ Event = %d ... Channel %d", _event_id,ch));

    //
    // Report parameters to stdout stream
    // 
    sprintf(_buf,"\n\n");
    sprintf(_buf,"%s Event ID       : %d\n",_buf,_event_id);
    sprintf(_buf,"%s Channel        : %d\n",_buf,ch);
    sprintf(_buf,"%s Start T        : %g\n",_buf,_pulse_tstart[ch][index]);
    sprintf(_buf,"%s End T          : %g\n",_buf,_pulse_tend[ch][index]);
    sprintf(_buf,"%s Ped. Mean      : %g\n",_buf,_pulse_pedbase[ch][index]);
    sprintf(_buf,"%s Ped. RMS       : %g\n",_buf,_pulse_pedrms[ch][index]);
    sprintf(_buf,"%s Peak Amp.      : %g\n",_buf,_pulse_amp[ch][index]);
    sprintf(_buf,"%s Charge         : %g\n",_buf,_pulse_charge[ch][index]);
    Message::send(MSG::NORMAL,__FUNCTION__,_buf);

    //
    // Report parameters to a canvas
    //
    _fNote->Clear();
    _fNote->AddText(Form("Event ID       : %d",_event_id));
    _fNote->AddText(Form("Frame  Number  : %d",_pulse_frame[ch][index]));
    _fNote->AddText(Form("Sample Number  : %d\n",_pulse_sample[ch][index]));
    _fNote->AddText(Form("Start T        : %g",_pulse_tstart[ch][index]));
    _fNote->AddText(Form("End T          : %g",_pulse_tend[ch][index]));
    _fNote->AddText(Form("Ped. Mean      : %g",_pulse_pedbase[ch][index]));
    _fNote->AddText(Form("Ped. RMS       : %g",_pulse_pedrms[ch][index]));
    _fNote->AddText(Form("Peak Amp.      : %g",_pulse_amp[ch][index]));
    _fNote->AddText(Form("Charge         : %g",_pulse_charge[ch][index]));

    //
    // Make waveform related  objects to be drawn
    //  
    _lBase = new TLine(_hWF->GetXaxis()->GetXmin(), _pulse_pedbase[ch][index], 
		       _hWF->GetXaxis()->GetXmax(), _pulse_pedbase[ch][index]);
    _lBase->SetLineWidth(2);
    _lBase->SetLineStyle(2);
    _lBase->SetLineColor(kBlack);

    _lRMSTop = new TLine(_hWF->GetXaxis()->GetXmin(), _pulse_pedbase[ch][index] + _pulse_pedrms[ch][index],
			 _hWF->GetXaxis()->GetXmax(), _pulse_pedbase[ch][index] + _pulse_pedrms[ch][index]);
    _lRMSTop->SetLineWidth(2);
    _lRMSTop->SetLineStyle(2);
    _lRMSTop->SetLineColor(kGray);

    _lRMSBottom = new TLine(_hWF->GetXaxis()->GetXmin(), _pulse_pedbase[ch][index] - _pulse_pedrms[ch][index],
			    _hWF->GetXaxis()->GetXmax(), _pulse_pedbase[ch][index] - _pulse_pedrms[ch][index]);
    _lRMSBottom->SetLineWidth(2);
    _lRMSBottom->SetLineStyle(2);
    _lRMSBottom->SetLineColor(kGray);

    _lStart = new TLine(_pulse_tstart[ch][index]-0.5, _pulse_pedbase[ch][index],
			_pulse_tstart[ch][index]-0.5, _pulse_pedbase[ch][index]+_pulse_amp[ch][index]);
    _lStart->SetLineWidth(2);
    _lStart->SetLineStyle(2);
    _lStart->SetLineColor(kBlue);

    _lEnd = new TLine(_pulse_tend[ch][index]+0.5, _pulse_pedbase[ch][index],
		      _pulse_tend[ch][index]+0.5, _pulse_pedbase[ch][index]+_pulse_amp[ch][index]);
    _lEnd->SetLineWidth(2);
    _lEnd->SetLineStyle(2);
    _lEnd->SetLineColor(kRed);

    _lTop = new TLine(_pulse_tstart[ch][index]-0.5, _pulse_pedbase[ch][index] + _pulse_amp[ch][index],
		      _pulse_tend[ch][index]+0.5,   _pulse_pedbase[ch][index] + _pulse_amp[ch][index]);
    _lTop->SetLineWidth(2);
    _lTop->SetLineStyle(2);
    _lTop->SetLineColor(kMagenta-7);

    // Draw
    _cWF->cd();

    // Waveform
    _pWF->cd();
    _hWF->Draw();
    _lBase->Draw();
    _lStart->Draw();
    _lEnd->Draw();
    _lRMSTop->Draw();
    _lRMSBottom->Draw();
    _lTop->Draw();
    _pWF->Modified();
    _pWF->Update();

    // Title
    //_pTitle->cd();
    //_fTitle->Draw();
    //_pTitle->Update();

    // Pulse parameters
    _pNote->cd();
    _fNote->Draw();
    _pNote->Update();
    _pData->Update();

    _cWF->Update();

    return _hWF;
  }

  bool pulse_viewer::finalize() {

    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    delete _cWF;
    clear_viewer();
    return true;
  }

  void pulse_viewer::display_cut_ranges(){

    std::string msg("Displaying the currently set values for cuts...\n");

    msg = "\n";
    msg += Form(" Event-wise parameters...\n");
    msg += Form("    Event ID            : %d -> %d\n",_cut_event_id.first,_cut_event_id.second);
    msg += Form("    Summed Charge       : %g -> %g\n",_cut_sum_charge.first,_cut_sum_charge.second);
    msg += Form("    Summed Peak         : %g -> %g\n",_cut_sum_peak.first,_cut_sum_peak.second);
    msg += Form("    Num. Pulses         : %d -> %d\n",_cut_npulse.first,_cut_npulse.second);
    msg += "\n";
    msg += Form(" Pulse-wise parameters...\n");
    msg += Form("    Channel Number      : %d -> %d\n",_cut_channels.first,_cut_channels.second);
    msg += Form("    Pulse Charge        : %g -> %g\n",_cut_charge.first,_cut_charge.second);
    msg += Form("    Pulse Peak          : %g -> %g\n",_cut_amp.first,_cut_amp.second);
    msg += Form("    Pulse Start T       : %g -> %g\n",_cut_tstart.first,_cut_tstart.second);
    msg += Form("    Pulse End T         : %g -> %g\n",_cut_tend.first,_cut_tend.second);
    msg += Form("    Pedestal Mean       : %g -> %g\n",_cut_pedbase.first,_cut_pedbase.second);
    msg += Form("    Pedestal RMS        : %g -> %g\n",_cut_pedrms.first,_cut_pedrms.second);

    Message::send(MSG::NORMAL,__FUNCTION__,msg.c_str());

  }

  void pulse_viewer::set_style(){

    TGaxis::SetMaxDigits(10);
    //TGaxis::SetNoExponent();
    gROOT->SetStyle("Plain");
    gStyle->SetEndErrorSize(0);
    gStyle->SetStatX(0.88);
    gStyle->SetOptFit(1);
    gStyle->SetOptDate(0);

    // Canvas
    gStyle->SetCanvasDefW(600);
    gStyle->SetCanvasDefH(500);
    gStyle->SetCanvasColor(0);
    gStyle->SetStatColor(0);
    gStyle->SetStatBorderSize(1);
    gStyle->SetFrameFillColor(10);
    gStyle->SetPadColor(23);
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    gStyle->SetTitleColor(0, "");
    gStyle->SetPadTopMargin(0.11);
    gStyle->SetPadRightMargin(0.08);
    gStyle->SetPadLeftMargin(0.13);
    gStyle->SetPadBottomMargin(0.10);

    // Font ... use large Times-Roman fonts
    gStyle->SetTextFont(42);
    gStyle->SetTextSize(0.01);
    gStyle->SetLabelFont(22,"xyz");
    gStyle->SetLabelSize(0.04,"xy");
    gStyle->SetLabelOffset(0.01,"xy");
    gStyle->SetTitleFont(22,"xyz");
    gStyle->SetTitleSize(0.04,"xy");
    gStyle->SetTitleOffset(1.2,"x");
    gStyle->SetTitleOffset(1.5,"y");


    // Gradient Color
    const Int_t NRGBs = 5;  
    const Int_t NCont = 99;
    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00};
    Double_t red[NRGBs] =   { 0.00, 0.00, 0.87, 1.00, 0.51};
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00};
    Double_t blue[NRGBs] =  { 0.51, 1.00, 0.12, 0.00, 0.00};
    TColor::CreateGradientColorTable(NRGBs,stops,red,green,blue,NCont);
    gStyle->SetNumberContours(NCont);

  }
}
#endif
