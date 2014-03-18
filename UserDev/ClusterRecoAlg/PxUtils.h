struct PxPoint {
  public:
    double w; // wire distance in cm
    double t; // time distance in cm (drift distance)
    unsigned int plane; // plane 0, 1, 2
   
    PxPoint(){
	plane=0;
	w=0;
	t=0;
	std::cout<< "This is the default point ctor." << std::endl;
    }
};

struct PxHit : PxPoint {

    double charge; //charge

    PxHit(){
        plane = 0;
        w = 0;
        t = 0;
        charge = 0;
        std::cout << "This is the $^_*&ing default ctor." << std::endl;
    }

};

