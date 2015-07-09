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

// This file is loaded by the fanjet.run file (and by fanjet.test, for regression testing
// purposes)

// This file is used to run the fanjet model at a design point, then several off-design 
// points in a row at varying PLACS values, then a transient run.
// If the thermopackage is GasTbl, then this file will also perform a data reduction run.
// (The other thermopacakges do support data reduction mode, but the slight variation in 
// results due to the different thermopackages will cause this particular model to fail
// unless the thermopackage is GasTbl)

//cout << "Input File List \n\n" << inputFileList << endl;
//-------------------------------------------------
// Run a design and off design case
//-------------------------------------------------



real results[];
string indep[];
real alt[] = { 0, 20000, 40000};
real MN[][]={ { 0., .1, .3 },{ .4, .5, .6 },{ .7, .8, .85 }};
real PC[]= {50, 40, 30, 21 };
void saveGuesses(){
	string indep[]=solver.list( "Independent" );
	real results[];
	int i, j,k,l;
	for ( i=0; i < alt.entries(); i++){
		real tempMN[] = MN[i];
		for ( j=0; j < tempMN.entries(); j++ ){
			for ( k=0; k < PC.entries(); k++ ){
				CONTROL.PLACS = PC[k];
				Amb.alt_in= alt[i]; 
				Amb.MN_in= tempMN[j];
				cerr << i << " " << j << " " << k << endl;
				run();
				for (  l=0; l < indep.entries(); l++ ){
					cerr << indep[l]->getParentName()+"."+indep[l]->varName << endl;
					results.append( (indep[l]->getParentName()+"."+indep[l]->varName )->value );
				}
				ncpView.display();
				
				
			}
			
			for ( k=PC.entries()-1; k >-1; k-- ){
				CONTROL.PLACS = PC[k];
				Amb.alt_in= alt[i]; 
				Amb.MN_in= tempMN[j];
				cerr << i << " " << j << " " << k << endl;
				run();
				
				ncpView.display();
				
			}
			
		}
		
	}
	
}

void setGuesses(){
	
	int i, j,k,l;
	
	for ( i=0; i < alt.entries(); i++){
		if ( Amb.alt_in < alt[i] ){
			
			real tempMN[] = MN[i];
			k = k+tempMN.entries()*PC.entries()*indep.entries();
		}else{
			for ( j=0; j < tempMN.entries(); j++ ){
				if ( Amb.alt_MN < tempMN[j] ){
					k = k+PC.entries()*indep.entries();;	
				}else{
					for ( k=0; k < PC.entries(); k++ ){
						if( CONTROL.PLACS < PC[k]){
							k = k+indep.entries();;	
							
						}else{
							
							for (  l=0; l < indep.entries(); l++ ){
								//cerr << indep[l]->getParentName()+"."+indep[l]->varName << endl;
								string temp  =(indep[l]->getParentName()+"."+indep[l]->varName )->value;
								temp=results[k];
								k++;
							}
							
							
							
						}
						
					}
				}
			}
		}
		
		
		
	}
	
	
}
setOption( "switchDes", "DESIGN" );
autoSolverSetup();

cout << "\n\nDesign Dependents:\n" << solver.dependentNames;

CASE = CASE + 1;
//guessAll();

run();

ncpView.display();



setOption( "switchDes", "OFFDESIGN" );
autoSolverSetup();


cout << "\n\nOff-Design Dependents:\n" << solver.dependentNames;

CASE = CASE + 1;
run();
ncpView.display();



//-------------------------------------------------
// Run a throttle hook using the controller
//-------------------------------------------------

Independent FuelControl {
	varName = "BrnPri.Wfuel";
}

Dependent ControlErr {
	eq_lhs = "CONTROL.ERR1";
	eq_rhs = "0.0";
}

solver.addIndependent( "FuelControl" );
solver.addDependent( "ControlErr" );

//CR1929 - Use set/get function to get Independent names and values,
//Dependent names, and the Jacobian matrix after the PLA 50 point


