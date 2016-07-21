#include "nfa.h"
#include <iostream>
#include <string>
#include <set>
#include <list>
#include <vector>

using namespace std;
Nfa::Nfa(string regularExpression)
{
    /*
     * A sintaxe da expressao regular:
     * concatenacao de 'a' e 'b' =>  'ab'
     * alternacao de 'a' ou 'b' => 'a|b'
     * presenca ou ausencia de 'a' => a?
     * quantidade variavel de 'a' => a*
     * quantidade variavel nao nula de 'a' => a+
     * caractere vazio: _ (underline)
     * precedencia padrao:
     * primeiro: a?, a*, a+
     * segundo: ab
     * terceiro: a|b
     * ou precedencia por (parenteses).
     * */

    regExp = regularExpression;
    size = 2;
    matrix[0][1] = regExp;
    parse(0,1);

}

int Nfa::getType(char simbolo){
    if(simbolo == '?'){
        return 1;
    }
    if(simbolo == '*'){
        return 2;
    }
    if(simbolo == '+'){
        return 3;
    }
    if(simbolo == '(' || simbolo == ')'){
        return 4;
    }
    if(simbolo == '|'){
        return 5;
    }
    return 0;
}

bool Nfa::hasParenteses(string w){
    for(uint i = 0; i < w.length(); i ++){
        if(w[i] == '('){
            return true;
        }
    }
    return false;
}

bool Nfa::hasAlternation(string w){
    int stack = 0;
    for(uint i = 0; i < w.length(); i ++){
        if(w[i] == '('){
            stack ++;
        }
        if(w[i] == ')'){
            stack --;
        }
        if(stack == 0 && w[i] == '|'){
            return true;
        }
    }
    return false;
}

bool Nfa::hasConcat(string w){
    list<string> a = this->divideConcat(w);
    if(a.size() > 1){
        return true;
    }
    return false;
}

list<string> Nfa::divideParenteses(string regularExpression){
    //Essa funcaoo sempre divide a expressao regular em tres partes
    //Quando nao ha parentes então ela retorna uma lista com apenas um elemento
    int init = -1;
    int end = -1;
    int stack = 0;
    list<string> divided;
    int tam = regularExpression.length();
    for(int i = 0; i < tam; i++){
        if(regularExpression[i] == '('){
            if(stack == 0){
                init = i;
            }
            stack ++;
        }
        if(regularExpression[i] == ')'){
            if(stack == 1){
                end = i;
            }
            stack --;
        }
    }
    if(end <= init || stack != 0 ){
        //retornando lista vazia em caso de erro com parenteses..
        return divided;
    }
    if(end == -1 && init == -1){
        divided.push_back(regularExpression);
        return divided;
    }else if(end == -1 || init == -1){
        //retornando lista vazia em caso de erro com parenteses..
        return divided;
    }
    //    aba(abnml)+efa
    if(this->getType(regularExpression[end + 1]) < 4){
        divided.push_back(regularExpression.substr(0, init));
        divided.push_back(regularExpression.substr(init, (end + 2) - init));
        divided.push_back(regularExpression.substr(end + 2, tam - 1 - (end + 1)));
    }
    else{
        divided.push_back(regularExpression.substr(0, init));
        divided.push_back(regularExpression.substr(init + 1, (end - 1) - init));
        divided.push_back(regularExpression.substr(end + 1, tam - 1 - end));
    }
    return divided;
}

list<string> Nfa::divideAlternations(string regularExpression){

    list<string> divided;
    int tam = regularExpression.length();
    int last = 0;
    int stack = 0;
    for(int i = 0; i < tam; i ++){
        if(stack == 0 && regularExpression[i] == '|'){
            divided.push_back(regularExpression.substr(last, i - last));
            last = i + 1;
        }
        if(regularExpression[i] == '('){
            stack ++;
        }
        if(regularExpression[i] == ')'){
            stack --;
        }
    }
    divided.push_back(regularExpression.substr(last, tam - last));
    return divided;
}

list<string> Nfa::divideConcat(string regularExpression){
    list<string> divided;
    int tam = regularExpression.length();
    int last = 0;
    int stack = 0;
    // dfd+(afdf)+afd
    for(int i = 0; i < tam; i ++){
        if(regularExpression[i] == '('){
            last = i;
            stack ++;
        }
        else if(regularExpression[i] == ')'){
            stack --;
            if(stack == 0){
                if(this->getType(regularExpression[i+1]) < 4){
                    divided.push_back(regularExpression.substr(last, i + 1 - (last - 1)));
                    i ++;
                }
                else{
                    divided.push_back(regularExpression.substr(last, i - (last - 1)));
                }
            }
        }
        else if(stack == 0){
            if(i == tam - 1){
                divided.push_back(regularExpression.substr(i, 1));
            }else if(this->getType(regularExpression[i + 1]) <= 3 && this->getType(regularExpression[i + 1]) >= 1){
                divided.push_back(regularExpression.substr(i, 2));
                i ++;
            }else{
                divided.push_back(regularExpression.substr(i, 1));
            }
        }
    }

    return divided;
}

void Nfa::printMatrix(){
    for(int i = 0; i < this->size; i ++){
        for(int j = 0; j < this->size; j ++){
            if(this->matrix[i][j].length() == 0){
                cout << ' ' << ',';
            }
            else{
                cout << this->matrix[i][j] << ',';
            }
        }
        cout << endl;
    }
}

