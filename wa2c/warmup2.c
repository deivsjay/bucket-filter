/*
 * Author:      Divya Jagadeesh
 * Created: 	9/15/17
 *
 */

#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#include "cs402.h" 
#include "my402list.h"
#include "warmup2.h"

void usage() {
	fprintf(stderr, "usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
}

// returns 0 for deterministic mode
// returns 1 for trace-driven mode
int argError(int argc, char *argv[]) {

	// -t command specified without tsfile name
	int tsFileFound = 0;
	int i = 0; 
	for (i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-t") == 0) {
			tsFileFound = 1;
			i++;
			if (i == argc) {
				fprintf(stderr, "ERROR: File not specified.\n");
				usage();
				exit(1);
			}
			const char *test = argv[i];
			if (test[0] == '-') {
				fprintf(stderr, "ERROR: File not specified.\n");
				usage();
				exit(1);
			}
    		FILE *file = NULL;
    		file = fopen(argv[i], "r");
			if (!file){
    			fprintf(stderr, "ERROR: %s can't be opened. %s\n", argv[i], strerror(errno));
    			usage();
    			exit(1);
			}
			else {
				fileName = argv[i];
   				fclose(file);
			}
		}
	}

	if (tsFileFound == 1) {
		mode = 1;
		return 1;
	}

		
	for (i = 0; i < argc; i++) {
		const char *test = argv[i];
		// if there isn't a value followed by -B, -P, -num
		if (strcmp(test, "-B") == 0 || strcmp(test, "-P") == 0 || strcmp(test, "-n") == 0) {
			i++;
			if (i == argc) {
				fprintf(stderr, "ERROR: %s does not have a corresponding input value.\n", test);
				usage();
				exit(1);
			}
			int intVal = 0;
			if (sscanf(argv[i], "%d", &intVal) != 1) {
				fprintf(stderr, "ERROR: %s does not have a corresponding input value.\n", test);
				usage();
				exit(1);
			}
		}
		// there isn't a value followed by -lambda, -mu, -r
		if (strcmp(test, "-lambda") == 0 || strcmp(test, "-mu") == 0 || strcmp(test, "-r") == 0) {
			i++;
			if (i == argc) {
				fprintf(stderr, "ERROR: %s does not have a corresponding input value.\n", test);
				usage();
				exit(1);
			}
			int doubleVal = 0.0;
			if (sscanf(argv[i], "%d", &doubleVal) != 1) {
				fprintf(stderr, "ERROR: %s does not have a corresponding input value.\n", test);
				usage();
				exit(1);
			}
		}

		if (strcmp(test, "./warmup2") != 0 && strcmp(test, "-B") != 0 && strcmp(test, "-P") != 0 && strcmp(test, "-n") != 0 && strcmp(test, "-lambda") != 0 && strcmp(test, "-mu") != 0 && strcmp(test, "-r") != 0) {
			fprintf(stderr, "ERROR: %s is not a valid command.\n", test);
			usage();
			exit(1);
		}
	}

	// incorrect -command inputed
	for (i = 0; i < argc; i++) { 
		const char *test = argv[i];
		if (test[0] == '-') {
			if (strcmp(test, "-lambda") != 0 && strcmp(test, "-mu") != 0 && strcmp(test, "-r") != 0 && 
				strcmp(test, "-B") != 0 && strcmp(test, "-P") != 0 && strcmp(test, "-n") != 0 && 
				strcmp(test, "-t") != 0){
				fprintf(stderr, "ERROR: %s is not a correct command.\n", test);
				usage();
				exit(1);
			}
		}
	}

	mode = 0;
	return 0;
}

void deterministicInput(int argc, char *argv[]) {
	int i = 0;
	for (i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-lambda") == 0){
			i++;
			lambdaString = argv[i];
			lambda = atof(argv[i]);
		}
		if (strcmp(argv[i], "-mu") == 0){
			i++;
			muString = argv[i];
			mu = atof(argv[i]);
		}
		if (strcmp(argv[i], "-r") == 0){
			i++;
			rString = argv[i];
			r = atof(argv[i]);
		}
		if (strcmp(argv[i], "-B") == 0){
			i++;
			B = atoi(argv[i]);
		}
		if (strcmp(argv[i], "-P") == 0){
			i++;
			P = atoi(argv[i]);
		}
		if (strcmp(argv[i], "-n") == 0){
			i++;
			num = atoi(argv[i]);
		}
	}

	printEmulationParameters();

	if (1.0/lambda > 10) {
		lambda = 0.1;
	}
	if (1.0/mu > 10) {
		mu = 0.1;
	}
	// printf("lambda: %f\n", lambda);
	// printf("mu: %f\n", mu);
	// printf("r: %f\n", r);
	// printf("B: %ld\n", B);
	// printf("P: %ld\n", P);
	// printf("num: %ld\n", num);
}

void traceInput(int argc, char *argv[]) {

	int i = 0;
	for (i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-r") == 0){
			i++;
			rString = argv[i];
			r = atof(argv[i]);
		}
		if (strcmp(argv[i], "-B") == 0){
			i++;
			B = atoi(argv[i]);
		}
	}

}

