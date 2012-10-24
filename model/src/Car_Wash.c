/******************************************/
/* SIGMA RUN DEFAULTS FOR TRANSLATED MODEL */
/*******************************************/
/*

         MODEL DEFAULTS
         --------------

Model Name:         Car Wash
Model Description:  AN AUTOMATIC CARWASH
Output File:        untitled.out
Output Plot Style:  NOAUTO_FIT
Run Mode:           SINGLE_STEP
Trace Vars:         QUEUE,SERVERS
Random Number Seed: 12345
Initial Values:     5,3
Ending Condition:   STOP_ON_TIME
Ending Time:        500.000
Trace Events:       ALL EVENTS TRACED
Hide Edges:         

*/
/*******************************************/


#include "sigmafns.h"
#include "sigmalib.h"

/* EVENT FUNCTIONS USED IN THE SIMULATION MODEL */
void   RUN(void);	/*** THE SIMULATION RUN IS STARTED ***/
void   ENTER(void);	/*** CARS ENTER THE LINE ***/
void   START(void);	/*** SERVICE STARTS ***/
void   LEAVE(void);	/*** CARS LEAVE ***/

/*   STATE VARIABLE DELARATIONS   */
long   QUEUE;	/***  NUMBER OF CARS IN LINE  ***/
long   SERVERS;	/***  NUMBER OF AVAILABLE MACHINES  ***/

/*   EVENT FUNCTIONS   */
enum
   {
   run_end_event,
   RUN_event,
   ENTER_event,
   START_event,
   LEAVE_event,
   };

/*    MAIN PROGRAM     */
int main(int argc, char** argv)
{
  int  next_event;
  char keytoclose = 'p';

  if(!startup_check(0))
    return -1;

  /* Initialize csiglib and simulation */
  while (initialize(argc, (const char * *)argv)) {;

  /* Schedule beginning of simulation */
  event_time = current_time;
  event_type = RUN_event;
  schedule_event();

  /* Schedule end of simulation */
  event_time = stop_time;
  event_type = run_end_event;
  event_priority = 9999;
  schedule_event();

/*  EVENT EXECUTION CONTROL LOOP */
  while (!run_error && !done) {
    /* Pull next event from event list */
    next_event = c_timing();

    /* increment the event count for this event */
    event_count[next_event]++;

    /* Call appropriate event routine */
    switch ( next_event ) {
      case run_end_event:  run_end();
               break;

      case RUN_event:  RUN();
               event_trace("RUN",event_count[next_event]);
               break;

      case ENTER_event:  ENTER();
               event_trace("ENTER",event_count[next_event]);
               break;

      case START_event:  START();
               event_trace("START",event_count[next_event]);
               break;

      case LEAVE_event:  LEAVE();
               event_trace("LEAVE",event_count[next_event]);
               break;

      }
    }
  }
// experiments terminated
printf("Experiments ended! If runs end early: \n\r1. check fields in *.exp file. \n\r2. check if output file was already open. \n\r");
return 0;
}

void
event_trace(const char * name_of_event,const long count)
{
  c_timest(QUEUE, 1, 0);
  c_sampst(QUEUE, 1, 0);
  c_timest(SERVERS, 2, 0);
  c_sampst(SERVERS, 2, 0);
  if(trace_flag) fprintf(output_fp, "%9.3f\t %6s\t%6d ",current_time,name_of_event,count);
  if(trace_flag) fprintf(output_fp, "	%7.3g 	%7.3g \n"
,(double)QUEUE, (double)SERVERS);
}



