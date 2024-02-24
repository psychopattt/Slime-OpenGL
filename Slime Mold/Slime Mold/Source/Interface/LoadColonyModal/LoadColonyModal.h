#pragma once

#include <string>

using std::string;

class LoadColonyModal
{
	public:
		void Render();
		bool HasNewResult() const;
		const string& GetResult();

	private:
		bool hasNewResult = false;
		char input[512] = "";
		string result = "";

		void ExitPopup(bool hasNewResult);
};