void checkR() {
	if (1.0/r > 10) {
		printf("entered if statement\n");
		r = 0.1;
	}
}


void printEmulationParameters() {
	fprintf(stdout, "Emulation Parameters:\n");
	fprintf(stdout, "\tnumber to arrive = %lld\n", num);
	if (mode == 0) {
		// fprintf(stdout, "\tlambda = %f\n", lambda);
		// fprintf(stdout, "\tmu = %f\n", mu);
		fprintf(stdout, "\tlambda = %s\n", lambdaString);
		fprintf(stdout, "\tmu = %s\n", muString);
	}
	// fprintf(stdout, "\tr = %f\n", r);
	fprintf(stdout, "\tr = %s\n", rString);
	fprintf(stdout, "\tB = %ld\n", B);
	if (mode == 0) {
		fprintf(stdout, "\tP = %ld\n", P);
	}
	if (mode == 1) {
		fprintf(stdout, "\ttsfile = %s\n", fileName);
	}
	fprintf(stdout, "\n");
}

// rewrite function
// thread should die when 1. Ctrl+C is pressed or 2. last packet enters Q2
void* initializeBucket() {

	// pthread_cleanup_push()
	int i = 0; 
	// printf("1/r: %f\n", 1.0/r);
	int keepGoing = 1;
	// for (i = 1; i < num + 1; i++) {
	while (keepGoing == 1) {
		// printf("place 1\n");
		usleep(1000000.0/r);	// sleep for allotted time
		pthread_mutex_lock(&mutex);	
		// pthread_cleanup_push(cleanup, 0);
		// printf("place 2\n");
		// if (My402ListLength(Q2) < num) {
		if (packetsEnteredQ2 < num && packetsDropped != num) {
			// printf("place 3\n");
			if (My402ListLength(Q1) > 0){
				// printf("place 4\n");
				moveQ1();
				// printf("place 5\n");
			}
			// printf("place 6\n");
			i = i + 1;					// token number
			tokens = tokens + 1;		// increment token count
			allTokens = allTokens + 1;
			// printf("place 7\n");
			// get time of day
			struct timeval currTime;
			gettimeofday(&currTime, NULL);

			// too many tokens in bucket
			if (tokens > B) {
				// printf("place 9\n");
				tokens = tokens - 1;
				tokensDropped = tokensDropped + 1;
				fprintf(stdout, "%012.3fms: token t%d arrives, dropped\n", formattedTime(currTime, timeBegins), i);
			}
			else {
				// printf("place 10\n");
				if (tokens == 1) {
					fprintf(stdout, "%012.3fms: token t%d arrives, token bucket now has %d token\n", formattedTime(currTime, timeBegins), i, tokens);
				}
				else {
					fprintf(stdout, "%012.3fms: token t%d arrives, token bucket now has %d tokens\n", formattedTime(currTime, timeBegins), i, tokens);
				}
			}
			// printf("before move Q1 in bucket function\n");
			if (My402ListLength(Q1) > 0){
				// printf("place 11\n");
				moveQ1();
				// printf("place 12\n");
			}
		}
		else {
			// printf("should enter here in bucket function\n");
			// pthread_exit((void*)1);
			keepGoing = 0;
		}
		
		// pthread_cleanup_pop(0);
		pthread_mutex_unlock(&mutex);
	}

	// printf("token thread about to die\n");
	deadThreads = deadThreads + 1;
	return(0);
}

double formattedTime(struct timeval firstTime, struct timeval secondTime) {
	struct timeval resTime;

	// subtract seconds and microseconds
	time_t seconds = firstTime.tv_sec - secondTime.tv_sec;
	suseconds_t microseconds = firstTime.tv_usec - secondTime.tv_usec;
	// if microseconds subtraction is negative, add on to microseconds
	if (microseconds < 0) {
		microseconds = microseconds + 1000000;
		seconds = seconds - 1;
	}
	resTime.tv_sec = seconds;
	resTime.tv_usec = microseconds;

	// timersub(&firstTime, &secondTime, &resTime);
	double printTime = ((resTime.tv_sec) * 1000) + ((resTime.tv_usec) / 1000.0);
	return printTime;
}

