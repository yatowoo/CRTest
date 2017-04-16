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
#include "TGComboBox.h"
#include "TGTextView.h"
#include "TGLabel.h"
#include "TGStatusBar.h"
#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"

#include "TF1.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TRint.h"

class CRTestWindow : public TGObject {
	RQ_OBJECT("CRTestWindow")

private:
	TGMainFrame         *fMain;
	TGCompositeFrame    *fF1;
	TGCompositeFrame    *fFcanvasL;
	TGTab               *fTab;
	TGLayoutHints       *fLayout;
	TGLayoutHints       *fL3;
	TGLayoutHints       *fLCanvas;
	TGTextEntry         *fTxt;
	TGTextEntry         *fTent;
	TGTextEntry         *fCommand;
	TGComboBox          *fComboCmd;
	TGTextView          *fTextView;
	TString              fName;
	TGLabel             *fLabel;
	TRootEmbeddedCanvas *fCanvas;
	TGStatusBar         *fStatusBar;
public:
	CRTestWindow(const TGWindow* p, UInt_t w, UInt_t h);
	virtual ~CRTestWindow();
	const char* GetText(){return fTxt->GetText();}
	void DoTextChanged();
	void DoDraw();
	void DoClear();
	void DoEnteredCommand();
	// for Control buttons
	void DoConfigure();
	void DoBuild();
	void DoRun();
};

CRTestWindow::CRTestWindow(const TGWindow* p, UInt_t w, UInt_t h){

	fMain = new TGMainFrame(p,w,h,kVerticalFrame);

	fMain->SetCleanup(kDeepCleanup);

	//
	// FRAME - for tabs
	//
	fTab = new TGTab(fMain, 300, 300);
	//fTab->Connect("Selected(Int_t)", "TestDialog", this, "DoTab(Int_t)");

	fL3 = new TGLayoutHints(kLHintsTop | kLHintsLeft , 5, 5, 5, 5);
	TGCompositeFrame *tf = fTab->AddTab("File");
	fF1 = new TGCompositeFrame(tf, 60, 20, kHorizontalFrame);

	fF1->AddFrame(fTxt = new TGTextEntry(fF1, new TGTextBuffer(100)), fL3);
	tf->AddFrame(fF1, fL3);
	fTxt->Resize(150, fTxt->GetDefaultHeight());
	fTxt->Connect("ReturnPressed()","CRTestWindow",this,"DoTextChanged()");

	TGTextButton *tb1 = new TGTextButton(fF1, "&Clear", 0);
	tb1->Connect("Clicked()","CRTestWindow",this,"DoClear()");
	fF1->AddFrame(tb1, fL3);

	tf = fTab->AddTab("Info");
	fF1 = new TGCompositeFrame(tf, 60, 20, kVerticalFrame);
	fLabel = new TGLabel(fF1,"Normal Label");
	fF1->AddFrame(fLabel, fL3);
	tf->AddFrame(fF1, fL3);
	// Tab - Canvas
	tf = fTab->AddTab("Canvas");
	fF1 = new TGCompositeFrame(tf, 60, 20, kHorizontalFrame);

	fFcanvasL = new TGCompositeFrame(tf, 150, 100, kVerticalFrame);
	fTent = new TGTextEntry(fFcanvasL, new TGTextBuffer(50));
	fFcanvasL->AddFrame(fTent, fL3);

	TGTextButton* tb = new TGTextButton(fFcanvasL,"&Draw",0);
	tb->Connect("Clicked()","CRTestWindow",this,"DoDraw()");
	fFcanvasL->AddFrame(tb, fL3);

	fF1->AddFrame(fFcanvasL, fL3);
	// Embedded Canvas
	fCanvas = new TRootEmbeddedCanvas("MyCanvas",tf,200,200);
	fLCanvas = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 10,10,10,1);
	fF1->AddFrame(fCanvas, fLCanvas);

	tf->AddFrame(fF1, fLCanvas);
	fTent->Resize(50, fTent->GetDefaultHeight());
	tb->Resize(tb->GetDefaultWidth(), tb->GetDefaultHeight());

	//
	// FRAME - Create a horizontal frame containing button(s)
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
	gSystem->Exec("mkdir -p .build;cd .build;cmake ../code;make -j4;./CRTest");
}

void CRTestWindow::DoTextChanged(){
	cout << fTxt->GetText() << endl;
	fTxt->Clear();
}

void CRTestWindow::DoDraw(){
	TF1 *f1 = new TF1("f1","sin(x)/x",0,gRandom->Rndm()*10);
	f1->SetLineWidth(3);
	f1->Draw();
	TCanvas *canvas = fCanvas->GetCanvas();
	canvas->cd();
	canvas->Update();
}

void CRTestWindow::DoEnteredCommand()
{
	// Execute the CINT command after the ENTER key was pressed.

	const char *command = fCommand->GetTitle();
	TString prompt;

	if (strlen(command)) {
		// form temporary file path
		prompt = ((TRint*)gROOT->GetApplication())->GetPrompt();
		FILE *cintout = fopen(fName.Data(), "a+t");
		if (cintout) {
			fputs(Form("%s%s\n",prompt.Data(), command), cintout);
			fclose(cintout);
		}
		gSystem->RedirectOutput(fName.Data(), "a");
		gROOT->ProcessLine(command);
		fComboCmd->InsertEntry(command, 0, -1);
		Gl_histadd((char *)command);
		gSystem->RedirectOutput(0);
		fTextView->LoadFile(fName.Data());
		if (fTextView->ReturnLineCount() > 10)
			fTextView->SetVsbPosition(fTextView->ReturnLineCount());
		fCommand->Clear();
	} else {
		printf("No command entered\n");
	}
	fTextView->ShowBottom();
}

void CRTest_GUI(){
	CRTestWindow* yatowin = new CRTestWindow(gClient->GetRoot(),400,300);
	cout << yatowin << endl;
}

#endif // CRTest_WINDOW
