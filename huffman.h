#ifndef HUFFMANCODING_H
#define HUFFMANCODING_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <string>

// Node structure for Huffman Tree
struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// Comparator for priority queue
struct Compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

class HuffmanCoding {
private:
    Node* root;
    std::unordered_map<char, std::string> huffmanCode;
    std::unordered_map<std::string, char> reverseHuffmanCode;

    void buildTree(const std::unordered_map<char, int>& freq);
    void encode(Node* root, const std::string& str);
    void decode(Node* root, int& index, const std::string& str, std::string& decodedStr);

public:
    HuffmanCoding() : root(nullptr) {}
    ~HuffmanCoding();

    void compress(const std::string& inputFileName, const std::string& outputFileName);
    void decompress(const std::string& inputFileName, const std::string& outputFileName);
};

#endif // HUFFMANCODING_H
