/*
*   File : CRTest/src/GdmlConstruction.cc
*   Brief: Implementation of class GdmlConstruction
*/

#include "GdmlConstruction.hh"

#include "G4GDMLAuxStructType.hh"

#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"

#include<vector>

GdmlConstruction::GdmlConstruction(G4GDMLParser *gdml)
    : SysConstruction()
{
  assert(gdml != NULL);
  fWorldPV = gdml->GetWorldVolume();
  fWorld = gdml->GetVolume("World");
  fDetector = gdml->GetVolume("Detector");
  fTarget = gdml->GetVolume("Target");
  ReadAuxiliary(gdml);
}
GdmlConstruction::~GdmlConstruction()
{
}

G4VPhysicalVolume *GdmlConstruction::Construct()
{
  return fWorldPV;
}

void GdmlConstruction::ReadAuxiliary(G4GDMLParser* gdml){
	// Volume Auxiliary
	const G4LogicalVolumeStore* LVstore = G4LogicalVolumeStore::GetInstance();
	G4cout << "[+] INFO - Auxiliary Info. for Logical Volumes" << G4endl;
	std::vector<G4LogicalVolume*>::const_iterator LViter;
	for(LViter = LVstore->begin(); LViter != LVstore->end() ; LViter++)
	{
		G4GDMLAuxListType auxList = gdml->GetVolumeAuxiliaryInformation(*LViter);
		if(auxList.size() == 0 ) continue;
		G4cout << " - Logical Volume : "
			<< (*LViter)->GetName() << G4endl;
		PrintAuxiliary(&auxList," | ");
	}
	// Userinfo Auxiliary
	G4cout << "[+] INFO - Auxiliary Info. for Global/Userinfo " << G4endl;
	PrintAuxiliary(gdml->GetAuxList()," | ");

	return;
}

void GdmlConstruction::PrintAuxiliary(
	const G4GDMLAuxListType* auxList,G4String prefix){
	for(std::vector<G4GDMLAuxStructType>::const_iterator auxIter
			= auxList->begin();
		auxIter != auxList->end(); auxIter ++){
			G4cout << prefix << auxIter->type << " : " 
				<< auxIter->value << " " << auxIter->unit
				<< G4endl;
		if(auxIter->auxList)
			PrintAuxiliary(auxIter->auxList,prefix+" | ");
	}// print contend in <aux></aux>
	return;
}