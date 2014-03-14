//
// Code for the Arachne Event Display
// Author: Nathaniel Tagg ntagg@otterbein.edu
// 
// Licence: this code is free for non-commertial use. Note other licences may apply to 3rd-party code.
// Any use of this code must include attribution to Nathaniel Tagg at Otterbein University, but otherwise 
// you're free to modify and use it as you like.
//

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "SocketServer.h"
#include <TString.h>
#include <iostream>
using namespace std;

#ifdef __APPLE__
#define MSG_NOSIGNAL 0
#endif

SocketServer::SocketServer( int port )
{
  mPort = port;
  mListener = 0;
  mRecieveBuffer = 0;
  mRecieveBufferSize = 0;
  FD_ZERO(&mClients);
  mFdMax = mFdMin = 0;
}


SocketServer::~SocketServer( void )
{
  ShutDown();
  if(mRecieveBuffer) delete [] mRecieveBuffer;
}

int SocketServer::ShutDown( void )
{
  for(int i=mFdMin;i<mFdMax;i++) {
    if(FD_ISSET(i,&mClients)) {
      close(i);
      FD_CLR(i,&mClients);
    }
  }
  return 0;
}

int SocketServer::Setup( void )
{
  if((mListener = socket(AF_INET,SOCK_STREAM,0)) == -1 ){
    cout << Form("SocketServer::Setup Cannot create socket: %s\n",strerror(errno));
    return 1;
  }

  // lose the pesky "address already in use" error message
  int yes= 1;
  if (setsockopt(mListener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    cout << Form("SocketServer::Setup Cannot modify socket options: %s\n",strerror(errno));
    return 1;
  }

  // bind
  mMyAddress.sin_family = AF_INET;
  mMyAddress.sin_addr.s_addr = INADDR_ANY;
  mMyAddress.sin_port = htons(mPort);
  memset(&(mMyAddress.sin_zero), '\0', 8);
  if( -1 == ::bind(mListener, (struct sockaddr *)&mMyAddress, sizeof(mMyAddress)) ) {
    cout << Form("SocketServer::Setup Cannot bind socket: %s\n",strerror(errno));
    return 1;
    
    // cout << "  SocketServer::Setup What the heck, I'm going to force a bind() anyway..." << endl;
    // int yes= 1;
    // if (setsockopt(mListener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    //   cout << Form("  SocketServer::Setup Cannot modify socket options: %s\n",strerror(errno));
    //   return 1;
    // }
    // if (bind(mListener, (struct sockaddr *)&mMyAddress, sizeof(mMyAddress)) == -1) {
    //   cout << Form("  SocketServer::Setup Cannot bind socket: %s\n",strerror(errno));
    //   return 1;
    // }
    // cout << "  SocketServer::Setup Looks good. Continuing." << endl;
  }

  // listen
  if (listen(mListener, 20) == -1) {
    cout << Form("SocketServer::Setup Cannot listen socket: %s\n",strerror(errno));
    return 1;
  }

  // add the listener to the master set
  FD_SET(mListener, &mClients);

  // keep track of the biggest file descriptor
  mFdMax = mListener;
  mFdMin = mListener;

  //logDebug(1,"SocketServer::Setup created a listener socket on fd %d\n",mListener);

  return 0;
}


int SocketServer::Listen( double inWaitSecs, 
                          size_t inMsgSize, 
                          unsigned char* &outDataRecvd, 
                          int&  outWhichClient,
                          bool& outGotNewClient
                          )
{
  // See if there is a command waiting in the input socket.
  // Return immediately upon getting one.

  outDataRecvd = 0;
  outWhichClient = 0;
  outGotNewClient = false;

  if(mRecieveBufferSize < inMsgSize) {
    if(mRecieveBuffer) delete [] mRecieveBuffer;
    mRecieveBuffer = new unsigned char [inMsgSize+16];
    mRecieveBufferSize = inMsgSize + 16;
  }

  struct timeval timeout;
  timeout.tv_sec = (int)inWaitSecs;
  timeout.tv_usec = int((inWaitSecs-(int)(inWaitSecs))*1000000);    
  
  fd_set rfds = mClients;
  int selection = select(mFdMax+1, &rfds, NULL, NULL, &timeout);
  
  if(selection<0) {
    cout << Form("SocketServer::Listen(): select returned error: %s\n",strerror(errno));
    return -1;
  }

  if(selection==0) return -1;
  
  // run through the existing connections looking for data to read
  for(int i = mFdMin; i <= mFdMax; i++) {
    if (FD_ISSET(i, &rfds)) { // we got one!!
      
      if (i == mListener) {
        // handle new connections
        unsigned int addrlen = sizeof(mClientAddress);
        int newfd = accept(mListener, (struct sockaddr *)&mClientAddress, &addrlen);
        if (newfd == -1) {
           cout << Form("SocketServer::Listen() Error accepting new connection: %s\n",strerror(errno));
        } else {
          FD_SET(newfd, &mClients); // add to master set
          if (newfd > mFdMax) mFdMax = newfd;
          if (newfd < mFdMin) mFdMin = newfd;
          mClientTimers[newfd].Reset();
          cerr << Form("SocketServer::Listen() Got new client %s on socket %d\n", inet_ntoa(mClientAddress.sin_addr), newfd);

          outGotNewClient = true;
        }		
      } else {
        //cerr << Form("SocketServer::Listen() Got select hit on %d\n", i);
        // handle data from a client. Peek ahead to see if we have a full message.
        int nbytes  = recv(i, mRecieveBuffer, mRecieveBufferSize, MSG_PEEK);
        if (nbytes == 0) {
          // connection closed
          //logDebug(1,"SocketServer::Listen() Socket %d hung up\n", i);
          close(i); // bye!
          FD_CLR(i, &mClients); // remove from master set
        } else if(nbytes<0) {
          cout << Form("SocketServer::Listen() Socket %d got recv error:%s\n",i,strerror(errno));
          close(i); // bye!
          FD_CLR(i, &mClients); // remove from master set
          
        } else { 
          
          // we got some data from a client
          // wait until we're topped off.
          size_t bytesToRead = SufficientData(mRecieveBuffer, (size_t)nbytes, inMsgSize);
          if(bytesToRead>0) {	    
            // Read bytes from the input buffer to get rid of them.
            nbytes = recv(i, mRecieveBuffer, bytesToRead, 0 );
            // Dispatch the message.
            outDataRecvd = mRecieveBuffer;
            outWhichClient = i;

            return 1;
          } else {
            // There is insufficent data to make a message
            // so just exit Listen and do other things for a bit
            // We'll actually catch the message on the next call.
            //cerr << Form("SocketServer::Listen() Got some data on %d, (have %d bytes, need %d)\n",i,nbytes,inMsgSize);
            mRecieveBuffer[nbytes]=0;
            //cerr << "Current buffer: --" << mRecieveBuffer << "--" << endl;
          }
        }
      }
      
    } // it's SO UGLY!
  }

  return 0;
};


int SocketServer::Send(const unsigned char* data, 
                       size_t size)
{
  // Now, send it off.
  // Note the MSG_NOSIGNAL flag, which keeps send() from throwing a SIGPIPE when a client has
  // closed down.
  // These errors come because of a race condition: the client has closed down before I can reply,
  // but I have not yet polled select() again to see if the client has done so.
  // This is the simple solution to a problem faced only when debugging.
  // Also note the use of non-blocking send, which prevents lockups if the
  // client shuts down unexpectedly.
  
  fd_set writeToSet = mClients;
  FD_CLR(mListener,&writeToSet);

  int err = 0;
  for(int fd = mFdMin; fd <= mFdMax; fd++) {
    if (FD_ISSET(fd, &writeToSet)) {
      int result = SendTo(fd,data,size);
      if(result!=0) err++;
    }
  }

  return err;
}

int SocketServer::SendTo(int fd,
                         const unsigned char* data, 
                         size_t size)
{
  // Now, send it off.
  // Note the MSG_NOSIGNAL flag, which keeps send() from throwing a SIGPIPE when a client has
  // closed down.
  // These errors come because of a race condition: the client has closed down before I can reply,
  // but I have not yet polled select() again to see if the client has done so.
  // This is the simple solution to a problem faced only when debugging.
  // Also note the use of non-blocking send, which prevents lockups if the
  // client shuts down unexpectedly.
  
  if (!FD_ISSET(fd, &mClients)) {
    cout << Form("Requested send to client %d, who is no longer connected.\n",fd);
    return -1;
  }
  
  int bytessent = 0;
  mClientTimers[fd].Reset();

  while(bytessent < (int)(size)) {
    int res = send(fd, data+bytessent, size-bytessent, MSG_NOSIGNAL);
    if(res == -1) {
      if(errno == EAGAIN) {
        // Client is still sucking up the data. Give it a breather.
        usleep(500);        
      } else {
        cout << Form("Got error on send to client %d: %s\n",fd,strerror(errno));
        Close(fd);
        return -1;        
      }
    } else {
      bytessent += res;	    
    }	  

  }

  //logDebug(2,"Sent report to socket %d",fd);
  return 0;
}

int SocketServer::Close( int i )
{
  close(i); // bye!
  FD_CLR(i, &mClients); // remove from master set  
  return 0;
}


int SocketServer::ClientIsConnected( void )
{
  int num = 0;
  for(int fd = mFdMin; fd <= mFdMax; fd++) {
    if (FD_ISSET(fd, &mClients)) {
      if (fd != mListener) {
        num++;
      }
    }
  }
  return num;
}


void SocketServer::CheckForBadClients()
{
  // Kill off any clients who haven't been served in a while; they're probably bugged.
  for(int fd = mFdMin; fd <= mFdMax; fd++) {
    if (FD_ISSET(fd, &mClients)) {
      if (fd != mListener) {
        if(mClientTimers[fd].Test(15.0)) {
          // Been too long. Nuke it.
          cerr << "Client " << fd << " has been around for " << mClientTimers[fd].Count() << " seconds. Closing it." << endl;
          Close(fd);
        }
      }
    }
  }
  
}
