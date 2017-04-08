/**
*   FILE : CRTest/src/ActionRegister.cc
*   Brief: Implementation of class ActionRegister
*/

#include "ActionRegister.hh"

#include "Generator.hh"
#include "CryGenerator.hh"
#include "PduGenerator.hh"

#include "RunAction.hh"
#include "EventAction.hh"
#include "StackAction.hh"
#include "TrackingAction.hh"
#include "StepAction.hh"

ActionRegister::ActionRegister()
    : G4VUserActionInitialization()
{}

ActionRegister::~ActionRegister()
{}

void ActionRegister::BuildForMaster() const
{
    SetUserAction(new RunAction);
}

void ActionRegister::Build() const
{
    SetUserAction(new Generator);
    SetUserAction(new RunAction);
    SetUserAction(new EventAction);
    SetUserAction(new StackAction);
	SetUserAction(new TrackingAction);
	SetUserAction(new StepAction);
}