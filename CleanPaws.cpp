#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "bOpts.h"

//
//  CleanPaws. 
//      https://github.com/L3pu5/CleanPaws
//      By L3pu5, L3pu5_Hare
//

//Expects two arguments, a string pointer to a file and a directory to a word list.
int main(int argc, char* argv[])
{
    bOpts::MakeOpts(argc, argv);
    //Handle incorrect arugment number.
    if(argc != 3){
        std::cout << "Usage: CleanPaws <TextFile> <WordList>\n";
        std::cout << "    TextFile: The path to the plain text document to be parsed for matches.\n";
        std::cout << "    WordList: The file containing one word per line to be checked against the main document.\n";
        return 1;
    }

    

    //Open the files.
    std::ifstream _input;
    std::ifstream _wordlist;
    //Filter words
    std::vector<std::string> _filterWords;

    //Open the Word List, read all words, place into Vector.
    _wordlist.open(argv[2], std::ios::in);

    if(_wordlist.is_open()){
        std::string _word;
        while(std::getline(_wordlist, _word)){
            _filterWords.push_back(_word);
        }        
        _wordlist.close();
    }

    //Open Input
    _input.open(bOpts::GetFloatingOpt(0), std::ios::in);

    //Define Input vars
    int _lineNumber = 0;
    int _hitCounter = 0;

    printf("----------------------------------------------------------\n");
    printf("Reading the file %s against %s\n", bOpts::GetFloatingOpt(0), bOpts::GetFloatingOpt(1));
    printf("----------------------------------------------------------\n");
    printf(" Line | Word       | Text\n");

    if(_input.is_open()){
        std::string _line;
        while(std::getline(_input, _line)){

            //Convert to lower case.
            std::for_each(_line.begin(), _line.end(), [](char & c){
                c = ::tolower(c);
            });
            //For each word.
            for (int i = 0; i < _filterWords.size(); i++)
            {
                //Search that line for all possible illegal words.
                if(_line.find(_filterWords[i]) != std::string::npos)
                {
                    _hitCounter++;
                    printf("%5i | %10s | %s \n", _lineNumber, _filterWords[i].c_str(), _line.c_str());
                }
            }
            _lineNumber++;  
        }
    }
    _input.close();
    printf("----------------------------------------------------------\n");
    printf("%i lines were parsed. %i hits were identified.\n", _lineNumber, _hitCounter);
    return 0; 
}