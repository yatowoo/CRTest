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

class StepAction : public G4UserSteppingAction {

public:
    StepAction();
    virtual ~StepAction();
public:
    virtual void UserSteppingAction(const G4Step*);
private:

};

#endif /*CRTest_StepAction_h*/