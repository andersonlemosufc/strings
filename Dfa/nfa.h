#ifndef NFA_H
#define NFA_H
#include <string>
#include "dfa.h"
#include <list>
#include <vector>
using namespace std;

class Nfa
{
private:
    string regExp;
    int size;
    int getType(char simbolo);

    string matrix[100][100];
    int final_state;
    int initial_state;

public:
    Nfa(string regularExpression);
    void parse(int qi, int qj);
    list<string> divideParenteses(string regularExpression);
    list<string> divideAlternations(string regularExpression);
    list<string> divideConcat(string regularExpression);
    void addTransictionRE(int qi, int qj, string label);
    void removeTransitionRE(int qi, int qj);
    void makeAlternation(int qi, int qj);
    void makeConcat(int qi, int qj);
    void makeNullRepetition(int qi, int qj);
    void makeRepetition(int qi, int qj);
    void makeNullable(int qi, int qj);
    vector<bool> foward(int q, string w);
    bool accept(string word);
    bool hasParenteses(string w);
    bool hasAlternation(string w);
    bool hasConcat(string w);
    vector<bool> incVector(vector<bool> a, vector<bool> b);
    bool emptyVector(vector<bool> a);
    void printMatrix();
};

#endif // NFA_H
