/******************************************/
/* SIGMA RUN DEFAULTS FOR TRANSLATED MODEL */
/*******************************************/
/*

         MODEL DEFAULTS
         --------------

Model Name:         Model_Enhanced FC
Model Description:  
Output File:        untitled.out
Output Plot Style:  NOAUTO_FIT
Run Mode:           HI_SPEED
Trace Vars:         SERVER[0],SERVER[1],QUEUE[0],QUEUE[1],MISSFL,MISSFLFC
Random Number Seed: 12345
Initial Values:     0.1,20,20,20,20,20,20,20,20,20,20,20,20,
Ending Condition:   STOP_ON_TIME
Ending Time:        1440.000
Trace Events:       ALL EVENTS TRACED
Hide Edges:         

*/
/*******************************************/


#include "sigmafns.h"
#include "sigmalib.h"

/* EVENT FUNCTIONS USED IN THE SIMULATION MODEL */
void   RUN(void);	/*** Initialize the Run ***/
void   ARRIVE(void);	/*** Customers Arrive. The are first class customers with certain probability ,PERCFC. They enter the queue and the transient attributes are tracked ***/
void   STFCFC(void);	/*** First class servers start service for first class passengers ***/
void   FINFCFC(void);	/*** First class servers finish service for economy passengers ***/
void   STFCE(void);	/*** First class servers start service for economy passengers ***/
void   FINFCE(void);	/*** First class servers finish service for economy passengers ***/
void   STEE(void);	/*** Economy servers start service for economy passengers ***/
void   FINEE(void);	/*** Economy servers finish service for economy passengers ***/
void   SETSRVR(void);	/*** Set the number of expected economy servers in the system at the beginning of a shift. ***/
void   ADDECON(void);	/*** Add a server when their shift starts. ***/
void   ADDFC(void);	/*** Add a first class server when their shift starts ***/
void   ASSIGN(void);	/*** A first class server chooses to help a first class or economy passengers. ***/

/*   STATE VARIABLE DELARATIONS   */
double PERCFC;	/***  Percent of customers who are first class  ***/
long   SERVER[2];	/***  Number of free servers first (1) and economy (0)  ***/
double R;	/***  Random number generated to determine first class  ***/
long   TRACK;	/***  binary varaible. 1 for first 0 for econ  ***/
long   QUEUE[2];	/***  Queue size for first (1) or economy (0)  ***/
long   ECONBASE;	/***  Baseline time for Economy servers  ***/
long   ECONBAG;	/***  Incremental time for Economy passengers with bag  ***/
long   ECONTIX;	/***  Incremental time for Economy passengers with Tix  ***/
long   FCBASE;	/***  Baseline service time of FC servers  ***/
long   FCBAG;	/***  Incremental time for FC passengers with bags  ***/
long   FCTIX;	/***  Incremental time for FC passengers with tickets  ***/
long   DEPTIME;	/***  Departure Time of Flight  ***/
long   MISSFL;	/***  Count of Missed flights for Economy passengers  ***/
long   FCGS;	/***  Number of first class servers in entire system.  ***/
long   ECONGS;	/***  Number of economy servers in the entire system.  ***/
long   FCES[6];	/***  Number of first class servers in shifts of 4 hrs  ***/
long   ECONES[6];	/***  Number of economy servers in shifts of 4 hrs  ***/
long   ECONEX;	/***  Exp. number of economy servers for shift change  ***/
long   FCEX;	/***  Exp. number of FC servers during shift change  ***/
long   TIMEIDX;	/***  Index of the shift that we are in.  ***/
double DECSERV;	/***  Boolean to remove a server  ***/
long   MISSFLFC;	/***  Count of missed flgihts for first class customer  ***/
long   PASCOUNT;	/***    ***/
long   PASTOTAL;	/***    ***/