char* beginningOfNextString(char *startPtr) {
	if (*startPtr == ' ' || *startPtr == '\t'){
		// printf("print here1: %s\n", startPtr);
		int longestRemaining = 0;
		char *tempPtr;

		// see if 1 is next character in string
		char *retNum1 = strchr(startPtr, '1');
		int retLen1 = 0;
		if (retNum1 != NULL) {
			retLen1 = strlen(retNum1);
			if (retLen1 > longestRemaining) {
				longestRemaining = retLen1;
				tempPtr = retNum1;
			}
		}

		// see if 2 in next character in string
		char *retNum2 = strchr(startPtr, '2');
		int retLen2 = 0;
		if (retNum2 != NULL) {
			retLen2 = strlen(retNum2);
			if (retLen2 > longestRemaining) {
				longestRemaining = retLen2;
				tempPtr = retNum2;
			}
		}

		// see if 3 is next character in string
		char *retNum3 = strchr(startPtr, '3');
		int retLen3 = 0;
		if (retNum3 != NULL) {
			retLen3 = strlen(retNum3);
			if (retLen3 > longestRemaining) {
				longestRemaining = retLen3;
				tempPtr = retNum3;
			}
		}

		// see if 4 is next character in string
		char *retNum4 = strchr(startPtr, '4');
		int retLen4 = 0;
		if (retNum4 != NULL) {
			retLen4 = strlen(retNum4);
			if (retLen4 > longestRemaining) {
				longestRemaining = retLen4;
				tempPtr = retNum4;
			}
		}

		// see if 5 is next character in string
		char *retNum5 = strchr(startPtr, '5');
		int retLen5 = 0;
		if (retNum5 != NULL) {
			retLen5 = strlen(retNum5);
			if (retLen5 > longestRemaining) {
				longestRemaining = retLen5;
				tempPtr = retNum5;
			}
		}

		// see if 6 is next character in string
		char *retNum6 = strchr(startPtr, '6');
		int retLen6 = 0;
		if (retNum6 != NULL) {
			retLen6 = strlen(retNum6);
			if (retLen6 > longestRemaining) {
				longestRemaining = retLen6;
				tempPtr = retNum6;
			}
		}

		// see if 7 is next character in string
		char *retNum7 = strchr(startPtr, '7');
		int retLen7 = 0;
		if (retNum7 != NULL) {
			retLen7 = strlen(retNum7);
			if (retLen7 > longestRemaining) {
				longestRemaining = retLen7;
				tempPtr = retNum7;
			}
		}

		// see if 8 is next character in string
		char *retNum8 = strchr(startPtr, '8');
		int retLen8 = 0;
		if (retNum8 != NULL) {
			retLen8 = strlen(retNum8);
			if (retLen8 > longestRemaining) {
				longestRemaining = retLen8;
				tempPtr = retNum8;
			}
		}

		// see if 9 is next character in string
		char *retNum9 = strchr(startPtr, '9');
		int retLen9 = 0;
		if (retNum9 != NULL) {
			retLen9 = strlen(retNum9);
			if (retLen9 > longestRemaining) {
				longestRemaining = retLen9;
				tempPtr = retNum9;
			}
		}
		startPtr = tempPtr;
		// printf("print here2: %s\n", startPtr);
	}
	return startPtr;
}

void updatePLambdaMu () {

	// read next line
	char buf[1024] = "";
	fgets(buf, sizeof(buf), fp);

	// pointer starting at beginning of line
	char *startPtr = buf;
	char *replacePtr = NULL;

	// find end of first string
	char *ret1 = strchr(startPtr, ' ');
	char *ret2 = strchr(startPtr, '\t');

	// if either is null, declaring length of null string to 0
	int len1 = 0;
	if (ret1 != NULL) {
		len1 = strlen(ret1);
	}
	int len2 = 0;
	if (ret2 != NULL) {
		len2 = strlen(ret2);
	}
	
	// whichever one has bigger string, means the number is contained in that one without extra spaces
	if (len1 > len2) {
		replacePtr = ret1;
		*replacePtr = '\n';
	}
	else {
		replacePtr = ret2;
		*replacePtr = '\n';
	}
	int value = atoi(startPtr);
	lambda = value;
	// printf("inter-arrival time: %f\n", lambda);

	// find beginning of next string
	startPtr = ++replacePtr;
	startPtr = beginningOfNextString(startPtr);

	// find end of next string
	ret1 = strchr(startPtr, ' ');
	ret2 = strchr(startPtr, '\t');

	// if either is null, declaring length of null string to 0
	len1 = 0;
	if (ret1 != NULL) {
		len1 = strlen(ret1);
	}
	len2 = 0;
	if (ret2 != NULL) {
		len2 = strlen(ret2);
	}
	
	// whichever one has bigger string, means the number is contained in that one without extra spaces
	if (len1 > len2) {
		replacePtr = ret1;
		*replacePtr = '\n';
	}
	else {
		replacePtr = ret2;
		*replacePtr = '\n';
	}
	value = atoi(startPtr);
	P = value;
	// printf("number of tokens needed: %ld\n", P);

	// find beginning of last string
	startPtr = ++replacePtr;
	startPtr = beginningOfNextString(startPtr);

	// find value of last string
	ret1 = strchr(startPtr, '\n');
	value = atoi(startPtr);
	mu = value;
	// printf("service time: %f\n", mu);

}

