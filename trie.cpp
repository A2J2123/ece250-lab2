#include "trie.h"
#include <iostream>
#include <string>
#include <fstream>

/* Trie functions */

// Constructor for the Trie class
Trie::Trie(){
    root = new TrieNode(); // Creating a new root node for the trie
    numWord = 0; // Initializing the number of words in the trie to zero
}

// Destructor for the Trie class
Trie::~Trie(){
    clear(); // Clearing the trie
    delete root; // Deleting the root node
}

// Function to load words from a file into the trie
void Trie::load(){
    std::ifstream file("corpus.txt"); // Opening the file "corpus.txt" for reading

    // Checking if the file is opened successfully
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open the file." << std::endl; // Displaying an error message if file opening fails
        std::exit(-1); // Exiting the program with an error code
    }

    std::string word; // Variable to store each word from the file

    // Reading words from the file and inserting them into the trie
    while (file >> word) {
        insert(word);
    }
    
    std::cout << "success" << std::endl; //Prints success message 

    // Closing the file
    file.close();
}

// Function to insert a word into the trie
bool Trie::insert(const std::string& word){
    TrieNode* current = root;

    // Iterating through each character of the word
    for (char c : word) {
        c = toupper(c); // Converting the character to uppercase
        int index = c - 'A'; // Calculating the index for the character in the trie node array

        // Checking if the next node for the character exists
        if (!current->get_next(index)) {
            current->set_next(index, new TrieNode()); // Creating a new node if it doesn't exist
        }
        current = current->get_next(index);
    }

    // Checking if the word already exists in the trie
    if(current->get_EndWord()){
        return false; // Displaying failure message if the word already exists
    } else {
        current->set_EndWord(true); // Marks end of word, adds to count of words, and prints success
        numWord++;
        return true;
    }
}

// Function to count the occurrences of a word or its prefixes in the trie
void Trie::count(const std::string& word){
    TrieNode* current = root;
    int count = 0;

    // Iterating through each character of the word
    for (char c : word) {
        c = toupper(c); // Converting the character to uppercase
        int index = c - 'A'; // Calculating the index for the character in the trie node array

        // Checking if the next node for the character exists
        if (current->get_next(index) == nullptr) {
            std::cout << "not found" << std::endl; // Displaying message if the word or prefix is not found
            return;
        }
        current = current->get_next(index); // Moving to the next node
    }

    count = countHelper(current); // Counting occurrences using a helper function

    // Displaying the count of occurrences
    if(count == 0){
        std::cout << "not found" << std::endl; // Displaying message if the word or prefix is not found
    } else {
        std::cout << "count is " << count << std::endl; // Displaying the count of occurrences
    }
}

// Helper function to count occurrences recursively
int Trie::countHelper(TrieNode* current){
    if(current == nullptr){
        return 0; // Returning zero if the current node is null
    }
    int count = 0; // Initializing the count of occurrences to zero

    if(current->get_EndWord()){
        count++; // Incrementing count if the current node marks the end of a word
    }

    // Recursively counting occurrences for each child node
    for(int x = 0; x < 26; x++){
        count += countHelper(current->get_next(x)); // Accumulating counts from child nodes
    }

    return count; // Returning the total count of occurrences
}

// Function to erase a word from the trie
void Trie::erase(const std::string& word){
    TrieNode* current = root;
    bool exists = true;

    // Iterating through each character of the word
    for (char c : word) {
        c = toupper(c); // Converting the character to uppercase
        int index = c - 'A'; // Calculating the index for the character in the trie node array

        // Checking if the next node for the character exists
        if (current->get_next(index) == nullptr) {
            exists = false;
            break;
        }
        current = current->get_next(index); // Moving to the next node
    }

    // Only attempt to erase the word if it exists in the trie
    if (exists && current->get_EndWord()) {
        recursiveErase(root, word, 0);
        std::cout << "success" << std::endl; // Displaying success message
    } else {
        std::cout << "failure" << std::endl; // Displaying failure message if the word does not exist
    }
}

// Helper function for recursive deletion
void Trie::recursiveErase(TrieNode* current, const std::string& word, int depth) {
    // Base case: reached the end of the word
    if (depth == word.length()) {
        if (current->get_EndWord()) {
            current->set_EndWord(false); // Unmark the end of the word
            numWord--; // Decrement the count of words in the trie
        }
        return;
    }

    // Recursive case: continue to the next character
    char c = toupper(word[depth]); // Convert the character to uppercase
    int index = c - 'A'; // Calculate the index for the character in the trie node array

    // Check if the next node for the character exists
    if (current->get_next(index) != nullptr) {
        recursiveErase(current->get_next(index), word, depth + 1); // Recur for the next node

        // Check if the node at index can be deleted
        if (!current->get_next(index)->get_EndWord() && !hasChildren(current->get_next(index))) {
            delete current->get_next(index); // Deleting the node
            current->set_next(index, nullptr); // Setting the pointer to the child node to null
        }
    }
}

