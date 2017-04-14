/**
*   FILE : CRTest/src/VirtualSD.cc
*   Brief: Implementation of class VirtualSD
*/

#include "VirtualSD.hh"

VirtualSD::VirtualSD(G4String name)
	:G4VSensitiveDetector(name),fNvolume(0),fNphysvol(NULL),
	fHitCopyNo(NULL),fHitEk(NULL), fHitTime(NULL), 
	fHitX(NULL),fHitY(NULL),fHitZ(NULL),
	fHitPX(NULL),fHitPY(NULL),fHitPZ(NULL)
{
	// TODO : #ifdef CRTest_SD_MORE
	fHitCopyNo = new std::vector<int>;
	fHitEk= new std::vector<double>;
	fHitTime= new std::vector<double>;
	fHitX= new std::vector<double>;
	fHitY= new std::vector<double>;
	fHitZ= new std::vector<double>;
	fHitPX= new std::vector<double>;
	fHitPY= new std::vector<double>;
	fHitPZ= new std::vector<double>;
}

VirtualSD::~VirtualSD(){
	delete fNphysvol;
	// TODO : #ifdef CRTest_SD_MORE
	delete fHitCopyNo;
	delete fHitEk;
	delete fHitTime;
	delete fHitX;
	delete fHitY;
	delete fHitZ;
	delete fHitPX;
	delete fHitPY;
	delete fHitPZ;
}

void VirtualSD::CalculateNoPhysvols(const G4StepPoint* thePoint){
	// VERBOSE
	G4cout << "[+] VERBOSE - Physical Volume : Numbers" << G4endl;
	fNphysvol = new std::vector<int>;
	fNvolume = 1;

	G4TouchableHistory* touchable
		= (G4TouchableHistory*)(thePoint->GetTouchable());
	for(int i = 0 ; i < 10 ; i++){
		G4VPhysicalVolume* thePV = touchable->GetVolume(i);
		G4String pvName = thePV->GetName();
		if(pvName == "World_PV")
			break;
		int nSibling = 
			thePV->GetMotherLogical()->GetNoDaughters();
		int nPV = 0;
		for(int idxPV = 0 ; idxPV < nSibling ; idxPV++)
			if(thePV->GetMotherLogical()->GetDaughter(idxPV)->GetName()
				== pvName)
				nPV ++;
		fNphysvol->push_back(nPV);
		fNvolume *= nPV;
		// VERBOSE
		G4cout << "\t" << pvName << "\t" << nPV << G4endl;
	}
	// VERBOSE
	G4cout << "Total - " << SensitiveDetectorName << " x" << fNvolume << G4endl;
}

int VirtualSD::CalculateCopyNo(const G4StepPoint* thePoint){
	int copyNo = 0;

    G4TouchableHistory* touchable
		= (G4TouchableHistory*)(thePoint->GetTouchable());
	int factor = 1;
	for(int i = 0 ; i < 10 ; i++){
		G4VPhysicalVolume* thePV = touchable->GetVolume(i);
		if(thePV->GetName() == "World_PV")
			break;
		copyNo += thePV->GetCopyNo() * factor;
		factor *= fNphysvol->at(i);
	}

	return copyNo;
}