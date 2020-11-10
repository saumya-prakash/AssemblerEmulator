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
using std::make_pair;
using std::list;

using std::ifstream;
using std::ofstream;
using std::ostream;
using std::istringstream;
using std::stringstream;

using std::cin;
using std::cout;

using std::endl;
using std::ends;
using std::flush;
using std::unitbuf;
using std::nounitbuf;
using std::showbase;
using std::noshowbase;

using std::hex;
using std::oct;
using std::dec;



class Assembler;
class Error;

struct label;
struct line;



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
    unsigned line_no, addr, encoding;

    line(string a, unsigned b, unsigned c):s(a), line_no(b), addr(c), encoding(0) {}
};




class Assembler
{
    public:
        enum endianess {little_endian, big_endian};

    private:
        static map<string, pair<unsigned, unsigned> > optab;    // opcode table
        
        map<string, struct label> symtab;       // symbol table
        map<unsigned, unsigned> data_to_reserve;

        list<struct line> lines;       // stores lines required in pass-2
        list<struct line> aux_lines;    // stores lines that are not required in pass-2

        unsigned pc, data_addr, line_cnt;    // program counter

        static const int offset_min = -0x00800000;
        static const int offset_max = 0x007fffff;

        ifstream &src;      // reference to the openend file stream for reading
        string filename;    // to store the filename -> will be used to name output files


        multimap<unsigned, class Error> errors;    // stores errors line no. wise
        multimap<unsigned, class Warning> warnings; // stores warnings (some with line number, some independent)

        static enum endianess machine_type;
        bool assembled;

    public:

        Assembler(ifstream &a, const string &b): src(a), filename(b), pc(0x00000000), data_addr(0x00010000), line_cnt(0), assembled(false)   {}

        void assemble() ;

        void print_lines(ostream&) const;

        void print_aux_lines(ostream&) const;
        
        void print_errors(ostream&) const;

        void print_warnings(ostream&) const;

        void print_symtab(ostream&) const;

        void generate_listing_file() const;

        static enum endianess get_endianess() ;


    private:
        void first_pass();
        void analyze(string&);
        void second_pass();   

        unsigned encode_zeroth(const string&, const unsigned&, const unsigned&);
        unsigned encode_first(const string&, const unsigned&, const unsigned&, const unsigned& pc);

        inline int calculate_offset(const unsigned&, const unsigned&) const;


        void insert_into_symtab(struct label&);

        bool valid_number(const string&) const;
        int str_to_int(string&) const;

        void reset_pc() {pc=0x00000000;}
        void reset_data_addr()  {data_addr=0x00010000;}
        void reset_line_cnt()   {line_cnt=0;}
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


