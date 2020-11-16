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
    cout<<"\tx - Quit the emulator"<<endl;

    return;
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
