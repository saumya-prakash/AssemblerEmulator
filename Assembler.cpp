#include "Assembler.h"

map<string, pair<unsigned, unsigned> > Assembler::optab = {  
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
                                        {9, "Too few operands"},
                                        {10, "data directive should come after the code segment"}
                                    };


map<unsigned, string> Warning::warntab = {  
                                            {0, "Deafult warning"},
                                            {1, "Unused label"},
                                            {2, "No preceding label"}   /* When SET doesn't have a corresponding label; not to be used with data directive! */
};

Assembler::endianess Assembler::machine_type = Assembler::get_endianess();

const char Assembler::format_code[8] = "\\\'\'\"\t\f\b";



enum Assembler::endianess Assembler::get_endianess()
{
    unsigned int a = 1;

    char *ptr = (char *) &a;

    if(*ptr==1)
        return lil_endian;
    
    else
        return big_endian;
}





bool Assembler::valid_number(const string& s) const
{
    string::const_iterator it = s.begin();

    if(*it=='+' || *it=='-')
        ++it;

    if(it==s.end())     // only sign with no digit(s) not allowed
        return false;


    if(*it=='0')       // probably octal or hex
    {
        ++it;

        if(it==s.end())
            return true;    // represnts 0/ +0/ -0

        if(*it=='x' || *it=='X')       // a hex
        {
            it++;

            while(it!=s.end())
            {
                switch(*it)
                {   case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                                it++;
                                break;
                    

                    default:    return false;    
                }
            }

            return true;    // a valid hexadecimal number
        }

        else        // octal
        {
            while(it!=s.end())
            {
                switch(*it)
                {
                    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
                            it++;
                            break;
                    
                    default:    return false;
                }
            }
        }
        
    }


    while(it!=s.end())      // a decimal number
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
    if(assembled==true)
        return;

    lines.clear();
    aux_lines.clear();
    errors.clear();
    warnings.clear();
    reset_pc();
    reset_data_addr();
    reset_line_cnt();

    first_pass();

    second_pass();

    generate_warnings();
    
    if(errors.empty())
    {
        generate_object_file();
        assembled = true;
    }

    if(!warnings.empty())
        warn = true;

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
                l.line_no = line_cnt;

                insert_into_symtab(l);  // will take care of insertion, error-detection

                struct line aux(s, line_cnt, pc);
                aux.encoding = a;       // encoded form of opearnd of SET
                aux_lines.push_back(aux);
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

                data_to_reserve[pc] = value;
                
                l.addr = pc;
                l.line_no = line_cnt;
                insert_into_symtab(l);
                
                struct line aux(s, line_cnt, pc);
                aux.encoding = value;
                aux_lines.push_back(aux);

                pc++;
                data_addr++;     // incrementing by 1 as it is a word-addressable machine
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
            l.line_no = line_cnt;


            if(flag==0)
            { 
                insert_into_symtab(l);

                struct line aux(l.name+":", line_cnt, pc);
                aux_lines.push_back(aux);
            }


            if(!t1.empty())     // some mnemonic should be there after label -> otherwise no need to increment PC and insert line to linked list
            {
                struct line ll(tmp2, line_cnt, pc);
                pc++;
                lines.push_back(ll);
            }

       
            return;     // ANALYSIS COMPLETE HERE
        }
    }


    else        // not a labeled statement
    {
        istringstream iss(s);
        string t1;

        iss>>t1;

        if(t1=="SET")   // no label, so processing SET is not at all required
        {
            warnings.insert({line_cnt, Warning(2)});

            string less, more;
            int flag = 0;

            iss>>less;
            if(less.empty())
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

            iss>>more;
            if(!more.empty())   // more operands than required
            {
                errors.insert({line_cnt, Error(6)});
                flag = 1;
            }

            if(flag==0)
            {
                int a = str_to_int(t1);

                struct line aux(s, line_cnt, pc);
                aux.encoding = a;
                aux_lines.push_back(aux);
            }

            return ;    // ANALYSIS DONE HERE
        }

        else if(t1=="data")
        {
            string less, more;
            int flag = 0;

            iss>>less;
            if(less.empty())
            {
                errors.insert({line_cnt, Error(9)});
                return;     // nothing to do now
            }

            t1 = less;
            if(valid_number(t1)==false)
            {
                errors.insert({line_cnt, Error(8)});   // generate Number expected error
                flag = 1;
            }

            iss>>more;
            if(!more.empty())       // too many operands
            {
                errors.insert({line_cnt, Error(6)});
                flag = 1;
            }

            if(flag==0)     // okay to reserve memory
            {
                int a = str_to_int(t1);

                data_to_reserve[pc] = a;
                
                struct line aux(s, line_cnt, pc);
                aux.encoding = a;
                aux_lines.push_back(aux);

                pc++;
                data_addr++;
            }

            return;     // ANALYSIS COMPLETE HERE
        }

        else        // some simple statement - mnemonic followed by operands (0 or more)
        {
            struct line ll(s, line_cnt, pc);
            pc++;

            lines.push_back(ll);

            return ;
        }
    }

    return ;
}



