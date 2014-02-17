/**
 * \file vertex.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for vertex data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef VERTEX_HH
#define VERTEX_HH

#include "data_base.hh"

namespace larlight {
  /**
     \class vertex
     LArSoft Vertex class equivalent data container
  */
  class vertex : public data_base {
    
  public:
    
    /// Default constructor
    vertex(DATA::DATA_TYPE type=DATA::Vertex);

    /// Alternative constructor
    vertex(Double_t* xyz,
	   Int_t      id=DATA::INVALID_INT,
	   DATA::DATA_TYPE type=DATA::Vertex);
    
    /// Copy constructor
    vertex(const vertex &original) : data_base(original),
				     fID(original.fID)
    {original.XYZ(fXYZ);}
    
    /// Default destructor
    virtual ~vertex(){}

    /// Clear method
    virtual void clear_data();

    ///--- Getters ---///
    void XYZ(Double_t *xyz) const;
    Int_t ID() const   { return fID;     }
    Double_t X() const { return fXYZ[0]; }
    Double_t Y() const { return fXYZ[1]; }
    Double_t Z() const { return fXYZ[2]; }
    
  private:

    Double_t fXYZ[3];    ///< location of vertex
    Int_t    fID;        ///< id number for vertex
    
    ////////////////////////
    ClassDef(vertex,1)
    ////////////////////////
      
  };
  
  /**
     \class event_vertex
     A collection storage class of multiple vertex points
  */
  class event_vertex : public std::vector<larlight::vertex>,
		       public event_base {
    
  public:
    
    /// Default constructor
    event_vertex(DATA::DATA_TYPE type=DATA::Vertex);
    
    /// Default copy constructor
    event_vertex(const event_vertex& original) : std::vector<larlight::vertex>(original),
					      event_base(original)
    {}
    
    /// Default destructor
    ~event_vertex(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
  private:
    
    ////////////////////////
    ClassDef(event_vertex,1)
    ////////////////////////
  };
}
#endif

/** @} */ // end of doxygen group 
