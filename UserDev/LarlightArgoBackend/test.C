{
larlight::storage_manager mgr;
mgr.set_io_mode(mgr.READ);
mgr.add_in_filename("../../shower_larlight_11.root");
mgr.set_in_rootdir("scanner");
mgr.open();
mgr.go_to(0);
gSystem->Load("libLarlightArgoBackend.so");
JsonComposer jc;
ofstream out("out.json");
std::string a=jc.compose(&mgr);
out << a;
out.close();
}
