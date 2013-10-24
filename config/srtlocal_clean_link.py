import os,sys,commands

# check if SRT_LOCAL and MAKE_TOP_DIR are defined. If not, nothing to do.
if not 'SRT_LOCAL' in os.environ or not 'MAKE_TOP_DIR' in os.environ:
    sys.exit(1)

# clean up symlink under $SRT_LOCAL if they point to anything under $MAKE_TOP_DIR
dirs = []
dirs.append(os.environ['SRT_LOCAL'])
dirs.append('%s/lib/%s' % (os.environ['SRT_LOCAL'],os.environ['SRT_SUBDIR']))
symlinks=[]
for dir in dirs:
    symlinks += ['%s/%s' % (dir,x) for x in os.listdir(dir) if os.path.islink('%s/%s' % (dir,x))]

for link in symlinks:

    link_source=commands.getoutput("ls -l %s" % link)
    link_source=link_source.split(None)[len(link_source.split(None))-1]

    if link_source.find(os.environ['MAKE_TOP_DIR'])>=0:
        os.system('rm -f %s' % link)
        
    
