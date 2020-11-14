#include <iostream>
#include <iomanip>

#include <string>
#include <cctype>

#include <utility>
#include <map>
#include <set>
#include <list>

#include <fstream>
#include <ostream>
#include <sstream>

using std::cin;
using std::cout;

using std::string;
using std::map;
using std::set;
using std::multimap;
using std::pair;
using std::list;

using std::ifstream;
using std::ofstream;
using std::ostream;
using std::istringstream;

using std::endl;
using std::ends;
using std::flush;
using std::unitbuf;
using std::nounitbuf;
using std::showbase;
using std::noshowbase;
using std::uppercase;
using std::nouppercase;
using std::setfill;
using std::setw;

using std::hex;
using std::oct;
using std::dec;



class Assembler;
class Error;
class Warning;

struct label;
struct line;


struct label    // stores complete info about a label -> name, line_number, address, use_count
{
    string name;
    unsigned int addr, use_cnt, line_no;  

    label(string s, unsigned int a): name(validate(s)), addr(a), use_cnt(0), line_no(0) {}

    label(): name(string()), addr(-1), use_cnt(0), line_no(0) {}

    private:
        string validate(const string &s)    // checks for valid label name
        {
            if(isalpha(s[0])==0)    // first character should be an alphabet
                return string();
                
            for(string::const_iterator it=s.begin(); it!=s.end(); ++it)     // should consist of only alpha-numerics
                if(*it<=32 || isalnum(*it)==0)
                    return string();

            return s;
        }
};



struct line     // for storing lines of the source program
{
    string s;
    unsigned line_no, addr, encoding;

    line(string a, unsigned b, unsigned c):s(a), line_no(b), addr(c), encoding(0) {}
};




class Assembler     // class for ASSEMBLER
{
    public:
        enum endianess {lil_endian, big_endian};

    private:
        static const int offset_min = -0x00800000;
        static const int offset_max = 0x007fffff;

        static const unsigned pc_lower = 0x0;
        static const unsigned pc_upper = 0x3ff;
        static const unsigned data_lower = 0x400;
        static const unsigned address_end = 0xffff;

        static const char format_code[8];   // represents magic number for object file

        static map<string, pair<unsigned, unsigned> > optab;    // opcode table
        static endianess machine_type;      // stores endianess of local machine
        
        map<string, struct label> symtab;       // symbol table
        map<unsigned, unsigned> data_to_reserve;    // data table

        list<struct line> lines;       // stores lines required in pass-2
        list<struct line> aux_lines;    // stores lines that are not required in pass-2

        unsigned pc, data_addr, line_cnt;    // program counter, data_addr, and line counter



        ifstream &src;      // reference to the openend file stream for reading
        string filename;    // to store the filename -> will be used to name output files


        multimap<unsigned, class Error> errors;    // stores errors line no. wise
        multimap<unsigned, class Warning> warnings; // stores warnings (some with line number, some independent)

        bool assembled;     // stores result of assembly process
        bool warn;


    public:

        Assembler(ifstream &a, const string &b): src(a), filename(b), pc(pc_lower), data_addr(data_lower), line_cnt(0), assembled(false), warn(false)   {}

        void assemble() ;   // assembles the inputted file

        bool get_assembly_status() const { return assembled; }
        bool get_warning_status() const { return warn; }

        void print_lines(ostream&) const;   

        void print_aux_lines(ostream&) const;
        
        void print_errors(ostream&) const;

        void print_warnings(ostream&) const;

        void print_symtab(ostream&) const;

        static enum endianess get_endianess(); // finds endianess of local machine

        void generate_log_file() const;     // generates log file

        void generate_listing_file() const; // generates lisitng file


    private:
        void first_pass();      // first pass
        void analyze(string&);  // analyzes lines in first pass - labels, SET directive, data directive
        void second_pass();     // second pass

        void generate_object_file() const;

        void print_bytes(ostream&, unsigned) const;   // prints bytes in object file

        unsigned encode_zeroth(const string&, const unsigned&, const unsigned&);    // encodes instructions with 0 operands
        unsigned encode_first(const string&, const unsigned&, const unsigned&, const unsigned& pc); // encodes instructions with 1 operand

        inline int calculate_offset(const unsigned&, const unsigned&) const;    // calculates offset based on PC and label values


        void insert_into_symtab(struct label&);  // inserts into symbol table

        bool valid_number(const string&) const; // checks if number is valid - decimal, octal or hexadecimal
        int str_to_int(string&) const;

        void reset_pc() {pc = pc_lower;}    
        void reset_data_addr()  {data_addr = data_lower;}
        void reset_line_cnt()   {line_cnt = 0;}
};  


class Error     // class that represents ERRORS in source program
{
    friend class Assembler;

    private:
        string message;
        static map<unsigned, string> errtab;

        Error(unsigned e): message(errtab[e]) {} 

        // Error(): message()
};



class Warning   // class for WARNINGS
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


