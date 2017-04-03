/*
*   File : CRTest/src/GdmlConstruction.cc
*   Brief: Implementation of class GdmlConstruction
*/

#include "GdmlConstruction.hh"

#include "G4GDMLParser.hh"
#include "G4GDMLAuxStructType.hh"

#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"

#include "G4LogicalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"

#include<vector>

GdmlConstruction::GdmlConstruction(G4GDMLParser *gdml)
	: SysConstruction(), fWorldPV(NULL), fGdml(gdml)
{
	Init();
}

GdmlConstruction::GdmlConstruction(G4String gdmlFileName)
	: SysConstruction(), fWorldPV(NULL), fGdml(NULL)
{
	fGdml = new G4GDMLParser;
	fGdml->Read(gdmlFileName, false);

	this->Init();
}

GdmlConstruction::~GdmlConstruction()
{
	G4cout << "[-] INFO - GdmlConstruction deleted. " << G4endl;
	delete fGdml;fGdml = NULL;
}

void GdmlConstruction::Init(){
  assert(fGdml != NULL);
  fWorldPV = fGdml->GetWorldVolume();
  if(!fWorldPV){
	  G4cout << "[#] ERROR - CAN NOT FOUND WORLD SETUP - "
		  << __FILE__ << " " << __func__ << G4endl;
	  exit(1);
  }
  fWorld = fGdml->GetVolume("World");
  fDetector = fGdml->GetVolume("Detector");
  fTarget = fGdml->GetVolume("Target");
  ReadAuxiliary();
}

G4VPhysicalVolume *GdmlConstruction::Construct()
{
  return fWorldPV;
}

// TODO : ADD format check, exception handle and output
void GdmlConstruction::ReadAuxiliary(){
	// Volume Auxiliary
	const G4LogicalVolumeStore* LVstore = G4LogicalVolumeStore::GetInstance();
	G4cout << "[-] INFO - Auxiliary Info. for Logical Volumes" << G4endl;
	std::vector<G4LogicalVolume*>::const_iterator LViter;
	for(LViter = LVstore->begin(); LViter != LVstore->end() ; LViter++)
	{
		G4GDMLAuxListType auxList = fGdml->GetVolumeAuxiliaryInformation(*LViter);
		if(auxList.size() == 0 ) continue;
		G4cout << " - Logical Volume : "
			<< (*LViter)->GetName() << G4endl;
		PrintAuxiliary(&auxList," | ");
	}
	// Userinfo Auxiliary
	G4cout << "[-] INFO - Auxiliary Info. for Global/Userinfo " << G4endl;
	PrintAuxiliary(fGdml->GetAuxList()," | ");

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
		if(! auxIter->auxList) continue;

		if(auxIter->type == "Property")
			ReadProperty(auxIter->auxList, prefix+" + ");
		else
			PrintAuxiliary(auxIter->auxList,prefix+" | ");
	}// print contend in <aux></aux>
	return;
}

void GdmlConstruction::ReadProperty(
	const G4GDMLAuxListType* auxList,G4String prefix){
	
	G4cout << prefix << " Property for OpticalSurface" << G4endl;
	
	for(std::vector<G4GDMLAuxStructType>::const_iterator auxIter
		= auxList->begin();
		auxIter != auxList->end(); auxIter ++){
		if(auxIter->type == "Skin"){
			G4cout << prefix << "Skin Surface Property : "
				<< auxIter->value << G4endl;
			if(auxIter->auxList)
				ReadSkinProperty(auxIter->auxList,prefix + " + ");
		}
		else if(auxIter->type == "Border"){
			G4cout << prefix << "Border Surface Property : "
				<< auxIter->value << G4endl;
			if(auxIter->auxList)
				ReadBorderProperty(auxIter->auxList,prefix + " + ");
		}
		else{
			G4cout << "[#] - ERROR - WRONG AUXTYPE for Property - "
				<< " Support 'Skin' and 'Border' ONLY " << G4endl;
		}
	}
}

