//
//  main.cpp
//  Assignment2
//
//  Created by MSSV on 9/7/22.
//

#include <iostream>
using namespace std;

class Scanner
{
public:
    void nexttoken()
    {
        return;
    }
private:
    string table[200][2];
};

string nextToken(istream& in){
    int c;
    string word;
    while (!in.eof())
    {
        c = in.get();
        if (c == ' ' || c == '\t' || c == '\n') break;
        word += c;
    }
    return word;
}

int main(){
    string word;
    do {
        word = nextToken(cin);
        cout << "[" << word << "]"; //link with hash table symbol table for output

    } while (word != "-1");

    return 0;
} 
