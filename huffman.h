#include <bits/stdc++.h>
#include <vector>
#include <unordered_map>
using namespace std;

class heapNode {
private:
    int value; // frequency
    char character; // ASCII char
    heapNode* leftChild;
    heapNode* rightChild;

public:
    heapNode(char character, int value);
    int getValue();
    char getCharacter();
    heapNode* getLeftChild();
    heapNode* getRightChild();
    void setLeftChild(heapNode *node);
    void setRightChild(heapNode *node);
};

heapNode::heapNode(char character, int value) {
    this->value = value;
    this->character = character;
}

int heapNode::getValue() {
    return this->value;
}

char heapNode::getCharacter() {
    return this->character;
}

heapNode* heapNode::getLeftChild() {
    return this->leftChild;
}

heapNode* heapNode::getRightChild() {
    return this->rightChild;
}

void heapNode::setLeftChild(heapNode* node) {
    this->leftChild = node;
}

void heapNode::setRightChild(heapNode* node) {
    this->rightChild = node;
}


// template<typename T>
class minHeap {
    private:
        vector<heapNode*> vec;
        void swapElement(int index1, int index2) {
            swap(vec[index1], vec[index2]);
        }

        int parentIndex(int index) {
            return (index - 1) / 2;
        }

        int leftChildIndex(int index) {
            return (index * 2) + 1;
        }

        int rightChildIndex(int index) {
            return (index * 2) + 2;
        }

        int compare(int index1 , int index2) {
            return vec[index1]->getValue() - vec[index2]->getValue();
        }

        void upHeap(int index) {
            if(index == 0)
                return;
            int p = this->parentIndex(index);
            if(this->compare(index,p) < 0) {
                this->swapElement(index, p);
                this->upHeap(p);
            }
        }

        void downHeap(int index) {
            int min = index;
            int left = this->leftChildIndex(index);
            int right = this->rightChildIndex(index);

            if(left<vec.size() && this->compare(min,left) > 0) {
                min = left;
            }

            if(right < vec.size() && this->compare(min,right) > 0) {
                min = right;
            }

            if(min != index) {
                this->swapElement(min, index);
                this->downHeap(min);
            }
        }

    public:
        minHeap() {}
        void insert(heapNode* node);
        heapNode* remove();
        vector<heapNode *> getVev();
        int getVecSize();
};

void minHeap::insert(heapNode* node) {
    vec.push_back(node);
    this->upHeap(vec.size() - 1);
}

heapNode* minHeap::remove() {
    if(vec.size() == 0) {
        return nullptr;
    }

    heapNode *temp = vec[0];
    heapNode *last = vec[vec.size() - 1];
    vec.pop_back();

    if(vec.size()>0) {
        vec[0] = last;
        this->downHeap(0);
    }

    return temp;
}

vector<heapNode*> minHeap::getVev() {
    return this->vec;
}

int minHeap::getVecSize() {
    return vec.size();
}

class huffmanCodeing {
    private:
        unordered_map<char, string> encoder;
        unordered_map<string, char> decoder;

        void initEncoderDecoder(heapNode *node, string osf) {
            if(node == nullptr) {
                return;
            }

            if(node->getLeftChild() == nullptr && node->getRightChild() == nullptr) {
                this->encoder.insert(make_pair(node->getCharacter(), osf));
                this->decoder.insert(make_pair(osf, node->getCharacter()));
            }
            this->initEncoderDecoder(node->getLeftChild(), osf + "0");
            this->initEncoderDecoder(node->getRightChild(), osf + "1");
        }

    public:
        huffmanCodeing(string feeder);
        string encode(string sourceFileName);
        string decode(string codedString);
        string encodeWithDecoder(string sourceFileName);
        string decodeWithoutDecoder(string codedString);
};

huffmanCodeing::huffmanCodeing(string fileName) {
    unordered_map<char, int> freqMap;

    ifstream file(fileName); // Open file for reading
    if (!file.is_open()) {
        cout << "Failed to open the file " << fileName << "\n";
        exit(0);
    }

    char ch;
    while (file.get(ch)) { // Read each character until end of file
        // std::cout << ch; // Print character (you can process it as needed)
        auto it = freqMap.find(ch);
        if (it != freqMap.end()) {
            freqMap[ch]++;
        } 
        else {
            freqMap.insert({ch,1});
        }
    }

    file.close(); // Close the file

    freqMap.insert({'\0', 1});

    minHeap* heap = new minHeap();
    for (auto& pair : freqMap) {
        heapNode *node = new heapNode(pair.first, pair.second);
        heap->insert(node);
    }

    while(heap->getVecSize() != 1) {
        heapNode *first = heap->remove();
        heapNode *second = heap->remove();

        heapNode *newNode = new heapNode('\0', first->getValue() + second->getValue());
        newNode->setLeftChild(first);
        newNode->setRightChild(second);

        heap->insert(newNode);
    }

    heapNode *finalNode = heap->remove();
    this->initEncoderDecoder(finalNode, "");
}

string huffmanCodeing::encode(string sourceFileName) {
    string ans = "";

    ifstream file(sourceFileName); // Open file for reading
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        exit(0);
    }

    char ch;
    while (file.get(ch)) { // Read each character until end of file
        ans += this->encoder[ch];
    }

    file.close(); // Close the file

    ans += this->encoder['\0'];

    return ans;
}

