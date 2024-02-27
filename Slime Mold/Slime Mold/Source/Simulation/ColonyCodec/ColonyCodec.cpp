#include "ColonyCodec.h"

#include <sstream>

#include "Settings/LogString/LogString.h"
#include "Settings/MainSettings.h"
#include "Settings/SlimeMoldSettings.h"
#include "Settings/Colony/ColonySettings.h"

using std::string, std::to_string, std::vector;

string ColonyCodec::FormatNumber(int number) const
{
	return to_string(number);
}

string ColonyCodec::FormatNumber(float number) const
{
	string str = to_string(number);
	str.erase(str.find_last_not_of('0') + 1);
	str.erase(str.find_last_not_of('.') + 1);

	return str;
}

string ColonyCodec::EncodeColony(const vector<SpeciesSettings>& colony) const
{
	string colonyString =
		FormatNumber(SlimeMoldSettings::DiffuseRate) + paramDelimiter +
		FormatNumber(SlimeMoldSettings::DecayRate) + speciesDelimiter;

	for (const SpeciesSettings& species : colony)
	{
		if (!species.enabled)
			continue;

		for (size_t channelId = 0; channelId < 3; channelId++)
			colonyString += FormatNumber(species.mainColor[channelId]) + paramDelimiter;

		for (size_t channelId = 0; channelId < 3; channelId++)
			colonyString += FormatNumber(species.edgeColor[channelId]) + paramDelimiter;

		colonyString +=
			FormatNumber(species.moveSpeed) + paramDelimiter +
			FormatNumber(species.turnSpeed) + paramDelimiter +
			FormatNumber(species.sensorSize) + paramDelimiter +
			FormatNumber(species.sensorOffset) + paramDelimiter +
			FormatNumber(species.sensorAngleDegrees) + paramDelimiter +
			FormatNumber(species.trailWeight) + paramDelimiter +
			FormatNumber(species.cellCount) + paramDelimiter +
			FormatNumber(species.spawnMode) + speciesDelimiter;
	}

	colonyString.pop_back();
	return colonyString;
}

vector<string> ColonyCodec::SplitString(const string& str, char delimiter) const
{
	string token;
	vector<string> tokens;
	std::stringstream stream(str);

	while (!stream.eof())
	{
		std::getline(stream, token, delimiter);
		tokens.push_back(token);
	}

	return tokens;
}

vector<SpeciesSettings> ColonyCodec::DecodeColony(const string& colonyString) const
{
	try
	{
		vector<SpeciesSettings> colony(Colony.size());
		vector<string> colonyParams = SplitString(colonyString, speciesDelimiter);
		size_t colonySize = std::min(colony.size(), colonyParams.size() - 1);

		for (size_t i = 0; i < colonySize; i++)
		{
			SpeciesSettings species = SpeciesSettings();
			vector<string> speciesParams = SplitString(colonyParams[i + 1], paramDelimiter);

			for (size_t channelId = 0; channelId < 3; channelId++)
				species.mainColor[channelId] = stof(speciesParams.at(channelId));

			for (size_t channelId = 0; channelId < 3; channelId++)
				species.edgeColor[channelId] = stof(speciesParams.at(channelId + 3));

			species.moveSpeed = stof(speciesParams.at(6));
			species.turnSpeed = stof(speciesParams.at(7));
			species.sensorSize = stoi(speciesParams.at(8));
			species.sensorOffset = stof(speciesParams.at(9));
			species.sensorAngleDegrees = stof(speciesParams.at(10));
			species.trailWeight = stof(speciesParams.at(11));
			species.cellCount = stoi(speciesParams.at(12));
			species.spawnMode = SpawnMode(stoi(speciesParams.at(13)));
			species.enabled = true;
			colony[i] = species;
		}
		
		for (int i = 0; i < Colony.size(); i++)
		{
			colony[i].wasEnabled = Colony[i].wasEnabled;

			for (int j = 0; j < std::size(colony[i].mask); j++)
				colony[i].mask[j] = Colony[i].mask[j];
		}

		vector<string> globalParams = SplitString(colonyParams.at(0), paramDelimiter);
		SlimeMoldSettings::DiffuseRate = stof(globalParams.at(0));
		SlimeMoldSettings::DecayRate = stof(globalParams.at(1));

		return colony;
	}
	catch (...)
	{
		MainSettings::Log << "Colony Codec Error - Failed to decode colony\n" <<
			"Colony: " << colonyString << "\n\n";

		return vector<SpeciesSettings>();
	}
}
