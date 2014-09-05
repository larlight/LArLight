/**
 * \file Algo_Class_Name.hh
 *
 * \ingroup PulseReco
 * 
 * \brief Class definition file of Algo_Class_Name
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup PulseReco

    @{*/

#ifndef ALGO_CLASS_NAME_HH
#define ALGO_CLASS_NAME_HH

#include "preco_algo_base.hh"

namespace optreco {

  /**
     \class Algo_Class_Name
     DOCUMENT ME ... $SHELL_USER_NAME !
   */
  class Algo_Class_Name : public preco_algo_base {

  public:

    /// Default constructor
    Algo_Class_Name();

    /// Default destructor
    virtual ~Algo_Class_Name(){};

    /// Implementation of preco_algo_base::reco() method
    virtual bool reco(const std::vector<unsigned short> &wf);

    /// Implementation of preco_algo_base::reset() method
    virtual void reset();

  protected:

  };
}
#endif

/** @} */ // end of doxygen group
