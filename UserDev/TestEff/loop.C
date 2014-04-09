


{


gSystem->Load("libTestEff");

larlight::ana_processor proc;

//Specify Io mode

proc.set_io_mode(larlight::storage_manager::READ);

proc.set_input_rootdir("scanner");

proc.add_input_file("mac/larlight_tracks.root");


proc.add_process(new larlight::TestEff);

proc.run();

}
