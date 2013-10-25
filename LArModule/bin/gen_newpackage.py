import sys, os

if len(sys.argv)<2:
   sys.stderr.write('Usage: %s PACKAGE_NAME\n\n' % sys.argv[0])
   sys.exit(1)
if not 'LAR_MODULE_DIR' in os.environ.keys():
   sys.stderr.write('$LAR_MODULE_DIR not defined!\n\n')
   sys.exit(1)

name=sys.argv[1]
target_dir='%s/%s' % (os.environ['LAR_MODULE_DIR'],name)
source_dir='%s/bin/tmp' % os.environ['LAR_MODULE_DIR']

if not os.path.exists(target_dir):
   try:
      os.makedirs(target_dir)
      if os.path.isdir(os.environ['SRT_LOCAL']):
         os.system('ln -sf %s %s/' % (target_dir,os.environ['SRT_LOCAL']))
   except OSError:
      sys.stderr.write('Failed to create a dir: %s\n' % target_dir)
      sys.stderr.write('Maybe lacking a permission?\n\n')
      sys.exit(1)
else:
   sys.stderr.write('Package \"%s\" already exists under %s\n' % (name,target_dir))
   sys.stderr.write('Please remove before running this script.\n\n')
   sys.exit(1)

in_makefile='%s/GNUmakefile.tmp' % source_dir
in_source='%s/cc.tmp' % source_dir
in_fcl='%s/fcl.tmp' % source_dir
in_mod_fcl='%s/config_fcl.tmp' % source_dir

src_list = { in_makefile : '%s/GNUmakefile'   % target_dir,
             in_source   : '%s/%s_module.cc'  % (target_dir,name),
             in_fcl      : '%s/%s.fcl'        % (target_dir,name.lower()),
             in_mod_fcl  : '%s/%smodules.fcl' % (target_dir,name.lower())}
for src in src_list.keys():
   contents=open(src,'r').read()
   contents=contents.replace('MYMODNAME',name.upper())
   contents=contents.replace('mymodname',name.lower())
   contents=contents.replace('MyModName',name)
   contents=contents.replace('SHELL_USER_NAME',os.environ['USER'])
   fout=open(src_list[src],'w')
   fout.write(contents)
   fout.close()

print
print 'Generated the followings under %s.' % target_dir
for key in src_list.keys():
   print '    %s' % src_list[key]
print
print 'To compile your program, try:'
print '    > make --directory=%s' % target_dir
print ' OR'
print '    > cd %s; make;' % target_dir
print
print 'If you would like this package to be compiled always,'
print 'append it in LAR_MODULE shell environment variable.'
print
sys.exit(0)
