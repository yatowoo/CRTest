/*
*   File : CRTest/inc/GdmlConstruciton.hh
*
*   Brief: Define of Detector System by GDML
*
*/

#ifndef CRTest_GdmlConstruction_h
#define CRTest_GdmlConstruction_h

#include "SysConstruction.hh"

#include "G4VPhysicalVolume.hh"

class GdmlConstruction : public SysConstruction
{
public:
    GdmlConstruction(G4VPhysicalVolume* world)
        : SysConstruction(){
        fWorld = world;
    };
    virtual ~GdmlConstruction(){};

    virtual G4VPhysicalVolume* Construct()
    {return fWorld;};

    virtual void ConstructSDandField(){};

private:
    G4VPhysicalVolume* fWorld;
};

#endif // CRTest_GdmlConstruction_h