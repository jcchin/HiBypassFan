//******************************************************************************
//*   © Copyright 2003.  The U.S. Government, as Represented by the Administrator
//*     of the National Aeronautics and Space Administration (NASA).
//*   © Copyright 2008 - 2013.     The Ohio Aerospace Institute, on behalf of the NPSS(TM)
//*     Consortium.  All rights reserved.  Includes content licensed from the 
//*     U.S. Government, National Aeronautics and Space Administration under 
//*     United States Copyright Registration Numbers  V3503D364 and V3482D344.
//*******************************************************************************

//******************************************************************************
//*  NPSSTM software and related documentation is export controlled
//*  with an Export Control Classification Number(ECCN) of 9D991, controlled for
//*  Anti-Terrorism reasons, under U.S. Export Administration Regulations 15 CFR
//*  730-774. It may not be transferred to a country checked under anti-terrorism
//*  on the Commerce Country Chart structure or to foreign nationals of those
//*  countries in the U.S. or abroad without first obtaining a license from the
//*  Bureau of Industry and Security, United States Department of Commerce. 
//*  Violations are punishable by fine, imprisonment, or both.
//*******************************************************************************

//
//  ***********************************************
//  ***********************************************
//  **                                           **
//  **  Macros/Functions needed are defined now  **
//  **                                           **
//  ***********************************************
//  ***********************************************
//



//
// ****
// **** RunPoint
// ****
void RunPoint(){

 solver.debugLevel = "ITERATION_DETAILS";
 solver.diagnosticFile = "output/solver.DR1";
//  Routine to run the model and update TEST and viewers
 TEST = TEST + 10000;
 run();
 
}


//
// ****
// **** SetupSolver routines
// ****
// **** SetupSolverPredicted  - Setup solver for DRMode = Predicted
// **** SetupSolverAsMeasured - Setup solver for DRMode = AsMeasured
// **** SetupSolverCorrected  - Setup solver for DRMode = Corrected
// **** SetupSolverNames      - Setup solver independent/dependent names
// ****
void SetupSolverPredicted(){

//  Routine to setup Predicted mode
//  This routine will probably be unique to an engine configuration

 solver.clear();
 setOption( "switchDes", "OFFDESIGN" );       // set Model mode to off-design
  //setOption( "BRN36.switchBurn", "FAR" ) ;

  setOption( "DRMode", "Predicted" );
  autoSolverSetup();

}

void SetupSolverAsMeasured(){

//  Routine to setup AsMeasured mode
//  This routine will probably be unique to an engine configuration

 solver.clear();
 setOption( "switchDes", "OFFDESIGN" );       // set Model mode to off-design
 setOption( "DRMode", "AsMeasured" );       
  autoSolverSetup();
  //solver.debugLevel = "ITERATION_DETAILS";
}

void SetupSolverCorrected(){

//  Routine to setup Corrected mode
//  This routine will probably be unique to an engine configuration

 solver.clear();
 setOption( "switchDes", "OFFDESIGN" );       // set Model mode to off-design


  setOption( "DRMode", "Corrected" );       
  autoSolverSetup();
}


//
// ****
// **** ValidData
// ****
int ValidData(string Mode){

//  Routine to check for valid data
//  This routine will probably be unique to an engine configuration

// For now, this routine returns TRUE if data setup is OK,
// and FALSE if point shouldn't run.

 string Predicted="Predicted";
 string AsMeasured="AsMeasured";


// These valid checks are performed independent of DRMode ...
 if ( ! (N25SE.valid * TAMBSE.valid * PAMBSE.valid * P25SE.valid * T25SE.valid) ) {
      return FALSE; // point can't run
   }

// Additional valid checks for DRMode = Predicted
 if (Mode == Predicted){

      ;  // No additional valid checks for now
 }


// Additional valid checks for DRMode = AsMeasured
 if (Mode == AsMeasured){
    if ( ! WF36SE.valid)  { return FALSE; } // point can't run
 }

// No problems encountered, so return TRUE
 return TRUE;
 
}

