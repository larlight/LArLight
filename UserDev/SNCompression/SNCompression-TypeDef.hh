//
// type-def file for ROOT
//

#ifndef SNCOMPRESSION_TYPEDEF_HH
#define SNCOMPRESSION_TYPEDEF_HH

// Header files included here
#include "CompressionAlgoBase.hh"
#include "CompressionAlgoThresh.hh"
#include "ExecuteCompression.hh"
#include "StudyCompressionBufferSize.hh"
#include "ViewCompression.hh"
#include "HitMaker.hh"
#include "HitViewer.hh"
#include "WFViewer.hh"
#include "WFSaver.hh"
#include "CompressionAlgosncompress.hh"
#include "CompressionAlgoTwoThresh.hh"
#include "CompressionAlgoOneThresh.hh"
#include "CompressionStudyBase.hh"
#include "CompressionStudyHits.hh"
//ADD_NEW_HEADER ... do not change this comment line

// Class forward declaration here
namespace larlight {

  class ExecuteCompression;
  class ViewCompression;
  class HitMaker;
  class StudyCompressionBufferSize;
  class HitViewer;
  class WFViewer;
  class WFSaver;
  //ADD_ANA_CLASS ... do not change this comment line
  
}


namespace compression {

  class CompressionAlgoBase;
  class CompressionAlgoThresh;
  class CompressionAlgoTwoThresh;
  class CompressionAlgoOneThresh;
  class CompressionAlgosncompress;
  class CompressionStudyBase;
  class CompressionStudyHits;
  //ADD_COMP_CLASS ... do not change this comment line
}

//ADD_EMPTY_CLASS ... do not change this comment line
#endif




