#include <stdio.h>
#include <iostream>

using namespace std;

// a long long int can store 4 bytes (32 bits) of data
typedef int BLKSIZE;

// get file size
long getFileSize(FILE *file)
{
  long lCurPos, lEndPos;
  lCurPos = ftell(file);
  fseek(file, 0, 2);
  lEndPos = ftell(file);
  fseek(file, lCurPos, 0);
  return lEndPos;
}

// main routine
void newformat_xmitdecoder(const char *fileName){

  // input binary file name
  const char *filePath = "./lartf_run_data/";
  BLKSIZE *fileBuf;      // pointer to buffered data
  FILE *file = NULL;     // file pointer 

  int debug = 0;
  int forcecontinue = 1;
  int promptcontinue = 1; //xxxx
  int icont;

  //some decoding variables
  int fem_mod_address = 11; //hardware configuration-dependent
  
  int evtcount = 0; //how many ffffffff in file
  int evtendcount = 0;//how many e0000000 in file
  int femcount; //how many fems per event
  int hdcount = 12; //keeps track of fem header words 
  int adcount; //keeps track of adc data words in event
  int rdcount; //keeps track of readouts in event
  int dtcount; //keeps track of adc values in readout
  int previousevtno = -1; //checks that event numbers are continuous
  int checksumon = 0;
  long long int checksum;
  int rdhcount; //keeps track of words in readout
  int dtcount; //keeps track of ADC values count in a readout
  int lastreadend = 1;

  //variables which are part of FEM header
  int hmodadd; //module address
  int hmodid; //module ID
  int hnwords; //number of adc words
  int hevt; //fem event number
  int hframe; //fem frame number
  int hchecksum;
  int htrigframe; //trig frame number last 4 bits
  int htrigsample; //trig sample number
  int femid;

  //variables which are part of channel header
  int ch;
  int chdiscrid;
  int chframe;
  int chsample;

  //output variables
 
  char filename[500]; sprintf(filename,"%s.root",fileName);
  TFile *outf = new TFile(filename,"RECREATE");
  printf("\n\n==========> Output file name:\t%s\n\n",outf->GetName());

  TTree *data = new TTree("data","data");
  
  //fem data
  //assumes 1 fem max
  int entry,modadd[1],modid[1],evtno[1],evtfrmno[1],trigfrm4[1],trigsamp[1];
  //channel data
  //assumes nreadmax reads per per event, max
  const int nreadmax = 600;
  int readch[nreadmax],readlen[nreadmax],readid[nreadmax],readfrm3[nreadmax],readsamp[nreadmax]; //second index is read#
  int readadc[nreadmax][1500];
  int nreads;
  data->Branch("entry",&entry,"entry/I");
  data->Branch("modadd",modadd,"modadd[1]/I");
  data->Branch("modid",modid,"modid[1]/I");
  data->Branch("evtno",evtno,"evtno[1]/I");
  data->Branch("evtfrmno",evtfrmno,"evtfrmno[1]/I");
  data->Branch("trigfrm4",trigfrm4,"trigfrm4[1]/I");
  data->Branch("trigsamp",trigsamp,"trigsamp[1]/I");
  data->Branch("readch",&readch,"readch[600]/I");
  data->Branch("readlen",readlen,"readlen[600]/I");
  data->Branch("readid",readid,"readid[600]/I");
  data->Branch("readfrm3",readfrm3,"readfrm3[600]/I");
  data->Branch("readsamp",readsamp,"readsamp[600]/I");
  data->Branch("readadc",readadc,"readadc[600][1500]/I");
  data->Branch("nreads",&nreads,"nreads/I");

  entry=0;    
  int evtstatus;
  int q;
  
  // open binary file and check if open was successful
  sprintf(filename,"%s%s",filePath,fileName);
  if ( (file=fopen(filename,"rb")) == NULL )
    cout << "\nCould not open file." << endl;
  else
    cout << "\nFile opened successfully." << endl;

  // get file size in bytes
  long fileSize = getFileSize(file);
  cout << "File size is " << fileSize << " bytes." << endl;
  cout << "Block size is " << sizeof(BLKSIZE) << " bytes." << endl;

  // allocate space in the buffer for the whole file
  fileBuf = new BLKSIZE[fileSize/sizeof(BLKSIZE)];

  // read whole file into buffer
  fread(fileBuf, fileSize, 1, file);

  //=========================================================
  // decode, one BLKSIZE-length buffer at a time
  for (int i=0; i<fileSize/sizeof(BLKSIZE); i++){

    if (checksumon == 1){
      checksum = checksum + (fileBuf[i] & 0xffff) + ((fileBuf[i]>>16) & 0xffff);
    }

    //decode 32-bit word
    if ((evtstatus==0) && ((fileBuf[i] & 0xffffffff) != 0xffffffff)){
      printf("##################### Skipping event...\n");
      
    }

    else if ( (fileBuf[i] & 0xffffffff) == 0xffffffff ){
      printf("\n=====> New event\n");
      rdhcount=0;//resets channel header counter, in case of missing data at beginning of event
      femcount=0;
      evtcount++;
      evtstatus=1;
      if (lastreadend == 0){
	printf("############# ERROR: MISSING END OF READOUT, POSSIBLY DATA! #############\n");
	evtstatus=0;
	if (forcecontinue==0) return;
	if (promptcontinue==1) scanf("%d",&icont);
      }
    }

    else if ( (fileBuf[i] & 0xffffffff) == 0xe0000000 ){
      printf("=====> End of event\n");
      evtendcount++;
      if (femcount!=1){
	printf("############# ERROR: WRONG FEM COUNT! #############\n");
	evtstatus=0;
	if (forcecontinue==0) return;
	if (promptcontinue==1) scanf("%d",&icont);
      }
      if (evtstatus==1){
	data->Fill();
	entry++;
      }
      //      printf("Enter 1 to continue\n");
      //      scanf("%d",&q);
    }

    else { //split in two 16-bit words and decode

      //check lower 16 bits first

      if ( ((fileBuf[i] & 0xffff)&0xf000) == 0xf000 ){
	if (debug==1) printf("Debug: -----> FEM header word\n");
	if ( (hdcount==12)&& (((fileBuf[i] & 0xffff)&0xffff) == 0xffff) ){ //first header word
	  printf("-----> New FEM header\n");
	  hdcount=1;
	  femcount++;
	  adcount=0;
	  checksum=0x0;
	}
	else { //following header words
	  hdcount++;
	  if (hdcount==2){
	    hmodadd = ((fileBuf[i]>>16) & 0xfff) & 0x1f;
	    hmodid  = (((fileBuf[i]>>16) & 0xfff)>>5) & 0x7f;
	    printf("\t---\n");
	    printf("\tModule address: %d\n",hmodadd);
	    printf("\tModule ID     : %d\n",hmodid);
	    if (hmodadd==fem_mod_address) femid = 0;
	    modadd[femid] = hmodadd;
	    modid[femid] = hmodid;
	  }
	  else if (hdcount==3){
	    hnwords = ((fileBuf[i]>>16) & 0xfff)+((fileBuf[i]& 0xfff)<<12);
	    printf("\tNo. of words  : %d\n",hnwords);
	  }
	  else if (hdcount==4){
	    //part of hnwords, above
	  }
	  else if (hdcount==5){
	    hevt = ((fileBuf[i]>>16) & 0xfff) + ((fileBuf[i]& 0xfff) <<12);
	    printf("\tFEM Event No  : %d\n",hevt);
	    evtno[femid] = hevt;
	    if ((evtno[femid]-previousevtno)!=1){
	      printf("############# ERROR: Event number missing!! %d %d #############\n",evtno[femid],previousevtno);
	      if (forcecontinue==0) return;
	      if (promptcontinue==1) scanf("%d",&icont);//return;
	    }
	    previousevtno = evtno[femid];
	  }
	  else if (hdcount==6){
	    //part of hevt, above
	  }
	  else if (hdcount==7){
	    hframe = ((fileBuf[i]>>16) & 0xfff) + ((fileBuf[i]& 0xfff) <<12);
	    printf("\tFEM Frame No  : %d\n",hframe);
	    evtfrmno[femid] = hframe;
	  }
	  else if (hdcount==8){
	    //part of frame number, above
	  }
	  else if (hdcount==9){
	    hchecksum = ((fileBuf[i]>>16) & 0xfff) + ((fileBuf[i]& 0xfff) <<12);
	    printf("\tChecksum      : %x\n",hchecksum);
	  }
	  else if (hdcount==10){
	    //part of checksum, above
	  }
	  else if (hdcount==11){
	    //new header word:
	    htrigframe = ((fileBuf[i] & 0xfff) >> 4) & 0xf;
	    htrigsample = (((fileBuf[i]>>16) & 0xfff) & 0xff) + (((fileBuf[i] & 0xfff) & 0xf)<<8);
	    trigfrm4[femid] = htrigframe;
	    trigsamp[femid] = htrigsample;
	    printf("\tTrig Frm[4] No: %d(%d)\n",htrigframe,hframe&0xf);
	    printf("\tTrig Sample No: %d\n",htrigsample);
	  }
	  else if (hdcount==12){
	    //part of htrigsample, above
	    checksumon = 1;
	  }
	  else {
	    printf("############# ERROR: Unknown header word! #############\n");
	    evtstatus=0;
	    if (forcecontinue==0) return;
	    if (promptcontinue==1) scanf("%d",&icont);
	  }
	}
      }//end if header word

      else if ( ((fileBuf[i] & 0xffff)&0xc000) == 0x4000){
        if (hdcount!=12){
          printf("############# ERROR: Missing headers! #############\n");
	  evtstatus=0;
          if (forcecontinue==0) return;
	  if (promptcontinue==1) scanf("%d",&icont);
        }
	if (lastreadend == 0){
	  printf("############# ERROR: MISSING END OF READOUT, POSSIBLY DATA! #############\n");
	  evtstatus=0;
	  if (forcecontinue==0) return;
	  if (promptcontinue==1) scanf("%d",&icont);
	}
	printf("-----> First ADC word in FEM\n");
	adcount++;
	rdcount=0;
      }
      else if ( ((fileBuf[i] & 0xffff)&0xc000) == 0xc000){
	printf("-----> Last ADC word in FEM\n");
	adcount++;
	checksumon = 0;
	checksum = checksum - 0xc000;
	if ((checksum & 0xffffff) != (hchecksum & 0xffffff)){
	  printf("############# ERROR: WRONG CHECKSUM! #############\n");
	  evtstatus=0;
	  if (forcecontinue==0) return;
	  if (promptcontinue==1) scanf("%d",&icont);
	}
	if (adcount!=hnwords && (adcount-1)!=hnwords){
	  printf("############# ERROR: Wrong number of adc words! #############\n");
	  printf("                     Read %i, Expected from header %i\n",adcount,hnwords);
	  evtstatus=0;
	  if (forcecontinue==0) return;
	  if (promptcontinue==1) scanf("%d",&icont);
	}
      }
      else if ( ((fileBuf[i] & 0xffff)&0xc000) == 0x8000 ){
	if (debug==1) printf("\n----->Intermediate ADC word\n");
	adcount++;
	if ( ((fileBuf[i] & 0xffff)&0x3000) == 0x1000 ){
	  if (lastreadend == 0){
	    printf("############# ERROR: MISSING END OF READOUT, POSSIBLY DATA! #############\n");
	    evtstatus=0;
	    if (forcecontinue==0) return;
	    if (promptcontinue==1) scanf("%d",&icont);
	  }
	  rdhcount = 1; //first readout header
	  printf("--------> New readout\t");
	  rdcount++;
	  nreads = rdcount;
	  ch = ((fileBuf[i] & 0xfff) & 0x3f);
	  chdiscrid = (((fileBuf[i] & 0xfff) >> 9) & 0x7);
	  readch[rdcount-1] = ch;
	  readid[rdcount-1] = chdiscrid;
	  printf("ch %d\tID %d\t",ch,chdiscrid);
	  dtcount = 0; //reset adc value word count for readout
	}
	else if ( ((fileBuf[i] & 0xffff)&0x3000) == 0x3000 ){
	  rdhcount++;
	  dtcount++;
	  //last adc value word
	  lastreadend = 1;
	  readlen[rdcount-1] = dtcount;
	  readadc[rdcount-1][dtcount-1] = (fileBuf[i] & 0xfff);
	  if (debug==1) printf("%d\n",((fileBuf[i]) & 0xfff));
	  printf("nwords %d\n",dtcount);
	}
	else if ( ((fileBuf[i] & 0xffff)&0x3000) == 0x2000 ){
	  rdhcount++;
	  if (rdhcount==2){//readout header
	    chframe = ( (fileBuf[i] & 0xfff) >> 5 ) & 0x7;
	    chsample = ( (fileBuf[i] & 0xfff) & 0x1f) << 12;
	    readfrm3[rdcount-1] = chframe;
	    printf("fr3(pkt3) %d(%d)\t",chframe,hframe&0x7);
	  }
	  else if (rdhcount==3){//readout header
	    chsample = chsample + (fileBuf[i] & 0xfff);
	    readsamp[rdcount-1] = chsample;
	    printf("samp %d\t",chsample);
	    if (debug==1) printf("\n");
	  }
	  else {//intermediate adc value word
	    if (rdhcount<4){
	      printf("############# ERROR: MISSING BEGINNING OF CHANNEL DATA! #############\n");
	      evtstatus=0;
	      if (forcecontinue==0) return;
	      if (promptcontinue==1) scanf("%d",&icont);
	    }
	    if (evtstatus==1){
	      dtcount++;
	      readadc[rdcount-1][dtcount-1] = (fileBuf[i] & 0xfff);
	      if (debug==1){
		printf("%d ",(fileBuf[i]) & 0xfff);
		if (dtcount%8==0) printf("\n");
	      }
	    }
	  }
	}
	
      }


      //check upper 16 bits next

      if ( (((fileBuf[i] >> 16) & 0xffff)&0xf000) == 0xf000 ){
	if (debug==1) printf("Debug: -----> FEM header word\n");
	if ( (hdcount==12) && ((((fileBuf[i]>>16) & 0xffff)&0xffff) == 0xffff) ){ //first header word
	  printf("-----> New FEM header\n");
	  hdcount=1;
	  femcount++;
	  adcount=0;
	  checksum=0x0;
	}
	else { //following header words
	  hdcount++;
	  if (hdcount==2){
	    hmodadd = ((fileBuf[i]>>16) & 0xfff) & 0x1f;
	    hmodid  = (((fileBuf[i]>>16) & 0xfff)>>5) & 0x7f;
	    printf("\t---\n");
	    printf("\tModule address: %d\n",hmodadd);
            printf("\tModule ID     : %d\n",hmodid);
	    if (hmodadd==fem_mod_address) femid = 0;
	    modadd[femid] = hmodadd;
	    modid[femid] = hmodid;
	  }
	  else if (hdcount==3){
	    hnwords = ((fileBuf[i]>>16) & 0xfff)+((fileBuf[i]& 0xfff)<<12);            
	    printf("\tNo. of words  : %d\n",hnwords);
	  }
	  else if (hdcount==4){
	    //part of hnwords, above
	  }
	  else if (hdcount==5){
	    hevt = ((fileBuf[i]>>16) & 0xfff) + ((fileBuf[i]& 0xfff) <<12);
	    printf("\tFEM Event No  : %d\n",hevt);
	    evtno[femid] = hevt;
	    if ((evtno[femid]-previousevtno)!=1){
	      printf("############# ERROR: Event number missing!! %d %d #############\n",evtno[femid],previousevtno);
	      if (forcecontinue==0) return;
	      if (promptcontinue==1) scanf("%d",&icont);//return;
	    }
	    previousevtno = evtno[femid];
	  }
	  else if (hdcount==6){
	    //part of hevt, above
	  }
	  else if (hdcount==7){
	    hframe = ((fileBuf[i]>>16) & 0xfff) + ((fileBuf[i]& 0xfff) <<12);
	    printf("\tFEM Frame No  : %d\n",hframe);
	    evtfrmno[femid] = hframe;
	  }
	  else if (hdcount==8){
	    //part of frame number, above
	  }
	  else if (hdcount==9){
	    hchecksum = ((fileBuf[i]>>16) & 0xfff) + ((fileBuf[i]& 0xfff) <<12);
	    printf("\tChecksum      : %x\n",hchecksum);
	  }
	  else if (hdcount==10){
	    //part of checksum, above
	  }
	  else if (hdcount==11){
            //new header word:                                                  
            htrigframe = ((fileBuf[i] & 0xfff) >> 4) & 0xf;
            htrigsample = (((fileBuf[i]>>16) & 0xfff) & 0xff) + (((fileBuf[i] & 0xfff) & 0xf)<<8);
	    trigfrm4[femid] = htrigframe;
            trigsamp[femid] = htrigsample;
	    printf("\tTrig Frm[4] No: %d(%d)\n",htrigframe,hframe&0xf);
            printf("\tTrig Sample No: %d\n",htrigsample);
          }
          else if (hdcount==12){
            //part of htrigsample, above 
	    checksumon = 1;
          }
	  else {
	    printf("############# ERROR: Unknown header word! #############\n");
	    evtstatus=0;
	    if (forcecontinue==0) return;
	    if (promptcontinue==1) scanf("%d",&icont);
	  }
	}
      }//end if header word

      else if ( (((fileBuf[i] >> 16) & 0xffff)&0xc000) == 0x4000){
	if (hdcount!=12){
	  printf("############# ERROR: Missing headers! #############\n");
	  evtstatus=0;
	  if (forcecontinue==0) return;
	  if (promptcontinue==1) scanf("%d",&icont);
	}
	if (lastreadend == 0){
          printf("############# ERROR: MISSING END OF READOUT, POSSIBLY DATA! #############\n");
          evtstatus=0;
          if (forcecontinue==0) return;
	  if (promptcontinue==1) scanf("%d",&icont);
        }
	printf("-----> First ADC word in FEM\n");
	adcount++;
	rdcount=0;
      }
      else if ( (((fileBuf[i] >> 16) & 0xffff)&0xc000) == 0xc000){
	printf("-----> Last ADC word in FEM\n");
	adcount++;
	checksumon = 0;
	checksum = checksum - 0xc000;
	if ((checksum & 0xffffff) != (hchecksum & 0xffffff)){
	  printf("############# ERROR: WRONG CHECKSUM! #############\n");
	  evtstatus=0;
	  if (forcecontinue==0) return;
	  if (promptcontinue==1) scanf("%d",&icont);
	}
	if (adcount!=hnwords && (adcount-1)!=hnwords){
	  printf("############# ERROR: Wrong number of adc words! #############\n");
	  printf("                     Read %i, Expected from header %i\n",adcount,hnwords);
	  evtstatus=0;
	  if (forcecontinue==0) return;
	  if (promptcontinue==1) scanf("%d",&icont);
	}
      }
      else if ( (((fileBuf[i] >> 16) & 0xffff)&0xc000) == 0x8000 ){
        if (debug==1) printf("\n----->Intermediate ADC word\n");
	adcount++;
	if ( (((fileBuf[i]>>16) & 0xffff)&0x3000) == 0x1000 ){
          if (lastreadend == 0){
            printf("############# ERROR: MISSING END OF READOUT, POSSIBLY DATA! #############\n");
            evtstatus=0;
            if (forcecontinue==0) return;
	    if (promptcontinue==1) scanf("%d",&icont);
          }
          rdhcount = 1; //first readout header
	  printf("--------> New readout\t");
	  rdcount++;
	  nreads = rdcount;
          ch = (((fileBuf[i]>>16) & 0xfff) & 0x3f);
          chdiscrid = ((((fileBuf[i]>>16) & 0xfff) >> 9) & 0x7);
	  readch[rdcount-1] = ch;
	  readid[rdcount-1] = chdiscrid;
	  printf("ch %d\tID %d\t",ch,chdiscrid);
          dtcount = 0; //reset adc value word count for readout
        }
        else if ( (((fileBuf[i]>>16) & 0xffff)&0x3000) == 0x3000 ){
          rdhcount++;
          dtcount++;
          //last adc value word
          lastreadend = 1;
	  readlen[rdcount-1] = dtcount;
	  readadc[rdcount-1][dtcount-1] = ((fileBuf[i]>>16) & 0xfff);
	  if (debug==1) printf("%d\n",((fileBuf[i]>>16) & 0xfff));
	  printf("nwords %d\n",dtcount);
        }
        else if ( (((fileBuf[i]>>16) & 0xffff)&0x3000) == 0x2000 ){
          rdhcount++;
          if (rdhcount==2){//readout header
            chframe = ( ((fileBuf[i]>>16) & 0xfff) >> 5 ) & 0x7;
            chsample = ( ((fileBuf[i]>>16) & 0xfff) & 0x1f) << 12;
	    readfrm3[rdcount-1] = chframe;
	    printf("fr3(pkt3) %d(%d)\t",chframe,hframe&0x7);
          }
          else if (rdhcount==3){//readout header
            chsample = chsample + ((fileBuf[i]>>16) & 0xfff);
	    readsamp[rdcount-1] = chsample;
	    printf("samp %d\t",chsample);
	    if (debug==1) printf("\n");
          }
          else {//intermediate adc value word
	    if (rdhcount<4){
	      printf("############# ERROR: MISSING BEGINNING OF CHANNEL DATA! #############\n");
	      evtstatus=0;
	      if (forcecontinue==0) return;
	      if (promptcontinue==1) scanf("%d",&icont);
	    }
	    if (evtstatus==1){
	      dtcount++;
	      readadc[rdcount-1][dtcount-1] = (fileBuf[i]>>16) & 0xfff;
	      if (debug==1){
		printf("%d ",(fileBuf[i]>>16) & 0xfff);
		if (dtcount%8==0) printf("\n");
	      }
	    }
          }
        }

      }

    }//end else

  }//end for loop

  printf("\n\n");

  printf("Found %i NEW events in input file.\n",evtcount);
  printf("Found %i CLOSED/ERROR-FREE events in input file.\n",evtendcount);

  if ((fileBuf[fileSize/sizeof(BLKSIZE)-1]&0xffffffff) != 0xe0000000){
    printf("\nWARNING: buffer does not end on 'last event word'. Last event may be incomplete.\n");
  }
  
  printf("\n\n");
  
  delete fileBuf;
  fclose(file);
  cout << "File closed. Buffer deleted.\n" << endl;
  
  data->Write();
  outf->Close();

  gROOT->ProcessLine(".q");

  return;

}
