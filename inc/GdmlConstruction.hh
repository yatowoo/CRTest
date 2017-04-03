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
#include "G4GDMLAuxStructType.hh"

#include "G4VPhysicalVolume.hh"

class GdmlConstruction : public SysConstruction
{
public:
    GdmlConstruction(G4GDMLParser* gdml);
	GdmlConstruction(G4String gdmlFileName);
    virtual ~GdmlConstruction();

    virtual G4VPhysicalVolume* Construct();

private:
	void Init();
	void ReadAuxiliary();
	void PrintAuxiliary(const G4GDMLAuxListType*,G4String);
	void ReadProperty(const G4GDMLAuxListType*,G4String);
	G4bool ReadBorderProperty(const G4GDMLAuxListType*,G4String);
	G4bool ReadSkinProperty(const G4GDMLAuxListType*,G4String);

private:
    G4VPhysicalVolume* fWorldPV;
	G4GDMLParser* fGdml;
};

#endif // CRTest_GdmlConstruction_h