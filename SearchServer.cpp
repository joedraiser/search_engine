#include "SearchServer.h"

std::vector<Entry>::iterator findDocid(std::vector<Entry>& array, int doc_id)
{
    int index=0;

    for(auto currentEntry = array.begin(); currentEntry!=array.end(); currentEntry++)
    {
        if(currentEntry->doc_id==doc_id)
            return currentEntry;
    }

return array.end();
}

void fillArrayTotalEntriesForQuery(const std::string& query, std::vector<Entry>& arrayToFill, InvertedIndex invIndex)
{
    std::stringstream ss;
    std::string requestWord;
    ss << query;
    std::vector<Entry> wordToCheckEntriesCount;
    std::vector<Entry>::iterator currentDocidEntry;

    while(!ss.eof())
    {
        ss >> requestWord;
        wordToCheckEntriesCount = invIndex.GetWordCount(requestWord);

        for(auto&& entryInDocid : wordToCheckEntriesCount)
        {
            currentDocidEntry = findDocid(arrayToFill, entryInDocid.doc_id);

            if(currentDocidEntry != arrayToFill.end())
                currentDocidEntry->count+=entryInDocid.count;
            else
                arrayToFill.push_back( Entry()={entryInDocid.doc_id, entryInDocid.count} );
        }
    }
}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> &queries_input)
{
    std::stringstream ss;
    std::string requestWord;
    std::vector<Entry> totalFreq;
    std::vector<Entry> wordToCheckEntriesCount;

    std::vector<RelativeIndex> currentQuery;
    std::vector< std::vector<RelativeIndex> > overall;
    int biggestDocid=0, biggestCount=0, maxAbsRelevancy=0;
    std::vector<Entry>::iterator toErase;
    int maxResponses, responsesCount=0;
    maxResponses=ConverterJSON().GetResponsesLimit();

    for(auto&& query : queries_input)
    {
        fillArrayTotalEntriesForQuery(query, totalFreq, _index);

        for(auto&& entry : totalFreq)
        {
            if(entry.count>maxAbsRelevancy)
                maxAbsRelevancy=entry.count;
        }

        while(!totalFreq.empty())
        {
            for(auto&& entry : totalFreq)
            {
                if(entry.count>biggestCount)
                {
                    biggestCount=entry.count;
                    biggestDocid=entry.doc_id;

                    toErase = findDocid(totalFreq, biggestDocid);
                }
                else if(entry.count==biggestCount)
                {
                    if(entry.doc_id<biggestDocid)
                    {
                        biggestDocid=entry.doc_id;
                        toErase = findDocid(totalFreq, biggestDocid);
                    }
                }
            }
            currentQuery.push_back(RelativeIndex() = {(size_t)biggestDocid, (float)biggestCount / maxAbsRelevancy});
            totalFreq.erase(toErase);
            responsesCount++;
            if(responsesCount==maxResponses)
                break;
            biggestCount=0;
            biggestDocid=0;
        }
        overall.push_back(currentQuery);
        currentQuery.clear();
        maxAbsRelevancy=0;
    }

    ConverterJSON().putAnswers(overall);

    return overall;
}