void Assembler::second_pass()
{
    for(list<struct line>::iterator it=lines.begin(); it!=lines.end(); ++it)
    {
        string s = it->s;

        istringstream iss(s);

        string mnemonic;

        iss>>mnemonic;

        map<string, pair<unsigned, unsigned> >::iterator mt=optab.find(mnemonic);

        if(mt==optab.end())     // unknown mnemonic error
        {
            errors.insert({it->line_no, Error(3)});
            continue;           // process next line
        } 

        unsigned i_type = mt->second.first;
        unsigned opcode = mt->second.second;

        if(i_type==0)   // 0-operand mnemonic
        {
            unsigned res = encode_zeroth(s, opcode, it->line_no);

            if(res!=0xffffffff)
                it->encoding = res;
        }

        else        // 1-operand mnemonic
        {
            unsigned res = encode_first(s, opcode, it->line_no, it->addr);

            if(res!=0xffffffff)
                it->encoding = res;
        }
    }


    return ;
}


void Assembler::generate_warnings()
{
    for(map<string, struct label>::const_iterator it=symtab.begin(); it!=symtab.end(); ++it)
    {
        if(it->second.use_cnt==0)
            warnings.insert({it->second.line_no, Warning(1)});
    }
}

unsigned Assembler::encode_zeroth(const string& s, const unsigned& opcode, const unsigned& line_no)
{
    istringstream iss(s);
    string t;

    iss>>t;     // 'dump' mnemonic

    string more;
    iss>>more;

    if(!more.empty())   // more operands than required
    {
        errors.insert({line_no, Error(6)});
        return 0xffffffff;
    }

    return opcode;
}


unsigned Assembler::encode_first(const string& s, const unsigned& opcode, const unsigned& line_no, const unsigned& pc)
{
    istringstream iss(s);

    string t;
    iss>>t;     // 'dump' mnemonic

    string less, more;

    iss>>less;
    if(less.empty())    // no operand - generate too few operands error
    {
        errors.insert({line_no, Error(9)});
        return 0xffffffff;
    }

    t = less;

    iss>>more;
    if(!more.empty())   // more than 1 operand - generate too many operands error
    {
        errors.insert({line_no, Error(6)});
        return 0xffffffff;
    }

    if(valid_number(t)==true)   // a numeric operand - applicable to all mnemonics of one-operand type
    {
        int a = str_to_int(t);

        a = a<<8;
        a = a + opcode;
                                // HOPE CONVERSION DOESN'T POSE ANY PROBLEMS
        unsigned tmp = a;

        return tmp;
    }

    else        // either a label or an invalid operand
    {
        struct label tmp(t, 0);
        if(tmp.name.empty())    // not a label -> so, an invalid operand
        {
            errors.insert({line_no, Error(4)});
            return 0xffffffff;
        }

                        // a label name is there
                        // first check if it is in the symbol table
        map<string, struct label>::iterator st = symtab.find(t);

        if(st==symtab.end())    // label not in symbol table - generate unknown label name error
        {
            errors.insert({line_no, Error(2)});
            return 0xffffffff;
        }

        ++(st->second.use_cnt);

        int offset;     // all one-operand mnemonics can have label as operand as well
        unsigned b;

        switch(opcode)
        {
            case 13:    // call
            case 15:    // brz
            case 16:    // brlz
            case 17:    // br
                    offset = calculate_offset(pc, st->second.addr);

                    b = offset;
                    b = b<<8;
                    b = b + opcode;

                    return b;
                    break;

            default:        // others directly take the address as operand
                        b = st->second.addr;
                        b = b<<8;
                        b = b + opcode;
                        return b;
                        break;
        }
    }
}


