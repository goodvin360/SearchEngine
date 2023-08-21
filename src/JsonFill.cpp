#include "JsonFill.h"

void JsonFill::fillFile(std::string type)
{
    if (type=="config")
    {
        configExample.config.insert({"name", "SkillboxSearchEngine"});
        configExample.config.insert({"version", "0.1"});
        configExample.config.insert({"max_responses", "5"});
        configExample.files.push_back("../resources/file001.txt");
        configExample.files.push_back("../resources/file002.txt");
        configExample.files.push_back("../resources/file003.txt");
        configExample.files.push_back("../resources/file004.txt");
    }
    else if(type=="request")
    {
        requestExample.requests.push_back("milk water");
        requestExample.requests.push_back("sugar");
        requestExample.requests.push_back("It pepper salt milk number one saw");
    }
}

void JsonFill::printFile(std::string type, std::string path)
{
    if (type=="config")
    {
        std::ofstream file(path, std::ios::out);
        nlohmann::json configFile;
        configFile["config"] = configExample.config;
        configFile["files"] = configExample.files;
        std::string toOut;
        toOut = configFile.dump(4);
        file << std::setw(4) << toOut << std::endl;
        //    file << std::setw(4) << configFile << std::endl;

    }
    else if(type=="request")
    {
        std::ofstream file(path, std::ios::out);
        nlohmann::json requestFile;
        requestFile["requests"] = requestExample.requests;
        file << std::setw(4) << requestFile << std::endl;
    }

}
