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
    cout<<"Lines for pass-2: "<<endl;
    a1.print_lines(cout);
    cout<<endl;
    
    cout<<"Errors: "<<endl;
    a1.print_errors(cout);
    cout<<endl;

    cout<<"Symbol Table: "<<endl;
    a1.print_symtab(cout);
    cout<<endl;


    // string s;
    // unsigned line_cnt = 0;
    // list<struct line> lines;


    // while(getline(src, s))
    // {
    //     ++line_cnt;

    //     string::iterator it = s.begin();

    //     while(it!=s.end() && *it <= 32)     // remove leading spaces
    //         ++it;
        
    //     string::iterator jt=it;

    //     while(jt!=s.end() && *jt!=';')      // remove comment part
    //         ++jt;

    //                             // removing trailing spaces -> NO NEED
    //     string tmp(it, jt);

        
    //     if(tmp.size()>0)            // break the 'cleaned' line into tokens -> it will be a labelled line or not a labelled line
    //     {
    //         // cout<<"Line "<<line_cnt<<": "<<endl;

    //         string::size_type ind = tmp.find(":");

    //         if(ind!=string::npos)   // labelled line found
    //         {
    //             string::iterator jt = tmp.begin()+ind;

    //             string label(tmp.begin(), jt);      // get that label

    //             tmp = string(jt+1, tmp.end());      // process the line to remove the label part
    //         }

    //         struct line t1 = {tmp, line_cnt};
    //         lines.push_back(t1);            // put the line in lines list
            
    //     }
    // }
    //     /* 
    //         Now lines contains formatted lines to be used by pass-2
    //         Note that there may be leading spaces after removing the inital label in pass-1.
    //         Comments have already been removed.
    //         Now we just have to 'scan' a line, get instruction and convert it into machine code, if there is no error. 
    //         Also, labels that are found to be used in instructions can be looked up in the symbol table constructed during pass-1.

    //     */
    // for(list<struct line>::iterator it=lines.begin(); it!=lines.end(); ++it)
    // {
    //     cout<<it->line_no<<"\t"<<it->s<<endl;
    // }


    exit(EXIT_SUCCESS);
}