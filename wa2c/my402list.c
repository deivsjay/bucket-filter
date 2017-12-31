/******************************************************************************/
/* Important CSCI 402 usage information:                                      */
/*                                                                            */
/* This fils is part of CSCI 402 programming assignments at USC.              */
/*         53616c7465645f5f2e8d450c0c5851acd538befe33744efca0f1c4f9fb5f       */
/*         3c8feabc561a99e53d4d21951738da923cd1c7bbd11b30a1afb11172f80b       */
/*         984b1acfbbf8fae6ea57e0583d2610a618379293cb1de8e1e9d07e6287e8       */
/*         de7e82f3d48866aa2009b599e92c852f7dbf7a6e573f1c7228ca34b9f368       */
/*         faaef0c0fcf294cb                                                   */
/* Please understand that you are NOT permitted to distribute or publically   */
/*         display a copy of this file (or ANY PART of it) for any reason.    */
/* If anyone (including your prospective employer) asks you to post the code, */
/*         you must inform them that you do NOT have permissions to do so.    */
/* You are also NOT permitted to remove or alter this comment block.          */
/* If this comment block is removed or altered in a submitted file, 20 points */
/*         will be deducted.                                                  */
/******************************************************************************/

/*
 * Author:      Divya Jagadeesh
 *
 */

#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <sys/time.h>

#include "cs402.h" 

#include "my402list.h"

// static char gszProgName[MAXPATHLENGTH];

// int gnDebug=0;

// Returns the number of elements in the list. 
int  My402ListLength(My402List* divyaList){
	return divyaList->num_members;
	// return 0;
}

// Returns TRUE if the list is empty. Returns FALSE otherwise. 
int  My402ListEmpty(My402List* divyaList){
	if (divyaList->num_members == 0){
		return 1;
	}
	else {
		return 0;
	}
}

// If list is empty, just add obj to the list. Otherwise, add obj after Last(). This 
// function returns TRUE if the operation is performed successfully and 
// returns FALSE otherwise. 
int  My402ListAppend(My402List* divyaList, void* listObj){
	if (divyaList == NULL) {
		printf("Attempting to Append to an unintialized list\n");
		return 0;
	}
	My402ListElem* addToEnd = (My402ListElem*)malloc(sizeof(My402ListElem));
	if (!addToEnd){
		fprintf(stderr, "Allocation error!!!\n");
		return 0;
	}
	if (divyaList->num_members == 0) {
		divyaList->anchor.next = addToEnd;
		divyaList->anchor.prev = addToEnd;
		addToEnd->next = &(divyaList->anchor);
		addToEnd->prev = &(divyaList->anchor);
		// addToEnd->obj = listObj;
	}
	else {
		divyaList->anchor.prev->next = addToEnd;
		addToEnd->prev = divyaList->anchor.prev;
		addToEnd->next = &(divyaList->anchor);
		divyaList->anchor.prev = addToEnd;
	}
	divyaList->num_members = divyaList->num_members + 1;
	addToEnd->obj = listObj;
	return 1;
}

// If list is empty, just add obj to the list. Otherwise, add obj before First(). This 
// function returns TRUE if the operation is performed successfully and 
// returns FALSE otherwise. 
int  My402ListPrepend(My402List* divyaList, void* listObj){
	if (divyaList == NULL) {
		printf("Attempting to Prepend to an unintialized list\n");
		return 0;
	}
	My402ListElem* addToBeginning = (My402ListElem*)malloc(sizeof(My402ListElem));
	if (divyaList->num_members == 0) {
		divyaList->anchor.next = addToBeginning;
		divyaList->anchor.prev = addToBeginning;
		addToBeginning->next = &(divyaList->anchor);
		addToBeginning->prev = &(divyaList->anchor);
		// addToEnd->obj = listObj;
	}
	else {
		My402ListElem* firstElem = My402ListFirst(divyaList);
		divyaList->anchor.next = addToBeginning;
		addToBeginning->next = firstElem;
		firstElem->prev = addToBeginning;
		addToBeginning->prev = &(divyaList->anchor);
	}
	divyaList->num_members = divyaList->num_members + 1;
	addToBeginning->obj = listObj;
	return 1;
	// return 0;
}

// Unlink and delete elem from the list. Please do not delete the object pointed
// to by elem and do not check if elem is on the list. 
void My402ListUnlink(My402List* divyaList, My402ListElem* getRidOf){
	if (getRidOf == NULL) {
		printf("Trying to Unlink a NULL pointer\n");
	}
	My402ListElem* firstOne = My402ListFirst(divyaList);
	if (firstOne == getRidOf) {
		divyaList->anchor.next = getRidOf->next;
		// getRidOf->next->prev = getRidOf->prev;
		getRidOf->next->prev = &(divyaList->anchor);
	}
	else {
		getRidOf->prev->next = getRidOf->next;
		getRidOf->next->prev = getRidOf->prev;
	}
	free(getRidOf);
	divyaList->num_members = divyaList->num_members - 1;
}

