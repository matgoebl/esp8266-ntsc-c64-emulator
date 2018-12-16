extern void exec6502(int32_t tickcount);
extern void reset6502();

extern uint8_t read6502(uint16_t address);
extern void write6502(uint16_t address, uint8_t value);

const uint16_t addr_cursor_x=211;
const uint16_t addr_cursor_y=214;
const uint16_t addr_cursor_flash=204;

const uint16_t addr_keybuf_len=198;
const uint16_t addr_keybuf=631;
