#ifndef _SOCKETSERVER_H_
#define _SOCKETSERVER_H_

#include "Timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>

class SocketServer;

class SocketServer {
 private:
  int    mListener;
  int    mPort;
  fd_set mClients;
  int    mFdMax;
  int    mFdMin;
  struct sockaddr_in mMyAddress;
  struct sockaddr_in mClientAddress;
  unsigned char*     mRecieveBuffer;
  size_t             mRecieveBufferSize;
  std::map<int,Timer> mClientTimers;
  
 public:
  SocketServer( int port );  
  virtual ~SocketServer( void );
  int Setup( void );
  int ShutDown( void );
  int Listen( double inWaitSecs, 
	      size_t inMsgSize, 
	      unsigned char* &outDataRecvd, 
	      int&  outWhichClient,
	      bool& outGotNewClient
	      ); // return 0 for no data, 1 for data, -ve for error

  virtual size_t SufficientData( const unsigned char* inData, size_t inSize, size_t inMaxSize) =0;
  //{ // return number of bytes in valid message. 
  // i.e. if (inSize >= inMaxSize) return inMaxSize; else return 0; };
			
  int Send( const unsigned char* data, size_t size );
  int SendTo( int fd, const unsigned char* data, size_t size);
  int Close( int fd );
  int ClientIsConnected( void );
  void CheckForBadClients();
};

#endif
