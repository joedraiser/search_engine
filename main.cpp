#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main()
{
    ConverterJSON someStuff;

    auto resources = someStuff.GetTextDocuments();
    auto requests = someStuff.GetRequests();

    InvertedIndex invertedVar;
    invertedVar.UpdateDocumentBase(resources);
    std::vector<Entry> totalFreq;
    fillArrayTotalEntriesForQuery("fish milk bread", totalFreq, invertedVar);

    return 0;
}
