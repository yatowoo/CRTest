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

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include<iostream>

int main (int argc, char** argv){
    
    std::cout << std::endl << "CRTest Start!"
        << std::endl << std::endl;

    // UI Session

    // Run manager

    // User defined classes 
        // Detector
        // PhysList
        // Generator
        // RunAction
        // EventAction
    
    // Visualization Manager

    // UIManager

    // Execute Initialization Macro by UIManager

    // Start Session
    // ui->SessionStart();

    // delete 

    return 0;
}