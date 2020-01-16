class chip8 {
    public:
        chip8();
        ~chip8();
        unsigned char gfx[64 * 32];

        void loadROM(char*);
        void emulateCycle();
        void printScreen();

    private:
        unsigned short opcode; 
        unsigned char memory[4096];
        unsigned char V[16]; // Register
        unsigned short index;
        unsigned short pc;

        unsigned char delay_timer;
        unsigned char sound_timer;

        unsigned short stack[16];
        unsigned short sp;

        unsigned char key[16];
        void init();
        void runOpcode();
        void updateTimers();
};