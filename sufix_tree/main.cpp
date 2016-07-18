#include <iostream>
#include "sufix_tree.h"
using namespace std;

int main()
{
    //sufix_tree t("xabxac$");
    sufix_tree t("abcabxabcd$");
    cout<<"subr "<<t.is_substring("ab")<<endl;
    cout<<"occr "<<t.occurrences("ab")<<endl;
    return 0;

}

