#pragma once 

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "MyFrame1.h"

#include "MosquittoClient.h"

class Widget : public MosquittoClient {
	public:
		Widget(std::string type);
		~Widget();

		virtual void Update(MyFrame1 *frame) = 0;
	protected:
		nlohmann::json configuration;
	private:
		std::string type;
		std::vector<std::string> topics;
	private:
		void LoadConfig();
		void ParseConfig();
		void Subscribe();	
};
