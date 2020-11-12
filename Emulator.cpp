#include "Emulator.h"


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

const char Emulator::format_code[8] = "\\\'\'\"\t\f\b";




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
    SP = 0xffff;    // set SP to last memory address

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
