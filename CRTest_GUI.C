/*
// CRTest ROOT GUI Application
//
// Version : Developing
// Created : 2017.01.26 10:51
*/

#ifndef CRTest_WINDOW
#define CRTest_WINDOW

#include "RQ_OBJECT.h"
#include "TGObject.h"
#include "TGWindow.h"
#include "TGFrame.h"
#include "TGButton.h"
#include "TGLayout.h"
#include "TGTab.h"
#include "TGTextEntry.h"
#include "TGNumberEntry.h"
#include "TGComboBox.h"
#include "TGTextView.h"
#include "TGLabel.h"
#include "TGStatusBar.h"
#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"

#include "TMath.h"
#include "TF1.h"
#include "TRandom.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TRint.h"

#include<vector>
#include<map>

// Option Candidates
const char* optBuild[] = {"None","Debug","Release","RelWithDebInfo","MiniSizeRel"};
const char* optScintPacking[] = {"Wrapper","Coating"};
const char* optFiber[] = {"Single-cladding","Multi-cladding"};
const char* optGenerator[] = {"beam","CRY","Pdu"};
const char* optRun[] = {"Show","Normal","Jobs"};
const char* optReadout[] = {"Normal","SD_MORE","Optical","Optical_MORE"};
const char* optOutput[] = {"Silent","Debug","All"};

class CRTestWindow : public TGObject {
	RQ_OBJECT("CRTestWindow")

private: // Display
	TGMainFrame         *fMain;
	TGCompositeFrame    *fF1;
	TGCompositeFrame    *fFcanvasL;
	
	TGTab               *fTab;
	TGCompositeFrame *fTabBasic;
	TGCompositeFrame *fTabGeom;
	TGCompositeFrame *fTabAbout;

	TGLayoutHints       *fLayout;
	TGLayoutHints       *fL3;
	TGLayoutHints       *fLCanvas;
	TGTextEntry         *fTxt;
	TGTextEntry         *fCommand;
	TGNumberEntry		*fDrawParam;
	TGComboBox          *fComboCmd;
	TGTextView          *fTextView;
	TString              fName;
	TGLabel             *fLabel;
	TRootEmbeddedCanvas *fCanvas;
	TGStatusBar         *fStatusBar;

private: // Layout
	TGLayoutHints* fLayoutLabel;
	TGLayoutHints* fLayoutInput;
	TGLayoutHints* fLayoutHFrame;
	TGLayoutHints* fLayoutUnit;

private: // Local Variable
	TF1* fDrawFunc;
	UInt_t fHFrameH;
	UInt_t fHFrameW;

private: // Input Param & Options
	std::map<const char*,TGComboBox*>* fOptions;
	std::map<const char*,TGNumberEntry*>* fParams;
	std::map<const char*,TGTextEntry*>* fTextInput;

	const char* fConfigFileName;
	const char* fGdmlFileName;
	const char* fRootFileName;
	const char* fMacroFileName;
	const char* fBuildDir;
	const char* fInstallDir;

public:
	CRTestWindow(const TGWindow* p, UInt_t w, UInt_t h);
	virtual ~CRTestWindow();
	const char* GetText(){return fTxt->GetText();}
	void DoTextChanged();
	void DoDraw();
	void DoClear();
	// for Control buttons
	void DoConfigure();
	void DoBuild();
	void DoRun();
	void BuildTab_Basic();
	void BuildTab_Geometry();
	void BuildTab_About();
	void BuildComboBox(
		TGCompositeFrame* tab, const char* name, const char* opt[], size_t n);
	void BuildNumberEntry(TGCompositeFrame* tab, 
		const char* name, double val, const char* unit, 
		TGNumberFormat::EStyle format = TGNumberFormat::EStyle::kNESRealTwo);
	void BuildTextEntry(TGCompositeFrame* tab, const char* name, const char* val, size_t buf_size = 100);
};

