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
                                        {6, "Too many operands"},
                                        {7, "Duplicate label name"},
                                        {8, "Number expected"},
                                        {9, "Too few operands"}
                                    };


map<unsigned, string> Warning::warntab = {  
                                            {0, "Deafult warning"},
                                            {1, "Unused variable"}
};



bool Assembler::valid_number(const string& s) const
{
    string::const_iterator it = s.begin();

    if(*it=='+' || *it=='-')
        ++it;

    if(*it=='0')       // probably octal or hex
    {
        ++it;

        if(it!=s.end() && (*it=='x' || *it=='X'))       // a hex
            ++it;
    }

    while(it!=s.end())
    {
        if(isdigit(*it)==0)
            return false;

        it++;
    }

    return true;
}


int Assembler::str_to_int(string &num) const
{
    istringstream iss(num);

    int a = 0;

    string::iterator it=num.begin();
    
    if(*it=='+' || *it=='-')
        ++it;
    
    if(*it=='0')    // oct or hex
    {
        ++it;

        if(it==num.end())
            return 0;

        if(*it=='x' || *it=='X')
            iss>>hex>>a;
            

        else
            iss>>oct>>a;
        
    }

    else        // normal decimal
        iss>>a;


    iss>>dec;
    return a;
}


void Assembler::assemble()
{   

    first_pass();

    second_pass();

    return ;
}


void Assembler::first_pass()
{
    string s;

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
        {
            analyze(tmp);
        }

    }

    return ;
}

            /* Note that labels and mnemonics have different namespaces!*/

void Assembler::analyze(string& s)
{
    string::size_type ind = s.find(':');

    if(ind!=string::npos)       // : found -> label found
    {
        string tmp1(s.begin(), s.begin()+ind);      // label name
        string tmp2(s.begin()+ind+1, s.end());     // get after the label part

        struct label l(tmp1, -1);

        istringstream iss(tmp2);

        string t1;
        iss>>t1;

        if(t1=="SET")   // get the value/label specified with SET -> may need to process in pass-2
        {   
            int flag = 0;
            string less, more;

            if(l.name.empty())      // invalid label name -> no need to process SET
            {
                errors.insert({line_cnt, Error(1)});           // generate invalid_label error
                flag = 1;
            }
            
            iss>>less;
            if(less.empty())           // too few operands
            {
                errors.insert({line_cnt, Error(9)});
                return;     // nothing to be done now
            }

            t1 = less;

            if(valid_number(t1)==false)             // not a valid numer following SET
            {
                errors.insert({line_cnt, Error(8)});   // generate Number expected error
                flag = 1;
            }

            string num = t1;
            
            iss>>more;
            if(!more.empty())     // more operands found
            {    
                errors.insert({line_cnt, Error(6)});    // generate too many opearands error
                flag = 1;
            }

            if(flag==0)     // no error -> safe to process SET
            {
                int a = str_to_int(num);

                l.addr = a;

                insert_into_symtab(l);  // will take care of insertion, error-detection
            }

            return;     // ANALYSIS COMPLETE HERE
        }

        else if(t1=="data")     // data directive -> process it
        {
            if(l.name.empty())      // invalid label name
                errors.insert({line_cnt, Error(1)});    // invalid label name error
            
            int flag = 0;
            string less, more;

            iss>>less;
            if(less.empty())       // too few operands
            {
                errors.insert({line_cnt, Error(9)});
                return;     // nothing to be done now
            }

            t1 = less;
            if(valid_number(t1)==false)
            {
                errors.insert({line_cnt, Error(8)});   // generate Number expected error
                flag = 1;
            }

            string num = t1;

            iss>>more;
            if(!more.empty())
            {
                errors.insert({line_cnt, Error(6)});    // generate too many opearands error
                flag = 1;
            }

            if(flag==0)
            {
                int value = str_to_int(num);

                data_to_reserve[data] = value;

                data++;     // incrementing by 1 as it is a word-addressable machine
            }

            return ;    // ANALYSIS COMPLETE HERE
        }

        else    // label followed by a statement -> modify line and leave it for pass-2
        {
            int flag = 0;

            if(l.name.empty())
            {
                errors.insert({line_cnt, Error(1)});    // invalid label name error
                flag = 1;
            }

            l.addr = pc;
            if(!t1.empty())     // some mnemonic should be there after label -> otherwise no need to increment PC and insert line to linked list
            {
                struct line ll(tmp2, line_cnt, pc);
                pc++;
                lines.push_back(ll);
            }

            if(flag==0)
            { 
                insert_into_symtab(l);
            }

            return;     // ANALYSIS COMPLETE HERE
        }

    }


    // else        // not a labeled statement
    // {
    //     istringstream iss(s);
    //     string t2;

    //     iss>>t2;

    //     if(t2=="data")      // directive -> process this
    //     {
    //         ;           // store at some suitable place to process later
    //     }

    //     else        // some statement with instruction
    //     {
    //         it->pc = tpc;
    //         ++tpc;
    //     }     
    // }
    

    return ;
}

void Assembler::second_pass()
{
    

    return ;
}


            /* assembler helper functions  */



void Assembler::insert_into_symtab(struct label& l)
{
    if(symtab.find(l.name)!=symtab.end())
    {
        errors.insert({line_cnt, Error(7)});
    }

    else
    {
        symtab[l.name] = l;
    }
    
    return;
}



void Assembler::print_lines(ostream &os) const
{
    for(list<struct line>::const_iterator it=lines.begin(); it!=lines.end(); ++it)
        os<<it->line_no<<": "<<it->s<<endl;
}

void Assembler::print_errors(ostream &os) const
{
    for(map<unsigned, class Error>::const_iterator it=errors.begin(); it!=errors.end(); ++it)
    {
        unsigned line_no = it->first;

        class Error tmp = it->second;

        os<<"Line "<<line_no<<": "<<tmp.message<<endl;

    }
}


void Assembler::print_symtab(ostream& os) const
{
    for(map<string, struct label>::const_iterator it=symtab.begin(); it!=symtab.end(); ++it)
    {
        string name = it->first;
        struct label l = it->second;

        os<<name<<'\t'<<l.addr<<endl;
    }
}