#pragma once

#include <string>

using std::string;

class LoadColonyModal
{
	public:
		void Render();
		void Show();
		bool HasNewResult() const;
		const string& GetResult();

	private:
		bool hasNewResult = false;
		bool visible = false;
		char input[512] = "";
		string result = "";

		void ExitPopup(bool hasNewResult);
};
