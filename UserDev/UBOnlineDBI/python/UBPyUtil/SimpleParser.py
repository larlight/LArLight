from UBPyLogger import UBPyLogger
import sys,os

class SimpleParser :

    _loggerName = __name__
    _logger     = UBPyLogger.GetME(__name__)
    
    (DEBUG,INFO,WARNING,ERROR,FATAL) = (_logger.debug,
                                        _logger.info,
                                        _logger.warning,
                                        _logger.error,
                                        _logger.fatal)

    (OPENLOG, CLOSELOG) = (UBPyLogger.OpenFile, UBPyLogger.CloseFile)

    def __init__(self):
        
        self.fname=''
        self.hstore_keys   = []
        self.hstore_values = {}

    def Read(self,fname):

        if os.path.isfile(fname):

            raise SimpleParserException("File does not exist: %s" % fname)
            
            return

        contents = open(fname,'r').read().split("\n")

        active_block=False

        for line in contents:

            if len(line.split(None)) < 1: continue
            if line.split(None)[0].startswith("#"): continue