void Nfa::parse(int qi, int qj){
    /*
     *
     */


    cout << qi << ',' << qj << ':' << this->matrix[qi][qj] << string("   ");

    string regularExpression = this->matrix[qi][qj];
    if(regularExpression.front() == '(' && regularExpression.back() == ')'){
        regularExpression = regularExpression.substr(1, regularExpression.length() - 2);
        this->matrix[qi][qj] = regularExpression;
    }
    if(regularExpression.length() <= 1){
        cout << string("caso base") << endl;
        return;
    }
    if(this->hasAlternation(regularExpression)){
        cout << string("aternation") << endl;
        int old_size = this->size;
        this->makeAlternation(qi, qj);
        int new_size = this->size;
        cout << string("   ") << old_size << ',' << new_size << endl;
        for(int i = old_size; i < new_size; i ++){
            this->parse(qi, i);
        }
    }
    else if(this->hasConcat(regularExpression)){
        cout << string("concat") << endl;
        int old_size = this->size;
        this->makeConcat(qi, qj);
        int new_size = this->size;
        this->parse(qi, old_size);
        for(int i = old_size; i < new_size - 1; i ++){
            this->parse(i, i + 1);
        }
        this->parse(new_size - 1, qj);
    }
    else {
        if(regularExpression.back() == '+'){
            cout << string("op +") << endl;
            this->makeRepetition(qi, qj);
            int new_size = this->size;
            this->parse(qi, new_size - 1);
            this->parse(new_size - 1, new_size - 1);
        }
        else if(regularExpression.back() == '*'){
            cout << string("op *") << endl;
            this->makeNullRepetition(qi, qj);
            this->parse(this->size - 1, this->size - 1);

        }
        else if(regularExpression.back() == '?'){
            cout << string("op ?") << endl;
            this->makeNullable(qi, qj);
            this->parse(qi, this->size - 1);
        }

    }

}

void Nfa::addTransictionRE(int qi, int qj, string label){
    this->matrix[qi][qj] = label;
}

void Nfa::removeTransitionRE(int qi, int qj){
    this->matrix[qi][qj] = string ();
}

void Nfa::makeAlternation(int qi, int qj){
    string original_exp = this->matrix[qi][qj];
    this->matrix[qi][qj] = string();
    list<string> exps_list = this->divideAlternations(original_exp);
    int offset = this->size;
    this->size += exps_list.size();
    for(uint i = 0; exps_list.size() > 0; i++){
        this->matrix[qi][i + offset] = exps_list.front();
        exps_list.pop_front();
        this->matrix[i + offset][qj] = string("_");
    }

}

void Nfa::makeConcat(int qi, int qj){

    string original_exp = this->matrix[qi][qj];
    this->matrix[qi][qj] = string();
    list<string> exps_list = this->divideConcat(original_exp);
    int offset = this->size;
    this->size += exps_list.size() - 1;
    this->matrix[qi][offset] = exps_list.front();
    exps_list.pop_front();

    for(uint i = 0; exps_list.size() > 1; i ++){

        this->matrix[offset + i][offset + i + 1] = exps_list.front();
        exps_list.pop_front();
    }

    this->matrix[this->size - 1][qj] = exps_list.front();

    exps_list.pop_front();

}

void Nfa::makeNullable(int qi, int qj){
    // essa eh a expressao regular: a?
    string original_exp = this->matrix[qi][qj];
    this->matrix[qi][qj] = string("_");
    string exp = original_exp.substr(0, original_exp.length() - 1);
    this->matrix[qi][this->size] = exp;
    this->matrix[this->size][qj] = string("_");
    this->size ++;
}
void Nfa::makeRepetition(int qi, int qj){
    string original_exp = this->matrix[qi][qj];
    this->matrix[qi][qj] = string();
    string exp = original_exp.substr(0, original_exp.length() - 1);
    this->matrix[qi][this->size] = exp;
    this->matrix[this->size][this->size] = exp;
    this->matrix[this->size][qj] = string("_");
    this->size ++;
}

void Nfa::makeNullRepetition(int qi, int qj){
    string original_exp = this->matrix[qi][qj];
    this->matrix[qi][qj] = string();
    string exp = original_exp.substr(0, original_exp.length() - 1);
    this->matrix[qi][this->size] = string("_");
    this->matrix[this->size][this->size] = exp;
    this->matrix[this->size][qj] = string("_");
    this->size ++;

}

//   fa|fd+(exp)?bg
vector<bool> Nfa::incVector(vector<bool> a, vector<bool> b){
    vector<bool> c(this->size, false);
    for(int i = 0; i < this->size; i ++){
        c[i] = a[i] || b[i];
    }
    return c;
}

vector<bool> Nfa::foward(int q, string w){
    vector<bool> next(this->size, false);
    for(int i = 0; i < this->size; i++){
        if(this->matrix[q][i] == w){
            next[i] = true;
            for(int j = 0; j < this->size; j ++){
                if(this->matrix[i][j] == "_"){
                    next[j] = true;
                }
            }
        }
    }
    return next;
}

bool Nfa::emptyVector(vector<bool> a){
    for(int i = 1; i < this->size; i ++){
        if(a[i]){
            return true;
        }
    }
    return false;
}


bool Nfa::accept(string word){

    vector<bool> current_states(this->size, false);
    current_states[0] = true;
    for(int i = 0; i < this->size; i++){
        if(this->matrix[0][i] == "_"){
            current_states[i] = true;
        }
    }
    vector<bool> next_states(this->size, false);

    uint k = 0;
    while(k < word.length()){
        for(int i = 0; i < this->size; i++){
            if(current_states[i]){
                next_states = this->incVector(next_states, this->foward(i, word.substr(k, 1)));
            }
        }
        if(this->emptyVector(next_states)){
            break;
        }
        current_states = next_states;
        next_states = vector<bool> (this->size, false);
        k ++;
    }
    if(current_states[this->final_state]){
        return true;
    }
    return false;

}
