/**
*   FILE : CRTest/src/RunAction.cc
*   Brief: Implementation of class RunAction
*/

#include "RunAction.hh"

#include "Analysis.hh"

#include "G4RunManager.hh"

RunAction::RunAction()
    : G4UserRunAction()
{
    G4RunManager::GetRunManager()->SetPrintProgress(1);

    // Initialize .root file
	Analysis::Instance()->CreateNtupleForRun();
}

RunAction::~RunAction()
{

	delete Analysis::Instance();
	G4cout << "[-] INFO - RunAction deleted. " << G4endl;
}

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
    G4cout << G4endl << "[-] INFO - Run " << aRun->GetRunID()
        << " begins. - by RunAction" << G4endl;
    
    // Initialize .root file
    Analysis::Instance()->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run* aRun)
{
    // Write data to file
	Analysis::Instance()->SaveFile();

    G4cout << "[-] INFO - Run " << aRun->GetRunID()
        << " ends. - by RunAction" << G4endl;
}
