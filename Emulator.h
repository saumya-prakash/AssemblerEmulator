#include <iostream>
#include <ostream>
#include <fstream>
#include <string>

using std::cout;
using std::endl;
using std::flush;
using std::ends;

using std::ostream;
using std::ifstream;
using std::string;



class Emulator
{
    private:
    
        static const unsigned pc_lower = 0x0;      // PC start address
        static const unsigned pc_upper = 0x3ff;    // PC end address    - TOTAL 1K words text segment
        static const unsigned static_data_lower = 0x400;                                                
        static const unsigned end_address = 0xffff;    // TOTAL 16K words memory space

        static const char format_code[8];


        enum endianess {lil_endian, big_endian};

        static const endianess machine_type;

        string obj_filename;
        int A, B;
        unsigned PC, SP;
        unsigned *mempory_space;   // 1 MB memory space

        unsigned text_size;
        unsigned data_size;

    public:
        Emulator(const string& s): obj_filename(s), A(0), B(0), PC(0), SP(0), mempory_space(new unsigned[end_address+1]) {}     // PC and SP will be put to corect values by the loader
        ~Emulator() { delete [] mempory_space; }

        bool loader();
        void memory_dump(ostream&) const;
        
    private:
        static endianess get_endianess();

        unsigned get_int(ifstream&) const;
           
};
