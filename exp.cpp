#include <bits/stdc++.h>

using namespace std;

bool just_whitespace(const string& s)
{
    for(string::const_iterator it=s.begin(); it!=s.end(); ++it)
    {
        if(*it>32)
            return false;
    }

    return true;
}



int main()
{
    string s;
    getline(cin, s);

    istringstream iss(s);

    string t1;

    while(iss>>t1)
    {
        if(just_whitespace(t1)==true)
        {
            cout<<"JUST WHITESPACE"<<endl;
        }
        else
        {
                cout<<t1<<endl;
        }
    }

    string extra;
    iss>>extra;

    if(extra.empty())
    {
        cout<<"finally"<<endl;
    }

    return 0;
}