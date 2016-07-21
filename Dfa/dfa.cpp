#include "dfa.h"
#include <set>

Dfa::Dfa(int s)
{
    size = s;
    initial_state = 0;
}

void Dfa::addTransiction(int qi, int qj, char label){
    this->matrix[qi][qj].insert(label);
}

void Dfa::addFinalState(int q){
    this->final_states.insert(q);
}

void Dfa::removeFinalState(int q){
    this->final_states.erase(q);
}

void Dfa::setInitialState(int q){
    this->initial_state = q;
}

void Dfa::addState(){
    this->size ++;
}

int Dfa::foward(int q, char word){
    for(int i = 0; i < this->size; i ++){
        if(this->matrix[q][i].find(word) != this->matrix[q][i].end()){
            return i;
        }
    }
    return -1;
}

bool Dfa::accept(std::string word){
    uint i = 0;
    int next_stt = this->foward(this->initial_state, word[i]);
    i ++;
    while(next_stt >= 0 && i < word.length()){
        next_stt = this->foward(next_stt, word[i]);
        i ++;
        if(this->final_states.find(next_stt) != this->final_states.end() && i == word.length()){
            return true;
        }
    }
    return false;
}
