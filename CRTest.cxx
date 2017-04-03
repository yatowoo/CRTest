/* Cosmic-ray Test Simulation Platform
*
*  Reference : example/basic/B5
*
*  Author : ytwu (torrence@mail.ustc.edu.cn)
*
*  Created : 2017.02.17
*
*/

/* file : CRTest.cxx
** desc.: Main Program for CRTest
*/

#include "SysConstruction.hh"
#include "GdmlConstruction.hh"
#include "ActionRegister.hh"
#include "SysMessenger.hh"
#include "PhysicsList.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

#include "G4VModularPhysicsList.hh"
#include "G4StepLimiterPhysics.hh"

#include "globals.hh"
#include "G4ios.hh"
#include<cstdlib>

void Usage(){
	G4cout << " Usage : ./CRTest [gdml] [mac] [output] [seed]"
		<< G4endl;
}

int main (int argc, char** argv){
       
    G4String gdmlFileName = "./mac/default.gdml";
    G4String macroFileName = "./mac/run.mac";
	G4String rootFileName = "CRTest";
	G4int rndFactor = 1;
	G4bool uiUse = true;
	G4bool visUse = true;

	// Handle Arguments
	switch(argc){
	case 5:
		rndFactor = std::atoi(argv[4]) + 1;
	case 4:
		rootFileName = argv[3];
	case 3:
		macroFileName = argv[2];
		visUse = false;
		uiUse = false;
	case 2:
		gdmlFileName = argv[1];
	case 1:break;
	default:
		Usage();
		exit(1);
	}
#ifndef G4UI_USE
	uiUse = false;
#endif
#ifndef G4UI_USE
	visUse = false;
#endif

    // UI Session
    G4UIExecutive* ui = NULL;
    if (uiUse)
        ui = new G4UIExecutive(argc, argv);

    // Random
    G4Random::setTheEngine(new CLHEP::RanecuEngine);
	G4Random::setTheSeed(time(NULL)*rndFactor,3);

    // Run manager
    G4RunManager* runManager = new G4RunManager;
    
    G4cout << G4endl << "[***] WELCOME - CRTest Start!" << G4endl << G4endl;
	
	// User defined classes 
        // Detector Construction
    runManager->SetUserInitialization(
        new GdmlConstruction(gdmlFileName));
    
    G4VModularPhysicsList* physicsList = new PhysicsList;
    runManager->SetUserInitialization(physicsList);
    
    runManager->SetUserInitialization(new ActionRegister);
    
    SysMessenger* messenger = new SysMessenger(runManager);
    // Visualization Manager
		// TODO : ADD verbosiry option and if/else by arg. mode
    G4VisManager* visManager = NULL;
	if(visUse){
		visManager = new G4VisExecutive("error");
		visManager->Initialize();
	}

    // UIManager
    G4UImanager* uiManager = G4UImanager::GetUIpointer();
    if( argc > 2){
        // Set outpu .root file name from argument
        G4String command = "/analysis/setFileName ";
		G4String fileName = rootFileName;
        uiManager->ApplyCommand(command+fileName);
        // Execute macro from argument
        command = "/control/execute ";
		fileName = macroFileName;
        uiManager->ApplyCommand(command+fileName);
    }
	else{
		// Execute Initialization Macro by UIManager
		uiManager->ApplyCommand("/control/execute ./mac/init.mac");
		if(uiUse){
			if(ui && ui->IsGUI())
				uiManager->ApplyCommand("/control/execute ./mac/gui.mac");
			// Start Session
			ui->SessionStart();
		}

	}// mode case
    
    // delete
	if(uiUse) delete ui;
	if(visUse)	delete visManager;
	delete messenger;
    delete runManager;

    return 0;
}