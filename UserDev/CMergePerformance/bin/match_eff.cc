//
// Example C++ routine to run your analysis module, CMergePerformance.
//

#include "Analysis-TypeDef.hh"
#include "CMTool-TypeDef.hh"
#include "CMergePerformance-TypeDef.hh"

int main(int argc, char** argv){

  if(argc<2) {

    std::cerr
      << std::endl
      << Form("Usage 1: %s $INPUT_ROOT_FILE",argv[0]) << std::endl
      << Form("Usage 2: %s $INPUT_ROOT_FILE $TDIRECTORY_NAME",argv[0]) << std::endl
      << std::endl;

    return 1;
  }

  std::cout
    << std::endl
    << "This is \"Analysis\" package simple test routine." << std::endl
    << std::endl;

  // Create ana_processor instance
  larlight::ana_processor my_proc;

  // Set input root file
  my_proc.add_input_file(argv[1]);

  // Specify IO mode
  my_proc.set_io_mode(larlight::storage_manager::READ);

  // Specify input TDirectory name if given
  if(argc>2)
    my_proc.set_input_rootdir(argv[2]);

  // Specify output root file name
  my_proc.set_ana_output_file("out.root");

  auto ana_module = new larlight::CMatchPerformance();

  auto priority_algo = new cmtool::CPAlgoNHits();

  auto match_algo = new cmtool::CFAlgoStartPointMatch();

  priority_algo->SetMinHits(20);

  ana_module->GetManager().AddMatchAlgo(match_algo);

  ana_module->GetManager().AddPriorityAlgo(priority_algo);

  my_proc.add_process(ana_module);

  std::cout
    << std::endl
    << "Finished configuring ana_processor. Start event loop!"<< std::endl
    << std::endl;

  // Let's run it.
  my_proc.run();

  // done!
  std::cout
    << std::endl
    << "Finished running ana_processor event loop!" << std::endl
    << std::endl;
  return 0;
}
