#pragma once

#include <string>
#include <vector>

using std::string, std::vector;

struct SpeciesSettings;

class ColonyCodec
{
	public:
		string EncodeColony(const vector<SpeciesSettings>& colony) const;
		vector<SpeciesSettings> DecodeColony(const string& colonyString) const;

	private:
		const char paramDelimiter = ',';
		const char speciesDelimiter = '|';

		string FormatNumber(int number) const;
		string FormatNumber(float number) const;
		vector<string> SplitString(const string& str, char delimiter) const;
};
