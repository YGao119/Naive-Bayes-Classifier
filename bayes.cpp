#include "bayes.h"

#include <iostream>
#include <set>
#include <sstream>

// constructor, open all files
Bayes::Bayes(char* a, char* b, char* c) {
    _trainFile.open(a);
    _classifyFile.open(b);
    _ofile.open(c);
}

// destructor, close all files
Bayes::~Bayes() {
    _trainFile.close();
    _classifyFile.close();
    _ofile.close();
}

// return number of occurence of this object
int Bayes::occ(std::string obj) { return _obj[obj]; }

// return number of occurence of this object with this trait
int Bayes::occ(std::string trait, std::string obj) { return _traits[obj][trait]; }

// return number of occurence of not this object
int Bayes::occNot(std::string obj) { return _numObj - occ(obj); }

// return number of occurence of not this object with this trait
int Bayes::occNot(std::string trait, std::string obj) {

    int count = 0;

    for (std::map<std::string, std::map<std::string, int>>::iterator it = _traits.begin(); it != _traits.end(); ++it) {

        if (it->first != obj) {
            count += (it->second)[trait];
        }
    }
    return count;
}

// fucntion to train
void Bayes::training() {

    _trainFile >> _numObj;
    std::string garbage;
    std::getline(_trainFile, garbage);

    // store parseddatas in train.txt into containers,
    for (int i = 0; i < _numObj; i++) {
        std::string temp;
        std::getline(_trainFile, temp);
        std::stringstream ss(temp);

        std::string obj;
        ss >> obj;

        if (_obj.find(obj) != _obj.end()) {
            _obj[obj]++;
        } else {
            _obj.insert(std::pair<std::string, int>(obj, 1));
            _prob.insert(std::pair<std::string, int>(obj, 0));
            std::map<std::string, int> traits;
            _traits.insert(std::pair<std::string, std::map<std::string, int>>(obj, traits));
        }

        // ignore duplicates in the same line
        std::string trait;
        std::set<std::string> descri;
        while (ss >> trait) {
            descri.insert(trait);
        }

        for (std::set<std::string>::iterator it = descri.begin(); it != descri.end(); ++it) {
            if (_traits[obj].find(*it) != _traits[obj].end()) {
                _traits[obj][*it]++;
            } else {
                _traits[obj].insert(std::pair<std::string, int>(*it, 1));
            }
        }
    }
}

// function to classify
void Bayes::classify() {

    int numLine;
    _classifyFile >> numLine;
    std::string garbage;
    std::getline(_classifyFile, garbage);

    // read in parsed datas in classify.txt one line per time
    for (int i = 0; i < numLine; i++) {

        std::string temp;
        std::getline(_classifyFile, temp);
        std::stringstream ss(temp);

        std::set<std::string> descriptions;
        std::string trait;
        while (ss >> trait) {
            descriptions.insert(trait);
        }

        double result;

        // iterate through all objects without repitition
        for (std::map<std::string, int>::iterator it_obj = _obj.begin(); it_obj != _obj.end(); ++it_obj) {
            double probX_givenC = 1.0;
            double probX_givenNC = 1.0;

            // iterate through all traits in one line without duplicates and calcultat P(x|C) and P(x|not C)
            for (std::set<std::string>::iterator it = descriptions.begin(); it != descriptions.end(); ++it) {
                probX_givenC = probX_givenC * (1.0 + occ(*it, it_obj->first)) / (1.0 + occ(it_obj->first));
                probX_givenNC = probX_givenNC * (1.0 + occNot(*it, it_obj->first)) / (1.0 + occNot(it_obj->first));
            }

            // calculate p(C|x) using Bayes' Theorem and Laplacian Smoothing
            double prc = (double)_obj[it_obj->first] / _numObj;
            result = probX_givenC * prc / (probX_givenC * prc + probX_givenNC * (1 - prc));
            _prob[it_obj->first] = result;
        }

        // find the object with the highest probabality
        std::string max_obj;
        double max = 0;
        for (std::map<std::string, double>::iterator it = _prob.begin(); it != _prob.end(); ++it) {

            if (it->second > max) {
                max_obj = it->first;
                max = it->second;
            }
        }

        if (i == numLine - 1)
            _ofile << max_obj;
        else
            _ofile << max_obj << std::endl;
    }
}

int main(int argc, char* argv[]) {

    Bayes bayes(argv[1], argv[2], argv[3]);
    bayes.training();
    bayes.classify();
}
