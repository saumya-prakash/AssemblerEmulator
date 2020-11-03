#include <string>
#include <map>
#include <utility>
#include <list>
#include <fstream>
#include <ostream>
#include <sstream>
#include <cctype>


using std::string;
using std::map;
using std::multimap;
using std::pair;
using std::list;
using std::ifstream;
using std::ostream;
using std::istringstream;

struct label
{
    string name;
    unsigned int address, use_cnt;  

    label(string s, unsigned int a): name(validate(s)), address(a), use_cnt(0)
    {
        ;        
    }

    private:
        string validate(const string &s)
        {
            for(string::const_iterator it=s.begin(); it!=s.end(); ++it)
                if(*it<=32 || isalnum(*it)==0)
                    return string();
            
            if(isalpha(s[0])==0)
                return string();

            return s;
        }
};


struct line
{
    string s;
    unsigned line_no, pc;

    line(string a, unsigned b):s(a), line_no(b), pc(-1) {}
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
        
        map<string, struct label> symtab;   // symbol table

        list<struct line> lines;       // to store the read source file

        ifstream &src;      // reference to the openend file stream for reading
        string filename;    // to store the filename -> will be used to name output files

        multimap<unsigned, class Error> errors;     // stores errors line no. wise

    public:

        Assembler(ifstream &a, const string &b): src(a), filename(b)   {}

        void assemble() ;

        void print_lines(ostream&);


    private:
        void read();
        void first_pass();
        void second_pass();   

};  




class Error
{
    friend class Assembler;

    private:
        string message;
        static map<unsigned, string> errtab;

        Error(unsigned e=0): message(errtab[e]) {} 

};




class Warning
{
    friend class Assembler;

    private:
        static map<unsigned, string> warntab;

        // more careful design required as wanings are usually collected after 
        // completing the assembling process
        // Focus on error part first. Warnings can be taken care of later as well.

};  

