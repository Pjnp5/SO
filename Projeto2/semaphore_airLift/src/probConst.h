/**
 *  \file probConst.h (interface file)
 *
 *  \brief Problem name: Air Lift.
 *
 *  Problem simulation parameters.
 *
 *  \author Nuno Lau - January 2022
 */

#ifndef PROBCONST_H_
#define PROBCONST_H_

/* Generic parameters */

/** \brief number of passengers */
#define  N        21 

/** \brief min flight capacity */
#define  MINFC     5 

/** \brief max flight capacity */
#define  MAXFC    10

/** \brief max flight capacity */
#define  MAXNF    10

/** \brief max flight capacity */
#define  MAXTRAVEL   30000.0 

/** \brief max flight capacity */
#define  MAXFLIGHT   2000.0 

/* Pilot state constants */

/** \brief pilot flying to starting airport */
#define  FLYING_BACK                  0
/** \brief pilot signals ready for boarding */
#define  READY_FOR_BOARDING           1
/** \brief pilot wait for boarding to complete */
#define  WAITING_FOR_BOARDING         2
/** \brief pilot takes passengers to destination */
#define  FLYING                       3
/** \brief pilot drops passengers at destination */
#define  DROPING_PASSENGERS           4

/* Hostess state constants */

/** \brief hostess waits for plane to be ready for boarding */
#define  WAIT_FOR_FLIGHT              0
/** \brief hostess waits for passenger to arrive */
#define  WAIT_FOR_PASSENGER           1
/** \brief hostess checks passenger passport */
#define  CHECK_PASSPORT               2
/** \brief hostess signals boarding is complete */
#define  READY_TO_FLIGHT              3

/* Passenger state constants */

/** \brief passenger is going to the airport */
#define  GOING_TO_AIRPORT             0
/** \brief passenger is waiting in queue */
#define  IN_QUEUE                     1
/** \brief passenger is flying */
#define  IN_FLIGHT                    2
/** \brief passenger arrives at destination */
#define  AT_DESTINATION               3

#endif /* PROBCONST_H_ */
