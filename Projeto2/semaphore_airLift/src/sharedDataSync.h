/**
 *  \file sharedDataSync.h (interface file)
 *
 *  \brief Problem name: Air Lift.
 *
 *  Synchronization based on semaphores and shared memory.
 *  Implementation with SVIPC.
 *
 *  \brief Definition of the shared data and the synchronization devices.
 *
 *  Both the format of the shared data, which represents the full state of the problem, and the identification of
 *  the different semaphores, which carry out the synchronization among the intervening entities, are provided.
 *
 *  \author Nuno Lau - January 2022
 */

#ifndef SHAREDDATASYNC_H_
#define SHAREDDATASYNC_H_

#include "probConst.h"
#include "probDataStruct.h"

/**
 *  \brief Definition of <em>shared information</em> data type.
 */
typedef struct
        { /** \brief full state of the problem */
          FULL_STAT fSt;

          /* semaphores ids */
          /** \brief identification of critical region protection semaphore – val = 1 */
          unsigned int mutex;
          /** \brief identification of semaphore used by hostess to wait for passengers - val = 0 */
          unsigned int passengersInQueue;
          /** \brief identification of semaphore used by passengers to wait for hostess – val = 0 */
          unsigned int passengersWaitInQueue;
          /** \brief identification of semaphore used by passengers to wait for flight to end – val = 0 */
          unsigned int passengersWaitInFlight;
          /** \brief identification of semaphore used by hostess to wait for starting boarding – val = 0  */
          unsigned int readyForBoarding;
          /** \brief identification of semaphore used by pilot to wait for boarding to complete - val = 0 */
          unsigned int readyToFlight;
          /** \brief identification of semaphore used by hostess to wait for passenger identification - val = 0 */
          unsigned int idShown;
          /** \brief identification of semaphore used by pilot to wait for last passenger to leave plane - val = 0 */
          unsigned int planeEmpty;

        } SHARED_DATA;

/** \brief number of semaphores in the set */
#define SEM_NU                    (8)

#define MUTEX                      1
#define PASSENGERSINQUEUE          2
#define PASSENGERSWAITINQUEUE      3
#define PASSENGERSWAITINFLIGHT     4
#define READYFORBOARDING           5
#define READYTOFLIGHT              6
#define IDSHOWN                    7
#define PLANEEMPTY                 8

#endif /* SHAREDDATASYNC_H_ */
