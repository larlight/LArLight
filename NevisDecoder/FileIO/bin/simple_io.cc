#include "FileIO-TypeDef.hh"

void   compare_content(std::string filename1, std::string filename2);
double write_file(std::string filename,
		  const PMT::word_t* word_array, 
		  size_t length,
		  bool use_array_write);

int main(){

  // Prepare fake words to use in this test
  const size_t length=10000;
  PMT::word_t word_array[length]={0};
  for(size_t i=0; i<length; i++)
    word_array[i]=(PMT::word_t)i;

  std::string fname1="test_word.dat";
  std::string fname2="test_array.dat";

  double word_time  = write_file(fname1,word_array,length,false);
  double array_time = write_file(fname2,word_array,length,true);

  compare_content(fname1,word_array);
  compare_content(fname2,word_array);
  
  std::cout
    << "Time profile result for 10000 loop to write 10000 words..." << std::endl
    << "word-by-word: " << word_time  << " [s]" << std::endl
    << "array       : " << array_time << " [s]" << std::endl
    << std::endl;

}

double write_file(std::string filename,
		  const PMT::word_t* word_array, 
		  size_t length,
		  bool use_array_write){

  // Prepare file i/o instance
  bin_io_handler file;
  file.set_mode(bin_io_handler::WRITE);
  file.set_format(FORMAT::BINARY);
  file.set_filename(filename.c_str());
  file.open();

  clock_t start=clock();
  // write words
  if(use_array_write){
    for(size_t i=0; i<10000; i++)
      file.write_multi_word(word_array,length);
  }else{
    for(size_t i=0; i<10000; i++){
      for(size_t j=0; j<length; j++){
	file.write_word(word_array[j]);
      }
    }
  }
  clock_t end=clock();
  double duration = (end-start)/(double)(CLOCKS_PER_SEC);
  file.close();

  return duration;
}

void compare_content(std::string filename1, std::string filename2){

  //
  // This is a simple test of read/write.
  //

  // Make an instance
  bin_io_handler file1;
  bin_io_handler file2;

  // First: write fake words.
  file1.set_mode(bin_io_handler::READ);
  file1.set_format(FORMAT::BINARY);
  file1.set_filename(filename1.c_str());
  file1.set_verbosity(MSG::DEBUG);
  file1.open();

  // First: write fake words.
  file2.set_mode(bin_io_handler::READ);
  file2.set_format(FORMAT::BINARY);
  file2.set_filename(filename2.c_str());
  file2.set_verbosity(MSG::DEBUG);
  file2.open();

  bool status=true;
  while(1){
    PMT::word_t word1=file1.read_word();
    PMT::word_t word2=file2.read_word();

    if(word1!=word2){
      std::cout
	<< "Found an inconsistent word pair! "
	<< word1
	<< " : "
	<< word2
	<< std::endl;
      status=false;
      break;
    }
    if(file1.eof()){
      if(!file2.eof()){
	std::cout
	  << filename1.c_str()
	  << " contains more words than "
	  << filename2.c_str()
	  << std::endl;
      }
      break;      
    }
  }

  if(status) std::cout << " two files have identical contents!" << std::endl;
  else std::cout << " there is a difference among two files ..." << std::endl;

}
