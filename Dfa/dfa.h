#ifndef DFA_H
#define DFA_H
#include <set>
#include <string>

class Dfa
{
private:
    std::set<char> matrix[100][100];
    int size;
    std::set<int> final_states;
    int initial_state;

public:
    Dfa(int s);
    void addTransiction(int qi, int qj, char label);
    void addFinalState(int q);
    void addState();
    void removeFinalState(int q);
    void setInitialState(int q);
    int foward(int q, char word);
    bool accept(std::string word);
};

#endif // DFA_H
