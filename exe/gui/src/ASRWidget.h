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

#include "MosquittoClient.h"

class ASRWidget : public MosquittoClient {
	
	public:
		ASRWidget(std::vector<std::string> topics, std::string playername, std::queue<std::string> *queue, std::mutex *mutex);
		~ASRWidget();

	protected:
		void on_message(const std::string& topic,
                    const std::string& message) override;

	private:
		std::mutex *mutex;
		std::queue<std::string> *queue;
		std::string playername;
};

class ASRWidgetFrameManipulator {
	public:
		ASRWidgetFrameManipulator(std::queue<std::string> *queue, std::mutex *mutex, MyFrame1 *frame);
		~ASRWidgetFrameManipulator();

		void Update();
		
	private:
		std::queue<std::string> *queue;
		std::mutex *mutex;
		MyFrame1 *frame;

		void UpdatePrivate(std::string text);
};
