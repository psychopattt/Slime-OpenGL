#include "ColonyCodec.h"

#include <sstream>

#include "Settings/LogString/LogString.h"
#include "Settings/MainSettings.h"
#include "Settings/SpeciesSettings.h"
#include "Settings/SlimeMoldSettings.h"

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

string ColonyCodec::EncodeColony(vector<SpeciesSettings> colony) const
{
	string colonyString =
		FormatNumber(SlimeMoldSettings::DiffuseRate) + "," +
		FormatNumber(SlimeMoldSettings::DecayRate) + "|";

	for (const SpeciesSettings& species : colony)
	{
		if (!species.enabled)
			continue;

		for (int channelId = 0; channelId < 3; channelId++)
			colonyString += FormatNumber(species.color[channelId]) + ",";

		colonyString +=
			FormatNumber(species.moveSpeed) + "," +
			FormatNumber(species.turnSpeed) + "," +
			FormatNumber(species.sensorSize) + "," +
			FormatNumber(species.sensorOffset) + "," +
			FormatNumber(species.sensorAngleDegrees) + "," +
			FormatNumber(species.trailWeight) + "," +
			FormatNumber(species.cellCount) + "," +
			FormatNumber(species.spawnMode) + "|";
	}

	colonyString.pop_back();
	return colonyString;
}

vector<string> ColonyCodec::SplitString(string str, char delimiter) const
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

vector<SpeciesSettings> ColonyCodec::DecodeColony(string colonyString) const
{
	try
	{
		vector<SpeciesSettings> colony(Colony.size());
		vector<string> colonyParams = SplitString(colonyString, '|');

		for (int i = 0; i < colonyParams.size() - 1; i++)
		{
			vector<string> speciesParams = SplitString(colonyParams[i + 1], ',');
			colony[i] = SpeciesSettings(
				{ stof(speciesParams.at(0)), stof(speciesParams.at(1)), stof(speciesParams.at(2)), 0 },
				stof(speciesParams.at(3)), stof(speciesParams.at(4)), stoi(speciesParams.at(5)),
				stof(speciesParams.at(6)), stof(speciesParams.at(7)), stof(speciesParams.at(8)),
				stoi(speciesParams.at(9)), SpawnMode(stoi(speciesParams.at(10))), true
			);
		}
		
		vector<string> globalParams = SplitString(colonyParams.at(0), ',');
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
