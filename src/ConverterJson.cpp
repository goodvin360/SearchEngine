#include "ConverterJson.h"

ConverterJson::ConverterJson()
{
    std::ifstream configRead("../config.json");
    if(!configRead.is_open())
    {
        throw failFileOpenException();
    }
    try
    {
        configRead >> configFile;
    }
    catch (const failFileOpenException &x)
    {
        std::cout << "Caught exception: can't open file" << std::endl;
    }
    configRead.close();

    readConfig(); //check for config field in the file and max.resp. limit

    std::ifstream requestRead("../request.json");
    requestRead >> requestFile;
    requestRead.close();
}

void ConverterJson::readConfig()
{
    auto it_find = configFile.find("config");
    if (it_find==configFile.end())
    {
        throw failConfigException();
    }
    try
    {
        filePaths.clear();
        auto it_files = configFile.find("files");
            if (it_files!=configFile.end())
            {
                for (auto it2 = it_files.value().begin(); it2!= it_files.value().end(); it2++)
                {
                    filePaths.push_back(it2.value());
    //                std::cout << it2.value() << std::endl;
                }
            }

    }
    catch (const failConfigException &x)
    {
        std::cout << "Can't find 'config' field in the file 'config.json'. Check this file." << std::endl;
    }

    ResponseLimit();
}

std::vector<std::string> ConverterJson::GetTextDocuments()
{
    textFromDocs.clear();

    for (auto it: filePaths)
    {
        int counts = 0;
        std::ifstream file(it);
        if (file.is_open())
        {
            std::string tmp1{}, tmp2{};
            while (!file.eof())
            {
                file >> tmp1;
                if (!tmp1.empty() && tmp1.length()<100 && counts < 1000)
                {
                    tmp2 += (" " + tmp1);
                    counts++;
                }
                else if (tmp1.length()>100)
                {
                    std::cout << "Oops! Some word in the " << it << " have length more then 100 symbols." << std::endl;
                    std::cout << "This word will be excluded from search base." << std::endl;
                }
            }
            if (counts >= 1000)
            {
                std::cout << "Oops! File " << it << " have more than 1000 words. Only first 1000 words will be used for searching." << std::endl;
            }
            if (!tmp2.empty())
            textFromDocs.push_back(tmp2);
            else
            {std::cout << "File ' " << it << " ' is empty. Please check." << std::endl;}
        }
        else
        {
            std::cout << "Can't open file ' " << it << " '. Please check data in 'config.json' and file address." << std::endl;
        }
    }

    if (textFromDocs.empty())
    {
        throw failEmptyDataBase();
    }
    try
    {
        return textFromDocs;
    }
    catch (const failEmptyDataBase &x)
    {
        std::cout << "All documents in search base are empty!" << std::endl;
    }

/*    for (auto it2:textFromDocs)
    {
        std::cout << it2 << std::endl;
        std::cout << std::endl;
    }*/


}

void ConverterJson::ResponseLimit()
{
    auto it_config = configFile.find("config");
    if (it_config != configFile.end())
    {
        auto it_resp = it_config->find("max_responses");
        if (it_resp!=it_config->end())
        {
            std::string tmp{};
            tmp = it_resp.value();
            respLimit = std::stoi(tmp);
        }
        else if (it_resp==it_config->end())
        {
            std::cout << "Can't find field 'max_response' in 'config.json'. " << std::endl;
            std::cout << "Default value of the responses is: " << respLimit << std::endl;
        }
    }
}

int ConverterJson::GetResponseLimit()
{
    int var = respLimit;
    return var;
}

std::vector<std::string> ConverterJson::GetRequests()
{
    requests.clear();
    for (auto it = requestFile.begin().value().begin(); it!= requestFile.begin().value().end(); it++)
    {
        if (requests.size()<1000)
            requests.push_back(it.value());
        else
        {
            std::cout << "Oops! There is more than 1000 requests in 'request.json'." << std::endl;
            std::cout << "Only first 1000 requests will be used for searching." << std::endl;
            break;
        }
    }
/*    for (auto it2:requests)
    {
        std::cout << it2 << std::endl;
        std::cout << std::endl;
    }*/
    return requests;
}

void ConverterJson::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers)
{
    std::ofstream file("../answers.json", std::ios::out);
    std::map<std::string,nlohmann::json> outerLayer;
    std::vector<std::pair<std::pair<std::string,int>,std::pair<std::string,float>>> vecData;
    for (int i=0; i<answers.size(); i++)
    {
        float check=0;
        for (int j=0; j<answers[i].size();j++)
        {
            check+=answers[i][j].second;
        }
        if (check>0)
            ans.flag = true;
        else ans.flag = false;

        nlohmann::json tmp;
        tmp[" result"] = ans.flag;

        if (ans.flag)
        {
            int var=0;
            for (int j = 0; j < answers[i].size(); j++)
            {
                if (answers[i][j].second>0)
                {
                    std::pair<std::string, int> pair1;
                    std::pair<std::string, float> pair2;
                    pair1.first = "docID";
                    pair1.second = answers[i][j].first;
                    pair2.first = "rank";
                    pair2.second = floorf(100*answers[i][j].second)/100;
                    vecData.push_back(std::make_pair(pair1, pair2));
                    var++;
                }
            }
            if (var>1)
                tmp["relevance"] = vecData;
            else
            {
                tmp["docID"] = answers[i].begin()->first;
                tmp["rank"] = answers[i].begin()->second;
            }
            vecData.clear();
        }

        std::stringstream request;
        request << "request00" << i+1;
        outerLayer.insert({request.str(),tmp});
    }
    answerFile["answers"] = outerLayer;
    file.precision(4);
    file << std::setw(4) << answerFile << std::endl;
}

const std::vector<std::string> ConverterJson::GetRequestsData()
{
    GetRequests();
    const std::vector<std::string> res = requests;
    return res;
}

int ConverterJson::GetFilesNum()
{
    numOfFiles = textFromDocs.size();
    return numOfFiles;
}

