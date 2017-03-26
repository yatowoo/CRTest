/*
*   File : CRTest/inc/StepAction.hh
*
*   Brief: Define user action during kernel stepping
*
*/

#ifndef CRTest_StepAction_h
#define CRTest_StepAction_h

#include "G4UserSteppingAction.hh"

#include "G4Step.hh"
#include "G4OpBoundaryProcess.hh"

class StepAction : public G4UserSteppingAction {

public:
    StepAction();
    virtual ~StepAction();
public:
    virtual void UserSteppingAction(const G4Step*);
private:
    G4bool BoundaryStats(G4OpBoundaryProcess*);
};

#endif /*CRTest_StepAction_h*/