int
initialize(int argc, const char** argv)
{
static int first_time = 1;     /* First time in initialize? */
static FILE *input_fp;     /* For reading from the input file */
char *exp_file_name;       /* For constructing input file name */
char y_n = 'p';            /* yes/no for file overwrite*/

       char dir[256];
       char fname[256];
       char ext[256];
       char simulation[1024];
       char experient_name[1024];
        _splitpath( argv[0], NULL, dir, fname, ext );
       strcpy(simulation, fname);
       strcat(simulation, ext);
       strcpy(experient_name, fname);
       strcat(experient_name, ".exp");
     printf("Running the simulation: %s\n", simulation);
    if(strlen(dir) !=0)
       printf("In Path: %s\n",dir);
  if (first_time) {
    exp_file_name = _strdup(argv[0]);
    exp_file_name[strlen(exp_file_name)-1] = 'p';
    printf("\nLooking for experiment file: %s\n",experient_name);
    }

  if ((first_time && (input_fp=fopen(exp_file_name,"r"))!=NULL)
                                           || input_fp!=NULL) {
  if (first_time) {
     first_time = 0; /* Reset for next time into initialize */
     printf("Found. Use [Control]-C to abort replications.\n");
     }

  /* We have run control file of type *.exp          */
  /* Read next set of data from run control file.    */
  if (fscanf(input_fp,"%s %1s %ld %lf %d", output_file_name, &y_n, &rndsd, &stop_time, &trace_flag)<4
     || fscanf(input_fp,"%ld", &QUEUE)<1
     || fscanf(input_fp,"%ld", &SERVERS)<1
     ) {
     /* End of run control file */
     fclose(input_fp);
     return 0;
     }

  if (y_n != 'y' && y_n != 'Y' && y_n != 'n' && y_n != 'N') { 
  fprintf(stderr,"INPUT ERROR: Invalid append file flag in *.exp file: (y=append, n=overwrite old file)\n"); 
  return 0; 
  }

  if (y_n == 'y' || y_n == 'Y') {
     if ((output_fp = fopen(output_file_name,"a"))==NULL) { 
     /* Output file can't be opened. */
     fprintf(stderr,"\nINPUT ERROR: Cannot open output file %s in *.exp file\n",argv[1]);
  return 0; 
  }
  }
if (y_n == 'n' || y_n == 'N') {
     if ((output_fp = fopen(output_file_name,"w"))==NULL) { 
     /* Output file can't be opened. */
     fprintf(stderr,"\nINPUT ERROR: Cannot open output file %s in *.exp file\n",argv[1]);
     return 0;
     }
     }

  if (rndsd < 1 || rndsd > 65534) {
     fprintf(stderr,"\nINPUT ERROR: Random seed %ld is not between 0 and 65534\n",rndsd);
     return 0;
     }

  if (stop_time <= 0.0) {
     fprintf(stderr,"\nINPUT ERROR: Stopping time %lf is negative!\n",stop_time);
     return 0;
     }

  if (trace_flag != 0 && trace_flag != 1) {
     fprintf(stderr,"\nINPUT ERROR: Invalid trace_flag=%d: (1=full trace, 0=summary only)\n",trace_flag);
     return 0;
     }

  done = 0;
     }

 else if (first_time) { /* And open failed, implies data from stdin */
    first_time = 0; /* Reset for next time into initialize */
    printf("Not found, input data at the keyboard.\n");
     /* Give output file name */
     while(y_n != 'y' && y_n != 'Y' && y_n != 'n' && y_n != 'N')
         {
         printf("\nOUTPUT FILE (Enter File Name with Path):\n");
         scanf("%s", output_file_name);
         fflush(stdin);
         sprintf(filename,"%.20s", output_file_name);
         printf("WARNING:File %.20s must not be open!!\n If file does not exist it will be created.\n",filename);
         printf("Do you want the new output appended to this file? (yes/[no])\n");
         scanf("%1s",&y_n);
         fflush(stdin);
         }
     if(y_n == 'y' || y_n == 'Y') output_fp = fopen(filename,"a");
     if(y_n == 'n' || y_n == 'N') output_fp = fopen(filename,"w");

     /* Read in random number seed */
     printf("\n\nRANDOM NUMBER SEED (Enter Integer Between 0 and 65534):\n");
     scanf("%ld", &rndsd);
     fflush(stdin);

     /* Read in run stopping time */
     printf("\nSTOPPING TIME (Enter number of time units until run termination):\n");
     scanf("%lf", &stop_time);
     fflush(stdin);

     /* Read in trace_flag */
     printf("\n\nTRACE (1 = Event Trace, 0 = Summary Only):\n");
     scanf("%d", &trace_flag);
     fflush(stdin);

     /* Parameters for the initial event */;
     printf ( "\nEnter initial value for QUEUE: \n");
     scanf  ( "%ld", &QUEUE);
     printf ( "\nEnter initial value for SERVERS: \n");
     scanf  ( "%ld", &SERVERS);
   }

 else {
    /* this is not the first time and there is no .exp file */
    return 0;
    }
  
   /* PLACE CUSTOMIZED INITIALIZATIONS HERE */

if (trace_flag)
   {
   fprintf(output_fp,"    Time\t  Event\t Count");
   fprintf(output_fp,"	           QUEUE");
   fprintf(output_fp,"	           SERVERS ");
   fprintf(output_fp,"\n");
   }
  /* Initialize CSIGLIB variables and files */
  c_initlk(rndsd);
  c_initfiles();

  return(1);
}



