import sys
from ROOT import gSystem
gSystem.Load("libFileIO")
from ROOT import *

k=bin_word_search()

k.set_format(FORMAT.BINARY)
k.set_filename(sys.argv[1])
exec("k.set_target_word(0x%s)" % sys.argv[2])
if len(sys.argv)>3:
    k.set_nwords(int(sys.argv[3]))
if len(sys.argv)>4:
    exec("k.set_word_bytes(k.%s)" % sys.argv[4])
k.set_continue_mode(True)

k.run()


