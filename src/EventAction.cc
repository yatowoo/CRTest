/**
*   FILE : CRTest/src/EventAction.cc
*   Brief: Implementation of class EventAction
*/

#include "EventAction.hh"

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4ios.hh"

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
}
