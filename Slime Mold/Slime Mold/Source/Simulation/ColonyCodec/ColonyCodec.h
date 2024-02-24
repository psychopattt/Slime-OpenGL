#pragma once

#include <string>
#include <vector>

using std::string, std::vector;

struct SpeciesSettings;

class ColonyCodec
{
	public:
		string EncodeColony(vector<SpeciesSettings> colony) const;
		vector<SpeciesSettings> DecodeColony(string colonyString) const;

	private:
		string FormatNumber(int number) const;
		string FormatNumber(float number) const;
		vector<string> SplitString(string str, char delimiter) const;
};
