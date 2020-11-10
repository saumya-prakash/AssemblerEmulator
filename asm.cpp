// #include <iostream>

#include "Assembler.h"


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
    
    // cout<<"Lines for pass-2: "<<endl;
    // a1.print_lines(cout);
    // cout<<endl;

    // cout<<"Aux lines: "<<endl;
    // a1.print_aux_lines(cout);
    // cout<<endl;
    
    cout<<endl<<"Errors: "<<endl;
    a1.print_errors(cout);
    cout<<endl;

    cout<<endl<<"Symbol Table: "<<endl;
    a1.print_symtab(cout);
    cout<<endl;

    cout<<endl<<"Warnings: "<<endl;
    a1.print_warnings(cout);
    cout<<endl;

    

    exit(EXIT_SUCCESS);
}