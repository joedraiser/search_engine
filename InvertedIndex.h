#ifndef SEARCH_ENGINE_INVERTEDINDEX_H
#define SEARCH_ENGINE_INVERTEDINDEX_H

#include <cstddef>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <thread>
#include <mutex>

struct Entry {
    size_t doc_id, count;

    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id &&
                count == other.count);
    }
};

class InvertedIndex {
public:
    InvertedIndex() = default;
    InvertedIndex(InvertedIndex &idx)
    {
        docs=idx.docs;
        freq_dictionary=idx.freq_dictionary;
    };
/**
* Обновить или заполнить базу документов, по которой будем совершать
поиск
* @param texts_input содержимое документов
*/
    void UpdateDocumentBase(std::vector<std::string> input_docs);
/**
 * * Метод определяет количество вхождений слова word в загруженной базе
документов
* @param word слово, частоту вхождений которого необходимо определить
* @return возвращает подготовленный список с частотой слов
*/
    std::vector<Entry> GetWordCount(const std::string& word);
    std::vector<std::string>& getDocs() {return docs;};
private:
    std::vector<std::string> docs; // список содержимого документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь
};

#endif //SEARCH_ENGINE_INVERTEDINDEX_H