void* startPacketFlow() {
	// int i = 0;
	for (iPacket = 1; iPacket < num + 1; iPacket++) {

		if (mode == 1){
			pthread_mutex_lock(&mutex);
			updatePLambdaMu();
			pthread_mutex_unlock(&mutex);
			usleep(1000.0*lambda);
			// printf("entered correct mode\n");
		}
		else {
			usleep(1000000.0/lambda);
		}
		
		pthread_mutex_lock(&mutex);
		// pthread_cleanup_push(cleanup, 0);

		// check to see if you can move anything out of Q1
		// if (My402ListLength(Q1) > 0) {
		// 	moveQ1(); 
		// }

		// create new packet, append packet to list, hold on to node that points to packet
		PacketInfo *newPacket = (PacketInfo*)malloc(sizeof(PacketInfo));
		PacketInfoInit(newPacket);
		My402ListAppend(Q1, (void*) newPacket);
		// My402ListElem* nodeOfPacket = My402ListLast(Q1);

		PacketInfo *newPacket1 = (PacketInfo*)malloc(sizeof(PacketInfo));
		PacketInfoInit(newPacket1);
		My402ListAppend(allPackets, (void*) newPacket1);
		My402ListElem* nodeOfPacket1 = My402ListLast(allPackets);
		// printf("length of Q1 after check: %d\n", My402ListLength(Q1));

		// get current time
		struct timeval currTime;
		gettimeofday(&currTime, NULL);

		newPacket->packetNum = iPacket;	// packet number
		newPacket->tokensNeeded = P;
		newPacket->requestedServiceTime = mu;
		newPacket->arrives = formattedTime(currTime, timeBegins);		// packet arrives at this time relative to time begins
		newPacket->packetStatus = 2;

		newPacket1->packetNum = iPacket;	// packet number
		newPacket1->tokensNeeded = P;
		newPacket1->requestedServiceTime = mu;
		newPacket1->arrives = formattedTime(currTime, timeBegins);
		newPacket1->packetStatus = 2;

		// calculate inter-arrival time of packet
		if (iPacket == 1) {	// if first packet in Q1			// if not first packet in Q1
			newPacket->interarrivalTime = formattedTime(currTime, timeBegins);
			newPacket1->interarrivalTime = formattedTime(currTime, timeBegins);
			// printf("entered\n");
		}
		else {			// not first packet in Q1
			// My402ListElem* prevNode = nodeOfPacket->prev;
			// PacketInfo* prevPacket = prevNode->obj;
			My402ListElem* prevNode1 = nodeOfPacket1->prev;
			PacketInfo* prevPacket1 = prevNode1->obj;
			newPacket->interarrivalTime = formattedTime(currTime, timeBegins) - prevPacket1->arrives;
			newPacket1->interarrivalTime = formattedTime(currTime, timeBegins) - prevPacket1->arrives;
		}

		// if tokens needed to graduate package > bucket size... drop, else continue onto Q1
		if (P > B) {
			fprintf(stdout, "%012.3fms: p%d arrives, needs %ld tokens, inter-arrival time = %.3fms, dropped\n", newPacket->arrives, newPacket->packetNum, P, newPacket->interarrivalTime);
			packetsDropped = packetsDropped + 1;
			// printf("length of Q1: %d\n", My402ListLength(Q1));
			My402ListUnlink(Q1, My402ListLast(Q1));
			// printf("unlinked\n");
			newPacket1->packetStatus = 2;
			packetsEnteredQ2 = packetsEnteredQ2 + 1;
			// printf("before broadcast in packet flow function\n");
			pthread_cond_broadcast(&cv);
			// printf("after broadcast in packet flow function\n");
		}
		else {
			// packet arrives
			fprintf(stdout, "%012.3fms: p%d arrives, needs %ld tokens, inter-arrival time = %.3fms\n", newPacket->arrives, newPacket->packetNum, P, newPacket->interarrivalTime);

			// officially enters Q1
			gettimeofday(&currTime, NULL);
			newPacket->entersQ1 = formattedTime(currTime, timeBegins);
			newPacket1->entersQ1 = formattedTime(currTime, timeBegins);
			newPacket->packetStatus = 3;
			newPacket1->packetStatus = 3;
			fprintf(stdout, "%012.3fms: p%d enters Q1\n", newPacket->entersQ1, newPacket->packetNum);

			
			// moveQ1(); // check to see if it can move out of Q1 immediately
		}
		
		// pthread_cleanup_pop(0);
		pthread_mutex_unlock(&mutex);
	}

	// cannot call move to Q2 without this function, so if there's leftover elements in Q1, call this function
	// pthread_mutex_lock(&mutex);
	// if (My402ListLength(Q1) > 0) {
	// 	// printf("ENTERED\nENTERED\nENTERED: %d\n", My402ListLength(Q1));
	// 	moveQ1();
	// }
	// pthread_mutex_unlock(&mutex);

	// printf("made it here\n");
	deadThreads = deadThreads + 1;
	// printf("deadThreads = %d\n", deadThreads);
	// printf("packet thread about to die\n");
	return(0);
}

My402ListElem *findInAllPackets (My402ListElem *origPacket) {
	PacketInfo *currPacket = origPacket->obj;
	int findPacketNum = currPacket->packetNum;
	My402ListElem *traverse = My402ListFirst(allPackets);
	int i = 0;
	for (i = 0; i < findPacketNum - 1; i++) {
		traverse = traverse->next;
	}
	return traverse;
}

