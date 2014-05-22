/**
 * \file evb_algo_base.hh
 *
 * \ingroup EventBuilder
 * 
 * \brief Event builder algorithm base class def
 *
 * @author Kazu - Nevis 2014
 */

/** \addtogroup EventBuilder

    @{*/

#ifndef EVB_ALGO_BASE_HH
#define EVB_ALGO_BASE_HH

#include "Base-TypeDef.hh"
#include "DataFormat-TypeDef.hh"
#include "evb_exception.hh"
namespace larlight {

  class evb_algo_base : public larlight_base {

  public:

    evb_algo_base(DATA::DATA_TYPE type=DATA::DATA_TYPE_MAX);

    virtual ~evb_algo_base(){}

    void add_input_file(std::string fname){_storage->add_in_filename(fname);}

    DATA::DATA_TYPE data_type() const {return _type;}

    bool eof() const { return _eof;}
    
    virtual bool initialize();
    
    bool process(storage_manager* out_storage, UInt_t id=DATA::INVALID_UINT);

    virtual bool check_event_quality(){return true;}

    virtual bool finalize();

    void SetPlotFile(TFile* fout) { _fout = fout; _analyze=true;}

  protected:

    UInt_t get_event_number() const;

    virtual bool build(storage_manager *out_storage,UInt_t id)
    {return true;}

    bool find_next(UInt_t id);

  protected:

    bool _eof;

    bool _analyze;

    UInt_t _current_event_number;

    std::vector<std::string> _input_files;

    DATA::DATA_TYPE _type;

    storage_manager *_storage;

    TFile *_fout;
    
  };
}
#endif
/** @} */ // end of doxygen group 
