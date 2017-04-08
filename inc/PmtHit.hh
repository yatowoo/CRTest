#ifndef CRTest_PmtHit_h
#define CRTest_PmtHit_h

#include "G4VHit.hh"
#include "G4THitsCollection.hh"

class PmtHit : public G4VHit {
public:
	PmtHit();
	virtual ~PmtHit();
	virtual void Draw();
	virtual void Print();

private:
	G4int fPmtID;

public:
	G4int GetPmtID() {return fPmtID;};
	void SetPmtID(G4int id){fPmtID = id;};

};
// HitCollection
typedef G4THitsCollection<PmtHit> PmtHC;

#endif /*CRTest_PmtHit_h*/