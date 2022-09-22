//
//  main.cpp
//  Assignment-3
//
//  Created by Team 3 on 9/21/22.
//

#include <fstream>
#include "inc/scanner.h"
#include "inc/Parser.h"

using namespace std;

int main(int argc, const char* argv[], std::string filename) {
    Scanner sc;
    string word;

    std::string inTestFileName = "reference-files/HelloWorld.txt";
    std::string outTestFileName = "test-out.txt";
    //std::string inTestFileName = "test-in-original.txt";
    //std::string outTestFileName = "test-out-original.txt";

    ifstream testFile;
    testFile.open(inTestFileName);
    
    ofstream out;
    out.open(outTestFileName);

    if (!testFile.is_open()) {
        cout << "Error while opening " + inTestFileName << endl;
    }
    else {
        while (word != "-1") {
            word = nextToken(sc, testFile, out);
        } 
    }

    Parser parser(filename);
    Node* parse_tree = parser.parse();

    testFile.close();
    cout << "Program complete, check: " << outTestFileName << endl;
    return 0;
}
