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

    while(!e1.execute().empty())
    {
        cout<<e1.current_state()<<endl;
    }

    cout<<nounitbuf;
    return EXIT_SUCCESS;
}
