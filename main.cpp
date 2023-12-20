#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        }
        else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

struct Document {
    int id;
    double relevance;
};

struct Query {
    set<string> plusword;
    set<string> minusword;
};

class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const string& document) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        double TF = 1.0 / words.size();
        ++document_count_;

        for (auto& word : words) {  
            word_to_document_freqs_[word][document_id] += TF;
        }
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query);

        sort(matched_documents.begin(), matched_documents.end(),
            [](const Document& lhs, const Document& rhs) {
                return lhs.relevance > rhs.relevance;
            });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

private:

    map<string, map<int, double>> word_to_document_freqs_;

    set<string> stop_words_;

    int document_count_ = 0;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    vector<Document> FindAllDocuments(const Query query) const {
        vector<Document> matched_documents;
        map<int, double> doc_for_relevance;    
        int count;
        double IDF;

        for (const string& word : query.plusword) {
            if (word_to_document_freqs_.count(word)) {
                count = word_to_document_freqs_.at(word).size();
                IDF = log(document_count_ / static_cast<double>(count));
                for (auto& [key, TF] : word_to_document_freqs_.at(word)) {
                    doc_for_relevance[key] += IDF * TF;
                }
            }
        }

        for (const string& word : query.minusword) {
            if (word_to_document_freqs_.count(word)) {
                for (const auto& [key, value] : word_to_document_freqs_.at(word)) {
                    doc_for_relevance.erase(key);
                }
            }
        }

        for (auto& [key, value] : doc_for_relevance) {
            matched_documents.push_back({ key, value });
        }
        return matched_documents;
    }

    Query ParseQuery(const string& text) const {
        Query temp;
        for (const string& word : SplitIntoWords(text)) {
            if (word[0] == '-') {
                if (!IsStopWord(word)) {
                    temp.minusword.insert(word.substr(1));
                }
            }
            else
                temp.plusword.insert(word);
        }
        return temp;
    }
};

SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        search_server.AddDocument(document_id, ReadLine());
    }

    return search_server;
}

int main() {
    const SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    for (const auto& [document_id, relevance] : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", "
            << "relevance = "s << relevance << " }"s << endl;
    }
}