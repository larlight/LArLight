#
# Example decoder script.
#

# Basic python module import
import os, sys, __main__

# Expect the 1st argument to be an input file.
if len(sys.argv)<2:
    sys.stderr.write('\n   Usage: %s $INPUT_FILE\n\n' % __main__.__file__)
    sys.exit(1)
elif not os.path.isfile(sys.argv[1]):
    sys.stderr.write('\n   Could not recognize an input file: \"%s\"\n\n' % sys.argv[1])
    sys.exit(1)

# Temporary place input argument array in elsewhere
# otherwise ROOT recognize them as input argument.
argv=sys.argv
sys.argv=[]

# Import ROOT
import ROOT

# Load decoder library
ROOT.gSystem.Load("libDecoder")

# Load decoder class/namespace
from ROOT import *
from ROOT import larlight as fmwk

#
# Four lines to just get the output filename from input file name.
# Given the input file XXX.yyy, output file would be XXX.root.
# The output file is created in $PWD defined in out_dir variable below.
#
out_dir = "./"
fpath   = argv[1]
#fname   = fpath.split('/')[len(fpath.split('/'))-1]
#outname = out_dir + fname[0:fname.rfind('.')] + ".root"
outname = fpath.replace('.dat','.root')

# Decoder algorithm instance ... currently xmit, slow, or trigger
#algo=fmwk.algo_pmt_xmit()
#algo=fmwk.algo_slow_readout_decoder()
algo=fmwk.algo_tpc_xmit()
#algo=fmwk.algo_trig_decoder()
#algo=fmwk.algo_tpc_huffman()

# Set algorithm's back-trace mode with positive int argument (=# words to be backtraced)
algo.set_backtrace_mode(200)

# Create the decoder instance
decoder=fmwk.decoder_manager()

# Attach algorithm to the decoder.
decoder.set_decoder(algo);

# Set input file format ... ASCII or BINARY
#decoder.set_format(fmwk.FORMAT.ASCII)
decoder.set_format(fmwk.FORMAT.BINARY)

# Set whether or not to ready by block 
decoder.set_read_by_block(True)

# Set read-block size 
#decoder.set_read_block_size(200)

# Set input file path
decoder.add_input_filename(fpath)

# Set output file path
decoder.set_output_filename(outname)

# Set message level
#    - DEBUG   ... for debugging purpose. Very verbose.
#    - INFO    ... for validation purpose. Spits out summary info of channel/event
#    - NORMAL  ... default mode. Only spits out run-relevant information
#    - WARNING ... suppress NORMAL information
#    - ERROR   ... suppress WARNING information
# For the given info level, all lower level information will be suppressed.
#decoder.set_verbosity(fmwk.MSG.DEBUG)
#decoder.set_verbosity(fmwk.MSG.INFO)
#decoder.set_verbosity(fmwk.MSG.NORMAL)

# Set debug mode True if you wish to continue in the decoding event
# loop with exception handling. This avoids saving an event with
# missing event words or conflicting encoding algorithm, and continue
# in the decoding event loop. When turned off, the program exits as
# soon as it encounters any issue.
decoder.debug_mode(False)

# Finish configuration. Now run it.
status=decoder.run()
# Reset properly before exits.
decoder.reset()

print status


    