// update elements in allPackets list with Q2 updates
void moveQ1() {

	// first element in Q1
	My402ListElem *currNode = My402ListFirst(Q1);
	PacketInfo *currPacket = My402ListFirst(Q1)->obj;

	// find this element in allPackets list
	My402ListElem *traverse = findInAllPackets(currNode);
	PacketInfo *traversePacket = traverse->obj;

	// start from beginning of list, take off as many packages as possible, stop when package can't be taken off
	while (My402ListLength(Q1) > 0 && currPacket->tokensNeeded <= tokens) {

		// update tokens and Q1 and Q2
		tokens = tokens - currPacket->tokensNeeded;
		My402ListAppend(Q2, currPacket);
		My402ListUnlink(Q1, currNode);

		struct timeval currTime;
		gettimeofday(&currTime, NULL);	// current time

		currPacket->leavesQ1 = formattedTime(currTime, timeBegins);				// officially leaves Q1
		currPacket->timeInQ1 = currPacket->leavesQ1 - currPacket->entersQ1;		// updates time in Q1

		traversePacket->leavesQ1 = formattedTime(currTime, timeBegins);						// officially leaves Q1
		traversePacket->timeInQ1 = traversePacket->leavesQ1 - traversePacket->entersQ1;		// updates time in Q1

		// prints time in Q1
		if (tokens == 0 || tokens == 1){
			fprintf(stdout, "%012.3fms: p%d leaves Q1, time in Q1 = %.3fms, token bucket now has %d token\n", currPacket->leavesQ1, currPacket->packetNum, currPacket->timeInQ1, tokens);
		}
		else {
			fprintf(stdout, "%012.3fms: p%d leaves Q1, time in Q1 = %.3fms, token bucket now has %d tokens\n", currPacket->leavesQ1, currPacket->packetNum, currPacket->timeInQ1, tokens);
		}
		gettimeofday(&currTime, NULL);	// gets current time again
		currPacket->entersQ2 = formattedTime(currTime, timeBegins);		// officially enters Q2
		traversePacket->entersQ2 = formattedTime(currTime, timeBegins);
		fprintf(stdout, "%012.3fms: p%d enters Q2\n", currPacket->entersQ2, currPacket->packetNum);

		// increment enters Q2 variable
		packetsEnteredQ2 = packetsEnteredQ2 + 1;

		pthread_cond_broadcast(&cv);

		// move to next node in list
		if (My402ListLength(Q1) > 0) {
			currNode = My402ListFirst(Q1);
			currPacket = My402ListFirst(Q1)->obj;
			traverse = findInAllPackets(currNode);
			traversePacket = traverse->obj;
		}
	}
}

void openFile(int argc, char *argv[]) {
	char buf[1024] = "";

	// find which argument is file name
	int i = 0;
	int fileName = 0;
	for (i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-t") == 0) {
			i++;
			fileName = i;
		}
	}

	// open file
	fp = fopen(argv[fileName], "r");
	fgets(buf, sizeof(buf), fp);

	// make sure length of line is less than 1024 characters
	char* startPtr = buf;
	if (strlen(startPtr) > 1023) {
	    fprintf(stderr, "A mistake was made on line 1.\n");
	    fprintf(stderr, "Each transaction is limited to 1024 characters per line.\n");
	    exit(1);
	}

	// set num value
	int intVal = 0;
	if (sscanf(buf, "%d", &intVal) != 1){
		fprintf(stderr, "ERROR in file: File input on first line is incorrect.\n");
		exit(1);
	}
	num = atoi(buf);
	// printf("number of lines in file is: %ld\n", num);
}

void* runServer1() {
	
	while ((My402ListLength(Q2) != 0 && signalFlag == 0) || (packetsEnteredQ2 != num && signalFlag == 0 && packetsDropped != num && (My402ListLength(allPackets) == 0 || My402ListLength(Q1) != 0 || My402ListLength(Q2) != 0))) {
		pthread_mutex_lock(&mutex);
		while (My402ListLength(Q2) == 0 && signalFlag == 0 && packetsDropped != num && packetsEnteredQ2 != num) {
			pthread_cond_wait(&cv, &mutex);
		}
		if (signalFlag == 0 && packetsDropped != num && My402ListLength(Q2) > 0) {
			My402ListElem *arg = dequeueFromQ2();
			pthread_mutex_unlock(&mutex);
			servicePacket(arg, 1);
		}
		else {
			pthread_mutex_unlock(&mutex);
		}

	}

	deadThreads = deadThreads + 1;
	return(0);
}