CRTestWindow::CRTestWindow(const TGWindow* p, UInt_t w, UInt_t h)
	: fTabBasic(NULL), fTabGeom(NULL), fTabAbout(NULL)
{
	// Initialize Local Variable
	fOptions = new std::map<const char*,TGComboBox*>;
	fParams = new std::map<const char*,TGNumberEntry*>;
	fTextInput = new std::map<const char*,TGTextEntry*>;

	fGdmlFileName = "mac/default.gdml";
	fRootFileName = "CRTest.root";
	fConfigFileName = "mac/default.config";
	fMacroFileName = "mac/test_muon.mac";
	fBuildDir = "./build";
	fInstallDir = "./install";

	fDrawFunc = NULL;
	fHFrameH = 20;
	fHFrameW = w*0.75;

	fLayoutLabel = new TGLayoutHints(kLHintsExpandX, 5, 5, 3, 4);
	fLayoutInput = new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5);
	fLayoutHFrame = new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1);
	fLayoutUnit = new TGLayoutHints(0, 2, 2, 5, 1);

	/*
	**	Initialize CRTestWindow
	*/
	fMain = new TGMainFrame(p,w,h,kVerticalFrame);

	fMain->SetCleanup(kDeepCleanup);

	//
	// FRAME - for tabs
	//
	fTab = new TGTab(fMain, 300, 300);
	//fTab->Connect("Selected(Int_t)", "TestDialog", this, "DoTab(Int_t)");
	fL3 = new TGLayoutHints(kLHintsTop | kLHintsLeft , 5, 5, 5, 5);
		
		// Tab - Basic
	fTabBasic = fTab->AddTab("Basic");
	BuildTab_Basic();
		
		// Tab - Geometry
	fTabGeom = fTab->AddTab("Geometry");
	BuildTab_Geometry();

		// Tab - About
	fTabAbout = fTab->AddTab("About");
	BuildTab_About();

		// Tab - Canvas
	TGCompositeFrame* tf = fTab->AddTab("Canvas");
	fF1 = new TGCompositeFrame(tf, 60, 20, kHorizontalFrame);

	fFcanvasL = new TGCompositeFrame(tf, 150, 100, kVerticalFrame);

	fDrawParam = new TGNumberEntry(fFcanvasL, 1.0, 5, 0);
	fFcanvasL->AddFrame(fDrawParam, fL3);

	TGTextButton* tb = new TGTextButton(fFcanvasL,"&Draw",0);
	tb->Connect("Clicked()","CRTestWindow",this,"DoDraw()");
	fFcanvasL->AddFrame(tb, fL3);

	fF1->AddFrame(fFcanvasL, fL3);
	// Embedded Canvas
	fCanvas = new TRootEmbeddedCanvas("MyCanvas",tf,200,200);
	fLCanvas = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 10,10,10,1);
	fF1->AddFrame(fCanvas, fLCanvas);

	tf->AddFrame(fF1, fLCanvas);
	fDrawParam->Resize(50, fDrawParam->GetDefaultHeight());
	tb->Resize(tb->GetDefaultWidth(), tb->GetDefaultHeight());

	//
	// FRAME - for button(s)
	//
	TGHorizontalFrame *hframe = new TGHorizontalFrame(fMain, 350, 20, kFixedWidth);
	TGTextButton *generate = new TGTextButton(hframe, "&Configure ");
	generate->Connect("Pressed()", "CRTestWindow", this, "DoConfigure()");
	hframe->AddFrame(generate, new TGLayoutHints(kFixedWidth | kLHintsCenterX, 5, 5, 3, 4));
	TGTextButton *save = new TGTextButton(hframe, "&Build ");
	save->Connect("Pressed()", "CRTestWindow", this, "DoBuild()");
	hframe->AddFrame(save, new TGLayoutHints(kFixedWidth | kLHintsCenterX, 5, 5, 3, 4));
	TGTextButton *exit = new TGTextButton(hframe, "&Run ");
	exit->Connect("Pressed()", "CRTestWindow", this, "DoRun()");	
	hframe->AddFrame(exit, new TGLayoutHints(kFixedWidth | kLHintsCenterX, 5, 5, 3, 4));

	// Add TGTab
	TGLayoutHints *fLayout = new TGLayoutHints(kLHintsTop | kLHintsExpandX |
		kLHintsExpandY, 2, 2, 5, 1);
	fMain->AddFrame(fTab, fLayout);
	// Add Buttons Frame
	fMain->AddFrame(hframe, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));


	fMain->MapSubwindows();
	fMain->Resize();
	fMain->SetWindowName("CRTest Window");
	fMain->MapWindow();
}

CRTestWindow::~CRTestWindow(){
	fMain->DeleteWindow();
}

void CRTestWindow::BuildTextEntry(TGCompositeFrame* tab,
	const char* name, const char* val, size_t buf_size = 100)
{
	TGHorizontalFrame* hframe = 
		new TGHorizontalFrame(tab, fHFrameW, fHFrameH, kLHintsExpandX);
	TGLabel* label = new TGLabel(hframe,name);

	TGTextEntry* entry = new TGTextEntry(hframe,val);
	entry->Resize(fHFrameW,fHFrameH);

	hframe->AddFrame(label, fLayoutLabel);
	hframe->AddFrame(entry, fLayoutInput);
	tab->AddFrame(hframe, fLayoutHFrame);

	fTextInput->insert(std::pair<const char*,TGTextEntry*>(name,entry));
}

