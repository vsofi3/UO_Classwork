//
// Main function for testing *Response classes.
//
// Takes a file containing keywords, responses, and happy / sad indicators and an integer
// for the number of input strings as arguments.
//
// YOU MUST NOT CHANGE ANYTHING IN THIS FILE (unless we find a bug and give you a patch)
//
// Note that you should still read through this file to understand how your classes will be used.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "response.hh"

using namespace std;

void usage(int argc, char **argv);
void load_file(vector <Response*> &respList, char *fileName);
void get_input(vector <Response*> respList, int numInput);
void respondToInput(vector <Response*> respList, string input);

int main(int argc, char **argv)
{
    vector <Response*> responseList;
    int nInputs;

    // Verify given arguments
    usage(argc, argv);

    // Load keywords and responses from file
    load_file(responseList, argv[1]);

    // Get the number of runs
    stringstream ss(argv[2]);
    ss >> nInputs;

    // Execute the input / response runs
    get_input(responseList, nInputs);

    return 0;
}

void usage(int argc, char **argv)
{
    if(argc < 3) {
        cout << "usage: " << argv[0] << " <response file> <num tries>" << endl;
        exit(EXIT_FAILURE);
    }
}

void load_file(vector <Response*> &respList, char *fileName)
{
    ifstream inFile;
    inFile.open(fileName);
    string word;
    while(inFile >> word) {
        string keyword = word;

        string response;
        inFile >> response;
        
        int happy;
        inFile >> word;
        stringstream tmp(word);
        tmp >> happy;
              
        if (happy == 0) {
            Response *r = new Response(keyword, response);
            respList.push_back(r);
        } else if (happy > 0) {
            HappyResponse *hr = new HappyResponse(keyword, response);
            respList.push_back(hr);
        } else {
            AngryResponse *ar = new AngryResponse(keyword, response);
            respList.push_back(ar);
        }
    }
}

void get_input(vector <Response*> respList, int numInput)
{
    cout << "Enter " << numInput << " messages separated by <enter>: " << endl;
    for(int i = 0; i < numInput; i++) {
        string inp;
        getline(cin, inp);
        transform(inp.begin(), inp.end(), inp.begin(), ::toupper);
        respondToInput(respList, inp);
    }
}

void respondToInput(vector <Response*> respList, string input)
{
    bool found = false;
    for (unsigned int i = 0; i < respList.size(); i++) {

        bool exists = respList[i]->checkAndRespond(input, cout);
        if(exists) {
            found = true;
        }
    }
    if(!found) {
        cout << "I don't understand what you are saying\n" << endl;
    }
}
