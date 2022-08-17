#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main()
{
    ConverterJSON someStuff;

    auto resources = someStuff.GetTextDocuments();
    auto requests = someStuff.GetRequests();

    //std::vector<std::vector<std::pair<int, float>>> answers;

    //std::vector<std::pair<int, float>> a, b, c;
    //a.push_back(std::pair<int, float> (0, 0.892));
    //a.push_back(std::pair<int, float> (1, 0.452));
    //a.push_back(std::pair<int, float> (2, 0.231));

    //b.push_back(std::pair<int, float> (0, 0.777));

    //answers.push_back(a);
    //answers.push_back(c);
    //answers.push_back(b);

    //someStuff.putAnswers(answers);

    InvertedIndex invertedVar;
    invertedVar.UpdateDocumentBase(resources);
    std::vector<Entry> totalFreq;
    fillArrayTotalEntriesForQuery("fish milk bread", totalFreq, invertedVar);

    return 0;
}
