//
// Header file with class definitions for Response, AngryReponse, and HappyResponse
//
// Note that we don't use `using namespace std;` incase some other file which includes this
// header doesn't want to use all of the std namespace.
//
#include <iostream>
#include <string>

class Word {
  std::string theWord;
public:
  Word(std::string newWord) : theWord(newWord) {}
  std::string normal() { return theWord; }
  std::string upper();
  std::string lower();
};

//
// Base class with common functionality
//
class Response {

protected:
    //
    // These fields keep track of the word to listen for and the response for that word
    //
    Word keyword;
    std::string response;

private:
    //
    // This method insert's the object's response into the given output stream
    //
    virtual void respond(std::ostream &toWhere);

public:
    //
    // This is a constructor which creates a new object to listen for the given keyword and respond with the given response
    //
    Response(std::string str1, std::string str2) : keyword(str1), response(str2) {} 
    //
    // This is a destructor which does have much to do
    //
    ~Response() {}
    //
    // If this object's keyword is found in the message, then this method inserts this object's response into the given stream.
    //
    bool checkAndRespond(const std::string &inMessage, std::ostream &toWhere);
    //
    // This is a utility function to print the object's keyword and response
    //
    void printKeyword() { std::cout << keyword.normal() << " " << response << std::endl; }
};


class AngryResponse : public Response {

    //
    // Overide respond method for angry responses
    //
    void respond(std::ostream &toWhere);

public:
    //
    // Simple constructor which just calls the parent's constructor
    //
    AngryResponse(std::string str1, std::string str2) : Response(str1, str2) {}
};

class HappyResponse : public Response {

	void respond(std::ostream &toWhere);
public:
	HappyResponse(std::string str1, std::string str2) : Response(str1, str2) {}	
};
