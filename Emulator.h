#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <string>

#include <utility>
#include <map>

using std::pair;
using std::map;

using std::cout;
using std::endl;
using std::flush;
using std::ends;
using std::hex;
using std::dec;

using std::ostream;
using std::ostringstream;
using std::ifstream;
using std::string;



class Emulator
{
    private:
    
        static const unsigned pc_lower = 0x0;      // PC start address
        static const unsigned pc_upper = 0x3ff;    // PC end address    - TOTAL 1K words text segment
        static const unsigned static_data_lower = 0x400;                                                
        static const unsigned end_address = 0xffff;    // TOTAL 16K words memory space

        static const unsigned total_memory = end_address+1;

        static const char format_code[8];

        static const map<unsigned, pair<unsigned, string> > decoder;

        enum endianess {lil_endian, big_endian};
        static const endianess machine_type;

        string obj_filename;
        int A, B;
        int PC, SP;
        int *mempory_space;   // 1 MB memory space

        unsigned long long int instr_cnt;

        unsigned text_size;
        unsigned data_size;

        bool finished;

    public:
        Emulator(const string& s): obj_filename(s), A(0), B(0), PC(0), SP(0), mempory_space(new int[total_memory]), instr_cnt(0), finished(false) {}     // PC and SP will be put to corect values by the loader
        ~Emulator() { delete [] mempory_space; }

        bool loader();
        void memory_dump(ostream&) const;
        string current_state() const;
        unsigned long long int instructions_executed() const { return instr_cnt; }
        int get_program_status() const;
        
        string execute();   // executes one instruction



    private:
        static endianess get_endianess();

        unsigned get_int(ifstream&) const;

        string reverse_decode(unsigned) const;
           
};