void* runServer2() {
	
	while ((signalFlag == 0 && My402ListLength(Q2) != 0) || (packetsEnteredQ2 != num && signalFlag == 0 && packetsDropped != num && (My402ListLength(allPackets) == 0 || My402ListLength(Q1) != 0 || My402ListLength(Q2) != 0))) {
		pthread_mutex_lock(&mutex);
		while (My402ListLength(Q2) == 0 && signalFlag == 0 && packetsDropped != num && packetsEnteredQ2 != num) {
			pthread_cond_wait(&cv, &mutex);
		}
		if (signalFlag == 0 && packetsDropped != num && My402ListLength(Q2) > 0) {
			My402ListElem *arg = dequeueFromQ2();
			pthread_mutex_unlock(&mutex);
			servicePacket(arg, 2);
		}
		else {
			pthread_mutex_unlock(&mutex);
		}
		// unlock
		// service Q2
		// printf("server 2: at point 5\n");	
	}

	deadThreads = deadThreads + 1;
	return(0);
}

My402ListElem* dequeueFromQ2 () {

	// get current time
	struct timeval currTime;
	gettimeofday(&currTime, NULL);

	// first element in Q2
	My402ListElem *firstNode = My402ListFirst(Q2);
	PacketInfo *firstPacket = My402ListFirst(Q2)->obj;

	// find first element in Q2 in allPackets list
	My402ListElem *allPacketsNode = findInAllPackets(firstNode);
	PacketInfo *allPacketsPacket = allPacketsNode->obj;

	// update leaves Q2 and time in Q2
	firstPacket->leavesQ2 = formattedTime(currTime, timeBegins);
	allPacketsPacket->leavesQ2 = formattedTime(currTime, timeBegins);
	firstPacket->timeInQ2 = firstPacket->leavesQ2 - firstPacket->entersQ2;
	allPacketsPacket->timeInQ2 = allPacketsPacket->leavesQ2 - allPacketsPacket->entersQ2;

	// leaves Q2 message
	fprintf(stdout, "%012.3fms: p%d leaves Q2, time in Q2 = %.3fms\n", firstPacket->leavesQ2, firstPacket->packetNum, firstPacket->timeInQ2);

	My402ListUnlink(Q2, firstNode);		// unlink from Q2 list

	return allPacketsNode;
}

void servicePacket (My402ListElem *serviceNode, int serverNum) {

	My402ListElem *allPacketsNode = findInAllPackets(serviceNode);
	PacketInfo *allPacketsPacket = allPacketsNode->obj;

	// current time for server message
	struct timeval currTime;
	gettimeofday(&currTime, NULL);

	// enters server
	allPacketsPacket->entersServer = formattedTime(currTime, timeBegins);
	allPacketsPacket->server = serverNum;

	// begins service at S1 or S2
	if (mode == 1){
		fprintf(stdout, "%012.3fms: p%d begins service at S%d, requesting %dms of service\n", allPacketsPacket->entersServer, allPacketsPacket->packetNum, serverNum, (int) allPacketsPacket->requestedServiceTime);
	}
	else {
		int secondsInService = 1000.0/allPacketsPacket->requestedServiceTime;
		fprintf(stdout, "%012.3fms: p%d begins service at S%d, requesting %dms of service\n", allPacketsPacket->entersServer, allPacketsPacket->packetNum, serverNum, secondsInService);
	}

	if (mode == 1){
		double sleepTime = allPacketsPacket->requestedServiceTime;
		usleep(1000.0*sleepTime);
	}
	else {
		usleep(1000000.0/mu);
	}

	// current time for server message
	gettimeofday(&currTime, NULL);

	// leaves server, calculate server time
	allPacketsPacket->leavesServer = formattedTime(currTime, timeBegins);
	allPacketsPacket->timeInServer = allPacketsPacket->leavesServer - allPacketsPacket->entersServer;
	allPacketsPacket->timeInSystem = allPacketsPacket->leavesServer - allPacketsPacket->arrives;
	allPacketsPacket->packetStatus = 1;

	// print leaves server statistic
	fprintf(stdout, "%012.3fms: p%d departs from S%d, service time = %.3fms, time in system = %.3fms\n", allPacketsPacket->leavesServer, allPacketsPacket->packetNum, serverNum, allPacketsPacket->timeInServer, allPacketsPacket->timeInSystem);


}

void PacketInfoInit(PacketInfo *initThis) {
	initThis->packetNum = 0;			// packet number should be greater than 1
	initThis->tokensNeeded = -1;				// tokens needed should be 0 or positive number
	initThis->arrives = 0.0;				// positive time value > 0.0
	initThis->interarrivalTime = 0.0;		// positive time value > 0.0
	initThis->entersQ1 = 0.0;				// positive time value > 0.0
	initThis->leavesQ1 = 0.0;				// positive time value > 0.0
	initThis->timeInQ1 = 0.0;				// positive time value > 0.0	
	initThis->entersQ2 = 0.0;				// positive time value > 0.0
	initThis->leavesQ2 = 0.0;				// positive time value > 0.0		
	initThis->timeInQ2 = 0.0;				// positive time value > 0.0
	initThis->requestedServiceTime = 0.0;	// positive time value > 0.0
	initThis->entersServer = 0.0;			// positive time value > 0.0
	initThis->leavesServer = 0.0;			// positive time value > 0.0
	initThis->timeInServer = 0.0;			// positive time value > 0.0
	initThis->timeInSystem = 0.0;			// positive time value > 0.0 
	initThis->packetStatus = 0;				// packet status is 1, 2, or 3
	initThis->server = 0;					// server is 1 or 2
}

