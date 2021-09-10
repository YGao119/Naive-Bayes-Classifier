CXX=g++
CXXFLAGS=-g -Wall -std=c++11

bayes: bayes.cpp
	$(CXX) $(CXXFLAGS) bayes.cpp -o bayes


clean:
	rm -f bayes