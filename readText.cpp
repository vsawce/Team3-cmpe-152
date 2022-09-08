#include <iostream>
#include <fstream>
#include <string>

int main (){
    std::ifstream myfile;
    myfile.open("masterTestCase.txt");
    std::string myline;
    if ( myfile.is_open() ) {
        while ( myfile ) { 
        std::getline (myfile, myline);
        std::cout << myline << '\n';
    }
}

else {
std::cout << "Couldn't open file\n";
}

return 0;      
}
