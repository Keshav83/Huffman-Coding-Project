#include "huffman.h";
#include <bits/stdc++.h>;
using namespace std;

void HuffmanCoding::buildTree(const  unordered_map<char, int>& freq) {
     priority_queue<Node*,  vector<Node*>, Compare> pq;
    
    for (auto pair : freq) {
        pq.push(new Node(pair.first, pair.second));
    }

    while (pq.size() != 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        int sum = left->freq + right->freq;
        Node* node = new Node('\0', sum);
        node->left = left;
        node->right = right;
        pq.push(node);
    }

    root = pq.top();
}

void HuffmanCoding::encode(Node* root, const  string& str) {
    if (!root) return;

    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
        reverseHuffmanCode[str] = root->ch;
    }

    encode(root->left, str + "0");
    encode(root->right, str + "1");
}

void HuffmanCoding::decode(Node* root, int& index, const  string& str,  string& decodedStr) {
    if (!root) return;

    if (!root->left && !root->right) {
        decodedStr += root->ch;
        return;
    }

    index++;

    if (str[index] == '0')
        decode(root->left, index, str, decodedStr);
    else
        decode(root->right, index, str, decodedStr);
}

HuffmanCoding::~HuffmanCoding() {
    // Free memory (to be implemented)
}

void HuffmanCoding::compress(const  string& inputFileName, const  string& outputFileName) {
     ifstream inputFile(inputFileName,  ios::binary);
     ofstream outputFile(outputFileName,  ios::binary);

    if (!inputFile || !outputFile) {
         cerr << "Error opening files!" <<  endl;
        return;
    }

     unordered_map<char, int> freq;
    char ch;

    while (inputFile.get(ch)) {
        freq[ch]++;
    }

    buildTree(freq);
    encode(root, "");

    inputFile.clear();
    inputFile.seekg(0);

     string encodedString;
    while (inputFile.get(ch)) {
        encodedString += huffmanCode[ch];
    }

    int extraBits = 8 - (encodedString.size() % 8);
    for (int i = 0; i < extraBits; i++) {
        encodedString += "0";
    }

    outputFile << extraBits <<  endl;
    for (auto pair : huffmanCode) {
        outputFile << pair.first << " " << pair.second <<  endl;
    }
    outputFile << "----" <<  endl;

    for (size_t i = 0; i < encodedString.size(); i += 8) {
         bitset<8> bits(encodedString.substr(i, 8));
        outputFile.put(static_cast<unsigned char>(bits.to_ulong()));
    }

    inputFile.close();
    outputFile.close();
}

void HuffmanCoding::decompress(const  string& inputFileName, const  string& outputFileName) {
     ifstream inputFile(inputFileName,  ios::binary);
     ofstream outputFile(outputFileName,  ios::binary);

    if (!inputFile || !outputFile) {
         cerr << "Error opening files!" <<  endl;
        return;
    }

    int extraBits;
    inputFile >> extraBits;

     unordered_map< string, char> reverseHuffmanCode;
    char ch;
     string code;

    while (inputFile >> ch >> code) {
        if (ch == '-') {
            inputFile.ignore(4); // ignore "----"
            break;
        }
        reverseHuffmanCode[code] = ch;
    }

     string encodedString;
    while (inputFile.get(ch)) {
         bitset<8> bits(static_cast<unsigned char>(ch));
        encodedString += bits.to_string();
    }

    encodedString.erase(encodedString.end() - extraBits, encodedString.end());

     string decodedString;
    Node* currentNode = root;
    int index = -1;
    while (index < (int)encodedString.size() - 1) {
        decode(root, index, encodedString, decodedString);
    }

    outputFile << decodedString;

    inputFile.close();
    outputFile.close();
}

int main() {
    HuffmanCoding huffman;
     string inputFile = "input.txt";
     string compressedFile = "compressed.bin";
     string decompressedFile = "decompressed.txt";

    huffman.compress(inputFile, compressedFile);
    huffman.decompress(compressedFile, decompressedFile);

    return 0;
}

