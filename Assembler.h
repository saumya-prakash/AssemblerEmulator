// #include <string>
// #include <map>
// #include <utility>
// #include <list>
// #include <fstream>
// #include <ostream>
// #include <sstream>
// #include <cctype>
// #include <ios>

#include <bits/stdc++.h>

// using namespace std;

using std::string;
using std::map;
using std::set;
using std::multimap;
using std::pair;
using std::list;
using std::ifstream;
using std::ostream;
using std::istringstream;
using std::stringstream;
using std::hex;
using std::oct;
using std::dec;
using std::endl;
using std::ends;
using std::flush;

struct label
{
    string name;
    unsigned int addr, use_cnt, line_no;  

    label(string s, unsigned int a): name(validate(s)), addr(a), use_cnt(0), line_no(0) {}

    label(): name(string()), addr(-1), use_cnt(0), line_no(0) {}

    private:
        string validate(const string &s)
        {
            if(isalpha(s[0])==0)    // first character should be an alphabet
                return string();
                
            for(string::const_iterator it=s.begin(); it!=s.end(); ++it)     // should consist of only alpha-numerics
                if(*it<=32 || isalnum(*it)==0)
                    return string();

            return s;
        }
};



struct line
{
    string s;
    unsigned line_no, addr;

    line(string a, unsigned b, unsigned c):s(a), line_no(b), addr(c) {}
};


class Assembler;
class Error;


class Assembler
{

    public:
        typedef unsigned int encoding;
        typedef unsigned int i_type;

    private:
        static map<string, pair<i_type, encoding> > optab;    // opcode table
        
        map<string, struct label> symtab;       // symbol table
        map<unsigned, unsigned> data_to_reserve;

        list<struct line> lines;       // to store the read source file

        ifstream &src;      // reference to the openend file stream for reading
        string filename;    // to store the filename -> will be used to name output files

        unsigned pc, data_addr, line_cnt;    // program counter

        multimap<unsigned, class Error> errors;    // stores errors line no. wise
        multimap<unsigned, class Warning> warnings; // stores warnings (some with line number, some independent)

    public:

        Assembler(ifstream &a, const string &b): src(a), filename(b), pc(0), data_addr(0x00010000), line_cnt(0)   {}

        void assemble() ;

        void print_lines(ostream&) const;

        void print_errors(ostream&) const;

        void print_warnings(ostream&) const;

        void print_symtab(ostream&) const;

    private:
        void first_pass();
        void analyze(string&);
        void second_pass();   

        void insert_into_symtab(struct label&);

        bool valid_number(const string&) const;
        int str_to_int(string&) const;
};  




class Error
{
    friend class Assembler;

    private:
        string message;
        static map<unsigned, string> errtab;

        Error(unsigned e): message(errtab[e]) {} 

        // Error(): message()
};



class Warning
{
    friend class Assembler;
    private:
        static map<unsigned, string> warntab;
        string message;

        Warning(unsigned w): message(warntab[w]) {}

        // more careful design required as wanings are usually collected after 
        // completing the assembling process
        // Focus on error part first. Warnings can be taken care of later as well.

};  

