#include <vector>
#include <string>
#include <iostream>
#include <queue>
#include <mutex>

#include <nlohmann/json.hpp>

#include "MyFrame1.h"

#include "ASRWidget.h"

using namespace std;

ASRWidget::ASRWidget(vector<string> topics, string playername, MyFrame1 *frame) : MosquittoClient(topics){
	this->playername = playername;
	this->frame = frame;
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
	this->mutex.lock();
		this->queue.push(message_text);
	this->mutex.unlock();

}

void ASRWidget::Update(){
	this->mutex.lock();
		if(this->queue.empty()){
			this->mutex.unlock();
			return;
		}
		std::string text = this->queue.front();
		this->queue.pop();
	this->mutex.unlock();
	this->UpdatePrivate(text);
}

void ASRWidget::UpdatePrivate(string text){
	std::cout << text << std::endl;
	this->frame->m_staticText1->SetLabel(text);
}
