#include "Emulator.h"
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::ends;
using std::flush;
using std::unitbuf;
using std::nounitbuf;
using std::hex;
using std::dec;


void print_options()
{
    cout<<"Options :-"<<endl;

    cout<<"\tt - Execute one instruction with trace"<<endl;
    cout<<"\ts - Execute all instrutions with trace"<<endl;
    cout<<"\tn number - Execute specified number of instructions"<<endl;
    cout<<"\ta - Execute all instrucions without trace"<<endl;
    cout<<"\tq - Show current architectural state"<<endl;
    cout<<"\tc - Display number of instructions executed so far"<<endl;
    cout<<"\tu - Disassemble"<<endl;
    cout<<"\td - Show memory dump"<<endl;
    cout<<"\ti - Show complete ISA"<<endl;
    cout<<"\tx - Quit the emulator"<<endl;

    return;
}


void print_ISA()
{
    cout<<setw(6)<<"Opcode"<<'\t'<<setw(8)<<"Mnemonic"<<'\t'<<setw(7)<<"Operand"<<'\n';

    cout<<setw(6)<<"0"<<'\t'<<setw(8)<<"ldc"<<'\t'<<setw(7)<<"value"<<'\n';
    cout<<setw(6)<<"1"<<'\t'<<setw(8)<<"adc"<<'\t'<<setw(7)<<"value"<<'\n';
    cout<<setw(6)<<"2"<<'\t'<<setw(8)<<"ldl"<<'\t'<<setw(7)<<"offset"<<'\n';
    cout<<setw(6)<<"3"<<'\t'<<setw(8)<<"stl"<<'\t'<<setw(7)<<"offset"<<'\n';
    cout<<setw(6)<<"4"<<'\t'<<setw(8)<<"ldnl"<<'\t'<<setw(7)<<"offset"<<'\n';
    cout<<setw(6)<<"5"<<'\t'<<setw(8)<<"stnl"<<'\t'<<setw(7)<<"offset"<<'\n';
    cout<<setw(6)<<"6"<<'\t'<<setw(8)<<"add"<<'\t'<<setw(7)<<""<<'\n';
    cout<<setw(6)<<"7"<<'\t'<<setw(8)<<"sub"<<'\t'<<setw(7)<<""<<'\n';
    cout<<setw(6)<<"8"<<'\t'<<setw(8)<<"shl"<<'\t'<<setw(7)<<""<<'\n';
    cout<<setw(6)<<"9"<<'\t'<<setw(8)<<"shr"<<'\t'<<setw(7)<<""<<'\n';
    cout<<setw(6)<<"10"<<'\t'<<setw(8)<<"adj"<<'\t'<<setw(7)<<"value"<<'\n';
    cout<<setw(6)<<"11"<<'\t'<<setw(8)<<"a2sp"<<'\t'<<setw(7)<<""<<'\n';
    cout<<setw(6)<<"12"<<'\t'<<setw(8)<<"sp2a"<<'\t'<<setw(7)<<""<<'\n';
    cout<<setw(6)<<"13"<<'\t'<<setw(8)<<"call"<<'\t'<<setw(7)<<"offset"<<'\n';
    cout<<setw(6)<<"14"<<'\t'<<setw(8)<<"return"<<'\t'<<setw(7)<<""<<'\n';
    cout<<setw(6)<<"15"<<'\t'<<setw(8)<<"brz"<<'\t'<<setw(7)<<"offset"<<'\n';
    cout<<setw(6)<<"16"<<'\t'<<setw(8)<<"brlz"<<'\t'<<setw(7)<<"offset"<<'\n';
    cout<<setw(6)<<"17"<<'\t'<<setw(8)<<"br"<<'\t'<<setw(7)<<"offset"<<'\n';
    cout<<setw(6)<<"18"<<'\t'<<setw(8)<<"HALT"<<'\t'<<setw(7)<<""<<'\n';
    cout<<'\n';
    cout<<setw(6)<<""<<'\t'<<setw(8)<<"data"<<'\t'<<setw(7)<<"value"<<'\n';
    cout<<setw(6)<<""<<'\t'<<setw(8)<<"SET"<<'\t'<<setw(7)<<"value"<<'\n';


    cout<<'\n';
}


