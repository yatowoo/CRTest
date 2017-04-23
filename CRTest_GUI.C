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
#include "TString.h"

#include<string>
#include<vector>
#include<map>

using namespace std;

// Option Candidates
string optBuild[] = {"None","Debug","Release","RelWithDebInfo","MiniSizeRel"};
string optScintPacking[] = {"Wrapper","Coating"};
string optFiber[] = {"Single-cladding","Multi-cladding"};
string optGenerator[] = {"beam","CRY","Pdu"};
string optRun[] = {"Show","Normal","Jobs"};
string optReadout[] = {"Normal","SD_MORE","Optical","Optical_MORE"};
string optOutput[] = {"Silent","Debug","All"};

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
	map<string,TGComboBox*>* fOptions;
	map<string,TGNumberEntry*>* fParams;
	map<string,TGTextEntry*>* fTextInput;

	TGTextBuffer* fConfigFileName;
	TGTextBuffer* fGdmlFileName;
	TGTextBuffer* fRootFileName;
	TGTextBuffer* fMacroFileName;
	TGTextBuffer* fBuildDir;
	TGTextBuffer* fInstallDir;

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
		TGCompositeFrame* tab, string name, string opt[], size_t n);
	void BuildNumberEntry(TGCompositeFrame* tab, 
		string name, double val, const char* unit, 
		TGNumberFormat::EStyle format = TGNumberFormat::EStyle::kNESRealTwo);
	void BuildTextEntry(TGCompositeFrame* tab, string name, TGTextBuffer* buf);
};

CRTestWindow::CRTestWindow(const TGWindow* p, UInt_t w, UInt_t h)
	: fTabBasic(NULL), fTabGeom(NULL), fTabAbout(NULL)
{
	// Initialize Local Variable
	fOptions = new map<string,TGComboBox*>;
	fParams = new map<string,TGNumberEntry*>;
	fTextInput = new map<string,TGTextEntry*>;

	fConfigFileName = new TGTextBuffer(100);
	fConfigFileName->AddText(0,"mac/default.config");
	fGdmlFileName = new TGTextBuffer(100);
	fGdmlFileName->AddText(0,"mac/default.gdml");
	fRootFileName = new TGTextBuffer(100);
	fRootFileName->AddText(0,"CRTest.root");
	fMacroFileName = new TGTextBuffer(100);
	fMacroFileName->AddText(0,"mac/test_muon.mac");
	fBuildDir = new TGTextBuffer(100);
	fBuildDir->AddText(0,"./build");
	fInstallDir = new TGTextBuffer(100);
	fInstallDir->AddText(0,"./install");

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
	string name, TGTextBuffer* buf)
{
	TGHorizontalFrame* hframe = 
		new TGHorizontalFrame(tab, fHFrameW, fHFrameH, kLHintsExpandX);
	TGLabel* label = new TGLabel(hframe,name.c_str());

	TGTextEntry* entry = new TGTextEntry(hframe,buf);
	entry->Resize(fHFrameW,fHFrameH);

	hframe->AddFrame(label, fLayoutLabel);
	hframe->AddFrame(entry, fLayoutInput);
	tab->AddFrame(hframe, fLayoutHFrame);

	fTextInput->insert(pair<string,TGTextEntry*>(name,entry));
}

void CRTestWindow::BuildNumberEntry(TGCompositeFrame* tab, 
	string name, double val, const char* unit, 
	TGNumberFormat::EStyle format = TGNumberFormat::EStyle::kNESRealTwo){

	TGHorizontalFrame* hframe = 
		new TGHorizontalFrame(tab, fHFrameW, fHFrameH, kLHintsExpandX);
	TGLabel* label = new TGLabel(hframe,name.c_str());
	TGLabel* labelUnit = new TGLabel(hframe,unit);

	TGNumberEntry* entry = 
		new TGNumberEntry(tab, val, 12, fParams->size(),format);
	entry->Resize(fHFrameW,fHFrameH);

	hframe->AddFrame(label, fLayoutLabel);
	hframe->AddFrame(entry, fLayoutInput);
	hframe->AddFrame(labelUnit, fLayoutUnit);
	tab->AddFrame(hframe, fLayoutHFrame);

	fParams->insert(std::pair<string,TGNumberEntry*>(name,entry));

}

void CRTestWindow::BuildComboBox(
	TGCompositeFrame* tab, 
	string name, string opt[], size_t n)
{
	TGHorizontalFrame* hframe = 
		new TGHorizontalFrame(tab, fHFrameW, fHFrameH, kLHintsExpandX);
	TGLabel* label = new TGLabel(hframe,name.c_str());
	TGComboBox* option = new TGComboBox(hframe,100);
	for(unsigned int i = 0; i < n; i++)
		option->AddEntry(opt[i].c_str(),i);
	option->EnableTextInput(false);
	option->Select(0);
	option->Resize(fHFrameW,fHFrameH);

	hframe->AddFrame(label, fLayoutLabel);
	hframe->AddFrame(option, fLayoutInput);
	tab->AddFrame(hframe, fLayoutHFrame);

	fOptions->insert(std::pair<string,TGComboBox*>(name,option));
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
		optBuild, sizeof(optBuild)/sizeof(string));
	BuildComboBox(fTabBasic, "Generator_Type",
		optGenerator, sizeof(optGenerator)/sizeof(string));
	BuildComboBox(fTabBasic, "Execute_Type", 
		optRun, sizeof(optRun)/sizeof(char*));
	BuildComboBox(fTabBasic, "Readout_Option", 
		optReadout, sizeof(optReadout)/sizeof(string));
	BuildComboBox(fTabBasic, "Output_Option", 
		optOutput, sizeof(optOutput)/sizeof(string));
}

void CRTestWindow::BuildTab_Geometry(){
	if(!fTabGeom) return;

	BuildNumberEntry(fTabGeom, "Detector_Size_X", 500, "mm");
	
	BuildComboBox(fTabGeom, "PS_Packing_Type",
		optScintPacking, sizeof(optScintPacking)/sizeof(string));
	BuildComboBox(fTabGeom, "Fiber_Type",
		optFiber, sizeof(optFiber)/sizeof(string));
}

void CRTestWindow::BuildTab_About(){
	if(!fTabAbout) return;
}

// Processing [TODO : generate cmd with variables] 
void CRTestWindow::DoConfigure(){
	TString cmd = "mkdir -p ";
	cmd += TString(fBuildDir->GetString());
	gSystem->Exec(cmd.Data());
}

void CRTestWindow::DoBuild(){
	DoConfigure();
	TString cmd = "cd ";
	cmd = cmd + TString(fBuildDir->GetString()) + TString(";");
	
	TGLBEntry* buildType = NULL;
	if(fOptions->find("Build_Type") != fOptions->end())
		buildType = fOptions->find("Build_Type")->second->GetSelectedEntry();
	if(buildType)
		cmd = cmd + TString("cmake -DCMAKE_BUILD_TYPE=")
		+TString(((TGTextLBEntry*)buildType)->GetText()->GetString()) + TString(" ")
		+TString(gSystem->pwd()) + TString(";");
		// TODO : find or input source dir?
	cmd = cmd + TString("make -j");

	gSystem->Exec(cmd.Data());
}

void CRTestWindow::DoRun(){
	TString cmd = TString(fBuildDir->GetString());
	cmd += TString("/CRTest");	
	gSystem->Exec(cmd.Data());
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
	TString fcn = "sin(" + to_string(fDrawParam->GetNumber())
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