G4bool GdmlConstruction::ReadSkinProperty(
	const G4GDMLAuxListType* auxList,G4String prefix){
	
	G4String SurfaceName;
	G4LogicalVolume* lvptr = NULL;
	G4Material* matptr = NULL;
	
	G4cout << prefix;
	for(std::vector<G4GDMLAuxStructType>::const_iterator auxIter
		= auxList->begin();
		auxIter != auxList->end(); auxIter ++){
		
		if(auxIter->type == "SurfaceName"){
			SurfaceName = auxIter->value;
			G4cout << "Surface-" << auxIter->value << " | ";
		}
		else if(auxIter->type == "LVname"){
			G4cout << "LogVol-" << auxIter->value << " | ";
			lvptr = 
				G4LogicalVolumeStore::GetInstance()->GetVolume(auxIter->value);
			if(!lvptr){
				G4cerr << "[#] ERROR - Logical Volume NOT FOUND" << G4endl;
				return false;
			}
		}else if(auxIter->type == "Material"){
			G4cout << "Material-" << auxIter->value << " | ";
			matptr = 
				G4Material::GetMaterial(auxIter->value);
			if(!matptr || !matptr->GetMaterialPropertiesTable()){
				G4cerr << "[#] ERROR - G4Material NOT FOUND" << G4endl;
				return false;
			}
		}else{
			G4cout << "[#] - ERROR - WRONG AUXTYPE for Skin - "
				<< " NEEDED 'LVname' and 'Material' " << G4endl;		
		}
	}
	G4cout << G4endl;

	G4LogicalSkinSurface* Surface = NULL;
	G4OpticalSurface* OpSurf = NULL;
	Surface = G4LogicalSkinSurface::GetSurface(lvptr);
	if(Surface) OpSurf = 
		dynamic_cast<G4OpticalSurface*>(Surface->GetSurfaceProperty());
	if(OpSurf && !OpSurf->GetMaterialPropertiesTable()){
		OpSurf->SetMaterialPropertiesTable(
			matptr->GetMaterialPropertiesTable());
		assert(OpSurf->GetName() == SurfaceName);
	}

	return true;
}

G4bool GdmlConstruction::ReadBorderProperty(
	const G4GDMLAuxListType* auxList,G4String prefix){

	G4String SurfaceName;
	G4VPhysicalVolume* thePrePV = NULL;
	G4VPhysicalVolume* thePostPV = NULL;
	G4Material* matptr = NULL;
	
	G4cout << prefix;
	for(std::vector<G4GDMLAuxStructType>::const_iterator auxIter
		= auxList->begin();
		auxIter != auxList->end(); auxIter ++){
		
		if(auxIter->type == "SurfaceName")
			G4cout << "Surface-" << auxIter->value << " | ";
		else if(auxIter->type == "PVname"){
			G4cout << "PhysVol-" << auxIter->value << " | ";
			G4VPhysicalVolume* physvol = 
				G4PhysicalVolumeStore::GetInstance()->GetVolume(auxIter->value);
			if(!physvol){
				G4cerr << "[#] ERROR - Physical Volume NOT FOUND" << G4endl;
				return false;
			}else if(!thePrePV)
				thePrePV = physvol;
			else if(!thePostPV)
				thePostPV = physvol;
			else{
				G4cerr << "[#] ERROR - TOO many Physical Volume FOUND" << G4endl;
				return false;
			}
		}else if(auxIter->type == "Material"){
			G4cout << "Material-" << auxIter->value << " | ";
			matptr = 
				G4Material::GetMaterial(auxIter->value);
			if(!matptr || !matptr->GetMaterialPropertiesTable()){
				G4cerr << "[#] ERROR - G4Material NOT FOUND" << G4endl;
				return false;
			}
		}else{
			G4cout << "[-] - ERROR - WRONG AUXTYPE for Skin - "
				<< " NEEDED 'PVname' and 'Material' " << G4endl;		
		}
	}
	G4cout << G4endl;

	G4LogicalBorderSurface* Surface = NULL;
	G4OpticalSurface* OpSurf = NULL;
	Surface = G4LogicalBorderSurface::GetSurface(thePrePV, thePostPV);
	if(Surface) OpSurf = 
		dynamic_cast<G4OpticalSurface*>(Surface->GetSurfaceProperty());
	if(OpSurf && !OpSurf->GetMaterialPropertiesTable()){
		OpSurf->SetMaterialPropertiesTable(
			matptr->GetMaterialPropertiesTable());
		assert(OpSurf->GetName() == SurfaceName);
	}

	return true;
}