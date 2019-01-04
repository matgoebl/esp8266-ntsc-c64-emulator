extern void exec6502(int32_t tickcount);
extern void reset6502();
extern void nmi6502();

extern uint8_t read6502(uint16_t address);
extern void write6502(uint16_t address, uint8_t value);

// compare to https://www.c64-wiki.de/wiki/C128-Zeropagebelegung
const uint16_t addr_cursor_x=211;
const uint16_t addr_cursor_y=214;
const uint16_t addr_cursor_flash=204;

const uint16_t addr_keybuf_len=198;
const uint16_t addr_keybuf=631;

const uint16_t addr_STKEY=145;
const uint16_t STKEY_stop=127;
const uint16_t STKEY_none=255;
