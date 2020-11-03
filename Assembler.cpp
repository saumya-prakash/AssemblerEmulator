#include "Assembler.h"

map<string, pair<Assembler::i_type, Assembler::encoding> > optab = {  
                                            {"ldc", {1, 0}},
                                            {"adc", {1, 1}},
                                            {"ldl", {1, 2}},
                                            {"stl", {1, 3}},
                                            {"ldnl", {1, 4}},
                                            {"stnl", {1, 5}},
                                            {"add", {0, 6}},
                                            {"sub", {0, 7}},
                                            {"shl", {0, 8}},
                                            {"shr", {0, 9}},
                                            {"adj", {1, 10}},
                                            {"a2sp", {0, 11}},
                                            {"sp2a", {0, 12}},
                                            {"call", {1, 13}},
                                            {"return", {0, 14}},
                                            {"brz", {1, 15}},
                                            {"brlz", {1 ,16}},
                                            {"br", {1, 17}},
                                            {"HALT", {0, 18}}
                                        };


map<unsigned, string> Error::errtab = {
                                        {0, "Invalid statement"},    /* default error message */
                                        {1, "Invalid label name"},
                                        {2, "Unknown label name"},
                                        {3, "Unknown mnemonic"},
                                        {4, "Invalid operand"},
                                        {5, "Operand expected"},
                                        {6, "Mnemonic doesn't reqiure an operand"}
                                    };

map<unsigned, string> Warning::warntab = {  
                                            {0, "Deafult warning"},
                                            {1, "Unused variable"}
};



void Assembler::assemble()
{
    read();     

    first_pass();

    second_pass();

    return ;
}


void Assembler::read()
{
    string s;
    unsigned line_cnt = 0;

    while(getline(src, s))
    {
        ++line_cnt;

        string::iterator it = s.begin();

        while(it!=s.end() && *it <= 32)     // remove leading whitespace
            ++it;
        
        string::iterator jt=it;

        while(jt!=s.end() && *jt!=';')      // remove comment part
            ++jt;

                                // removing trailing spaces -> NO NEED
        string tmp(it, jt);

        if(tmp.size()>0)    // put the non-empty line in the list
            lines.push_back({tmp, line_cnt});

    }

    return ;
}

            /* Note that labels and mnemonics have different namespaces!*/

void Assembler::first_pass()
{
    unsigned tpc = 0;

    for(list<struct line>::iterator it=lines.begin(); it!=lines.end(); ++it)
    {
        string s = it->s;

        string::size_type ind = s.find(':');
        if(ind!=string::npos)       // : found -> label found
        {
            string tmp(s.begin()+ind+1, s.end());     // get after the label part

            istringstream iss(tmp);

            string t2;
            iss>>t2;

            if(t2=="SET")   // get the value/label specified with SET -> may need to process in pass-2
            {
                ;
            }

            else if(t2=="data")     // directive -> process it
            {
                ;
            }

            else    // label followed by a statement -> modify line and leave it for pass-2
            {
                ;
            }
        }


        else        // not a labeled statement
        {
            istringstream iss(s);
            string t2;

            iss>>t2;

            if(t2=="data")      // directive -> process this
            {
                ;           // store at some suitable place to process later
            }

            else        // some statement with instruction
            {
                it->pc = tpc;
                ++tpc;
            }     
        }
        

    } 
    
    return ;
}

void Assembler::second_pass()
{
    

    return ;
}




void Assembler::print_lines(ostream &os)
{
    for(list<struct line>::iterator it=lines.begin(); it!=lines.end(); ++it)
        os<<it->line_no<<": "<<it->s<<"\n";
}