/**
 *  \file probDataStruct.h (interface file)
 *
 *  \brief Problem name: Air Lift
 *
 *  Definition of internal data structures.
 *
 *  They specify internal metadata about the status of the intervening entities.
 *
 *  \author Nuno Lau - January 2022
 */

#ifndef PROBDATASTRUCT_H_
#define PROBDATASTRUCT_H_

#include <stdbool.h>

#include "probConst.h"


/**
 *  \brief Definition of <em>state of the intervening entities</em> data type.
 */
typedef struct
{ /** \brief pilot state */
    unsigned int pilotStat;
    /** \brief hostess state */
    unsigned int hostessStat;
    /** \brief passengers state array */
    unsigned int passengerStat[N];

} STAT;


/**
 *  \brief Definition of <em>full state of the problem</em> data type.
 */
typedef struct
{ /** \brief state of all intervening entities */
    STAT st;
    /** \brief number of passengers at each flight */
    unsigned int nPassengersInFlight[MAXNF];
    /** \brief flight number */
    unsigned int nFlight;

    /** \brief number of passengers waiting */
    unsigned int nPassInQueue;
    /** \brief number of passengers flying */
    unsigned int nPassInFlight;
    /** \brief total number of passengers already boarded in every flight */
    unsigned int totalPassBoarded;
    /** \brief air lift finished */
    bool finished;
    /** \brief passenger id of last passenger to check passport */
    int passengerChecked;

} FULL_STAT;


#endif /* PROBDATASTRUCT_H_ */
