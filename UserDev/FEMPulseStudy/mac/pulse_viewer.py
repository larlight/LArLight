import ROOT,sys

ROOT.gSystem.Load("libAnalysis")

from ROOT import *

def main(pv_ptr) :

    proc=ana_processor()

    #    proc.set_verbosity(MSG.DEBUG)

    proc.set_ana_output_file("ana.root")
    
    proc.set_io_mode(storage_manager.READ)
    
    proc.set_data_to_read(DATA_STRUCT.PULSE_COLLECTION)
    proc.set_data_to_read(DATA_STRUCT.FIXED_WIN_PULSE_COLLECTION)
    proc.set_data_to_read(DATA_STRUCT.THRES_WIN_PULSE_COLLECTION)

    proc.add_input_file(sys.argv[1])
    
    proc.add_process(pv_ptr)

    while 1:
        
        proc.process_event()
        if not proc.get_process_status() == proc.PROCESSING:
            break
            
        # get list of channels that got a pulse
        ch = pv_ptr.next_channel()
            
        while not ch==PMT.INVALID_CH:
                
            for x in xrange(pv_ptr.get_npulse(ch)):
                print "pulse %d/%d" % (x+1,pv_ptr.get_npulse(ch))
                h=pv_ptr.next_pulse(ch)

                if not h:
                    break;

                options=['n','s','q','c']
                user_input=''
            
                while not user_input in options:
                    
                    if user_input:
                        print
                        print "\033[91m" + 'Invalid option: \"%s\"' % user_input + "\033[0m"
                    
                    print
                    sys.stdout.write("\033[95m" + "Options:" + "\033[0m")
                    sys.stdout.write(" ... \"s\" to save image, \"n\" for next pulse, \"c\" to alter cut values, \"q\" to exit.\n")
                    sys.stdout.flush()
                    print 
                    sys.stdout.write("\033[93m" + 'Input [s/n/c/q]: ' + "\033[0m")
                    sys.stdout.flush()
                    user_input=sys.stdin.readline().rstrip('\n')

                    if user_input=='q':
                        terminate=True
                        sys.exit(1)
                    if user_input=='n':
                        break;
                    if user_input=='s':
                        out_name='pulse_image.gif'
                        pv_ptr.get_canvas().SaveAs("%s+100" % out_name)
                        print 'Image saved in %s ... ' % out_name
                        user_input=''
                    if user_input=='c':
                        change_cut_values(pv_ptr)
                        user_input=''

            ch=pv_ptr.next_channel()

def change_cut_values(pv_ptr):

    valid_inputs=[ str(x+1) for x in xrange(10)]
    valid_inputs.append("q")
    valid_inputs.append("d")

    user_input=""
    continue_loop=True

    while continue_loop:
    
        pv_ptr.display_cut_ranges()

        msg  = "\n"
        msg += "\033[95m" + "To change the cut range values" + "\033[0m"
        msg += " ... type the option number, the range minimum, the range maximum separated by a space.\n"
        msg += "\033[95m" + "To exit this parameter setting routine" + "\033[0m"
        msg += " ... type \"q\"\n"
        msg += "\033[95m" + "To review set parameters" + "\033[0m"
        msg += " ... type \"d\"\n"
        msg += "\n"
        msg += "Options:\n"
        msg += "    1  ... Event ID\n"
        msg += "    2  ... Summed Charge\n"
        msg += "    3  ... Summed Peak\n"
        msg += "    4  ... Num. Pulses\n"
        msg += "    5  ... Channel Number\n"
        msg += "    6  ... Pulse Charge\n"
        msg += "    7  ... Pulse Peak\n"
        msg += "    8  ... Pulse Start T\n"
        msg += "    9  ... Pulse Start T (RECO-ed)\n"
        msg += "    10 ... Pulse End T\n"
        msg += "    11 ... Pedestal Mean\n"
        msg += "    12 ... Pedestal RMS\n"
        msg += "\n"
        
        print msg

        while not user_input in valid_inputs:
            
            if user_input and user_input.split(None)[0]:
                print "\033[91m" + 'Invalid option: \"%s\"' % user_input.split(None)[0] + "\033[0m"
                user_input=""

            sys.stdout.write("\033[93m" + 'Input: ' + "\033[0m")
            sys.stdout.flush()

            user_input=sys.stdin.readline().rstrip("\n")

            if not user_input: continue

            if not user_input.split(None)[0] in valid_inputs: continue

            if user_input=="q":
                continue_loop=False
                break

            elif user_input=="d":
                pv_ptr.display_cut_ranges()
                user_input=""
                continue

            elif not len(user_input.split(None))==3:
                msg  = "\033[91m"
                msg += 'Invalid format: \"%s\"' % user_input
                msg += "\033[0m"                
                msg += "\n"
                sys.stderr.write(msg)
                user_input=""
                continue
            
            option=user_input.split(None)[0]
            min_val=user_input.split(None)[1]
            max_val=user_input.split(None)[2]

            msg="\nSuccess!\n"
            try:
                if   option=="1" : pv_ptr.set_range_event_id(int(min_val),int(max_val))
                elif option=="2" : pv_ptr.set_range_sum_charge(float(min_val),float(max_val))
                elif option=="3" : pv_ptr.set_range_sum_peak(float(min_val),float(max_val))
                elif option=="4" : pv_ptr.set_range_npulse(int(min_val),int(max_val))
                elif option=='5' : pv_ptr.set_range_channel(int(min_val),int(max_val))
                elif option=="6" : pv_ptr.set_range_pulse_charge(float(min_val),float(max_val))
                elif option=="7" : pv_ptr.set_range_pulse_amp(float(min_val),float(max_val))
                elif option=="8" : pv_ptr.set_range_pulse_start_time(float(min_val),float(max_val))
                elif option=="9" : pv_ptr.set_range_pulse_start_time_reco(float(min_val),float(max_val))
                elif option=="10": pv_ptr.set_range_pulse_end_time(float(min_val),float(max_val))
                elif option=="11": pv_ptr.set_range_ped_mean(float(min_val),float(max_val))
                elif option=="12": pv_ptr.set_range_ped_rms(float(min_val),float(max_val))
                else :
                    msg  = "\033[91m"
                    msg += 'Option case \"%s\" not handled ... this is an issue of %s script!!!' % (option,__main__.__file__)
                    msg += "\033[0m"                
                    msg += "\n"
            except ValueError:
                    msg  = "\033[91m"
                    msg += 'Invalid value type'
                    msg += "\033[0m"           
                    msg += ' ... provided values (min=%s, max=%s) do not match the default value types!' % (min_val,max_val)
                    msg += "\n"
            print msg
            user_input=""


if __name__=="__main__":

    msg  = "\n"
    msg += "This program runs pulse_viewer instance in an interactive event loop.\n"
    msg += "pulse_viewer class instance allows you to filter out pulses of your interest\n"
    msg += "by setting specific range of values for parameters.\n"
    msg += "\n"
    msg += "Type anything to start the program now.\n\n"

    print msg

    user_input=sys.stdin.readline()

    pv_ptr=pulse_viewer()
    main(pv_ptr)
