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
        mempory_space[i+static_data_lower] = get_int(fi);
    }

    PC = 0x00000000;     // SET TO 0x0
    A = 0x00000000;
    B = 0x00000000;
    SP = 0xf000;    // set SP to ?? NOT CONFIRMED

    fi.close();

    return true;
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
    if(finished==true)
        return string();
    

    string res = current_state();

    int mach_code = mempory_space[PC];     // get current machine code

    int opcode = (mach_code<<24)>>24;
    int operand = (mach_code & static_cast<int>(0xffffff00)) / 256; // cast required for operations to work correctly work correctly

    switch(opcode)
    {
        case 0:         // ldc value
                B = A;
                A = operand;
                break;
        
        case 1:
                A = A + operand;
                break;

        case 2:             // proper checks to be included
                B = A;
                A = mempory_space[SP+operand];
                break;

        case 3:
                mempory_space[SP+operand] = A;
                A = B;
                break;

        case 4:
                A = mempory_space[A+operand];
                break;
        
        case 5:
                mempory_space[A+operand] = B;
                break;

        case 6:
                A = B + A;
                break;

        case 7:
                A = B - A;
                break;

        case 8:
                A = B<<A;
                break;
        
        case 9:
                A = B>>A;
                break;
        
        case 10:
                SP = SP + operand;
                break;
        
        case 11:
                SP = A;
                A = B;
                break;
        
        case 12:
                B = A;
                A = SP;
                break;
        
        case 13:
                B = A;
                A = PC;
                PC = PC+operand;
                break;

        case 14:
                PC = A;
                A = B;
                break;

        case 15:
                if(A==0)
                    PC = PC+operand;
                break;
        
        case 16:
                if(A<0)
                    PC = PC+operand;
                break;

        case 17:
                PC = PC+operand;
                break;
        
        case 18:
                finished = true;
                break;
        
        default:    
                return ("Invalid opcode");
                break;
    }
     
     PC++;

    return res;
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





void Emulator::memory_dump(ostream& os) const
{
    int per_line = 4;

    unsigned i = pc_lower;
    while(i<text_size)
    {
        os<<mempory_space[i]<<" ";
        i++;

        if(i%per_line==0)
            os<<'\n';
    }

    os<<"\n\n";

    i = 0;
    while(i<data_size)
    {
        os<<mempory_space[i+static_data_lower]<<" ";
        i++;

        if(i%per_line==0)
            os<<'\n';
    }

}


string Emulator::current_state() const
{
    ostringstream oss;

    oss<<"PC = "<<PC<<" , "<<"A = "<<A<<" , "<<"B = "<<B<<" , "<<"Memory_Content = "<<hex<<mempory_space[PC]<<dec<<" , "<<reverse_decode(mempory_space[PC]);

    return oss.str();

}