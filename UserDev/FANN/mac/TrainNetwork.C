



{
  gSystem -> Load("libFANN");
  FANN::training_data data;
  FANN::neural_net net;
  data.read_train_from_file("training.dat");
  data.shuffle_train_data();


  cluster::TrainingModule tm;
  tm.setFeatureVectorLength(13);
  tm.setOutputVectorLength(2);
  tm.init();

  net = tm.ann;
  net.print_parameters();

  net.cascadetrain_on_data(data, 25, 1, 0.001);
  net.print_parameters();


  // FANN::neural_net net2;
  // net2.create_from_file("cascade_net.net");
  net.train_on_data(data, 10000, 100,0.0001);
  net.print_parameters();

  net.save("cascade_net.net");


}