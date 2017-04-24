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
	virtual void ConstructSDandField();
	
	void DumpStructure();

private:
	void Init();
	void DumpVolume(G4VPhysicalVolume* physvol, 
		G4String prefix, G4bool expanded=true) const;

	void ReadAuxiliary();
	void PrintAuxiliary(const G4GDMLAuxListType*,G4String);
	void ReadProperty(const G4GDMLAuxListType*,G4String);
	G4bool ReadBorderProperty(const G4GDMLAuxListType*,G4String);
	G4bool ReadSkinProperty(const G4GDMLAuxListType*,G4String);

private:
    G4VPhysicalVolume* fWorldPV;
	G4LogicalVolume* fPmt;

	G4GDMLParser* fGdml;
	G4String fGdmlFileName;

public: // UIcmd
	G4String GetGdmlFileName(){return fGdmlFileName;};
	void SetGdmlFileName(G4String fileName){fGdmlFileName = fileName;};

	G4bool SetMaterial(G4String lvName, G4String matName);
	G4bool SetSkinSurface(G4String lvName, G4String surfName);
	G4bool SetPmtType(G4String type);
	void DumpMaterialAndSurface();
  void DumpOpticalSurface();

private:
	G4LogicalVolume* FindLogicalVolume(G4String lvName);
	G4Material* FindMaterial(G4String matName);
	G4SurfaceProperty* FindSurface(G4String surfName);
};

#endif // CRTest_GdmlConstruction_h