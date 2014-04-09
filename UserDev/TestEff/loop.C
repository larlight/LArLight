


{


gSystem->Load("libTestEff");

larlight::ana_processor proc;

//Specify Io mode

proc.set_io_mode(larlight::storage_manager::READ);

proc.set_input_rootdir("scanner");

proc.add_input_file("~/data/showers/combined_00.root");


proc.add_process(new larlight::TestEff);

proc.run();

}