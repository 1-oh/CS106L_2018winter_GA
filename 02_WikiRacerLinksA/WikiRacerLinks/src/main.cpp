#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <vector>

using std::cout;            using std::endl;
using std::string;          using std::unordered_set;

std::string Welcome();
std::vector<std::string> ReadHtmlToStringVector(std::ifstream& file);
bool IsValidLink(std::string str);
unordered_set<std::string> LinkFind(std::vector<std::string> htmlStringVector);

int main() {
	
    /* TODO: Write code here! */
    std::ifstream file;
    std::vector<string> HtmlStringVector = ReadHtmlToStringVector(file);
    std::unordered_set LinkSet = LinkFind(HtmlStringVector);
    for(auto& str : LinkSet){
        std::cout << str << std::endl;
    }
    return 0;
    /* Note if your file reading isn't working, please follow the
     * steps posted on the front page of the CS106L website. Essentially
     * go to the projects tab on the panel on the left, and in the run
     * section, uncheck the "Run in terminal" box and re-check it. This
     * should fix things.
     */

}

/*Read the filename from the terminal*/
std::string Welcome(){
    std::cout<<"Welcome to WikiRacer Phrase 1,please input the file name"<<std::endl;
    std::string ret;
    while(!getline(std::cin, ret)){};
    return ret;
}

/*Open the html file and turn it to a string*/
std::vector<std::string> ReadHtmlToStringVector(std::ifstream& file){
    while(true){
        std::string fileName = Welcome();
        file.open(fileName);
        if(!file.fail()){
            std::vector<std::string> ret;
            std::string line;
            while(getline(file, line)){
                ret.push_back(line);
            }
            return ret;
        }
        else cout<<"Invalid file, please try again"<<std::endl;
    }
}

unordered_set<std::string> LinkFind(std::vector<std::string> htmlStringVector){
    unordered_set<string> ret;
    std::string::iterator frontIt, endIt;

    //Initial the endIterator as the begin of the htmlString
    for(std::string htmlString : htmlStringVector){
        endIt = htmlString.begin();
        while(endIt < htmlString.end()){
            std::string beginSign = "<a href=\"/wiki/";
            std::string endSign = "\"";
            frontIt = std::search(endIt, htmlString.end(), beginSign.begin(), beginSign.end());
            if(frontIt == htmlString.end()){
                break;
            }
            endIt = std::search(frontIt + 15, htmlString.end(), endSign.begin(), endSign.end());
            string link(frontIt + 15, endIt);
            ret.insert(link);
        }

    }
    //Check the set to delele invalid string and save the valid ones
    for(auto& str : ret){
        if(!IsValidLink(str)){
            ret.erase(str);
        }
    }

    return ret;
}

bool IsValidLink(std::string str){
    auto it1 = std::find(str.begin(), str.end(), ':');
    auto it2 = std::find(str.begin(), str.end(), '#');
    if(it1 == str.end() && it2 == str.end()){
        return true;
    }
    return false;
}
