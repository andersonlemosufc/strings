#include <iostream>
#include <string>
#include "dfa.h"
#include "nfa.h"
using namespace std;

int main()
{
    string regExp ("(ab?(a|ba)+)*");
    string word ("aaabaa");

    Nfa meu_automato (regExp);

    cout << meu_automato.accept(word) << endl;
    meu_automato.printMatrix();
    return 0;
}