// Unlink and delete all elements from the list and make the list empty. Please
// do not delete the objects pointed to by the list elements. 
void My402ListUnlinkAll(My402List* divyaList){
	if (divyaList == NULL) {
		printf("The list is unintialized. You attempted to UnlinkAll, but no elements exist in the list.\n");
	}
	if (divyaList->num_members == 0) {
		printf("The list  is initialized, but has no elements. You attempted to UnlinkAll, but no elements exist in the list.\n");
	}
	int n = divyaList->num_members;
	while (n != 0) {
		n = n - 1;
		My402ListElem* currentElem = My402ListFirst(divyaList);
		My402ListUnlink(divyaList, currentElem);
	}
	divyaList->num_members = 0; 
}

// Insert obj between elem and elem->next. If elem is NULL, then this is the same as 
// Append(). This function returns TRUE if the operation is performed
// successfully and returns FALSE otherwise. Please do not check if elem is on
// the list. 
int  My402ListInsertAfter(My402List* divyaList, void* listObj, My402ListElem* beforeAddOn){
	if (divyaList == NULL) {
		printf("Attempting to InsertAfter to an unintialized list\n");
		return 0;
	}
	if (beforeAddOn == NULL) {
		return My402ListAppend(divyaList, listObj);
	}
	else {
		My402ListElem* addOn = (My402ListElem*)malloc(sizeof(My402ListElem));
		addOn->next = beforeAddOn->next;
		addOn->prev = beforeAddOn;
		beforeAddOn->next->prev = addOn;
		beforeAddOn->next = addOn;
		divyaList->num_members = divyaList->num_members + 1;
		addOn->obj = listObj;
		return 1;
	}

	// return 0;
}

// Insert obj between elem and elem->prev. If elem is NULL, then this is the same as
// Prepend(). This function returns TRUE if the operation is performed 
// successfully and returns FALSE otherwise. Please do not check if elem is on
// the list. 
int  My402ListInsertBefore(My402List* divyaList, void* listObj, My402ListElem* afterAddOn){
	if (divyaList == NULL) {
		printf("Attempting to InsertAfter to an unintialized list\n");
		return 0;
	}
	if (afterAddOn == NULL) {
		return My402ListPrepend(divyaList, listObj);
	}
	else {
		My402ListElem* addOn = (My402ListElem*)malloc(sizeof(My402ListElem));
		if (afterAddOn->prev == &(divyaList->anchor)) {
			divyaList->anchor.next = addOn;
			addOn->next = afterAddOn;
			afterAddOn->prev = addOn;
			addOn->prev = &(divyaList->anchor);
		}
		else {
			afterAddOn->prev->next = addOn;
			addOn->prev = afterAddOn->prev;
			addOn->next = afterAddOn;
			afterAddOn->prev = addOn;
		}
		divyaList->num_members = divyaList->num_members + 1;
		addOn->obj = listObj;
		return 1;
	}
	// return 0;
}

// Returns the first list element or NULL if the list is empty.
My402ListElem *My402ListFirst(My402List* divyaList){
		if (divyaList->num_members == 0){
		return NULL;
	}
	else{
		return divyaList->anchor.next;
	}
	// return NULL;
}

// Returns the last list element or NULL if the list is empty.
My402ListElem *My402ListLast(My402List* divyaList){
	if (divyaList->num_members == 0){
		return NULL;
	}
	else{
		return divyaList->anchor.prev;
	}
	// return NULL;
}

// Returns elem->next or NULL if elem is the last item on the list. Please do not
// check if elem is on the list. 
My402ListElem *My402ListNext(My402List* divyaList, My402ListElem* whosAfterMe){
	if (My402ListLast(divyaList) == whosAfterMe){
		return NULL;
	}
	else {
		return whosAfterMe->next;
	}
	// return NULL;
}

// Returns elem->prev or NULL if elem is the first item on the list. Please do not 
// check if elem is on the list.
My402ListElem *My402ListPrev(My402List* divyaList, My402ListElem* whosBeforeMe){
	if (My402ListFirst(divyaList) == whosBeforeMe) {
		return NULL;
	}
	else {
		return whosBeforeMe->prev;
	}
	// return NULL;
}

// Returns the list element elem such that elem->obj == obj. Returns NULL if no
// such element can be found. 
My402ListElem *My402ListFind(My402List* divyaList, void* findMe){
	if (divyaList->num_members == 0){	// only anchor in list
		return NULL;
	}
	else {	// more than anchor in list
		My402ListElem* traverseList = My402ListFirst(divyaList);
		while (traverseList != &(divyaList->anchor)) {
			if (traverseList->obj == findMe) {
				return traverseList;
			}
			else {
				traverseList = traverseList->next;
			}
		}
		return NULL;
	}
	// return NULL;
}


// Initialize the list into an empty list. Returns TRUE if all is well and returns 
// FALSE if there is an error initializing the list.
int My402ListInit(My402List* divyaList){
	if (divyaList == NULL){
		return 0;
	}
	divyaList->num_members = 0;
	// volatile int * x = (int *)&x;
	divyaList->anchor.next = (My402ListElem*) divyaList->anchor.next;
	divyaList->anchor.prev = (My402ListElem*) divyaList->anchor.prev;
	return 1;
}
