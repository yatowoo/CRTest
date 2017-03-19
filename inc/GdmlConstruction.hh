/*
*   File : CRTest/inc/GdmlConstruciton.hh
*
*   Brief: Define of Detector System by GDML
*
*/

#ifndef CRTest_GdmlConstruction_h
#define CRTest_GdmlConstruction_h

#include "SysConstruction.hh"

#include "G4GDMLParser.hh"

#include "G4VPhysicalVolume.hh"

class GdmlConstruction : public SysConstruction
{
public:
    GdmlConstruction(G4GDMLParser* gdml);
    virtual ~GdmlConstruction();

    virtual G4VPhysicalVolume* Construct();

private:
    G4VPhysicalVolume* fWorldPV;
};

#endif // CRTest_GdmlConstruction_h