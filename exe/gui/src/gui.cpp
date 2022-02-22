// wxWidgets "Hello World" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#include <wx/xrc/xmlres.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <memory>
#include <string>
#include <vector>

#include "MyFrame1.h"

#include "ASRWidget.h"

using namespace std;

class MyApp : public wxApp {
public:
  virtual bool OnInit();
  void onIdle(wxIdleEvent& evt);
  void Update();
  vector<ASRWidget *> list;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
  MyFrame1 *frame = new MyFrame1(NULL);
  
  // Generate ASRWidgets
  vector<string> asr_topics = {"agent/asr/final", "agent/asr/intermediate"};
  
  ASRWidget *widget1 = new ASRWidget(asr_topics, "x202201101", frame);
  ASRWidget *widget2 = new ASRWidget(asr_topics, "x202201102", frame);
  ASRWidget *widget3 = new ASRWidget(asr_topics, "x202201103", frame);
  list.push_back(widget1);
  list.push_back(widget2);
  list.push_back(widget3);

  // Show frame
  frame->Show();
  frame->Maximize();  
  frame->Refresh();
	
  Connect( wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MyApp::onIdle) ); 
  return true;
}

void MyApp::Update(){
	for(int i=0;i<list.size();i++){
		list[i]->Update();
	}
}

void MyApp::onIdle(wxIdleEvent& evt)
{
        this->Update();
	evt.RequestMore(); // render continuously, not only once on idle
}
