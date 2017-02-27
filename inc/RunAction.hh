/*
*   File : CRTest/inc/RunAction.hh
*
*   Brief: Initialize running settings and info/data output
*
*/

#ifndef CRTest_RunAction_h
#define CRTest_RunAction_h

#include "G4UserRunAction.hh"

#include "G4Run.hh"

class RunAction : public G4UserRunAction {

public:
    RunAction();
    virtual ~RunAction();
public:
    //virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run* aRun);
    virtual void EndOfRunAction(const G4Run* aRun);
private:

};

#endif /*CRTest_RunAction_h*/