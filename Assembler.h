#include <string>
#include <map>

using std::string;
using std::map;

struct label
{
    string name;
    unsigned int use_cnt, address;  

    label(string s, unsigned int a): name(s), use_cnt(0), address(a) 
    {
        ;
    }

};


class Assembler
{
    public:
        typedef unsigned int machine_word; 

    private:
        static const map<string, machine_word> optab;

        map<string, struct label> symtab;
    

};  

const map<string, Assembler::machine_word> optab = {  {"ldc", 0},
                                                                {"adc", 1},
                                                             };