/*   EVENT FUNCTIONS   */
enum
   {
   run_end_event,
   RUN_event,
   ARRIVE_event,
   STFCFC_event,
   FINFCFC_event,
   STFCE_event,
   FINFCE_event,
   STEE_event,
   FINEE_event,
   SETSRVR_event,
   ADDECON_event,
   ADDFC_event,
   ASSIGN_event,
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

      case ARRIVE_event:  ARRIVE();
               event_trace("ARRIVE",event_count[next_event]);
               break;

      case STFCFC_event:  STFCFC();
               event_trace("STFCFC",event_count[next_event]);
               break;

      case FINFCFC_event:  FINFCFC();
               event_trace("FINFCFC",event_count[next_event]);
               break;

      case STFCE_event:  STFCE();
               event_trace("STFCE",event_count[next_event]);
               break;

      case FINFCE_event:  FINFCE();
               event_trace("FINFCE",event_count[next_event]);
               break;

      case STEE_event:  STEE();
               event_trace("STEE",event_count[next_event]);
               break;

      case FINEE_event:  FINEE();
               event_trace("FINEE",event_count[next_event]);
               break;

      case SETSRVR_event:  SETSRVR();
               event_trace("SETSRVR",event_count[next_event]);
               break;

      case ADDECON_event:  ADDECON();
               event_trace("ADDECON",event_count[next_event]);
               break;

      case ADDFC_event:  ADDFC();
               event_trace("ADDFC",event_count[next_event]);
               break;

      case ASSIGN_event:  ASSIGN();
               event_trace("ASSIGN",event_count[next_event]);
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
  c_timest(SERVER[0], 1, 0);
  c_sampst(SERVER[0], 1, 0);
  c_timest(SERVER[1], 2, 0);
  c_sampst(SERVER[1], 2, 0);
  c_timest(QUEUE[0], 3, 0);
  c_sampst(QUEUE[0], 3, 0);
  c_timest(QUEUE[1], 4, 0);
  c_sampst(QUEUE[1], 4, 0);
  c_timest(MISSFL, 5, 0);
  c_sampst(MISSFL, 5, 0);
  c_timest(MISSFLFC, 6, 0);
  c_sampst(MISSFLFC, 6, 0);
  if(trace_flag) fprintf(output_fp, "%9.3f\t %6s\t%6d ",current_time,name_of_event,count);
  if(trace_flag) fprintf(output_fp, "	%7.3g 	%7.3g 	%7.3g 	%7.3g 	%7.3g 	%7.3g \n"
,(double)SERVER[0], (double)SERVER[1], (double)QUEUE[0], (double)QUEUE[1], (double)MISSFL, (double)MISSFLFC);
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
     || fscanf(input_fp,"%lf", &PERCFC)<1
     || fscanf(input_fp,"%ld", &ECONES[0])<1
     || fscanf(input_fp,"%ld", &ECONES[1])<1
     || fscanf(input_fp,"%ld", &ECONES[2])<1
     || fscanf(input_fp,"%ld", &ECONES[3])<1
     || fscanf(input_fp,"%ld", &ECONES[4])<1
     || fscanf(input_fp,"%ld", &ECONES[5])<1
     || fscanf(input_fp,"%ld", &FCES[0])<1
     || fscanf(input_fp,"%ld", &FCES[1])<1
     || fscanf(input_fp,"%ld", &FCES[2])<1
     || fscanf(input_fp,"%ld", &FCES[3])<1
     || fscanf(input_fp,"%ld", &FCES[4])<1
     || fscanf(input_fp,"%ld", &FCES[5])<1
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
     printf ( "\nEnter initial value for PERCFC: \n");
     scanf  ( "%lf", &PERCFC);
     printf ( "\nEnter initial value for ECONES[0]: \n");
     scanf  ( "%ld", &ECONES[0]);
     printf ( "\nEnter initial value for ECONES[1]: \n");
     scanf  ( "%ld", &ECONES[1]);
     printf ( "\nEnter initial value for ECONES[2]: \n");
     scanf  ( "%ld", &ECONES[2]);
     printf ( "\nEnter initial value for ECONES[3]: \n");
     scanf  ( "%ld", &ECONES[3]);
     printf ( "\nEnter initial value for ECONES[4]: \n");
     scanf  ( "%ld", &ECONES[4]);
     printf ( "\nEnter initial value for ECONES[5]: \n");
     scanf  ( "%ld", &ECONES[5]);
     printf ( "\nEnter initial value for FCES[0]: \n");
     scanf  ( "%ld", &FCES[0]);
     printf ( "\nEnter initial value for FCES[1]: \n");
     scanf  ( "%ld", &FCES[1]);
     printf ( "\nEnter initial value for FCES[2]: \n");
     scanf  ( "%ld", &FCES[2]);
     printf ( "\nEnter initial value for FCES[3]: \n");
     scanf  ( "%ld", &FCES[3]);
     printf ( "\nEnter initial value for FCES[4]: \n");
     scanf  ( "%ld", &FCES[4]);
     printf ( "\nEnter initial value for FCES[5]: \n");
     scanf  ( "%ld", &FCES[5]);
   }

 else {
    /* this is not the first time and there is no .exp file */
    return 0;
    }
  
   /* PLACE CUSTOMIZED INITIALIZATIONS HERE */