void CRTestWindow::BuildNumberEntry(TGCompositeFrame* tab, 
	const char* name, double val, const char* unit, 
	TGNumberFormat::EStyle format = TGNumberFormat::EStyle::kNESRealTwo){

	TGHorizontalFrame* hframe = 
		new TGHorizontalFrame(tab, fHFrameW, fHFrameH, kLHintsExpandX);
	TGLabel* label = new TGLabel(hframe,name);
	TGLabel* labelUnit = new TGLabel(hframe,unit);

	TGNumberEntry* entry = 
		new TGNumberEntry(tab, val, 12, fParams->size(),format);
	entry->Resize(fHFrameW,fHFrameH);

	hframe->AddFrame(label, fLayoutLabel);
	hframe->AddFrame(entry, fLayoutInput);
	hframe->AddFrame(labelUnit, fLayoutUnit);
	tab->AddFrame(hframe, fLayoutHFrame);

	fParams->insert(std::pair<const char*,TGNumberEntry*>(name,entry));

}

void CRTestWindow::BuildComboBox(
	TGCompositeFrame* tab, 
	const char* name, const char* opt[], size_t n)
{
	TGHorizontalFrame* hframe = 
		new TGHorizontalFrame(tab, fHFrameW, fHFrameH, kLHintsExpandX);
	TGLabel* label = new TGLabel(hframe,name);
	TGComboBox* option = new TGComboBox(hframe,100);
	for(unsigned int i = 0; i < n; i++)
		option->AddEntry(opt[i],i);
	option->EnableTextInput(false);
	option->Select(0);
	option->Resize(fHFrameW,fHFrameH);

	hframe->AddFrame(label, fLayoutLabel);
	hframe->AddFrame(option, fLayoutInput);
	tab->AddFrame(hframe, fLayoutHFrame);

	fOptions->insert(std::pair<const char*,TGComboBox*>(name,option));
}

void CRTestWindow::BuildTab_Basic(){
	if(!fTabBasic) return;

	BuildTextEntry(fTabBasic, "Config_FileName", fConfigFileName);
	BuildTextEntry(fTabBasic, "GDML_FileName", fGdmlFileName);
	BuildTextEntry(fTabBasic, "ROOT_FileName", fRootFileName);
	BuildTextEntry(fTabBasic, "Macro_FileName", fMacroFileName);
	BuildTextEntry(fTabBasic, "Build_Dir", fBuildDir);
	BuildTextEntry(fTabBasic, "Install_Dir", fInstallDir);

	BuildComboBox(fTabBasic, "Build_Type",
		optBuild, sizeof(optBuild)/sizeof(char*));
	BuildComboBox(fTabBasic, "Generator_Type",
		optGenerator, sizeof(optGenerator)/sizeof(char*));
	BuildComboBox(fTabBasic, "Execute_Type", 
		optRun, sizeof(optRun)/sizeof(char*));
	BuildComboBox(fTabBasic, "Readout_Option", 
		optReadout, sizeof(optReadout)/sizeof(char*));
	BuildComboBox(fTabBasic, "Output_Option", 
		optOutput, sizeof(optOutput)/sizeof(char*));
}

void CRTestWindow::BuildTab_Geometry(){
	if(!fTabGeom) return;

	BuildNumberEntry(fTabGeom, "Detector_Size_X", 500, "mm");
	
	BuildComboBox(fTabGeom, "PS_Packing_Type",
		optScintPacking, sizeof(optScintPacking)/sizeof(char*));
	BuildComboBox(fTabGeom, "Fiber_Type",
		optFiber, sizeof(optFiber)/sizeof(char*));
}

void CRTestWindow::BuildTab_About(){
	if(!fTabAbout) return;
}

// Processing [TODO : generate cmd with variables] 
void CRTestWindow::DoConfigure(){
	gSystem->Exec("mkdir -p .build");
}

void CRTestWindow::DoBuild(){
	gSystem->Exec("cd .build;cmake ../code;make -j4;");
}

void CRTestWindow::DoRun(){
	gSystem->Exec("./.build/CRTest");
}


void CRTestWindow::DoClear(){
	gSystem->Exec("cd .build; /bin/rm -rf CMake* cmake* Makefile");
}

void CRTestWindow::DoTextChanged(){
	cout << fTxt->GetText() << endl;
	fTxt->Clear();
}

void CRTestWindow::DoDraw(){
	if(fDrawFunc) delete fDrawFunc;
	TString fcn = "sin(" + std::to_string(fDrawParam->GetNumber())
		+ "*x)";
	fDrawFunc = new TF1("f1",fcn.Data(),0,TMath::Pi());
	fDrawFunc->SetLineWidth(3);
	fDrawFunc->Draw();
	TCanvas *canvas = fCanvas->GetCanvas();
	canvas->cd();
	canvas->Update();
}

void CRTest_GUI(){
	CRTestWindow* yatowin = new CRTestWindow(gClient->GetRoot(),400,300);
	cout << yatowin << endl;
}

#endif // CRTest_WINDOW
