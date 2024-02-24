#pragma once

#include <string>
#include <vector>

using std::string, std::vector;

struct SpeciesSettings;

class ColonyCodec
{
	public:
		string EncodeColony(vector<SpeciesSettings> colony);
		vector<SpeciesSettings> DecodeColony(string colonyString);

	private:
		string FormatNumber(int number);
		string FormatNumber(float number);
		vector<string> SplitString(string str, char delimiter);
};