int main(int argc, char** argv)
{
    cout<<unitbuf;
    cin.tie(&cout);

    if(argc==1)
    {
        cout<<"No input file specified"<<endl;
        return EXIT_FAILURE;
    }

    Emulator e1(argv[1]);

    int loading = e1.loader();

    if(loading)
        cout<<"Object file loaded successfully"<<endl<<endl;
    
    else
    {
        cout<<"Error in loading object file"<<endl;
        cout<<"Process terminated"<<endl;
        return EXIT_FAILURE;
    }

    print_options();
    cout<<endl;

    char c;     // interact with the user
    do
    {
        cin>>c;

        switch(c)
        {
            case 'a':       // execute all instructions in one go
                        if(e1.get_program_status()==-1)     // program in fault state
                        {
                            cout<<e1.get_fault_cause()<<endl<<endl;
                            break;
                        }

                        if(e1.get_program_status()==0)
                        {
                            cout<<"Program finished execution"<<endl<<endl;
                            break;
                        }
                        
                        while(e1.get_program_status()==1)
                            e1.execute();
                        
                        if(e1.get_program_status()==-1)     // program in fault state after completing execution
                        {
                            cout<<e1.get_fault_cause()<<endl<<endl;
                            break;
                        }

                        cout<<"Total instructions executed = "<<e1.instructions_executed()<<endl;
                        cout<<endl;
                        break;


            case 't':       // execute a single instruction with trace
                        if(e1.get_program_status()==-1)
                        {
                            cout<<e1.get_fault_cause()<<endl;
                            cout<<endl;
                            break;
                        }

                        if(e1.get_program_status()==0)
                        {   cout<<"Program finished execution"<<endl;
                            cout<<endl;
                            break;
                        }

                        cout<<e1.execute()<<endl;
                        cout<<endl;
                        break;

            case 's':   // execute all instrutions with trace
                        if(e1.get_program_status()==-1)     // program in fault state
                        {
                            cout<<e1.get_fault_cause()<<endl<<endl;
                            break;
                        }

                        if(e1.get_program_status()==0)
                        {
                            cout<<"Program finished execution"<<endl<<endl;
                            break;
                        }
                        

                        while(e1.get_program_status()==1)
                            cout<<e1.execute()<<endl;
                        cout<<endl;
                
                        cout<<"Total instructions executed = "<<e1.instructions_executed()<<endl;
                        cout<<endl;
                        break;


            case 'n':       // print specified number of instructions
                        {
                            int a = 0;
                            cin>>a;
                            int i = 0;
                            while(i<a && e1.get_program_status()==1)
                            {
                                i++;
                                cout<<e1.execute()<<endl;
                            }
                            cout<<endl;
                        }
                        
                        break;

            case 'q':       // show current architectural state
                        cout<<e1.current_state()<<endl;
                        cout<<endl;
                        break;
                        

            case 'u':       // disassemble
                        e1.disassemble(cout);
                        cout<<endl;
                        break;



            case 'd':       // outputs memory dump
                        e1.memory_dump(cout);
                        break; 


            case 'c':   // number of instructions executed thus far
                        cout<<"Number of instructions executed = "<<e1.instructions_executed()<<endl;
                        cout<<endl;
                        break;

            case 'i':   // show ISA
                        print_ISA();
                        break;
            
            case 'x':       // QUIT
                        cout<<"Emulation terminated"<<endl;
                        cout<<endl;
                        break;

                        
            default:    cout<<"Invalid option"<<endl;
                        print_options();
                        cout<<endl;

                        continue;
                        break;
        }


    } while (c!='x');
    

    cout<<nounitbuf;
    return EXIT_SUCCESS;
}
