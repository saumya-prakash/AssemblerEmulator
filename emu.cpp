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

    char c;

    do
    {
        cin>>c;

        switch(c)
        {
            case 'a':   while(e1.get_program_status()!=0)
                            e1.execute();
                        
                        cout<<"Total instructions executed = "<<e1.instructions_executed()<<endl;
                        cout<<endl;
                        break;


            case 't':   if(e1.get_program_status()==0)
                        {   cout<<"Program finished execution"<<endl;
                            break;
                        }
                        cout<<e1.execute()<<endl;
                        cout<<endl;
                        break;

            case 'd':   e1.memory_dump(cout);
                        break; 


            case 'x':   cout<<"Emulation terminated"<<endl;
                        cout<<endl;
                        break;

            default:    cout<<"Invalid option"<<endl;
                        cout<<endl;
                        break;
        
        }


    } while (c!='x');
    

    cout<<nounitbuf;
    return EXIT_SUCCESS;
}
