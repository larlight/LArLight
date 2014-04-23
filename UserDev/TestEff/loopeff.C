
{
gSystem->Load("libTestEff");


larlight::ana_processor proc;

//Specify Io mode


proc.set_io_mode(larlight::storage_manager::READ);
proc.set_input_rootdir("scanner");
proc.set_ana_output_file("cut_fuzzy_nufiltered_evts_wobadwires.root");

// proc.add_input_file("/data/argoneutnu/larlight_tracks_showers.root");  
proc.add_input_file("/data/argoneutnu/larlight_filtered_full_nu.root");  
//proc.add_input_file("../../combined_00.root") ; //~/data/combined_00.root");    //mac/larlight_tracks.root");

//proc.add_input_file("testing.root") ; //~/data/combined_00.root");    //mac/larlight_tracks.root");

//proc.add_input_file("/data/uboonemc/muons/combined_00.root");  
//proc.add_input_file("/data/uboonemc/electrons/combined_00.root");  

//proc.add_input_file("../../combined_02.root");    //mac/larlight_tracks.root");
//proc.set_verbosity(larlight::MSG::DEBUG);
proc.add_process(new larlight::CutEff);
proc.run(0);


}
