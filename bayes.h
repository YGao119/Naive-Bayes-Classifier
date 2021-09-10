#include <fstream>
#include <iostream>
#include <map>

class Bayes {
public:
    Bayes(char*, char*, char*);  // constructor, pass in train.txt and classify.txt
    ~Bayes();             // destructor, close all file streams
    void training();      // training
    void classify();      // classifying

private:
    int occ(std::string obj);                        // count occurence of this object
    int occ(std::string trait, std::string obj);     // count occurence of this object with this trait
    int occNot(std::string obj);                     // count occurence of not this object
    int occNot(std::string trait, std::string obj);  // count occurence of not this object with this trait

    std::map<std::string, int> _obj;  // container to store all objects, mapping object name to its occurence
    std::map<std::string, std::map<std::string, int>>
            _traits;  // container to store all traits, mapping object to a map which maps traits to its occurence
    std::map<std::string, double> _prob;  // container to store probablity in classifying

    int _numObj;  // total number of objects

    std::ifstream _trainFile;
    std::ifstream _classifyFile;
    std::ofstream _ofile;
};
