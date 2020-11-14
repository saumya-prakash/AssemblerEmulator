#include <bits/stdc++.h>

using namespace std;

const map<unsigned, pair<unsigned, string> > decoder = {  {0, {1, "ldc"}},
                                                                    {1, {1, "adc"}},
                                                                    {2, {1, "ldl"}},
                                                                    {3, {1, "stl"}},
                                                                    {4, {1, "ldnl"}},
                                                                    {5, {1, "stnl"}},
                                                                    {6, {0, "add"}},
                                                                    {7, {0, "sub"}},
                                                                    {8, {0, "shl"}},
                                                                    {9, {0, "shr"}},
                                                                    {10, {1, "adj"}},
                                                                    {11, {0, "a2sp"}},
                                                                    {12, {0, "sp2a"}},
                                                                    {13, {1, "call"}},
                                                                    {14, {0, "return"}},
                                                                    {15, {1, "brz"}},
                                                                    {16, {1, "brlz"}},
                                                                    {17, {1, "br"}},
                                                                    {18, {0, "HALT"}}
};


string reverse_decode(unsigned a)
{
    unsigned opcode = (a<<24)>>24;
    ostringstream oss;

    map<unsigned, pair<unsigned, string> >::const_iterator mt = decoder.find(opcode);

    string mnemonic = mt->second.second;    // storing the mnemonic

    oss<<mnemonic;

    if(mt->second.first==1)     // operand also present
    {
        int tmp = a & 0xffffff00;
        tmp /= 256;

        oss<<" "<<tmp;
    }

    return oss.str();
}


int main()
{
    unsigned long long int a = 0;

    a++;

    while(a!=0)
    {
        ++a;
        // cout<<a<<endl;
    }


    

    return 0;
}

