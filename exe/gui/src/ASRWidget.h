#pragma once 

#include <vector>
#include <string>
#include <queue>
#include <mutex>

#include <wx/wxprec.h>
#include <wx/xrc/xmlres.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "MyFrame1.h"

#include "Widget.h"

class ASRWidget : public Widget {
	
	public:
		inline static const std::string type = "ASRWidget";
		inline static int current_id = 1;
		int id = -1;
	public:
		ASRWidget();
		~ASRWidget();

		void Update(MyFrame1 *frame) override;

	protected:
		void on_message(const std::string& topic,
                    const std::string& message) override;

	private:
		std::mutex mutex;
		std::queue<std::string> queue;
		
		std::string playername;
		int component_id;

	private:
		void UpdatePrivate(std::string, MyFrame1 *frame);
};

