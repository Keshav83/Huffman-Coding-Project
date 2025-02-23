#include <bits/stdc++.h>
#include "huffman.h"
using namespace std;

int main(int argc, char const *argv[])
{
    ios_base::sync_with_stdio(false);
    //cin.tie(NULL);

    string fileName;
    char c;

    cout << "What you want, compression or decompression (write 'C' for compression and 'D' for decompression) : ";
    cin >> c;

    if(c == 'C') {
        cout << "Enter file name (.txt format) : ";
        cin >> fileName;
        huffmanCodeing *hf = new huffmanCodeing(fileName);
        string cs = hf->encodeWithDecoder(fileName);
        bitWiseWrite *bww = new bitWiseWrite(cs);
    }
    else if(c == 'D') {
        cout << "Enter file name (.bin format) : ";
        cin >> fileName;
        readCompressedFile *rcf = new readCompressedFile("output.bin");
        string bs = rcf->RCF_main();
        Decompress *dc = new Decompress();
        string ds = dc->decodeWithoutDecoder(bs);

        ofstream outputFile("outexample.txt");
        if (outputFile.is_open()) {
            // Write the string content to the file
            outputFile << ds;

            // Close the file
            outputFile.close();
            cout << "File 'outexample.txt' has been created and written successfully.\n";
        } else {
            cout << "Error: Unable to open file 'outexample.txt' for writing.\n";
        }
    }
    else {
        cout << "InValid Choice !!\n";
    }
    
    return 0;
}
