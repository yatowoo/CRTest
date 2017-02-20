/*
*   File : CRTest/inc/EventAction.hh
*
*   Brief: Handle event info and hit-collections for data output
*
*/

#ifndef CRTest_EventAction_h
#define CRTest_EventAction_h

#include "G4UserEventAction.hh"

class EventAction : public G4UserEventAction {

public:
    EventAction();
    virtual ~EventAction();
public:
    virtual void BeginOfEventAction(const G4Event* anEvent);
    virtual void EndOfEventAction(const G4Event* anEvent);
private:

};

#endif /*CRTest_EventAction_h*/