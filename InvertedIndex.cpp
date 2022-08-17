#include "InvertedIndex.h"

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs)
{
    docs=input_docs;
    for(auto freq_entry_for_each_string : freq_dictionary)
    {
        freq_entry_for_each_string.second = this->GetWordCount(freq_entry_for_each_string.first);
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word)
{
    if(freq_dictionary.find(word)!=freq_dictionary.end())
        return freq_dictionary[word];

    std::vector<Entry> entryArray;
    std::vector<std::thread> proccesingDocThreadArray;
    std::mutex entryArrayLock;
    int doc_id = 0;

    auto proccessDocFunc = [&entryArray, &entryArrayLock, word](std::string doc, int doc_id)
    {
        int wordCount=0;
        std::stringstream buff;
        std::string word_to_check;

        buff << doc;
        while(!buff.eof())
        {
            buff >> word_to_check;
            if(word_to_check == word)
            {
                wordCount++;
            }
        }
        if(wordCount!=0)
        {
            entryArrayLock.lock();
            entryArray.push_back(Entry() = {(size_t)doc_id, (size_t)wordCount});
            entryArrayLock.unlock();
        }
    };



    for(auto doc : docs)
    {
        proccesingDocThreadArray.push_back(std::thread(proccessDocFunc, doc, doc_id));
        doc_id++;
    }

    for(int i=0;i<doc_id;i++)
        proccesingDocThreadArray[i].join();

    freq_dictionary[word]=entryArray;

    return entryArray;
}