inline int Assembler::calculate_offset(const unsigned& pc, const unsigned& addr) const
{
    return addr - (pc + 1);
}






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



bool only_label(const string& s)
{
    string::const_iterator it = s.begin();

    while(it!=s.end() && *it!=':')
        ++it;
    
    if(it==s.end())
        return false;
    
    it++;           // ':' found -> move one ahead
    while(it!=s.end())  
    {
        if(*it>32)
            return false;
        
        it++;
    }

    return true;
}


void Assembler::generate_listing_file() const
{
    if(errors.empty())
    {
        string::size_type ind = filename.find_first_of('.');

        string lst_file = string(filename.begin(), filename.begin()+ind) ;

        ofstream lst(lst_file+".lst", ofstream::out);

        list<struct line>::const_iterator it = lines.begin();
        list<struct line>::const_iterator jt = aux_lines.begin();


        lst<<setw(10)<<"MemAddress"<<setw(10)<<"Encoding"<<'\t'<<"Statement"<<endl<<endl;

        lst<<uppercase<<setfill('0');

        while(it!=lines.end() && jt!=aux_lines.end())
        {
            if(it->line_no == jt->line_no)
            {

                lst<<setw(10)<<it->addr<<'\t';
                
                lst<<"0x"<<setw(8);
                lst<<hex;
                lst<<it->encoding<<'\t';
                lst<<dec;

                lst<<jt->s<<it->s;
                lst<<endl;

                it++;
                jt++;
            }

            else if(it->line_no > jt->line_no)
            {
                lst<<setw(10)<<jt->addr<<'\t';
                
                if(only_label(jt->s)==false)
                {
                    lst<<"0x"<<setw(8);
                    lst<<hex;
                    lst<<jt->encoding<<'\t';
                    lst<<dec;
                }
                else
                {
                    lst<<"        \t";
                }
                
                lst<<jt->s;
                lst<<endl;

                jt++;
            }

            else
            {
                lst<<setw(10)<<it->addr<<'\t';
                
                lst<<"0x"<<setw(8);
                lst<<hex;
                lst<<it->encoding<<'\t';
                lst<<dec;
                
                lst<<it->s;
                lst<<endl;

                it++;
            }
        }

        while(it!=lines.end())
        {
            lst<<setw(10)<<it->addr<<'\t';

            lst<<"0x"<<setw(8);
            lst<<hex;
            lst<<it->encoding<<'\t';
            lst<<dec;
            
            lst<<it->s;
            lst<<endl;

            it++;
        }

        while(jt!=aux_lines.end())
        {
            lst<<setw(10)<<jt->addr<<'\t';
            
            if(only_label(jt->s)==false)
            {   lst<<"0x"<<setw(8);
                lst<<hex;
                lst<<jt->encoding<<'\t';
                lst<<dec;
            }
            else
            {
                lst<<"        \t";
            }
            

            lst<<jt->s;
            lst<<endl;

            jt++;
        }
        
        lst<<setfill(' ')<<nouppercase;

        lst.close();
    }

    // ELSE, some error in source program - leave the previous listing file as it is
    
}


