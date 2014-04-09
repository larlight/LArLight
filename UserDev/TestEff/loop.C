


{


gSystem->Load("libTestEff");

larlight::ana_processor proc;

//Specify Io mode

proc.set_io_mode(larlight::storage_manager::READ);

proc.set_input_rootdir("scanner");

<<<<<<< HEAD
proc.add_input_file("mac/larlight_tracks.root");
=======
proc.add_input_file("~/data/showers/combined_00.root");
>>>>>>> 7423415193592c094a049d4ee12d8b6c3d9fbace


proc.add_process(new larlight::TestEff);

proc.run();

}
