#include <iostream>
#include "sufix_tree.h"
using namespace std;

int main()
{
    //sufix_tree t("xabxac$");
    sufix_tree t("abcabxabcd$");
    cout<<"subr "<<t.is_substring("ab")<<endl;
    cout<<"occr "<<t.occurrences("bc")<<endl;
    //cout<<a.substr(2,1)<<endl;
    return 0;

}

