// Решите загадку: Сколько чисел от 1 до 1000 содержат как минимум одну цифру 3?
// Напишите ответ здесь:
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
        } else {
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


class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const string& document) {
        vector<string> words = SplitIntoWordsNoStop(document);
        for(string& word : words){
            if(word_to_document_freqs_.count(word) != 0){
                word_to_document_freqs_[word][document_id] += (static_cast<double>(word_to_document_freqs_.count(word))/ words.size());

            }
            else{
                word_to_document_freqs_[word][document_id] = (static_cast<double>(word_to_document_freqs_.count(word) + 1.0)/ words.size());
            }
            
        }
        document_count_++;
        //map<string, map<int, double>> word_to_document_freqs_;
        // word_to_document_freqs_[word][document_id] = 0.0;
    }
    void showData() const{
        for(auto& [key , value] : word_to_document_freqs_){
            for(auto [document_id, TF] : value){
                cout << "word : "s << key << " document_id : "s << document_id << " TF : " << TF << endl;
            }
        }
        cout << "Document count = " << document_count_ << endl;
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query);

        sort(matched_documents.begin(), matched_documents.end(), 
             [](const Document& lhs, const Document& rhs) -> bool {
    return lhs.relevance > rhs.relevance;
});
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }


private:
        struct Query{
            set <string> minus_words;
            set <string> plus_words;
        };
    map<string, map<int, double>> word_to_document_freqs_;

    set<string> stop_words_;   
    int document_count_ = 0;

    bool checkMinusWord(string& word) const{
        if(word[0] == '-'){
            return true;
        }
        return false;
    }

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

    Query ParseQuery(const string& text)const {
        
        Query query;
        for(const string& word : SplitIntoWordsNoStop(text)) {
                if(word[0] == '-'){
                    string temp = word.substr(1);
                    query.minus_words.insert(temp);
                }
                else{
                    query.plus_words.insert(word);
                }
        }
        return query;
    }
        //map<string, map<int, double>> word_to_document_freqs_;
        // word_to_document_freqs_[word][document_id] = 0.0;
    vector<Document> FindAllDocuments(Query query) const {
        map<int, double> document_to_relevance;
        vector<Document> matched_documents;
        for (const string& plusW : query.plus_words) {
            if(word_to_document_freqs_.count(plusW) != 0){
                for(const auto& [id , tf] : word_to_document_freqs_.at(plusW)){
                    document_to_relevance[id] += tf * (log(document_count_ / static_cast<double>(word_to_document_freqs_.at(plusW).size())));
                }
            }
        }
        
        for(const string& minusW : query.minus_words){
            if(word_to_document_freqs_.count(minusW) != 0){
                for(const auto& [id, tf] : word_to_document_freqs_.at(minusW)){
                    document_to_relevance.erase(id);
                }
            }
        }
        
        for (const auto& [id , relevance] : document_to_relevance) {
            matched_documents.push_back({id , relevance});
        }

        return matched_documents;

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
// Закомитьте изменения и отправьте их в свой репозиторий.
