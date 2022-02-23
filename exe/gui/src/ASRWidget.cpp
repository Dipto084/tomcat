#include <vector>
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <queue>
#include <mutex>

#include <nlohmann/json.hpp>

#include "MyFrame1.h"

#include "ASRWidget.h"

using namespace std;

ASRWidget::ASRWidget() : Widget(this->type){
	// Update ID
	this->id = this->current_id;
	this->current_id++;
	
	// Get data from configuration
	this->playername = this->configuration[to_string(this->id)]["playername"];
	this->component_id = this->configuration[to_string(this->id)]["component_id"];
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

	// Check for playername
	if(response["data"].contains("participant_id")){
		if(response["data"]["participant_id"] == this->playername){	
			string message_text = response["data"]["text"];
			this->mutex.lock();
				this->queue.push(message_text);
			this->mutex.unlock();
		}
	}
	else{
		std::cout << response.dump() << std::endl;
	}
}

void ASRWidget::Update(MyFrame1 *frame){
	this->mutex.lock();
		if(this->queue.empty()){
			this->mutex.unlock();
			return;
		}
		std::string text = this->queue.front();
		this->queue.pop();
	this->mutex.unlock();
	this->UpdatePrivate(text, frame);
}

void ASRWidget::UpdatePrivate(string text, MyFrame1 *frame){
	wxStaticText *static_text = (wxStaticText *)((*frame).FindWindow(this->component_id));
	if(static_text == nullptr){
		std::cout << "Failed to find component: " << this->component_id << " in type: " << this->type << std::endl;  
		return;
	}
	static_text->SetLabel(text);
}
