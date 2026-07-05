#pragma once

#include <string>

class LoadColonyModal
{
	public:
		void Render();
		void Show();
		bool HasNewResult() const;
		const std::string& GetResult();

	private:
		bool hasNewResult = false;
		bool visible = false;
		char input[512] = "";
		std::string result = "";

		void ExitPopup(bool hasNewResult);
};
