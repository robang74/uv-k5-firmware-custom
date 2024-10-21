extern uint8_t flags;

#define rxmod 0x01
#define monly 0x02
#define dlwtc 0x04
#define crsbd 0x08
#define flight 0x10

#define bitset(m, x) { flags = x ? (flags|m) : (flags&~m); }
#define bitflp(m) { flags ^= m; }
