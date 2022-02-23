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
#include "Widget.h"
#include "ASRWidget.h"

using namespace std;

class MyApp : public wxApp {
public:
  virtual bool OnInit();
  void onIdle(wxIdleEvent& evt);
  void Update();
  
  MyFrame1 *frame;
  vector<Widget *> widgets;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
  this->frame = new MyFrame1(NULL);
  
  // Generate ASRWidgets 
  this->widgets.push_back(new ASRWidget());
  this->widgets.push_back(new ASRWidget());
  this->widgets.push_back(new ASRWidget());

  // Show frame
  this->frame->Show();
  this->frame->Maximize();  
  this->frame->Refresh();
	
  Connect( wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MyApp::onIdle) ); 
  return true;
}

void MyApp::Update(){
	for(int i=0;i<widgets.size();i++){
		widgets[i]->Update(this->frame);
	}
}

void MyApp::onIdle(wxIdleEvent& evt)
{
        this->Update();
	evt.RequestMore(); // render continuously, not only once on idle
}
