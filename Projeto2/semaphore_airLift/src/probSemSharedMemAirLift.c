/**
 *  \file probSemSharedMemAirLift.c (implementation file)
 *
 *  \brief Problem name: Air Lift.
 *
 *  Synchronization based on semaphores and shared memory.
 *  Implementation with SVIPC.
 *
 *  Generator process of the intervening entities.
 *
 *  Upon execution, one parameter is requested:
 *    \li name of the logging file.
 *
 *  \author Nuno Lau - January 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <math.h>

#include "probConst.h"
#include "probDataStruct.h"
#include "logging.h"
#include "sharedDataSync.h"
#include "semaphore.h"
#include "sharedMemory.h"

/** \brief name of pilot process */
#define   PILOT         "./pilot"

/** \brief name of hostess process */
#define   HOSTESS       "./hostess"

/** \brief name of passenger process */
#define   PASSENGER     "./passenger"

/**
 *  \brief Main program.
 *
 *  Its role is starting the simulation by generating the intervening entities processes (pilot, hostess and passengers)
 *  and waiting for their termination.
 */

int main (int argc, char *argv[])
{
    char nFic[51];                                                                              /*name of logging file */
    char nFicErr[] = "error_        ";                                                     /* base name of error files */
    int shmid,                                                                      /* shared memory access identifier */
        semgid;                                                                     /* semaphore set access identifier */
    unsigned int  m;                                                                             /* counting variables */
    SHARED_DATA *sh;                                                                /* pointer to shared memory region */
    int pidPT,                                                                             /* pilot process identifier */
        pidHT,                                                                     /* hostess process identifier array */
        pidPG[N];                                                             /* passengers processes identifier array */
    int key;                                                           /*access key to shared memory and semaphore set */
    char num[2][12];                                                     /* numeric value conversion (up to 10 digits) */
    int status,                                                                                    /* execution status */
        info;                                                                                               /* info id */
    int p;

    /* getting log file name */
    if(argc==2) {
        strcpy(nFic, argv[1]);
    }
    else strcpy(nFic, "");

    /* composing command line */

    if ((key = ftok (".", 'a')) == -1) {
        perror ("error on generating the key");
        exit (EXIT_FAILURE);
    }
    sprintf (num[1], "%d", key);

    /* creating and initializing the shared memory region and the log file */

    if ((shmid = shmemCreate (key, sizeof (SHARED_DATA))) == -1) { 
        perror ("error on creating the shared memory region");
        exit (EXIT_FAILURE);
    }
    if (shmemAttach (shmid, (void **) &sh) == -1) { 
        perror ("error on mapping the shared region on the process address space");
        exit (EXIT_FAILURE);
    }

    srandom ((unsigned int) getpid ());                                                      /* initialize random generator */


    /* initialize problem internal status */

    sh->fSt.st.pilotStat   = FLYING_BACK;                                   /* the pilot is flying towards starting airport */
    sh->fSt.st.hostessStat = WAIT_FOR_FLIGHT;                            /* the hostess is waiting for the flight to arrive */
    for (p = 0; p < N; p++) {
        sh->fSt.st.passengerStat[p] = GOING_TO_AIRPORT;                          /* the passengers are going to the airport */
    }
    sh->fSt.finished         = false;                                       
    sh->fSt.nPassInQueue     = 0;                                          
    sh->fSt.nPassInFlight    = 0;                                         
    sh->fSt.totalPassBoarded = 0;                                        

    /* initialize problem internal status */

    createLog (nFic);                                                                             /* log file creation */

    /* initialize semaphore ids */

    sh->mutex = MUTEX;                                                              /* mutual exclusion semaphore id */
    sh->passengersInQueue = PASSENGERSINQUEUE;                                       
    sh->passengersWaitInQueue = PASSENGERSWAITINQUEUE;                              
    sh->passengersWaitInFlight = PASSENGERSWAITINFLIGHT;                           
    sh->readyForBoarding = READYFORBOARDING;                                      
    sh->readyToFlight = READYTOFLIGHT;                                           
    sh->idShown = IDSHOWN;                                                      
    sh->planeEmpty = PLANEEMPTY;                                                      

    /* creating and initializing the semaphore set */

    if ((semgid = semCreate (key, SEM_NU)) == -1) { 
        perror ("error on creating the semaphore set");
        exit (EXIT_FAILURE);
    }
    if (semUp (semgid, sh->mutex) == -1) {                                      /* enabling access to critical region */
        perror ("error on executing the up operation for semaphore access");
        exit (EXIT_FAILURE);
    }

    /* generation of intervening entities processes */

    strcpy (nFicErr + 6, "PG");
    for (p = 0; p < N; p++) {                                                                  /* passenger processes */
        if ((pidPG[p] = fork ()) < 0) {
            perror ("error on the fork operation for the passenger");
            exit (EXIT_FAILURE);
        }
        sprintf(num[0],"%d",p);
        sprintf(nFicErr+8,"%02d",p); 
        if (pidPG[p] == 0)
            if (execl (PASSENGER, PASSENGER, num[0], nFic, num[1],nFicErr, NULL) < 0) { 
                perror ("error on the generation of the passenger process");
                exit (EXIT_FAILURE);
            }
    }

    strcpy (nFicErr + 6, "HT");
    if ((pidHT = fork ()) < 0)  {                                                               /* hostess process */
        perror ("error on the fork operation for the hostess");
        exit (EXIT_FAILURE);
    }
    if (pidHT == 0) {
        if (execl (HOSTESS, HOSTESS, nFic, num[1], nFicErr, NULL) < 0) {
            perror ("error on the generation of the hostess process");
            exit (EXIT_FAILURE);
        }
    }

    strcpy (nFicErr + 6, "PT");
    if ((pidPT = fork ()) < 0) {                                                                   /* pilot process */
        perror ("error on the fork operation for the pilot");
        exit (EXIT_FAILURE);
    }
    if (pidPT == 0)
        if (execl (PILOT, PILOT, nFic, num[1], nFicErr, NULL) < 0) { 
            perror ("error on the generation of the referee process");
            exit (EXIT_FAILURE);
        }

    /* signaling start of operations */

    if (semSignal (semgid) == -1) {
        perror ("error on signaling start of operations");
        exit (EXIT_FAILURE);
    }

    /* waiting for the termination of the intervening entities processes */

    m = 0;
    do {
        info = wait (&status);
        if (info == -1)
        { perror ("error on waiting for an intervening process");
            exit (EXIT_FAILURE);
        }
        m += 1;
    } while (m < N+2);

    saveAirLiftResult(nFic,&sh->fSt);

    /* destruction of semaphore set and shared region */

    if (semDestroy (semgid) == -1) {
        perror ("error on destructing the semaphore set");
        exit (EXIT_FAILURE);
    }
    if (shmemDettach (sh) == -1) { 
        perror ("error on unmapping the shared region off the process address space");
        exit (EXIT_FAILURE);
    }
    if (shmemDestroy (shmid) == -1) { 
        perror ("error on destructing the shared region");
        exit (EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
