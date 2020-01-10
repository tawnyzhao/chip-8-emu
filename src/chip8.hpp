class chip8 {
    public:
        chip8();
        ~chip8();
        
        void loadROM(char*);
        void emulateCycle();

    private:
        unsigned short opcode; 
        unsigned char memory[4096];
        unsigned char V[16]; // Register
        unsigned short index;
        unsigned short pc;
        unsigned char gfx[64*32];

        unsigned char delay_timer;
        unsigned char sound_timer;

        unsigned short stack[16];
        unsigned short sp;

        unsigned char key[16];
        void init();
        void runOpcode();
        void updateTimers();
};