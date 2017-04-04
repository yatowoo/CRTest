/**
*   FILE : CRTest/src/EventAction.cc
*   Brief: Implementation of class EventAction
*/

#include "EventAction.hh"

#include "Analysis.hh"
#include "CryHit.hh"
#include "OpRecorder.hh"

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4PrimaryVertex.hh"

#include "G4ios.hh"
#include "G4String.hh"
#include "G4SystemOfUnits.hh"

EventAction::EventAction() : G4UserEventAction()
{
}

EventAction::~EventAction()
{
	G4cout << "[-] INFO - EventAction deleted. " << G4endl;
}

void EventAction::BeginOfEventAction(const G4Event *anEvent)
{
    G4cout << "[-] INFO - Event " << anEvent->GetEventID()
           << " begins. - by EventAction" << G4endl;
    OpRecorder* Recorder = OpRecorder::Instance();
    Recorder->Reset();

	G4AnalysisManager *rootData = G4AnalysisManager::Instance();
	rootData->CreateNtuple(
		"Event"+std::to_string(anEvent->GetEventID()),"Event Info.");
	rootData->FinishNtuple();
}

void EventAction::EndOfEventAction(const G4Event *anEvent)
{
    OpRecorder* Recorder = OpRecorder::Instance();
    G4cout << "[+] INFO - Optical Process Track & Record - by EventAction." << G4endl;
    Recorder->Print();

    // Covert Hits info. into data
    G4int hcID = G4SDManager::GetSDMpointer()
                     ->GetCollectionID("CryPostionSD/CryHC");

    CryHC *cryHC = static_cast<CryHC *>(
        anEvent->GetHCofThisEvent()->GetHC(hcID));

    G4AnalysisManager *rootData = G4AnalysisManager::Instance();

    G4double sdEdep = 0.;
    for (int i = 0; i < cryHC->entries(); i++)
    {
        sdEdep += (*cryHC)[i]->GetEdep();
    }

    G4PrimaryVertex *priV = anEvent->GetPrimaryVertex();
    G4PrimaryParticle *priP = priV->GetPrimary();

    rootData->FillNtupleDColumn(0, sdEdep / MeV);
    rootData->FillNtupleIColumn(1, anEvent->GetNumberOfPrimaryVertex());
    rootData->FillNtupleIColumn(2, priP->GetPDGcode());
    rootData->FillNtupleDColumn(3, priV->GetX0() / cm);
    rootData->FillNtupleDColumn(4, priV->GetY0() / cm);
    rootData->FillNtupleDColumn(5, priV->GetZ0() / cm);
    rootData->FillNtupleDColumn(6, priP->GetPx() / MeV);
    rootData->FillNtupleDColumn(7, priP->GetPy() / MeV);
    rootData->FillNtupleDColumn(8, priP->GetPz() / MeV);
    rootData->FillNtupleDColumn(9, priP->GetTotalEnergy() / MeV);
    rootData->FillNtupleIColumn(10, Recorder->nScintTotal);
    rootData->FillNtupleIColumn(15, Recorder->nScintToFiber);
    rootData->AddNtupleRow();

    G4cout << "[-] INFO - Event " << anEvent->GetEventID()
           << " ends. - by EventAction" << G4endl;
}
