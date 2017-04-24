/*
*   File : CRTest/src/GdmlConstruction.cc
*   Brief: Implementation of class GdmlConstruction
*/

#include "GdmlConstruction.hh"

#include "Analysis.hh"
#include "CryPositionSD.hh"
#include "PmtSD.hh"

#include "G4GDMLParser.hh"
#include "G4GDMLAuxStructType.hh"

#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
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
	: SysConstruction(), fWorldPV(NULL), fGdml(gdml), fGdmlFileName("")
{}

GdmlConstruction::GdmlConstruction(G4String gdmlFileName)
	: SysConstruction(), fWorldPV(NULL), fGdml(NULL)
{
	fGdml = new G4GDMLParser;
	fGdmlFileName = gdmlFileName;
	this->Init();
}

GdmlConstruction::~GdmlConstruction()
{
	G4cout << "[-] INFO - GdmlConstruction deleted. " << G4endl;
	delete fGdml;fGdml = NULL;
}

void GdmlConstruction::Init(){
  assert(fGdml != NULL);
  fGdml->Read(fGdmlFileName, false);
  
  fWorldPV = fGdml->GetWorldVolume();
  if(!fWorldPV){
	  G4cout << "[#] ERROR - CAN NOT FOUND WORLD SETUP - "
		  << __FILE__ << " " << __func__ << G4endl;
	  exit(1);
  }
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  fWorld = lvStore->GetVolume("World", false);
  if(!fWorld)
	  fWorld = fWorldPV->GetLogicalVolume();
  fDetector = lvStore->GetVolume("Detector",false);
  fTarget = lvStore->GetVolume("Target", false);
  fPmt = lvStore->GetVolume("PMT",false);
  
  DumpStructure();
  
  ReadAuxiliary();
}

G4VPhysicalVolume *GdmlConstruction::Construct()
{
  if(fWorldPV){
	  G4GeometryManager::GetInstance()->OpenGeometry();
	  G4PhysicalVolumeStore::GetInstance()->Clean();
	  G4LogicalVolumeStore::GetInstance()->Clean();
	  G4SolidStore::GetInstance()->Clean();
	  G4LogicalSkinSurface::CleanSurfaceTable();
	  G4LogicalBorderSurface::CleanSurfaceTable();
	  fGdml->Clear();
  }

  this->Init();

  return fWorldPV;
}


void GdmlConstruction::ConstructSDandField(){
	if(fDetector){
		G4String sdName = "CryPositionSD";
		CryPositionSD* crySD = new CryPositionSD(sdName);

		SetSensitiveDetector(fDetector, crySD);

		Analysis::Instance()->RegisterSD(crySD);
	}
	if(fPmt){
		// Create, Set & Register PmtSD
		G4String sdName = "PmtSD";
		PmtSD* pmtSD = new PmtSD(sdName);

		SetSensitiveDetector(fPmt, pmtSD);

		Analysis::Instance()->RegisterSD(pmtSD);
	}
	return;
}

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
		assert(SurfaceName == OpSurf->GetName());
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
		
		if(auxIter->type == "SurfaceName"){
			SurfaceName = auxIter->value;
			G4cout << "Surface-" << SurfaceName << " | ";
		}
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
		assert(SurfaceName == OpSurf->GetName());
	}

	return true;
}

void GdmlConstruction::DumpStructure(){
	G4cout << "[-] INFO - CRTest Geometry Structure : " << G4endl;
	
	DumpVolume(fWorldPV," | ");
}

void GdmlConstruction::DumpVolume(
	G4VPhysicalVolume* physvol, 
	G4String prefix, G4bool expanded) const
{
	G4ThreeVector pos = physvol->GetTranslation();

	G4cout << prefix << physvol->GetName() 
		<< "[" << physvol->GetCopyNo() << "] : "
		<< "Position(" << pos.x() << ", " << pos.y() << ", " << pos.z() << ")"
		<< G4endl;

	if(!expanded) return;

	G4LogicalVolume* lvptr = physvol->GetLogicalVolume();
	G4cout << prefix << " + " << lvptr->GetName() << " : ";
	lvptr->GetSolid()->DumpInfo();

	G4String lastPVName = "";
	for(int i = 0 ; i < lvptr->GetNoDaughters() ; i++){
		G4VPhysicalVolume* thePV = lvptr->GetDaughter(i);
		if(thePV->GetName() != lastPVName){
			expanded = true;
			lastPVName = thePV->GetName();
		}
		else
			expanded = false;
		DumpVolume(thePV, prefix+" | ", expanded);
	}

}