string huffmanCodeing::decode(string codedString) {
    string key = "";
    string ans = "";
    for (int i = 0; i < codedString.size(); i++) {
        key += codedString[i];
        auto it = decoder.find(key);
        if (it != decoder.end()) {
            if(it->second == '\0') {
                break;
            }
            ans += it->second;
            key = "";
        }
    }
    return ans;
}

string huffmanCodeing::encodeWithDecoder(string sourceFileName) {
    unsigned int size_ = this->encoder.size();
    bitset<8> bits(size_);
    string bs = bits.to_string();

    for (auto& pair : this->encoder) {
        bitset<8> bits1(int(pair.first));
        bs += bits1.to_string();
        size_ = pair.second.size();
        bitset<8> bits2(size_);
        bs += bits2.to_string();
        bs += pair.second;
    }

    bs += this->encode(sourceFileName);
    return bs;
}

string huffmanCodeing::decodeWithoutDecoder(string codedString) {
    unordered_map<string, char> tempDecoder;
    string subString = codedString.substr(0, 8);
    codedString.erase(0, 8);
    bitset<8> bits(subString);
    unsigned int size_ = bits.to_ulong();

    char character;
    unsigned int size1;
    unsigned int size2;

    while(size_--) {
        subString = codedString.substr(0, 8);
        codedString.erase(0, 8);
        bitset<8> bits1(subString);
        size1 = bits1.to_ulong();
        character = char(size1);

        subString = codedString.substr(0, 8);
        codedString.erase(0, 8);
        bitset<8> bits2(subString);
        size2 = bits2.to_ulong();

        subString = codedString.substr(0, size2);
        codedString.erase(0, size2);
        tempDecoder.insert({subString, character});
    }

    string key = "";
    string ans = "";
    for (int i = 0; i < codedString.size(); i++) {
        key += codedString[i];
        auto it = tempDecoder.find(key);
        if (it != tempDecoder.end()) {
            if(it->second == '\0') {
                break;
            }
            ans += it->second;
            key = "";
        }
    }
    return ans;
}

class bitWiseWrite {
    private:
        vector<uint8_t> binaryStringToBytes(const string &binaryString) {
            vector<uint8_t> bytes;
            int length = binaryString.length();

            for (int i = 0; i < length; i += 8) {
                string byteString;
                if(i+8>=length)
                byteString = binaryString.substr(i, 8);
                else
                byteString = binaryString.substr(i, length-i);

                // Pad with zeros if the last chunk is less than 8 bits
                if (byteString.length() < 8) {
                    byteString.append(8 - byteString.length(), '0');
                }

                uint8_t byte = static_cast<uint8_t>(std::bitset<8>(byteString).to_ulong());
                bytes.push_back(byte);
            }

            return bytes;
        }

        void writeBytesToFile(const string &filename, const vector<uint8_t> &bytes) {
            ofstream outFile(filename, std::ios::binary);
            if (!outFile) {
                cout << "Error opening file for writing." << endl;
                return;
            }

            outFile.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
            outFile.close();
        }       
    public:
        bitWiseWrite(string binaryString);
};

bitWiseWrite::bitWiseWrite(string binaryString) {
    // Convert binary string to bytes
    vector<uint8_t> bytes = this->binaryStringToBytes(binaryString);

    // Write bytes to file
    this->writeBytesToFile("output.bin", bytes);

    std::cout << "Binary data written to output.bin" << std::endl;
}

class readCompressedFile {
    private:
        string fileName;
        vector<uint8_t> readBytesFromFile(const string &filename) {
            ifstream inFile(filename, std::ios::binary);
            if (!inFile) {
                cout << "Error opening file for reading.\n";
                return {};
            }

            vector<uint8_t> bytes((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
            inFile.close();

            return bytes;
        }       
    
        string bytesToBinaryString(const vector<uint8_t> &bytes) {
            string binaryString;
            for (const auto &byte : bytes) {
                binaryString += bitset<8>(byte).to_string();
            }
            return binaryString;
        }

    public:
        readCompressedFile(string fileName);
        string RCF_main();
};

readCompressedFile::readCompressedFile(string fileName) {
    this->fileName = fileName;
}

string readCompressedFile::RCF_main() {
    // Read bytes from the file
    vector<uint8_t> bytes = this->readBytesFromFile(this->fileName);

    // Convert bytes to a binary string
    string binaryString = this->bytesToBinaryString(bytes);

    // return the binary string
    return binaryString;
}

class Decompress
{
    public:
        Decompress() {}
        string decodeWithoutDecoder(string codedString);
};

string Decompress::decodeWithoutDecoder(string codedString) {
    unordered_map<string, char> tempDecoder;
    string subString = codedString.substr(0, 8);
    codedString.erase(0, 8);
    bitset<8> bits(subString);
    unsigned int size_ = bits.to_ulong();

    char character;
    unsigned int size1;
    unsigned int size2;

    while(size_--) {
        subString = codedString.substr(0, 8);
        codedString.erase(0, 8);
        bitset<8> bits1(subString);
        size1 = bits1.to_ulong();
        character = char(size1);

        subString = codedString.substr(0, 8);
        codedString.erase(0, 8);
        bitset<8> bits2(subString);
        size2 = bits2.to_ulong();

        subString = codedString.substr(0, size2);
        codedString.erase(0, size2);
        tempDecoder.insert({subString, character});
    }

    string key = "";
    string ans = "";
    for (int i = 0; i < codedString.size(); i++) {
        key += codedString[i];
        auto it = tempDecoder.find(key);
        if (it != tempDecoder.end()) {
            if(it->second == '\0') {
                break;
            }
            ans += it->second;
            key = "";
        }
    }
    return ans;
}
