/**
*   FILE : CRTest/src/EventAction.cc
*   Brief: Implementation of class EventAction
*/

#include "EventAction.hh"

#include "Analysis.hh"
#include "CryHit.hh"
#include "OpRecorder.hh"
#include "MuonRecorder.hh"

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
	OpRecorder::Instance()->Reset();
	MuonRecorder::Instance()->Reset();
	Analysis::Instance()->SetCurrentEvent(eventID);
}

void EventAction::EndOfEventAction(const G4Event *anEvent)
{
    OpRecorder* Recorder = OpRecorder::Instance();
    G4cout << "[+] INFO - Optical Process Track & Record - by EventAction." << G4endl;
    Recorder->Print();

	Analysis::Instance()->FillEntryForRun();

    G4cout << "[-] INFO - Event " << anEvent->GetEventID()
           << " ends. - by EventAction" << G4endl;
}
