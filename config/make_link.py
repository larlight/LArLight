import os,sys,commands

make_link   = False;
source_link = sys.argv[1]
target_link = sys.argv[2]

# Check if a symlink alrady exists or not
if os.path.islink(target_link):
    current_link=commands.getoutput("ls -l %s" % target_link)
    current_link=current_link.split(None)[len(current_link.split(None))-1]

    if not current_link == source_link:
        print "Found already existing link %s\n    => %s" % (target_link,current_link)
        print
        print "Can we replace it to: %s\n    => %s" % (target_link,current_link)
        user_input=''
        while not user_input in ['y','n']:
            if user_input:
                print 'Invalid input: %s' % user_input
            sys.stdout.write("[y/n]:")
            sys.stdout.flush()
            user_input = sys.stdin.read().rstrip('\n')

        if user_input == 'y':
            os.system('rm %s' % target_link)
            if os.path.islink(target_link):
                print 'ERROR:'
                print 'Failed to replace a link %s' % target_link
                print 'BE AWARE!'
            else:
                make_link=True
        else:
            print 'INFO:'
            print 'Intentionally keeping a softlink NOT POINTING TO THIS INSTALLATION!'
            print 'BE AWARE!'

elif target_link.find("/")>=0:
    # Check if the target parent directory exists or not
    target_dir = target_link[0:target_link.rfind("/")]
    os.system("mkdir -p %s" % target_dir)
    if not os.path.isdir(target_dir):
        print "Failed to create a directory: %s" % target_dir
    else:        
        make_link = True
# If make_link is True, create a symlink
if make_link:

    os.system("ln -sf %s %s" % (source_link,target_link))
        


    
