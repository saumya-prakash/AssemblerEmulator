#include "Emulator.h"


const char Emulator::format_code[8] = "\\\'\'\"\t\f\b";


const map<unsigned, pair<unsigned, string> > Emulator::decoder = {  {0, {1, "ldc"}},
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

const map<unsigned, string> Emulator::faults = {{0, "PC goes out of text segment"},
                                                {1, "SP goes out of segment"},
                                                {2, "Program tried to access text segment memory area"},
                                                {3, "Program tried to access out of range memory address"},
                                                {4, "Too large text segment"},
                                                {5, "Too large data segment"},
                                                {6, "Program took too long to finish"}
};



Emulator::endianess Emulator::get_endianess()
{
    unsigned a = 1;
    char* ptr = (char *) &a;

    if(*ptr==1)
        return lil_endian;
    
    else
        return big_endian;
}

const Emulator::endianess Emulator::machine_type = Emulator::get_endianess();





bool Emulator::loader()     // no size checking has been done now -> just simple loading
{    
    ifstream fi(obj_filename, ifstream::binary | ifstream::in);

    if(!fi) // ERROR in opening file
    {
        return false;
    }

    char waste;

    char aux[8] = "\0";
    for(unsigned i=0; i<8; ++i)
    {
        fi.read(aux+i, 1);
    }

    for(unsigned i=0; i<8; ++i)
    {
        if(aux[i]!=format_code[i])
        {
            cout<<i<<" ";
            cout<<"Unknown object file format"<<endl;
            return false;
        }
    }

    fi.read(&waste, 1);
    if(waste!='\n')
    {
        cout<<"Unknown onject file format"<<endl;
        return false; 
    }


    text_size = get_int(fi);

    fi.read(&waste, 1);     // dump newline character

    for(unsigned i=0; i<text_size; ++i)     // load text segment
    {
        mempory_space[i] = get_int(fi);
    }

    fi.read(&waste, 1);     // dump newline character
    fi.read(&waste, 1);     // dump newline character
    
    data_size = get_int(fi);

    fi.read(&waste, 1);     // dump newline character

    for(unsigned i=0; i<data_size; ++i)
    {
        mempory_space[i+data_lower] = get_int(fi);
    }


    PC = 0x00000000;     // set PC to 0x0
    A = 0x00000000;
    B = 0x00000000;
    data_upper = data_lower + data_size - 1;    // denotes upper boundary of data loaded
    SP = 0xff00;    // set SP to ?? NOT CONFIRMED


    fi.close();     // close the object file

    return true;    // loading successful
}


unsigned Emulator::get_int(ifstream& fi) const
{
    unsigned res = 0;

    char *ptr = (char *) &res;
    char byte = 0;

    if(machine_type==big_endian)    // just read byte and put it
    {
        fi.read(&byte, 1);
        *ptr = byte;
        ptr++;

        fi.read(&byte, 1);
        *ptr = byte;
        ptr++;

        fi.read(&byte, 1);
        *ptr = byte;
        ptr++;

        fi.read(&byte, 1);
        *ptr = byte;
    }

    else    // read and put in reverse order
    {
        ptr += 3;

        fi.read(&byte, 1);
        *ptr = byte;
        ptr--;

        fi.read(&byte, 1);
        *ptr = byte;
        ptr--;

        fi.read(&byte, 1);
        *ptr = byte;
        ptr--;

        fi.read(&byte, 1);
        *ptr = byte;
    }
    
    return res;
}


string Emulator::execute()
{   
    if(fault==true)
    {
        return fault_cause;
    }

    if(finished==true)
        return string();

    string res = current_state();

    int mach_code = mempory_space[PC];     // get current machine code

    int opcode = (mach_code<<24)>>24;
    int operand = (mach_code & static_cast<int>(0xffffff00)) / 256; // cast required for operations to work correctly work correctly


    PC++;   // PC incremented to point to next instruction

    switch(opcode)
    {
        case 0:         // ldc value
                B = A;
                A = operand;
                break;
        
        case 1:         // adc value
                A = A + operand;
                break;

        case 2:         // ldl value      // proper checks to be included
                B = A;

                check_memory_access(SP+operand);
                if(fault==true)
                    return fault_cause;

                A = mempory_space[SP+operand];
                break;

        case 3:         // stl value
                check_memory_access(SP+operand);
                if(fault==true)
                    return fault_cause;

                mempory_space[SP+operand] = A;
                A = B;
                break;

        case 4:         // ldnl offset
                check_memory_access(A+operand);
                if(fault==true)
                    return fault_cause;
                
                A = mempory_space[A+operand];
                break;
        
        case 5:         // stnl offset
                check_memory_access(A+operand);
                if(fault==true)
                    return fault_cause;

                mempory_space[A+operand] = B;
                break;

        case 6:         // add
                A = B + A;
                break;

        case 7:         // sub
                A = B - A;
                break;

        case 8:         // shl
                A = B<<A;
                break;
        
        case 9:         // shr
                A = B>>A;
                break;
        
        case 10:        // adj value
                check_SP(SP+operand);
                if(fault==true)
                    return fault_cause;

                SP = SP + operand;
                break;
        
        case 11:        //a2sp
                check_SP(A);
                if(fault==true)
                    return fault_cause;

                SP = A;
                A = B;
                break;
        
        case 12:        // sp2a
                B = A;
                A = SP;
                break;
        
        case 13:        // call offset
                B = A;
                A = PC;

                check_PC(PC+operand);
                if(fault==true)
                    return fault_cause;
                
                PC = PC+operand;
                break;

        case 14:        // return 
                check_PC(A);
                if(fault==true)
                    return fault_cause;
                
                PC = A;
                A = B;
                break;

        case 15:        // brz offset
                if(A==0)
                {   check_PC(PC+operand);
                    if(fault==true)
                        return fault_cause;
                
                    PC = PC+operand;
                }

                break;
        
        case 16:        // brlz offset
                if(A<0)
                {
                    check_PC(PC+operand);
                    if(fault==true)
                        return fault_cause;
                    
                    PC = PC+operand;
                }

                break;

        case 17:        // br offset
                check_PC(PC+operand);
                if(fault==true)
                    return fault_cause;
                
                PC = PC+operand;
                break;
        
        case 18:        // HALT
                finished = true;
                break;
        
        default:    
                return ("Invalid opcode");
                break;
    }

    instr_cnt++; 

    if(instr_cnt==0) // looped back to 0 -> limit crossed
    {
        cout<<"Program took too long to finish"<<endl;
        cout<<endl;                                    // indiacte program took too long to finish

        exit(EXIT_FAILURE);
    }

    return res;
}



bool Emulator::check_PC(int a)
{
    if(a<0 || a>pc_upper)
    {
        fault = true;
        string tmp = faults.find(0)->second;
        PC--;
        string tmp2 = current_state();
        PC++;

        fault_cause = tmp + " when executing " + tmp2;

        return false;
    }

    return true;
}

bool Emulator::check_SP(int a)
{
    if(a>end_address || a<=data_upper)
    {
        fault = true;
        string tmp = faults.find(1)->second;
        PC--;
        string tmp2 = current_state();
        PC++;

        fault_cause = tmp + " when executing " + tmp2;
        return false;
    }

    return true;
}

bool Emulator::check_memory_access(int a)
{
    string tmp;

    if(a<0 || a>end_address)
    {
        fault = true;
        tmp = faults.find(3)->second;
    }

    if(a<=pc_upper)
    {
        fault = true;
        tmp = faults.find(2)->second;
    }

    if(fault==false)
        return true;

    PC--;
    string tmp2 = current_state();
    PC++;

    fault_cause = tmp + " when executing " + tmp2;

    return false;
}



int Emulator::get_program_status() const
{
    if(finished==true)
        return 0;       // finished execution
    
    if(fault==true)
        return -1;

    return 1;    // active

}


string Emulator::reverse_decode(unsigned a) const
{
    unsigned opcode = (a<<24)>>24;

    ostringstream oss;

    map<unsigned, pair<unsigned, string> >::const_iterator mt = decoder.find(opcode);

    string mnem = mt->second.second;    // storing the mnemonic

    oss<<mnem;

    if(mt->second.first==1)     // operand also present
    {
        int tmp = a & 0xffffff00;
        tmp /= 256;

        oss<<" "<<tmp;
    }

    return oss.str();
}



void Emulator::disassemble(ostream& os) const
{
    for(int i=0; i<text_size; ++i)
    {
        string tmp = reverse_decode(mempory_space[i]);

        os<<"PC = ";
        os<<setw(10)<<setfill('0')<<i<<setfill(' ')<<"   ";

        os<<"Memory Content = ";
        os<<"0x"<<setw(8)<<hex<<setfill('0')<<PC<<setfill(' ')<<dec<<"   ";

        cout<<" "<<tmp<<endl;
    }
}



void Emulator::memory_dump(ostream& os) const
{
    int per_line = 4;

    unsigned i = pc_lower;
    while(i<text_size)
    {
        os<<i<<": "<<mempory_space[i]<<'\n';
        i++;
    }

    os<<"\n\n";

    i = 0;
    while(i<data_size)
    {
        os<<i+data_lower<<": "<<mempory_space[i+data_lower]<<'\n';
        i++;
    }

}


string Emulator::current_state() const
{
    ostringstream oss;

    oss<<"PC = ";
    oss<<setw(10)<<setfill('0')<<PC<<setfill(' ')<<"   ";

    oss<<"SP = ";
    oss<<setw(10)<<setfill('0')<<SP<<setfill(' ')<<"   ";

    oss<<"A = ";
    oss<<setw(10)<<setfill('0')<<A<<setfill(' ')<<"   ";

    oss<<"B = ";
    oss<<setw(10)<<setfill('0')<<B<<setfill(' ')<<"   ";
    
    oss<<"Memory Content = ";
    oss<<"0x"<<setw(8)<<hex<<setfill('0')<<mempory_space[PC]<<setfill(' ')<<dec<<"   ";

    oss<<" "<<reverse_decode(mempory_space[PC]);

    return oss.str();

}