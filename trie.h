#include <string>
// define your classes here...
class TrieNode;

class Trie{
    public:
    Trie();
    ~Trie();

    void load();
    bool insert(const std::string& word);
    void count(const std::string& word);
    int countHelper(TrieNode* node);
    void erase(const std::string& word);
    void recursiveErase(TrieNode* current, const std::string& word, int depth);
    bool hasChildren(TrieNode* node);
    void print();
    void printHelper(TrieNode* node, const std::string& prefix);
    void spellcheck(const std::string& word);
    void suggestions(const std::string& prefix, TrieNode* node);
    void empty();
    bool clear();
    void deleteNode(TrieNode* node);
    void size();
    void exit();

    private:
    TrieNode* root;
    int numWord;

};

class TrieNode{
    public:
    TrieNode* get_next(int index);
    bool get_EndWord();
    void set_next(int index, TrieNode* new_next);
    void set_EndWord(bool new_EndWord);
    TrieNode();

    private:
    TrieNode* next[26];
    bool EndWord;
    

};