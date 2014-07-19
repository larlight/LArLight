import os, sys
from UBPySQLException import UBPySQLException
from UBPyUtil import UBPyLogger
from ROOT import ubpsql, std

class ConfigUploader:

    _logger = UBPyLogger.GetME(__name__)
    (DEBUG, INFO, WARNING, ERROR, CRITICAL) = (_logger.debug,
                                               _logger.info,
                                               _logger.warning,
                                               _logger.error,
                                               _logger.critical)

    def __init__(self):
        self.data  = None
        self._conn = ubpsql.ConfigWriter()

    def FATAL(self, msg, args='',wargs=''):
        if   args and wargs: self.CRITICAL(msg,args,wargs)
        elif args:           self.CRITICAL(msg,args)
        else:                self.CRITICAL(msg)
        raise UBPySQLException(1)

    def upload(self,batch=False):
        
        if not batch:
            print '\033[95m' + 'ATTENTION!' + '\033[00m',
            print '... Attempting to upload Sub-Config!'
            print
            
            self.data.ls()
            
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
        if not self._conn.ExistSubConfig(self.data.Name()):
        
            self.FATAL('Sub-Configuration \"%s\" does not exist!' % self.data.Name())
            
        values = std.map("std::string","std::string")()

        return self._conn.InsertSubConfigValues(self.data)

    def read_file(self,fname):

        if not os.path.isfile(fname):
        
            self.FATAL("File does not exist: %s" % fname)

        # Check overall format first
        contents = open(fname,'r').read()
        block_contents = { 'UNIQUE_ID'  : '',
                           'PARAMETERS' : '' }

        # Identify block contents
        for keyword in block_contents.keys():

            if not contents.find(keyword) >=0:
            
                self.FATAL("Input file (%s) missing \'%s\' section!" % (fname,keyword))

            if not contents.find(keyword) == contents.rfind(keyword):

                self.FATAL("Input file (%s) has duplicate \'%s\' section!" % (fname,keyword))

            def_start   = contents.find(keyword)
            block_start = -1
            block_end   = -1
            for index in xrange(len(contents) - def_start):

                if contents[def_start + index] == '{':
                    
                    block_start = def_start + index + 1

                if contents[def_start + index] == '}':

                    block_end   = def_start + index
                    if block_start > 0: break
                    else: FATAL('Block (%s) does not have \'{\'' % keyword)

            if block_start < 0 or block_end < 0:

                self.FATAL("Block (%s) does not have {} clause following!" % keyword)


            tmp_contents = contents[block_start:block_end]
            sub_contents = {}
            for line in tmp_contents.split('\n'):
                if len(line.replace(' ','')) < 1 : continue
                if not len(line.split(':')) == 2:
                    self.FATAL("Block (%s) contains a line with an invalid format: %s" % (keyword,line))
                (key,value) = line.split(':')
                
                while key.find(' ')   == 0 : key   = key   [1 : len(key)   ]
                while value.find(' ') == 0 : value = value [1 : len(value) ]

                while key.rfind(' ')   == len(key)-1   : key   = key   [0 : key.rfind(' ')   ]
                while value.rfind(' ') == len(value)-1 : value = value [0 : value.rfind(' ') ]
                sub_contents[key]=value

            block_contents[keyword] = sub_contents

        # Process block contents
        self.data = ubpsql.ConfigData( block_contents['UNIQUE_ID']['name'    ],
                                       int(block_contents['UNIQUE_ID']['id'      ]),
                                       int(block_contents['UNIQUE_ID']['crate'   ]),
                                       int(block_contents['UNIQUE_ID']['channel' ]) )

        for key in block_contents['PARAMETERS'].keys():
            
            self.data.append(key,block_contents['PARAMETERS'][key])

if __name__ == '__main__':

    k = ConfigUploader()
    k.read_file(sys.argv[1])
    k.upload()
