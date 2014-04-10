
{
gSystem->Load("libTestEff");


larlight::ana_processor proc;

//Specify Io mode

proc.set_io_mode(larlight::storage_manager::READ);
proc.set_input_rootdir("scanner");
proc.set_ana_output_file("ana.root");
proc.add_input_file("~/data/combined_00.root");    //mac/larlight_tracks.root");
proc.add_process(new larlight::TestEff);
proc.run();

cluster::ClusterParamsExecutor algo ;

//larlight::event_cluster evt  ;


}
