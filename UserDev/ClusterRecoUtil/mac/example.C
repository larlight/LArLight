{
  gSystem->Load("libClusterRecoUtil");
  
  larlight::storage_manager mgr;

  mgr.set_io_mode(larlight::storage_manager::READ);

  mgr.add_in_filename("data.root");

  mgr.set_in_rootdir("scanner");

  mgr.open();

  TCanvas* c = new TCanvas("c","c",600,500);
  
  cluster::ClusterParamsExecutor algo;

  while(mgr.next_event()) {

    larlight::event_cluster* clusters = (larlight::event_cluster*)(mgr.get_data(larlight::DATA::ShowerAngleCluster));
    
    if(!clusters) continue;

    for(size_t i=0; i<clusters->size(); ++i) {

      algo.LoadCluster(clusters->at(i),(larlight::event_hit*)(mgr.get_data(clusters->get_hit_type())));
      algo.FillParams(true,true,true,true,true);
      std::cout
	<< Form("(%g,%g)=>(%g,%g)", 
		algo.StartPoint().w,
		algo.StartPoint().t,
		algo.EndPoint().w,
		algo.EndPoint().t)
	<< std::endl;
      c->cd();
      algo.GetHitView()->Draw("COLZ");
      c->Update();
      std::string msg;
      gSystem->Sleep(7e3);
    }

  }
}


