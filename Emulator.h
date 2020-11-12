



class Emulator
{
    private:
        int A, B;
        unsigned PC, SP;
        
        unsigned memory_space[0x100000] = {0};   // 1 MB memory space

    public:
        Emulator(): A(0), B(0), PC(0), SP(0x00010000) {}


        

};