#include <iostream>

#include "Assembler.h"

using std::cin;
using std::cout;

using namespace std;

int main(int argc, char **argv)
{
    if(argc==1)
    {
        cout<<"Fatal Error: No input file specified."<<endl;
        exit(EXIT_FAILURE);
    }

    ifstream src;

    src.open(argv[1], ofstream::in);

    if(!src)
    {
        cout<<"Error in opening file"<<endl;
        exit(EXIT_FAILURE);
    }

    Assembler a1(src, argv[1]);

    a1.assemble();
    a1.generate_listing_file();
    a1.generate_log_file();

    if(a1.get_assembly_status()==false)
    {   
        cout<<endl<<"Errors: "<<endl;
        a1.print_errors(cout);
        cout<<endl;
    }

    if(a1.get_assembly_status()==true)
    {
        cout<<"Assembled successfully"<<endl;
        cout<<endl;
    }


    if(a1.get_warning_status()==true)
    {
        cout<<endl<<"Warnings: "<<endl;
        a1.print_warnings(cout);
        cout<<endl;
    }


    src.close();
    

    exit(EXIT_SUCCESS);
}