if (trace_flag)
   {
   fprintf(output_fp,"    Time\t  Event\t Count");
   fprintf(output_fp,"	       SERVER[0]");
   fprintf(output_fp,"	       SERVER[1]");
   fprintf(output_fp,"	        QUEUE[0]");
   fprintf(output_fp,"	        QUEUE[1]");
   fprintf(output_fp,"	          MISSFL");
   fprintf(output_fp,"	          MISSFLFC ");
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
  c_timest(SERVER[0], 1, 1);
   fprintf(output_fp, "SERVER[0]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("SERVER[0]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(SERVER[0], 1, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(SERVER[1], 2, 1);
   fprintf(output_fp, "SERVER[1]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("SERVER[1]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(SERVER[1], 2, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(QUEUE[0], 3, 1);
   fprintf(output_fp, "QUEUE[0]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("QUEUE[0]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(QUEUE[0], 3, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(QUEUE[1], 4, 1);
   fprintf(output_fp, "QUEUE[1]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("QUEUE[1]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(QUEUE[1], 4, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(MISSFL, 5, 1);
   fprintf(output_fp, "MISSFL:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("MISSFL:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(MISSFL, 5, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(MISSFLFC, 6, 1);
   fprintf(output_fp, "MISSFLFC:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("MISSFLFC:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(MISSFLFC, 6, 1);
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

/*** Initialize the Run ***/
void
RUN()
{
int  _edge_condition[2];

  /* Attribute Value(s) Passed to this Event */

  /* state changes */
  ECONBASE=1;
  ECONBAG=3;
  ECONTIX=2;
  FCBASE=1;
  FCBAG=2;
  FCTIX=1;
  SERVER[0]=ECONES[0];
  SERVER[1]=FCES[0];
  ECONGS=ECONES[0];
  FCGS=FCES[0];
  TIMEIDX=0;
  PASTOTAL=DISK(DATA.DAT,0);
  PASCOUNT=0;

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );
  _edge_condition[1] = ( 1==1 );

  /* schedule future events */
  if (_edge_condition[0])
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = ARRIVE_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[1])
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = SETSRVR_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** Customers Arrive. The are first class customers with certain probability ,PERCFC. They enter the queue and the transient attributes are tracked ***/
void
ARRIVE()
{
int  _edge_condition[4];


  /* state changes */
  R=RND;
  TRACK=(R<PERCFC);
  ENT[0]=DISK(DATA.DAT,0);
  ENT[1]=DISK(DATA.DAT,0);
  ENT[2]=DISK(DATA.DAT,0);
  ENT[3]=DISK(DATA.DAT,0);
  ENT[4]=DISK(DATA.DAT,0);
  QUEUE[TRACK]=QUEUE[TRACK]+PUT(FIF,TRACK);
  PASCOUNT=PASCOUNT+1;

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( PASCOUNT<PASTOTAL );
  _edge_condition[1] = ( QUEUE[0]>0&&SERVER[0]>0 );
  _edge_condition[2] = ( QUEUE[1]>0&&SERVER[1]>0 );
  _edge_condition[3] = ( QUEUE[0]==1&&QUEUE[1]==0&&SERVER[0]==0&&SERVER[1]>0 );

  /* schedule future events */
  if (_edge_condition[0])
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + ENT[1]-CLK;
    event_type = ARRIVE_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[1])
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = STEE_event;
    event_priority = 2;
    schedule_event();
    }

  if (_edge_condition[2])
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = STFCFC_event;
    event_priority = 1;
    schedule_event();
    }

  if (_edge_condition[3])
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = STFCE_event;
    event_priority = 3;
    schedule_event();
    }

}


/*** First class servers start service for first class passengers ***/
void
STFCFC()
{
int  _edge_condition[1];


  /* state changes */
  QUEUE[1]=QUEUE[1]-GET(FST,1);
  SERVER[1]=SERVER[1]-1;
  DEPTIME=ENT[4];

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );

  /* schedule future events */
  if (_edge_condition[0])
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = DEPTIME;
    for ( t_index=4; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + FCBASE+FCBAG*RND*ENT[2]+FCTIX*RND*ENT[3];
    event_type = FINFCFC_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** First class servers finish service for economy passengers ***/
void
FINFCFC()
{
int  _edge_condition[1];

  /* Attribute Value(s) Passed to this Event */
  DEPTIME = (long) transfer[3];

  /* state changes */
  SERVER[1]=SERVER[1]+(FCGS<=FCEX);
  MISSFLFC=MISSFLFC+(CLK>(DEPTIME-30));
  DECSERV=(FCGS>FCEX);
  FCGS=FCGS-(FCGS>FCEX);

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( DECSERV==0 );

  /* schedule future events */
  if (_edge_condition[0])
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = ASSIGN_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** First class servers start service for economy passengers ***/
void
STFCE()
{
int  _edge_condition[1];


  /* state changes */
  QUEUE[0]=QUEUE[0]-GET(FST,0);
  SERVER[1]=SERVER[1]-1;
  DEPTIME=ENT[4];

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );

  /* schedule future events */
  if (_edge_condition[0])
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = DEPTIME;
    for ( t_index=4; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + ECONBASE+FCBAG*RND*ENT[2]+FCTIX*RND*ENT[3];
    event_type = FINFCE_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** First class servers finish service for economy passengers ***/
void
FINFCE()
{
int  _edge_condition[1];

  /* Attribute Value(s) Passed to this Event */
  DEPTIME = (long) transfer[3];

  /* state changes */
  SERVER[1]=SERVER[1]+(FCGS<=FCEX);
  MISSFL=MISSFL+(CLK>(DEPTIME-30));
  DECSERV=(FCGS>FCEX);
  FCGS=FCGS-(FCGS>FCEX);

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( DECSERV==0 );

  /* schedule future events */
  if (_edge_condition[0])
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = ASSIGN_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** Economy servers start service for economy passengers ***/
void
STEE()
{
int  _edge_condition[2];


  /* state changes */
  QUEUE[0]=QUEUE[0]-GET(FST,0);
  SERVER[0]=SERVER[0]-1;
  DEPTIME=ENT[4];
  DECSERV=0;

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );

  /* schedule future events */
  if (_edge_condition[0])
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = DEPTIME;
    for ( t_index=4; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + ECONBASE+ECONBAG*RND*ENT[2]+ECONTIX*RND*ENT[3];
    event_type = FINEE_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** Economy servers finish service for economy passengers ***/
void
FINEE()
{
int  _edge_condition[2];

  /* Attribute Value(s) Passed to this Event */
  DEPTIME = (long) transfer[3];

  /* state changes */
  SERVER[0]=SERVER[0]+(ECONGS<=ECONEX);
  MISSFL=MISSFL+(CLK>(DEPTIME-30));
  DECSERV=(ECONGS>ECONEX);
  ECONGS=ECONGS-(ECONGS>ECONEX);

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( QUEUE[0]>0&&DECSERV==0 );

  /* schedule future events */
  if (_edge_condition[0])
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = STEE_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** Set the number of expected economy servers in the system at the beginning of a shift. ***/
void
SETSRVR()
{
int  _edge_condition[3];


  /* state changes */
  ECONEX=ECONES[TIMEIDX];
  FCEX=FCES[TIMEIDX];
  TIMEIDX=TIMEIDX+1;

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( TIMEIDX<6 );
  _edge_condition[1] = ( ECONGS<ECONEX );
  _edge_condition[2] = ( FCGS<FCEX );

  /* schedule future events */
  if (_edge_condition[0])
    /***  Every four hours, update the number of servers in the system.  ***/
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 240;
    event_type = SETSRVR_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[1])
    /***  Add extra servers to economy staff if needed  ***/
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = ADDECON_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[2])
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = ADDFC_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** Add a server when their shift starts. ***/
void
ADDECON()
{
int  _edge_condition[2];


  /* state changes */
  ECONGS=ECONGS+1;
  SERVER[0]=SERVER[0]+1;

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( ECONGS<ECONEX );
  _edge_condition[1] = ( QUEUE[0]>0 );

  /* schedule future events */
  if (_edge_condition[0])
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = ADDECON_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[1])
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = STEE_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** Add a first class server when their shift starts ***/
void
ADDFC()
{
int  _edge_condition[2];


  /* state changes */
  FCGS=FCGS+1;
  SERVER[1]=SERVER[1]+1;

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );
  _edge_condition[1] = ( FCGS<FCEX );

  /* schedule future events */
  if (_edge_condition[0])
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = ASSIGN_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[1])
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = ADDFC_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** A first class server chooses to help a first class or economy passengers. ***/
void
ASSIGN()
{
int  _edge_condition[2];


  /* state changes */
  DECSERV=0;

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( QUEUE[0]>0&&QUEUE[1]==0&&SERVER[0]==0&&SERVER[1]>0 );
  _edge_condition[1] = ( QUEUE[1]>0 );

  /* schedule future events */
  if (_edge_condition[0])
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = STFCE_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[1])
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = STFCFC_event;
    event_priority = 5;
    schedule_event();
    }

}

