/*
 * Author:      Divya Jagadeesh
 *
 * Created: 	9/15/17
 */

#ifndef _WARMUP2_H_
#define _WARMUP2_H_

#include "cs402.h"
#include "my402list.h"
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

// global variables set to default values
int mode = 2;			// 1 = trace-drive, 0 = deterministic
double lambda = 1;		// specified on every line in t-file (1)
double mu = 0.35;		// specified on every line in t-file (3) 
double r = 1.5;
long int B = 10;		
long int P = 3;			// specified on every line in t-file (2)
long long int num = 20;		// first line in t-file
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 	// global mutex 
int tokens = 0;			// number of tokens in bucket at all times
struct timeval timeBegins;		// when emulation begins
My402List *Q1;
FILE *fp = NULL;
My402List *Q2;
My402List *allPackets;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
int packetsEnteredQ2 = 0;
int tokensDropped = 0;		// number of tokens dropped
int packetsDropped = 0;		// number of packets dropped
int allTokens = 0;			// all tokens that come into system
char *fileName = NULL;		// file name of t-file if specified
sigset_t set;				// ctrl+c signal control
int iPacket = 0;			// packet count in start packet flow function
pthread_t tokenArrival;	
pthread_t packetArrival;
pthread_t server1;
pthread_t server2;
int signalFlag = 0;
int deadThreads = 0;
char *lambdaString = "1";
char *muString = "0.35";
char *rString = "1.5";

// data structure My402ListElem's point to --> has all times and packet number
typedef struct tagPacketInfo {
	int packetNum;					// packet number in order of when it comes into system
	int tokensNeeded;				// tokens needed to move packet from Q1 to Q2
	double arrives;					// when packet arrives in startPacketFlow()
	double interarrivalTime;		// this packet's arrival minus last packet's arrival
	double entersQ1;				// officially enters Q1 list
	double leavesQ1;				// officially leaves Q1 list
	double timeInQ1;				// leaves Q1 - enters Q1		
	double entersQ2;				// officially enters Q2 list
	double leavesQ2;				// officially leaves Q2 list		
	double timeInQ2;				// leaves Q2 - enters Q2
	double requestedServiceTime;	// amount of time needs to sleep in server
	double entersServer;			// officially enters server
	double leavesServer;			// officially leaves server
	double timeInServer;			// leaves server - enters server
	double timeInSystem;			// leaves server - arrives		 
	int packetStatus;				// 1 - completed packets, 2 - didn't make it to Q1, 3 - didn't make it to server
	int server;						// server 1 or server 2, initialized to server 0 (doesn't exist)
} PacketInfo;

typedef struct tagTraceSpecs {
	int packetNum;
	double interarrivalTime;
	int P;
	double serviceTime;
} TraceSpecs;

// functions
extern void usage();
extern int argError(int, char *[]);
extern void deterministicInput(int, char *[]);
extern void traceInput(int, char *[]); 
extern void checkR();
extern void printEmulationParameters();
extern void* initializeBucket();
extern double formattedTime(struct timeval, struct timeval);
extern char* beginningOfNextString(char *);
extern void updatePLambdaMu();
extern void* startPacketFlow();
extern My402ListElem *findInAllPackets (My402ListElem *);
extern void moveQ1();
extern void openFile(int, char *[]);
extern void* runServer1();
// extern void dequeueFromQ2Server1();
extern void* runServer2();
// extern void dequeueFromQ2Server2();
extern My402ListElem* dequeueFromQ2 ();
extern void servicePacket (My402ListElem*, int);
extern void PacketInfoInit(PacketInfo *);
extern void printStatistics();
extern void *monitor();
// extern void* cleanup();


#endif /*_WARMUP2_H_*/