CONTROL.PLACS = 50;
solver.maxIterations = 100;
solver.maxJacobians = 100;
string indeps[] = { "Amb.alt_in", "Amb.MN_in", "PC" };
real indepsV[][];


saveGuesses();
setGuesses();
quit();
run();
ncpView.display();

// CR1929 - Save the key solver information
string DSVnames[]          = solver.DSVnames;
string DSVvalues[]         = solver.DSVvalues;
string independentNames[]  = solver.independentNames;
real   independentValues[] = solver.independentValues;


CONTROL.PLACS = 45;
cout << "\nPLACS = 45\n\n";

InletStart.W = InletStart.W - 5.0;
Splt.BPR = Splt.BPR + 0.05;
ShH.Nmech = ShH.Nmech - 100;



run();
cout << "PLACS = 45 case finished \n\n";

ncpView.display();

CONTROL.PLACS = 40;
cout << "\n\nPLACS = 40\n\n";
run();

cout << "PLACS = 40 case finished \n\n";

ncpView.display();

CONTROL.PLACS = 35;
cout << "\n\nPLACS = 35\n\n";
run();
ncpView.display();

CONTROL.PLACS = 30;
cout << "\n\nPLACS = 30\n\n";
run();
ncpView.display();


CONTROL.PLACS = 50;
solver.maxIterations = 100;
solver.maxJacobians = 100;

run();

Amb.alt_in = 10000.;
Amb.MN_in = .3;
run();
Amb.alt_in = 20000.;
Amb.MN_in = .6;
run();



Amb.alt_in = 40000.;
Amb.MN_in = .8;

CONTROL.PLACS = 50;
solver.maxIterations = 100;
solver.maxJacobians = 100;

run();
ncpView.display();

// CR1929 - Save the key solver information
string DSVnames[]          = solver.DSVnames;
string DSVvalues[]         = solver.DSVvalues;
string independentNames[]  = solver.independentNames;
real   independentValues[] = solver.independentValues;


CONTROL.PLACS = 45;
cout << "\nPLACS = 45\n\n";

InletStart.W = InletStart.W - 5.0;
Splt.BPR = Splt.BPR + 0.05;
ShH.Nmech = ShH.Nmech - 100;



run();
cout << "PLACS = 45 case finished \n\n";

ncpView.display();

CONTROL.PLACS = 40;
cout << "\n\nPLACS = 40\n\n";
run();

cout << "PLACS = 40 case finished \n\n";

ncpView.display();

CONTROL.PLACS = 35;
cout << "\n\nPLACS = 35\n\n";
run();
ncpView.display();

CONTROL.PLACS = 30;
cout << "\n\nPLACS = 30\n\n";
run();
ncpView.display();



//-------------------------------------------------
// Transient:  Control-governed snap accel to full power
//-------------------------------------------------


CONTROL.PLACS = 50;
cout << "\n\nRun back to PLACS = 50\n\n";

// CR1929 - Restore the key solver information

cout << "Restore Solver data saved after previous PLACS 50 run.\n"
<< "Restore Jacobian, so no Jacobian should be generated, \n"
<< "but restored independent values changed by 1% so point \n"
<< "may not reconverge in a single pass\n";


solver.DSVnames = DSVnames;
solver.DSVvalues = DSVvalues;
solver.independentNames = independentNames;
solver.independentValues = independentValues;

run();
ncpView.display();

quit();


cout << solver.stats();


solver.removeIndependent( "FuelControl" );
solver.removeDependent( "ControlErr" );

CONTROL.PLACS = 40;

setOption( "solutionMode", "TRANSIENT");

PlotDataViewer transientPlot {
	title = "Airflow and HPC Pressure over time";
	plotVariables = { "time", "InletStart.W", "CmpH.Fl_O.Pt" };
	plotNames = { "time", "Inlet Airflow", "HPC Pt" };
	label_X = "Time (sec)";
	label_Y = "Air flow (lbm/sec) & Pt3 (psia)";
	symbol = TRUE;
}

transient.stopTime = 5;
transient.baseTimeStep = .05;
postsolverSequence = { "transientTrace", "transientPlot" };

run();

transient.stopTime = 10;
CONTROL.PLACS = 50;

