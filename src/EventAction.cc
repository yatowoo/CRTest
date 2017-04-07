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
	G4int eventID = anEvent->GetEventID();
    G4cout << "[-] INFO - Event " << eventID
           << " begins. - by EventAction" << G4endl;
    OpRecorder* Recorder = OpRecorder::Instance();
    Recorder->Reset();

	G4int ntupleID = Analysis::Instance()->CreateNtupleForEvent(eventID);
		//ana::CreateNtupleForEvent(eventID);
	assert(ntupleID == eventID + 1);
	G4cout << " | + Create Event Vertex Ntuple " << ntupleID << G4endl;
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

    G4double sdEdep = 0.;
    for (int i = 0; i < cryHC->entries(); i++)
    {
        sdEdep += (*cryHC)[i]->GetEdep();
    }

	//ana::FillOutputForRun(anEvent, sdEdep);
	Analysis::Instance()->FillEntryForRun(anEvent);

    G4cout << "[-] INFO - Event " << anEvent->GetEventID()
           << " ends. - by EventAction" << G4endl;
}
