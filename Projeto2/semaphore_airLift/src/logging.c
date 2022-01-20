/**
 *  \file logging.c (implementation file)
 *
 *  \brief Problem name: Air Lift.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <unistd.h>


#include "probConst.h"
#include "probDataStruct.h"

static FILE *openLog(char nFic[], char mode[])
{
    FILE *fic;
    char *fName;                                                                                      /* log file name */

    if ((nFic == NULL) || (strlen (nFic) == 0)) {
        return stdout;
    }
    else fName = nFic;
    //fprintf(stderr,"%d opening log %s %s\n",getpid(),nFic,mode);
    if ((fic = fopen (fName, mode)) == NULL) {
        perror ("error on opening log file");
        exit (EXIT_FAILURE);
    }
    return fic;
}

static void closeLog(FILE *fic)
{
    if(fic==stderr || fic == stdout) {
         fflush(fic);
         return;
    }

    if (fclose (fic) == EOF) {
        perror ("error on closing of log file");
        exit (EXIT_FAILURE);
    }
}

static void printHeader(FILE *fic)
{
    fprintf(fic,"%3s","PT");
    fprintf(fic,"%3s","HT");
    fprintf(fic," ");
    int p;
    for(p=0; p < N; p++) {
        fprintf(fic," %s%02d","P",p);
    }

    fprintf(fic," ");
    fprintf(fic,"%4s","InQ");
    fprintf(fic,"%4s","InF");
    fprintf(fic,"%4s","toB");

    fprintf(fic,"\n");
}

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

void createLog (char nFic[])
{
    FILE *fic;                                                                                      /* file descriptor */

    fic = openLog(nFic,"w");

    /* title line + blank line */

    fprintf (fic, "%31cAir Lift - Description of the internal state\n\n", ' ');
    printHeader(fic);

    closeLog(fic);
}

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

void saveState (char nFic[], FULL_STAT *p_fSt)
{
    FILE *fic;                                                                                      /* file descriptor */

    fic = openLog(nFic,"a");

    fprintf(fic,"%3d",p_fSt->st.pilotStat);
    fprintf(fic,"%3d",p_fSt->st.hostessStat);
    fprintf(fic," ");
    int p;
    for(p=0; p < N; p++) {
        fprintf(fic,"%4d",p_fSt->st.passengerStat[p]);
    }

    fprintf(fic," ");
    fprintf(fic,"%4d",p_fSt->nPassInQueue);
    fprintf(fic,"%4d",p_fSt->nPassInFlight);
    fprintf(fic,"%4d",p_fSt->totalPassBoarded);

    fprintf(fic,"\n");

    closeLog(fic);
}
/**
 *  \brief Writing the start of Boarding Process and header.
 *
 *  If <tt>nFic</tt> is a null pointer or a null string, the lines are written to stdout
 *
 *  \param nFic name of the logging file
 *  \param p_fSt pointer to the location where the full internal state of the problem is stored
 */

void saveStartBoarding (char nFic[], FULL_STAT *p_fSt)
{
    FILE *fic;                                                                                      /* file descriptor */

    fic = openLog(nFic,"a");

    fprintf(fic,"Flight %d : Boarding Started\n", p_fSt->nFlight);
    printHeader(fic);


    closeLog(fic);
}

/**
 *  \brief Writing the start of Boarding Process and header.
 *
 *  If <tt>nFic</tt> is a null pointer or a null string, the lines are written to stdout
 *
 *  \param nFic name of the logging file
 *  \param p_fSt pointer to the location where the full internal state of the problem is stored
 */

void savePassengerChecked (char nFic[], FULL_STAT *p_fSt)
{
    FILE *fic;                                                                                      /* file descriptor */

    fic = openLog(nFic,"a");

    fprintf(fic,"Flight %d : Passenger %d checked\n", p_fSt->nFlight, p_fSt->passengerChecked);

    closeLog(fic);
}

/**
 *  \brief Writing the start of flight at end of the file.
 *
 *  If <tt>nFic</tt> is a null pointer or a null string, the lines are written to stdout
 *
 *  \param nFic name of the logging file
 *  \param p_fSt pointer to the location where the full internal state of the problem is stored
 */

void saveFlightDeparted (char nFic[], FULL_STAT *p_fSt)
{
    FILE *fic;                                                                                      /* file descriptor */

    fic = openLog(nFic,"a");

    fprintf(fic,"Flight %d : Departed with %d passengers\n", p_fSt->nFlight, p_fSt->nPassengersInFlight[p_fSt->nFlight-1]);
    printHeader(fic);

    closeLog(fic);
}


/**
 *  \brief Writing the flight arrival at the end of the file.
 *
 *  If <tt>nFic</tt> is a null pointer or a null string, the lines are written to stdout 
 *
 *  \param nFic name of the logging file
 *  \param p_fSt pointer to the location where the full internal state of the problem is stored
 */

void saveFlightArrived (char nFic[], FULL_STAT *p_fSt)
{
    FILE *fic;                                                                                      /* file descriptor */

    fic = openLog(nFic,"a");

    fprintf(fic,"Flight %d : Arrived \n", p_fSt->nFlight);
    printHeader(fic);

    closeLog(fic);
}

/**
 *  \brief Writing the flight returning at end of file.
 *
 *  If <tt>nFic</tt> is a null pointer or a null string, the lines are written to stdout 
 *
 *  \param nFic name of the logging file
 *  \param p_fSt pointer to the location where the full internal state of the problem is stored
 */

void saveFlightReturning (char nFic[], FULL_STAT *p_fSt)
{
    FILE *fic;                                                                                      /* file descriptor */

    fic = openLog(nFic,"a");

    fprintf(fic,"Flight %d : Returning \n", p_fSt->nFlight);
    printHeader(fic);

    closeLog(fic);
}

/**
 *  \brief Writing summary of air lift at the end of the file.
 *
 *  If <tt>nFic</tt> is a null pointer or a null string, the lines are written to stdout 
 *
 *  \param nFic name of the logging file
 *  \param p_fSt pointer to the location where the full internal state of the problem is stored
 */

void saveAirLiftResult (char nFic[], FULL_STAT *p_fSt)
{
    FILE *fic;                                                                                      /* file descriptor */
    fic = openLog(nFic,"a");

    fprintf(fic,"AirLift result\n");

    int f;
    fprintf(fic,"AirLift used %d Flights\n", p_fSt->nFlight);
    for(f=0; f<p_fSt->nFlight; f++) {
        fprintf(fic,"Flight %d took %2d passengers\n", f+1, p_fSt->nPassengersInFlight[f]);
    }

    closeLog(fic);
}
