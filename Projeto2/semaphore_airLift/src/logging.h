/**
 *  \file logging.h (interface file)
 *
 *  \brief Problem name: Air Lift
 *
 *  \brief Logging the internal state of the problem into a file.
 *
 *  Defined operations:
 *     \li file initialization
 *     \li writing the start of boarding at the end of the file
 *     \li writing the start of flight at end of the file.
 *     \li writing the present full state as a single line at the end of the file.
 *     \li Writing the flight arrival at the end of the file.
 *     \li Writing the flight returning at the end of the file.
 *     \li writing summary of air lift at the end of the file.
 *
 *  \author Nuno Lau - January 2022
 */

#ifndef LOGGING_H_
#define LOGGING_H_

#include "probDataStruct.h"

/**
 *  \brief File initialization.
 *
 *  The function creates the logging file and writes its header.
 *  If <tt>nFic</tt> is a null pointer or a null string, the file is created under a predefined name <em>log</em>.
 *
 *  The file header consists of
 *       \li a title line
 *       \li a blank line.
 *
 *  \param nFic name of the logging file
 */

extern void createLog (char nFic[]);

/**
 *  \brief Writing the start of Boarding Process and header.
 *
 *  If <tt>nFic</tt> is a null pointer or a null string, the lines are written to stdout
 *
 *  \param nFic name of the logging file
 *  \param p_fSt pointer to the location where the full internal state of the problem is stored
 */

extern void saveStartBoarding (char nFic[] , FULL_STAT *p_fSt);

/**
 *  \brief Writing the start of flight at end of the file.
 *
 *  If <tt>nFic</tt> is a null pointer or a null string, the lines are written to stdout
 *
 *  \param nFic name of the logging file
 *  \param p_fSt pointer to the location where the full internal state of the problem is stored
 */

extern void saveFlightDeparted (char nFic[], FULL_STAT *p_fSt);

/**
 *  \brief Writing the present full state as a single line at the end of the file.
 *
 *  If <tt>nFic</tt> is a null pointer or a null string, the lines written to stdout
 *
 *  The following layout is obeyed for the full state in a single line
 *    \li pilot state
 *    \li hostess state 
 *    \li passengers state 
 *    \li number of passengers waiting and flying
 *
 *  \param nFic name of the logging file
 *  \param p_fSt pointer to the location where the full internal state of the problem is stored
 */

extern void saveState (char nFic[], FULL_STAT *p_fSt);

/**
 *  \brief Writing the flight arrival at the end of the file.
 *
 *  If <tt>nFic</tt> is a null pointer or a null string, the lines are written to stdout 
 *
 *  \param nFic name of the logging file
 *  \param p_fSt pointer to the location where the full internal state of the problem is stored
 */

extern void saveFlightArrived (char nFic[], FULL_STAT *p_fSt);

/**
 *  \brief Writing the flight returning at the end of the file.
 *
 *  If <tt>nFic</tt> is a null pointer or a null string, the lines are written to stdout 
 *
 *  \param nFic name of the logging file
 *  \param p_fSt pointer to the location where the full internal state of the problem is stored
 */

extern void saveFlightReturning (char nFic[], FULL_STAT *p_fSt);

/**
 *  \brief Writing the start of Boarding Process and header.
 *
 *  If <tt>nFic</tt> is a null pointer or a null string, the lines are written to stdout
 *
 *  \param nFic name of the logging file
 *  \param p_fSt pointer to the location where the full internal state of the problem is stored
 */

void savePassengerChecked (char nFic[], FULL_STAT *p_fSt);

/**
 *  \brief Writing summary of air lift at the end of the file.
 *
 *  If <tt>nFic</tt> is a null pointer or a null string, the lines are written to stdout 
 *
 *  \param nFic name of the logging file
 *  \param p_fSt pointer to the location where the full internal state of the problem is stored
 */

extern void saveAirLiftResult (char nFic[], FULL_STAT *p_fSt);

#endif /* LOGGING_H_ */
