#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include "bOpts.h"

//
//  CleanPaws. 
//      https://github.com/L3pu5/CleanPaws
//      By L3pu5, L3pu5_Hare
//

void Help(){
    std::cout << "Usage: CleanPaws [-e] <TextFile> <WordList>\n";
    std::cout << "    TextFile: The path to the plain text document to be parsed for matches.\n";
    std::cout << "    WordList: The file containing one word per line to be checked against the main document.\n";
    std::cout << "\n";
    std::cout << "      Flag  : Meaning\n";
    std::cout << "          -e: Check each word as an exact word maches instead of as a substring per line.\n";
}

//Expects two arguments, a string pointer to a file and a directory to a word list.
int main(int argc, char* argv[])
{
    //Add a toggle for whether or not to read each word;
    bOpts::SetToggleOpts({"e", "help"});
    bOpts::SetOpts("help", {"h", "HELP", "-help", "-h"});
        //e for Exact match on a word, piecewise
    bOpts::MakeOpts(argc, argv);
    
    //Check to see if the 'Help' command was sent
    if(bOpts::GetToggleOpt("help") || argc < 3){
        Help();
        return 0;
    }
    

    //Open the files.
    std::ifstream _input;
    std::ifstream _wordlist;
    //Filter words
    std::vector<std::string> _filterWords;

    //Open the Word List, read all words, place into Vector.
    _wordlist.open(bOpts::GetFloatingOpt(1), std::ios::in);

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
    printf("Reading the file %s against %s\n", bOpts::GetFloatingOpt(0).c_str(), bOpts::GetFloatingOpt(1).c_str());
    printf("----------------------------------------------------------\n");
    printf(" Line | Word       | Text\n");


    if(_input.is_open()){
        std::string _line;
        //If we are matching exact
        if(bOpts::GetToggleOpt("e")){
            while(std::getline(_input, _line)){

                //Convert to lower case.
                std::for_each(_line.begin(), _line.end(), [](char & c){
                    c = ::tolower(c);
                });
                //For each word.
                for (int i = 0; i < _filterWords.size(); i++)
                {
                    std::regex r("\\b" + _filterWords[i] + "\\b");
                    std::smatch m;

                    //Search that line for all possible illegal words.
                    if(std::regex_search(_line,m, r)){
                        _hitCounter++;
                        printf("%5i | %10s | %s \n", _lineNumber, _filterWords[i].c_str(), _line.c_str());
                    }
                }
                _lineNumber++;  
            }
            goto EndOfCleanPaws;
        }
        //If we are not matching exact
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
    EndOfCleanPaws:
    _input.close();
    printf("----------------------------------------------------------\n");
    printf("%i lines were parsed. %i hits were identified.\n", _lineNumber, _hitCounter);
    return 0; 
}