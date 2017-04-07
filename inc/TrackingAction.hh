/*
*   File : CRTest/inc/TrackingAction.hh
*
*   Brief: Define user action before & after tracking
*
*/


#ifndef CRTest_TrackingAction_h
#define CRTest_TrackingAction_h

#include "G4UserTrackingAction.hh"

#include "G4Track.hh"

class TrackingAction : public G4UserTrackingAction {

public:
    TrackingAction();
    virtual ~TrackingAction();
public:
    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);
};

#endif /*CRTest_StepAction_h*/