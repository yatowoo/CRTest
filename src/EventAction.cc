/**
*   FILE : CRTest/src/EventAction.cc
*   Brief: Implementation of class EventAction
*/

#include "EventAction.hh"

#include "Analysis.hh"
#include "CryHit.hh"

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4PrimaryVertex.hh"

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

EventAction::EventAction() : G4UserEventAction()
{
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event *anEvent)
{
    G4cout << "[-] INFO - Event "
           << anEvent->GetEventID()
           << " begin."
           << " - by EventAction"
           << G4endl;
}

void EventAction::EndOfEventAction(const G4Event *anEvent)
{
    G4cout << "[-] INFO - Event "
           << anEvent->GetEventID()
           << " end."
           << " - by EventAction"
           << G4endl;
    
    // Covert Hits info. into data
    G4int hcID = G4SDManager::GetSDMpointer()
        ->GetCollectionID("CryPostionSD/CryHC");

    CryHC* cryHC = static_cast<CryHC*>(
        anEvent->GetHCofThisEvent()->GetHC(hcID));

    G4AnalysisManager* rootData = G4AnalysisManager::Instance();

    G4double sdEdep = 0.;
    for(int i = 0 ; i < cryHC->entries() ; i++){
        sdEdep += (*cryHC)[i]->GetEdep();
    }

    G4PrimaryVertex* priV = anEvent->GetPrimaryVertex();
    G4PrimaryParticle* priP = priV->GetPrimary();

    rootData->FillNtupleDColumn(0, sdEdep / eV);
    rootData->FillNtupleIColumn(1,anEvent->GetNumberOfPrimaryVertex());
    rootData->FillNtupleIColumn(2,priP->GetPDGcode());
    rootData->FillNtupleDColumn(3,priV->GetX0()/cm);
    rootData->FillNtupleDColumn(4,priV->GetY0()/cm);
    rootData->FillNtupleDColumn(5,priV->GetZ0()/cm);
    rootData->FillNtupleDColumn(6,priP->GetPx()/cm);
    rootData->FillNtupleDColumn(7,priP->GetPy()/cm);
    rootData->FillNtupleDColumn(8,priP->GetPz()/cm);
    rootData->FillNtupleDColumn(9,priP->GetTotalEnergy()/GeV);

    rootData->AddNtupleRow();
}