// Function to check if the node has any children
bool Trie::hasChildren(TrieNode* node) {
    for (int i = 0; i < 26; ++i) {
        if (node->get_next(i) != nullptr) {
            return true;
        }
    }
    return false;
}

// Function to print all words in the trie
void Trie::print(){
    std::string prefix = "";
    printHelper(root, prefix); // Calling the helper function to print words
    std::cout << std::endl;
}

// Helper function to recursively print words in the trie
void Trie::printHelper(TrieNode* node, const std::string& prefix){
    if (node == nullptr) {
        return; // Returning if the node is null
    }
    if (node->get_EndWord()) {
        std::cout << prefix << " "; // Printing the word if the current node marks the end of a word
    }
    // Recursively printing words for each child node
    for (int x = 0; x < 26; ++x) {
        if (node->get_next(x)) {
            printHelper(node->get_next(x), prefix + char('A' + x)); // Concatenating characters and calling the helper function
        }
    }
}

// Function to check the spelling of a word and suggest corrections if needed
void Trie::spellcheck(const std::string& word){
    std::string prefix = "";
    TrieNode* current = root;

    // Iterating through each character of the word
    for (char c : word){
        c = toupper(c); // Converting the character to uppercase
        int index = c - 'A'; // Calculating the index for the character in the trie node array

        // Checking if the next node for the character exists
        if(current->get_next(index) == nullptr){
            break; // Exiting the loop if the character is not found in the trie
        }
        prefix+=c; // Appending the character to the prefix
        current = current->get_next(index); // Moving to the next node
    }

    // Checking if the prefix matches the entire word and if it is marked as the end of a valid word
    if(prefix == word && current->get_EndWord()){
        std::cout << "correct" << std::endl; // Displaying "correct" if the word is spelled correctly
    } else {
        suggestions(prefix, current); // Generating suggestions for the misspelled word
        std::cout << std::endl;
    }
}

// Function to generate suggestions for a misspelled word
void Trie::suggestions(const std::string& prefix, TrieNode* node){
    if(node == nullptr){
        return; // Returning if the node is null
    }
    if(node->get_EndWord()){
        std::cout << prefix << " "; // Printing the prefix if it represents a valid word
    }
    // Recursively generating suggestions for each child node
    for(int x = 0; x < 26; x++){
        if(node->get_next(x) != nullptr){
            suggestions(prefix + char('A' + x), node->get_next(x)); // Concatenating characters and generating suggestions
        }
    }
}

// Function to check if the trie is empty
void Trie::empty(){
    for(int x = 0; x < 26; x++){
        if(root->get_next(x)){
            std::cout << "empty 0" << std::endl; // Printing "empty 0" if the trie is not empty
            return;
        }
    }
    std::cout << "empty 1" << std::endl; // Printing "empty 1" if the trie is empty
}

// Function to clear the trie and reset its state
bool Trie::clear(){
    deleteNode(root); // Deleting all nodes in the trie
    root = new TrieNode(); // Creating a new root node
    numWord = 0; // Resetting the count of words in the trie
    return true; //Prints success message
}

// Recursive function to delete all nodes in the trie
void Trie::deleteNode(TrieNode* node){
    if(node == nullptr){
        return; // Returning if the node is null
    }
    for(int x = 0; x < 26; x++){
        deleteNode(node->get_next(x)); // Recursively deleting child nodes
        node->set_next(x, nullptr); // Resetting the pointer to the child node
    }
    delete node; // Deleting the current node
}

// Function to print the number of words in the trie
void Trie::size(){
    std::cout << "number of words is " << numWord << std::endl; // Printing the count of words in the trie
}

/* TrieNode functions */

// Constructor for the TrieNode class
TrieNode::TrieNode(){
    EndWord = false; // Initializing the EndWord flag to false
    for(int x = 0; x < 26; x++){
        next[x] = nullptr; // Initializing pointers to child nodes as null
    }
}

// Function to get the next node for a given index
TrieNode* TrieNode::get_next(int index){
    return next[index]; // Returning the pointer to the next node
}
 
// Function to check if the current node marks the end of a word
bool TrieNode::get_EndWord(){
    return EndWord; // Returning the value of the EndWord flag
}

// Function to set the next node for a given index
void TrieNode::set_next(int index, TrieNode* new_next){
    next[index] = new_next; // Setting the pointer to the next node
}

// Function to set the EndWord flag for the current node
void TrieNode::set_EndWord(bool new_EndWord){
    EndWord = new_EndWord; // Setting the value of the EndWord flag
}