void printStatistics() {

	// current time for server message
	struct timeval currTime;
	gettimeofday(&currTime, NULL);
	
	// print emulation ends
	double emulationEnds = formattedTime(currTime, timeBegins);
	fprintf(stdout, "%012.3fms: emulation ends\n", emulationEnds);
	
	// calculate statistics
	// all packets' time in Q1, Q2, S1, S2 added up
	My402ListElem *traverse = My402ListFirst(allPackets);
	PacketInfo *packet = traverse->obj;
	double totalTimeSpentInQ1 = 0;
	double totalTimeSpentInQ2 = 0;
	double totalTimeSpentInS1 = 0;
	double totalTimeSpentInS2 = 0;
	int completedPackets = 0;
	double totalSystemTime = 0.0;
	double totalSystemTimeSquared = 0.0;
	double totalInterArrivalTime = 0.0;
	int servicedPackets = 0;
	double totalServiceTime = 0.0;
	int i = 0;
	for (i = 0; i < My402ListLength(allPackets); i++) {
		// came into the system, interArrival time
		if (packet->interarrivalTime != 0.0) {
			totalInterArrivalTime = totalInterArrivalTime + packet->interarrivalTime;
		}
		// serviced packets
		if (packet->server != 0) {
			totalServiceTime = totalServiceTime + packet->timeInServer;
			servicedPackets = servicedPackets + 1;
		}
		// entered and left Q1
		if (packet->timeInSystem != 0.0) {
			totalTimeSpentInQ1 = totalTimeSpentInQ1 + packet->timeInQ1;
		}
		// entered and left Q2
		if (packet->timeInSystem != 0.0) {
			totalTimeSpentInQ2 = totalTimeSpentInQ2 + packet->timeInQ2;
		}
		// entered and left S1
		if (packet->server == 1 && packet->timeInServer != 0.0) {
			totalTimeSpentInS1 = totalTimeSpentInS1 + packet->timeInServer;
		}
		// entered and left S2
		if (packet->server == 2 && packet->timeInServer != 0.0) {
			totalTimeSpentInS2 = totalTimeSpentInS2 + packet->timeInServer;
		}
		// entered and left system
		if (packet->timeInSystem != 0.0) {
			totalSystemTime = totalSystemTime + packet->timeInSystem;
			totalSystemTimeSquared = totalSystemTimeSquared + (packet->timeInSystem * packet->timeInSystem);
			completedPackets = completedPackets + 1;
		}
		if (i != My402ListLength(allPackets) - 1) {
			traverse = traverse->next;
			packet = traverse->obj;
		}
	}

	double average = 0.0;
	double variance = 0.0;
	if (completedPackets != 0) {
		average = totalSystemTime/(double) completedPackets;
		variance = (totalSystemTimeSquared/(double) completedPackets) + (average * average);
	}
	
	// print statistics
	fprintf(stdout, "Statistics:\n\n");
	if (My402ListLength(allPackets) != 0){
		fprintf(stdout, "\taverage packet inter-arrival time = %.6g\n", (totalInterArrivalTime/(double) My402ListLength(allPackets))/1000.0);
		//fprintf(stdout, "\taverage packet inter-arrival time = %.6g\n", (totalInterArrivalTime/(double) My402ListLength(allPackets))/1000.0);
	}
	else {
		fprintf(stdout, "average packet inter-arrival time = N/A, no packets arrived at this facility\n");
	}
	if (servicedPackets != 0) {
		fprintf(stdout, "\taverage packet service time = %.6g\n\n", (totalServiceTime/(double) servicedPackets)/1000.0);
	}
	else {
		fprintf(stdout, "\taverage packet service time = N/A, no packets were serviced\n\n");
	}
	fprintf(stdout, "\taverage number of packets in Q1 = %.6g\n", totalTimeSpentInQ1/emulationEnds);
	fprintf(stdout, "\taverage number of packets in Q2 = %.6g\n", totalTimeSpentInQ2/emulationEnds);
	fprintf(stdout, "\taverage number of packets in S1 = %.6g\n", totalTimeSpentInS1/emulationEnds);
	fprintf(stdout, "\taverage number of packets in S2 = %.6g\n\n", totalTimeSpentInS2/emulationEnds);
	if (completedPackets != 0) {
		fprintf(stdout, "\taverage time a packet spent in system = %.6g\n", (totalSystemTime/(double) completedPackets)/1000.0) ;
		fprintf(stdout, "\tstandard deviation for time spent in system = %.6g\n\n", sqrt(variance)/1000.0);
	}
	else {
		fprintf(stdout, "\taverage time a packet spent in system = N/A, there are no completed packets\n");
		fprintf(stdout, "\tstandard deviation for time spent in system = N/A, there are no completed packets\n\n");
	}
	if(allTokens != 0) {
		fprintf(stdout, "\ttoken drop probability = %.6g\n", (double) tokensDropped/(double) allTokens);
	}
	else {
		fprintf(stdout, "\ttoken drop probability = N/A, no tokens arrived at this facility\n");
	}
	if (My402ListLength(allPackets) != 0) {
		fprintf(stdout, "\tpacket drop proabability = %.6g\n", (double) packetsDropped/(double) My402ListLength(allPackets));
	}
	else {
		fprintf(stdout, "\tpacket drop probability = N/A, no packets arrived at this facility\n");
	}
	
}

