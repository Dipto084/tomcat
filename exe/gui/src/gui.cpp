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
  vector<ASRWidgetFrameManipulator*> list;  
  void Update();
  ASRWidgetFrameManipulator *manipulator1;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
  MyFrame1 *frame = new MyFrame1(NULL);
  
  // Generate ASRWidgets
  vector<string> asr_topics = {"agent/asr/final", "agent/asr/intermediate"};
  
  queue<string> *queue1 = new queue<string>();
  mutex *mutex1 = new mutex();
  ASRWidget *widget1 = new ASRWidget(asr_topics, "x202201101", queue1, mutex1);
  this->manipulator1 = new ASRWidgetFrameManipulator(queue1, mutex1, frame);
  
  queue<string> *queue2 = new queue<string>();
  mutex *mutex2 = new mutex();
  ASRWidget *widget2 = new ASRWidget(asr_topics, "x202201102", queue2, mutex2);
  ASRWidgetFrameManipulator *manipulator2 = new ASRWidgetFrameManipulator(queue2, mutex2, frame);
  
  queue<string> *queue3 = new queue<string>();
  mutex *mutex3 = new mutex();
  ASRWidget *widget3 = new ASRWidget(asr_topics, "x202201103", queue3, mutex3);
  ASRWidgetFrameManipulator *manipulator3 = new ASRWidgetFrameManipulator(queue3, mutex3, frame);
  

  // Show frame
  frame->Show();
  frame->Maximize();  
  frame->Refresh();
	
  Connect( wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MyApp::onIdle) ); 
  return true;
}

void MyApp::Update(){
	this->manipulator1->Update();
}

void MyApp::onIdle(wxIdleEvent& evt)
{
        this->Update();
	evt.RequestMore(); // render continuously, not only once on idle
}
