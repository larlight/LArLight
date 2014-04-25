



{
  gSystem -> Load("libFANN");
  FANN::training_data data_tracks;
  FANN::training_data data_showers;
  FANN::neural_net net;
  data_tracks.read_train_from_file("cctracks.dat");
  data_showers.read_train_from_file("fcshowers.dat");
  data_showers.merge_train_data(data_tracks);


  data_showers.shuffle_train_data();


  cluster::TrainingModule tm;
  tm.setFeatureVectorLength(13);
  tm.setOutputVectorLength(2);
  tm.init();

  net = tm.ann;
  net.print_parameters();

  net.cascadetrain_on_data(data_showers, 25, 1, 0.001);
  net.print_parameters();


  // FANN::neural_net net2;
  // net2.create_from_file("cascade_net.net");
  net.train_on_data(data_showers, 10000, 100,0.0001);
  net.print_parameters();

  net.save("cascade_argo_cctracks_fcshowers.net");


}