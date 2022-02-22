#include <vector>
#include <string>
#include <iostream>
#include <queue>
#include <mutex>

#include <nlohmann/json.hpp>

#include "MyFrame1.h"

#include "ASRWidget.h"

using namespace std;

ASRWidget::ASRWidget(vector<string> topics, string playername, std::queue<string> *queue, std::mutex *mutex) : MosquittoClient(topics){
	this->playername = playername;
	this->queue = queue;
	this->mutex = mutex;
}

ASRWidget::~ASRWidget(){

}

void ASRWidget::on_message(const std::string& topic, const std::string& message){

	// Parse JSON response
	nlohmann::json response;
	try{
		response = nlohmann::json::parse(message);
	}
	catch(exception e){
		std::cout << "Failed to parse ASR message" << std::endl;
		return;
	
	}
	
	string message_text = response["data"]["text"];
	this->mutex->lock();
		this->queue->push(message_text);
	this->mutex->unlock();

	// Update text in frame
	//std::cout << message_text << std::endl;
	//this->frame->m_staticText1->SetLabel(message_text.c_str());
	//this->static_text->Update();

}

ASRWidgetFrameManipulator::ASRWidgetFrameManipulator(std::queue<string> *queue, std::mutex *mutex, MyFrame1 *frame){
	this->queue = queue;
	this->mutex = mutex;
	this->frame = frame;
}
ASRWidgetFrameManipulator::~ASRWidgetFrameManipulator(){

}
void ASRWidgetFrameManipulator::Update(){
	this->mutex->lock();
		if(this->queue->empty()){
			this->mutex->unlock();
			return;
		}
		std::string text = this->queue->front();
		this->queue->pop();
	this->mutex->unlock();
	this->UpdatePrivate(text);
}
void ASRWidgetFrameManipulator::UpdatePrivate(string text){
	std::cout << text << std::endl;
	this->frame->m_staticText1->SetLabel(text);
}