void
run_end()
{
  printf("\r\nNormal completion after %f time units\n",current_time);
  printf("The Next Seed In the Random Input Stream is %ld\n",rndsd);

  ///  Summary statistics ///
  fprintf(output_fp,"SUMMARY STATISTICS\n");
  printf("SUMMARY STATISTICS\n");
  c_timest(QUEUE, 1, 1);
   fprintf(output_fp, "QUEUE:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("QUEUE:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(QUEUE, 1, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(SERVERS, 2, 1);
   fprintf(output_fp, "SERVERS:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("SERVERS:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(SERVERS, 2, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  printf("Output written to, %s\n",output_file_name);
  fclose(output_fp);
  c_closedisk();
  done = 1;
}



/****************************/
/*     EVENT FUNCTIONS      */
/****************************/

/*** THE SIMULATION RUN IS STARTED ***/
void
RUN()
{
int  _edge_condition[1];

  /* Attribute Value(s) Passed to this Event */

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );

  /* schedule future events */
  if (_edge_condition[0])
    /***  THE CAR WILL ENTER THE LINE  ***/
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = ENTER_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** CARS ENTER THE LINE ***/
void
ENTER()
{
int  _edge_condition[2];


  /* state changes */
  QUEUE=QUEUE+1;

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );
  _edge_condition[1] = ( SERVERS>0 );

  /* schedule future events */
  if (_edge_condition[0])
    /***  THE NEXT CUSTOMER ENTERS IN 3 TO 8 MINUTES  ***/
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 3+5*RND;
    event_type = ENTER_event;
    event_priority = 6;
    schedule_event();
    }

  if (_edge_condition[1])
    /***  THERE ARE AVAILABLE SERVERS TO START WASHING THE CAR  ***/
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = START_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** SERVICE STARTS ***/
void
START()
{
int  _edge_condition[2];


  /* state changes */
  SERVERS=SERVERS-1;
  QUEUE=QUEUE-1;

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );

  /* schedule future events */
  if (_edge_condition[0])
    /***  THE CAR WILL BE IN SERVICE FOR AT LEAST 5 MINUTES  ***/
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 5+20*RND;
    event_type = LEAVE_event;
    event_priority = 6;
    schedule_event();
    }

}


/*** CARS LEAVE ***/
void
LEAVE()
{
int  _edge_condition[2];


  /* state changes */
  SERVERS=SERVERS+1;

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( QUEUE>0 );

  /* schedule future events */
  if (_edge_condition[0])
    /***  THERE ARE CARS IN QUEUE, START SERVICE FOR THE NEXT CAR IN LINE  ***/
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = START_event;
    event_priority = 5;
    schedule_event();
    }

}

