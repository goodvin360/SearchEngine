#include "InvertedIndex.h"


std::mutex myMutex;

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs)
{
    docs = input_docs;
/*        for (auto it2:docs)
    {
        std::cout << it2 << std::endl;
        std::cout << std::endl;
    }
    */
}

bool InvertedIndex::characterCondition(char &symbol)
{
    if (symbol!=' ' && symbol!='\0' && symbol!=',' && symbol!='.'
        && symbol!='!' && symbol!='?' && symbol!=';' && symbol!=':')
    {
        return true;
    } else return false;
}

bool InvertedIndex::wordCondition(char &symbol, char &nextSymbol) {
    if (symbol==' '|| nextSymbol=='\0' || symbol==',' || symbol=='.'
        || symbol=='!' || symbol=='?' || symbol==';' || symbol==':')
    {
        return true;
    } else return false;
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word)
{
    std::vector<Entry> wordData{};
    for (int i=0; i<docs.size(); i++)
    {
        Entry wordEntry;
        wordEntry.doc_id = i;
        wordEntry.count=0;

        int j=0;
        std::string tempWord;
        while (docs[i][j])
        {
            if (characterCondition(docs[i][j]))
            {
                tempWord.push_back(docs[i][j]);
            }
            if (wordCondition(docs[i][j],docs[i][j+1]))
            {
                if (std::strcmp(word.c_str(),tempWord.c_str())==0)
                    wordEntry.count++;
                tempWord.clear();
            }
            j++;
        }
        if (wordEntry.count!=0)
            wordData.push_back(wordEntry);
    }
    return wordData;
}

void InvertedIndex::freqDictInfillThread(std::string &textFromDoc)
{
    /*myMutex.lock();
    std::cout << "This is thread number: " << std::this_thread::get_id() << std::endl;
    myMutex.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(2));*/
        int i=0;
        std::string singleWord;
        while (textFromDoc[i])
        {
            if (characterCondition(textFromDoc[i]))
            {
                singleWord.push_back(textFromDoc[i]);
            }
            if (wordCondition(textFromDoc[i], textFromDoc[i+1]))
            {
                freq_dictionary.insert({singleWord, GetWordCount(singleWord)});
                /*std::cout << test << '\t';
                std::cout << std::endl;*/
                singleWord.clear();
            }
            i++;
        }
}

void InvertedIndex::dataMerge()
{
    for (auto it:classParts)
    {
        for (auto it2 = it.freq_dictionary.begin(); it2!=it.freq_dictionary.end(); it2++)
        {
            freq_dictionary.insert({it2->first,it2->second});
        }
    }
}

std::map<std::string, std::vector<Entry>> *InvertedIndex::getFreqDictionary()
{
    return &freq_dictionary;
}

void InvertedIndex::threadsDistribution()
{
    for (auto it:docs)
    {
        InvertedIndex tempPart(jsonData);
        tempPart.UpdateDocumentBase(docs);
        classParts.push_back(tempPart);
    }

#ifdef SingleThread
    for (int i = 0; i < classParts.size(); i++)
    {
        classParts[i].freqDictInfillThread(docs[i]);
    }
#endif

#ifdef MultiThread
    std::vector<std::thread> ThreadVector;
    for (int i = 0; i < classParts.size(); i++) {
        ThreadVector.emplace_back([&, i]() {
            classParts[i].freqDictInfillThread(docs[i]);
        });
    }

    for(auto& t: ThreadVector)
    {
        t.join();
    }
#endif

    dataMerge();
}





