import os, sys
from UBPySQLException import UBPySQLException
from UBPyUtil import UBPyLogger
from ROOT import ubpsql, std
class ConfigCreator:

    _logger = UBPyLogger.GetME(__name__)
    (DEBUG, INFO, WARNING, ERROR, CRITICAL) = (_logger.debug,
                                               _logger.info,
                                               _logger.warning,
                                               _logger.error,
                                               _logger.critical)
    def __init__(self):
        self._cfg_name   = std.string()
        self._cfg_params = std.vector("std::string")()
        self._conn       = ubpsql.ConfigWriter()
        self.CRITICAL('ahoaho')
        
    def FATAL(self, msg, args='',wargs=''):
        if   args and wargs: self.CRITICAL(msg,args,wargs)
        elif args:           self.CRITICAL(msg,args)
        else:                self.CRITICAL(msg)
        raise UBPySQLException(1)

    def upload(self,batch=False):
        
        if not batch:
            print '\033[95m' + 'ATTENTION!' + '\033[00m',
            print ' ... Attempting to upload the following sub-config definition'
            print
            
            self.ls()
            
            user_input=''
            while not user_input:
                print 'Enter [y/n]: ',
                sys.stdout.flush()
                user_input = sys.stdin.readline().replace('\n','')

                if user_input in ['N','n']:
                    return False

                elif user_input in ['Y','y']:
                    break
                    
                else:
                    print 'Invalid input: \"%s\"' % user_input
                    user_input=''
                    
        self._conn.Connect()
        # Check if configuration already exists or not
        if self._conn.ExistSubConfig(self._cfg_name):
        
            self.FATAL('Sub-Configuration \"%s\" already exist!', self._cfg_name)
        
        return self._conn.CreateConfigType(self._cfg_name,self._cfg_params)

    def ls(self):

        print
        print 'Sub-Config Name: \"%s\"' % self._cfg_name
        print
        for x in xrange(self._cfg_params.size()):
            print self._cfg_params.at(x)
        print

    def read_file(self,fname):
        
        if not os.path.isfile(fname):
            
            sys.stderr.write("File does not exist: %s" % fname)
            
            raise UBPySQLException(1)
    
        self._cfg_name=''
        self._cfg_params.clear()

        contents    = open(fname,'r').read().split('\n')
        read_switch = False
        read_finish = False
        for line in contents:

            while line.find(' ') == 0:
                line = line[1:len(line)]
        
            if line.startswith('#') or line.startswith('//'):

                continue

            if line.find('DEFINE') >=0:

                if read_finish:
                    sys.stderr.write('ERROR: Found 2nd \'DEFINE\'! \n')
                    raise UBPySQLException(1)

                if read_switch:
                    sys.stderr.write('ERROR: Found \'DEFINE\' before \'END\'! \n')
                    raise UBPySQLException(1)        
                
                if not len(line.split(None)) == 2:
                    sys.stderr.write('ERROR: Found \'DEFINE\' line has more than 2 words!\n')
                    raise UBPySQLException(1)
                
                self._cfg_name = line.split(None)[1]            
                read_switch = True
                continue

            if line.find('END') >=0 and read_switch:
                read_switch = False
                read_finish = True
        
            if read_switch:

                for word in line.split(None):
                
                    self._cfg_params.push_back(word)

        if not self._cfg_name or self._cfg_params.size()<1:

            sys.stderr.write('Did not find a configuration definition block!')
            raise UBPySQLException(1)

if __name__ == '__main__':

    k=ConfigCreator()
    k.read_file(sys.argv[1])
    k.ls()
    k.upload()

