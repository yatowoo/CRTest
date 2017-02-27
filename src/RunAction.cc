/**
*   FILE : CRTest/src/RunAction.cc
*   Brief: Implementation of class RunAction
*/

#include "RunAction.hh"

#include "G4RunManager.hh"

RunAction::RunAction()
    : G4UserRunAction()
{
    G4RunManager::GetRunManager()->SetPrintProgress(1);
}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
    G4cout << "[-] INFO - Run " << aRun->GetRunID()
        << " begin. - by RunAction" << G4endl;
}

void RunAction::EndOfRunAction(const G4Run* aRun)
{
    G4cout << "[-] INFO - Run " << aRun->GetRunID()
        << " end. - by RunAction" << G4endl;    
}
