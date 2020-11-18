#include <iostream>
#include <iomanip>
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
using std::showbase;
using std::noshowbase;
using std::setw;
using std::setfill;
using std::left;
using std::right;

using std::ostream;
using std::ostringstream;
using std::ifstream;
using std::string;



class Emulator          // class for EMULATOR
{
    public:
        enum endianess {lil_endian, big_endian};
    private:
    
        static const unsigned pc_lower = 0x0;      // PC start address
        static const unsigned end_address = 0xffff;    

        static const unsigned total_memory = end_address+1;   // TOTAL 16K words memory space

        static const unsigned long long int max_instruction_count = 0x0ffffffff; // approx. 4 billion instructions can be executed 
                                                                                // before infinite loop exception triggers  

        static const char format_code[8];   // to check format of object file provided

        static const map<unsigned, pair<unsigned, string> > decoder;  // opcode to mnemonic converter
        static const map<unsigned, string> faults;

       
        static const endianess machine_type;    // to store endianess of local machine

        string obj_filename;    // stores object file's name
        int A, B;           // internal registers
        int PC, SP;         // Program Counter and Stack Pointer
        int *mempory_space;   // 16k words memory space
        int pc_upper;
        int data_lower;
        int data_upper;
        

        unsigned long long int instr_cnt;   // to store number of instructions executed so far 
                                  
        

        unsigned text_size; // text segment size of provided object file
        unsigned data_size; // data segment size of provided object file

        bool finished;      // whether execution of program finished
        bool fault;         // whether some fault has occurred
        string fault_cause; // stores the reason of fault and the instruction at which fault occurred

    public:
        Emulator(const string& s): obj_filename(s), A(0), B(0), PC(0), SP(0), mempory_space(new int[total_memory]), pc_upper(-1), data_lower(0), data_upper(0), instr_cnt(0), finished(false), fault(false), fault_cause(string()) {}     // PC and SP will be put to corect values by the loader
        ~Emulator() { delete [] mempory_space; }

        bool loader();  // loads machine code into memory_space

        void memory_dump(ostream&) const;   // memory-dumper function

        string current_state() const;   // returns current architectural state of the emulated machine

        unsigned long long int instructions_executed() const { return instr_cnt; }  // returns number of instructions executed so far
        int get_program_status() const;     // returns program status -> ACTIVE, FINISHED, FAULT
        
        string get_fault_cause() const { return fault_cause; }

        string execute();   // returns current architectural state and then executes one instruction

        void disassemble(ostream&) const;


    private:
        static endianess get_endianess();   // finds endianess of the local machine

        unsigned get_int(ifstream&) const;  // extracts operand's value from machine code

        string reverse_decode(unsigned) const;  // gets assembly-level statement from machine code

        bool check_PC(int) ;    // checks for PC value and raises fault
        bool check_SP(int) ;    // checks for SP value and raises fault
        bool check_memory_access(int) ; // checks for memory address value and raises fault
        
};
