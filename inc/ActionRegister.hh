/*
*   File : CRTest/inc/ActionRegister.hh
*   Brief: Regist user-defined actions
*   Description:
*       Action Initialization class
*/

#ifndef CRTest_ActionRegister_h
#define CRTest_ActionRegister_h

#include "G4VUserActionInitialization.hh"

class ActionRegister : public G4VUserActionInitialization
{
public:
    ActionRegister();
    virtual ~ActionRegister();
public:
    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif // CRTest_ActionRegister_h