run();

transientTrace.display();
postsolverSequence.remove("transientPlot");

setOption( "solutionMode", "STEADY_STATE");

run();
ncpView.display();

//-------------------------------------------------
//  Check out the Data Reduction Capabilties
//-------------------------------------------------

// With the changes made for CR428 the ExternalDB element was archived.
// This section of the test is no longer viable and has been commented out.

// As of now, this data reduction test only works with GasTbl.
// Do not go further if the thermo package is not GasTbl.
/*
if (THERMPACKAGE != "GasTbl") {
cout << endl;
cout << "Skipping the data reduction test because thermopackage is not GasTbl (this message sent from the file \"desOD.case\")." << endl;
cout << endl;
}

// else THERMPACKAGE == "GasTbl", then run data reduction
else {

solver.maxIterations = 200;
int TEST = 0;
#include "src/DRTest.fnc"


PAMBSE.switchSolverActive =  "Predicted";
TAMBSE.switchSolverActive =  "Predicted";
P25SE.switchSolverActive  =  "Predicted";
T25SE.switchSolverActive  =  "Predicted";
P3SE.switchSolverActive   =  "AsMeasured";
T3SE.switchSolverActive   =  "AsMeasured";
WF36SE.switchSolverActive =  "AsMeasured";
P42SE.switchSolverActive  =  "AsMeasured";
T42SE.switchSolverActive  =  "AsMeasured";
N25SE.switchSolverActive  =  "AsMeasured";

string case = "DB.CASE";
string test = "DB.TEST";


// DB Element info
// Look for DB files in typical places.
// (the current directory isn't always correct)
string fName="input/DRTestDataNames.dbn";
string fData="inputDRTestData.inp";

if (cin.fileExists(fName)) {
DB.F_Name = fName;
DB.F_Data = fData;
}
else {
string dir = getenv("NPSS_TOP")+"/ExampleModels/HiBypassFan/";
if (cin.fileExists(dir+fName)) {
DB.F_Name = dir+fName;
DB.F_Data = dir+fData;
}
else {
dir = getenv("NPSS_TOP")+"/Test/Model/HiBypassFan/";
if (cin.fileExists(dir+fName)) {
DB.F_Name = dir+fName;
DB.F_Data = dir+fData;
}
}
}

// Define how to read each record in the input file
DB.switchRecord="NEXT"; // Options are: FIRST, LAST, NEXT, PREVIOUS, SAME

while (DB.retrieve()) {

//
//  Map the TEST/RDG data into the model
CASE = case->value;
TEST = test->value;

cout << "\n***********************************"
<< "\nExecuting TEST, RDG = " << TEST << ", " << CASE
<< "\n***********************************" << endl << endl;

//
// Predicted
string DRMode = "Predicted";
int DataIsValid;

setOption("switchValid", "MISSING");    //  Set all instr. element check method to MISSING
D025.setOption("switchDP", "INPUT");    //  Set all instr. rakes to be in
DataIsValid = ValidData(DRMode); 
if (DataIsValid){
SetupSolverPredicted();
solver.addIndependent( "FuelControl" );
solver.addDependent( "ControlErr" );
RunPoint();
ncpView.display();
}


//
// AsMeasured
if (DataIsValid && solver.converged){
DRMode = "AsMeasured";
setOption("switchValid", "TOLERANCE");  //  Set all instr. element check method to be against Predicted level
DataIsValid = ValidData(DRMode);
if (DataIsValid){
SetupSolverAsMeasured();
RunPoint();	
ncpView.display();
}
}


//
// Corrected # 1
if (DataIsValid && solver.converged){
DRMode = "Corrected";
SetupSolverCorrected();
RunPoint();
ncpView.display();
}


//
// Corrected # 2
if (DataIsValid && solver.converged){
DRMode = "Corrected";
D025.setOption("switchDP", "OFF");    //  Set all instr. rakes to be out
Amb.alt_in = 0; Amb.MN_in = 0;
RunPoint();
ncpView.display();
}

}

//
//  Final cleanup of report and store data to database

} // end data reduction test
*/