void Assembler::generate_log_file() const
{
    string::size_type ind = filename.find_first_of('.');

    string log_name = string(filename.begin(), filename.begin()+ind);

    ofstream lg(log_name+".log", ofstream::out);

    if(!errors.empty())
    {
        lg<<"Errors -"<<endl;

        for(map<unsigned, class Error>::const_iterator et=errors.begin(); et!=errors.end(); ++et)
        {
            lg<<"Line "<<et->first<<": "<<et->second.message<<endl;
        }

        lg<<endl;
    }

    if(!warnings.empty())
    {
        lg<<"Warnings -"<<endl;

        for(map<unsigned, class Warning>::const_iterator wt=warnings.begin(); wt!=warnings.end(); ++wt)
        {
            lg<<"Line "<<wt->first<<": "<<wt->second.message<<endl;
        }

        lg<<endl;
    }

    lg.close();

}


void Assembler::generate_object_file() const    // format_code, newline, text size, newline, program code, newline, newline, data size, data
{
    string::size_type ind = filename.find_first_of('.');

    string object_file = string(filename.begin(), filename.begin()+ind);

    ofstream fo(object_file+".o", ofstream::binary | ofstream::out);

    const char nwln = '\n';

    const char* ptr = format_code;  // writing format_code
    for(unsigned i=0; i<8; ++i)
    {
        fo.write(format_code+i, 1);
    }

    fo.write(&nwln, 1);


    unsigned text_size = pc;
    unsigned data_size = data_to_reserve.size();    // more generic way to get data segment size

    print_bytes(fo, text_size);
    fo.write(&nwln, 1);

    list<struct line>::const_iterator it = lines.begin();

    for(unsigned i=0; i<pc; ++i)    // put text segment first
    {
        unsigned a = it->encoding;
        ++it;
        
        print_bytes(fo, a);
    }

    fo.write(&nwln, 1);
    fo.write(&nwln, 1);


    print_bytes(fo, data_size);
    fo.write(&nwln, 1);
    
    map<unsigned, unsigned>::const_iterator dt = data_to_reserve.begin();

    for(unsigned i=0; i<data_size; ++i)     // put data segment second
    {
        unsigned a = dt->second;
        ++dt;
        
        print_bytes(fo, a);
    }
    
    fo.close();
}



void Assembler::print_bytes(ostream& os, unsigned a) const
{
    char *ptr = (char *) &a;

    if(machine_type==lil_endian)     // reverse bytes
    {
        ptr += 3;

        os.write(ptr, 1);

        ptr--;
        os.write(ptr, 1);

        ptr--;
        os.write(ptr, 1);

        ptr--;
        os.write(ptr, 1);    
    }

    else        // no need to reverse bytes
    {
        os.write(ptr, 1);

        ptr++;
        os.write(ptr, 1);

        ptr++;
        os.write(ptr, 1);

        ptr++;
        os.write(ptr, 1);
    }
}


        /* PRINTER FUNCTIONS */


void Assembler::print_lines(ostream &os) const
{
    for(list<struct line>::const_iterator it=lines.begin(); it!=lines.end(); ++it)
        os<<it->line_no<<": "<<it->s<<'\t'<<it->addr<<endl;
}


void Assembler::print_aux_lines(ostream &os) const
{
    for(list<struct line>::const_iterator it=aux_lines.begin(); it!=aux_lines.end(); ++it)
        os<<it->line_no<<": "<<it->s<<"\t"<<it->addr<<endl;
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

void Assembler::print_warnings(ostream& os) const
{
    for(map<unsigned, class Warning>::const_iterator it=warnings.begin(); it!=warnings.end(); ++it)
    {
        unsigned a = it->first;
        class Warning b = it->second;

        os<<"Line "<<a<<": "<<b.message<<endl;
    }
}