#include "G4NistManager.hh"
G4Material* GdmlConstruction::FindMaterial(G4String matName)
{
	G4Material* material = G4Material::GetMaterial(matName);
	if(!material)
		G4NistManager::Instance()->FindOrBuildMaterial(matName);
	if(!material){
		G4cerr << "[#] ERROR - Material NOT FOUND" << G4endl;
		return NULL;
	}
	return material;
}

G4LogicalVolume* GdmlConstruction::FindLogicalVolume(G4String lvName)
{
	G4LogicalVolume* lvptr = G4LogicalVolumeStore::GetInstance()->GetVolume(lvName);
	if(!lvptr){
		G4cerr << "[#] ERROR - Logical Volume NOT FOUND" << G4endl;
		return NULL;
	}
	return lvptr;
}

G4bool GdmlConstruction::SetMaterial(G4String lvName, G4String matName)
{
	G4LogicalVolume* lvptr = FindLogicalVolume(lvName);
	if(!lvptr) return false;
	G4Material* material = FindMaterial(matName);
	if(!material) return false;
	lvptr->SetMaterial(material);

	return true;
}

G4SurfaceProperty* GdmlConstruction::FindSurface(G4String surfName)
{
	const G4SurfacePropertyTable* surfTable = 
		G4SurfaceProperty::GetSurfacePropertyTable();
	G4SurfaceProperty* surf = NULL;
	for(unsigned int i = 0; i < surfTable->size() ; i++){
		surf = (*surfTable)[i];
		if(surf->GetName() == surfName)
			break;
		else
			surf = NULL;
	}
	if(!surf)
		G4cerr << "[#] ERROR - Surface Property NOT FOUND" << G4endl;
	return surf;
}

G4bool GdmlConstruction::SetSkinSurface(G4String lvName, G4String surfName)
{
	if(lvName == "PMT") return SetPmtType(surfName);

	G4LogicalVolume* lvptr = FindLogicalVolume(lvName);
	if(!lvptr) return false;
	
  G4LogicalSkinSurface* surf = G4LogicalSkinSurface::GetSurface(lvptr);

	G4OpticalSurface* op_surf = (G4OpticalSurface*)FindSurface(surfName);
	if(!op_surf) return false;
  if(!surf){
    new G4LogicalSkinSurface(lvName+"_Skin",lvptr,op_surf);
  }else
    surf->SetSurfaceProperty(op_surf);

  if(!op_surf->GetMaterialPropertiesTable()){
	  G4Material* material = FindMaterial(G4String("OpSurface_") + surfName);
	  if(!material) return false;
	  op_surf->SetMaterialPropertiesTable(material->GetMaterialPropertiesTable());
  }
	return true;
}
// TODO : Merge with more if-else ?
G4bool GdmlConstruction::SetPmtType(G4String type)
{
	G4LogicalVolume* lvptr = FindLogicalVolume("PMT");
	if(!lvptr) return false;
	G4LogicalSkinSurface* surf = G4LogicalSkinSurface::GetSurface(lvptr);
	if(!surf){
		G4cerr << "[#] ERROR - PMT photocathod NOT SET" << G4endl;
		return false;
	}
	G4OpticalSurface* op_surf = (G4OpticalSurface*)(surf->GetSurfaceProperty());
	G4Material* material = FindMaterial("OpSurface_PMT_"+type);
	if(!material) return false;

	op_surf->SetMaterialPropertiesTable(material->GetMaterialPropertiesTable());

	return true;
}

void GdmlConstruction::DumpMaterialAndSurface()
{
	G4LogicalVolumeStore* lvTable =
		G4LogicalVolumeStore::GetInstance();
	
	G4cout << "[+] INFO - Dump Logical Volume Material & Surface Table" << G4endl;
	for(unsigned int i = 0 ; i < lvTable->size() ; i++)
	{
		G4LogicalVolume* lvptr = (*lvTable)[i];
		G4Material* material = lvptr->GetMaterial();
		
		G4cout << " | " << lvptr->GetName() << "\t| " << material->GetName()
			<< "\t| ";
		
		G4LogicalSkinSurface* surf = G4LogicalSkinSurface::GetSurface(lvptr);
		if(surf)
			G4cout << surf->GetSurfaceProperty()->GetName() << G4endl;
		else
			G4cout << "NONE" << G4endl; 
	}
}

void GdmlConstruction::DumpOpticalSurface()
{
  G4cout << "[+] INFO - Dump Surface Table" << G4endl;
  G4SurfaceProperty::DumpTableInfo();
}