void *monitor() {
	int sig=0;
	for (;;) {
	    sigwait(&set, &sig);				// waiting for Ctrl+C to be pressed
	    pthread_mutex_lock(&mutex);
	    signalFlag = 1;						// signal has been pressed
	    pthread_cond_broadcast(&cv);
    	pthread_cancel(tokenArrival);		// cancel token arrival thread
    	pthread_cancel(packetArrival);		// cancel packet arrival thread

    	// current time for server message
		struct timeval currTime;
		// gettimeofday(&currTime, NULL);
		int i = 0;
    	if (My402ListLength(Q1) > 0) {
    		My402ListElem *traverse = My402ListFirst(Q1);
    		PacketInfo *traversePacket = traverse->obj;
    		
    		for (i = 0; i < My402ListLength(Q1); i++) {
    			gettimeofday(&currTime, NULL);
    			fprintf(stdout, "%012.3fms: p%d removed from Q1\n", formattedTime(currTime, timeBegins), traversePacket->packetNum);
    			if (i != My402ListLength(Q1) - 1) {
    				traverse = traverse->next;
    				traversePacket = traverse->obj;
    			}
    		}
    	}

    	if (My402ListLength(Q2) > 0) {
    		My402ListElem *traverse = My402ListFirst(Q2);
    		PacketInfo *traversePacket = traverse->obj;
    		for (i = 0; i < My402ListLength(Q2); i++) {
    			gettimeofday(&currTime, NULL);
    			fprintf(stdout, "%012.3fms: p%d removed from Q2\n", formattedTime(currTime, timeBegins), traversePacket->packetNum);
    			if (i != My402ListLength(Q2) - 1) {
    				traverse = traverse->next;
    				traversePacket = traverse->obj;
    			}
    		}
    	}
	    // figure how how to stop server threads if all packets are dropped
	    pthread_mutex_unlock(&mutex);
	}
  	return 0;
}

void *cleanup () {
	fprintf(stdout, "cleanup is happening\n");

	return(0);
}


/* ----------------------- main() ----------------------- */

int main(int argc, char *argv[])
{

	My402List *list1 = (My402List*)malloc(sizeof(My402List));
	My402ListInit(list1);
	Q1 = list1;
	
	My402List *list2 = (My402List*)malloc(sizeof(My402List));
	My402ListInit(list2);
	Q2 = list2;

	My402List *list3 = (My402List*)malloc(sizeof(My402List));
	My402ListInit(list3);
	allPackets = list3;

    // argError(argc, argv);
    if (argError(argc, argv) == 0){
    	deterministicInput(argc, argv);
    }
    else if (argError(argc, argv) == 1) {
    	traceInput(argc, argv);
    	openFile(argc, argv);
    	printEmulationParameters();
    }
   	checkR();

   	gettimeofday(&timeBegins, NULL);	// initialize when emulation begins
   	fprintf(stdout, "00000000.000ms: emulation begins\n");

   	// instantiating threads
    pthread_t cntrlC;

    // signal catching stuff
    sigemptyset(&set);
  	sigaddset(&set, SIGINT);
  	sigprocmask(SIG_BLOCK, &set, 0);

  	// starting threads with given functions
    pthread_create(&tokenArrival, 0, initializeBucket, NULL);
    pthread_create(&packetArrival, 0, startPacketFlow, NULL);
    pthread_create(&server1, 0, runServer1, NULL);
    pthread_create(&server2, 0, runServer2, NULL);
    pthread_create(&cntrlC, 0, monitor, 0);

    // dead threads
    pthread_join(tokenArrival, 0);
    // printf("token thread died\n");
    pthread_join(packetArrival, 0);
    // printf("packet thread died\n");
    pthread_join(server1, 0);
    // printf("server 1 died\n");
    pthread_join(server2, 0);
    // printf("server 2 died\n");

    // printf("dead threads = %d\n", deadThreads);
    if(pthread_kill(tokenArrival, 0) != 0 && pthread_kill(packetArrival, 0) != 0 && pthread_kill(server1, 0) != 0 && pthread_kill(server2, 0) != 0)
	{
    	/* not still running */
		pthread_cancel(cntrlC);
	}
    // kill thread that Cntrl+C controls once other threads have died
    // if (deadThreads == 4){
    // 	pthread_cancel(cntrlC);
    // }
    pthread_join(cntrlC, 0);
	printStatistics();


    return 0;
}
