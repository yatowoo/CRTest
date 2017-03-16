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

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"

#include "G4VModularPhysicsList.hh"
#include "FTFP_BERT.hh"
#include "G4StepLimiterPhysics.hh"

#include "G4GDMLParser.hh"

#include "globals.hh"
#include "G4ios.hh"

int main (int argc, char** argv){
    
    G4cout << G4endl << "CRTest Start!"
        << G4endl << G4endl;

    // UI Session
#ifdef G4UI_USE
    G4UIExecutive* ui = NULL;
    if (argc == 1)
        ui = new G4UIExecutive(argc, argv);
    // else in Batch mode
#endif

    // Random
    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    srand(time(NULL));
    G4Random::setTheSeed(rand(),3);

    // Run manager
    G4RunManager* runManager = new G4RunManager;
    // User defined classes 
        // Detector Construction
    G4GDMLParser* gdml = new G4GDMLParser;
    gdml->Read("./mac/default.gdml");
    runManager->SetUserInitialization(
        new GdmlConstruction(gdml));
    //runManager->SetUserInitialization(new SysConstruction());
    
    G4VModularPhysicsList* physicsList = new FTFP_BERT;
    physicsList->RegisterPhysics(new G4StepLimiterPhysics());
    runManager->SetUserInitialization(physicsList);
    
    runManager->SetUserInitialization(new ActionRegister);
    
    SysMessenger* messenger = new SysMessenger(runManager);
    // Visualization Manager
#ifdef G4VIS_USE
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
#endif

    // UIManager
    G4UImanager* uiManager = G4UImanager::GetUIpointer();
    if( argc == 2){
        // Execute macro from argument
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        uiManager->ApplyCommand(command+fileName);
    }
    else{
        // Execute Initialization Macro by UIManager
        uiManager->ApplyCommand("/control/execute ./mac/init.mac");
        if(ui->IsGUI())
            uiManager->ApplyCommand("/control/execute ./mac/gui.mac");
        // Start Session
        ui->SessionStart();
    }// mode case
    
    // delete
    delete ui;
    delete gdml;
    delete messenger; 
    delete visManager;
    delete runManager;

    return 0;
}