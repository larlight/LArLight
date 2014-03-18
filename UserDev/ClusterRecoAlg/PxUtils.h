struct point {
  public:
    double w; // wire distance in cm
    double t; // time distance in cm (drift distance)
    double charge;  // charge of the hit
    unsigned int plane; // plane 0, 1, 2
   
    point(){
	plane=0;
	w=0;
	t=0;
        charge = 0;
	std::cout<< "This is the default point ctor." << std::endl;
    }
}

