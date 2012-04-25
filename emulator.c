// Constants
#define SIZE 0x10000
#define MAX_VALUE 0xffff
#define EXT_SIZE 0x1000d
#define REG_ADDR 0x10000
#define SP_ADDR 0x10008
#define PC_ADDR 0x10009
#define EX_ADDR 0x1000a
#define IA_ADDR 0x1000b
#define LT_ADDR 0x1000c

// Helper Macros
#define CYCLES(count) (emulator->cycle += (count))
#define RAM(address) (emulator->ram[(address)])
#define REG(index) (emulator->ram[REG_ADDR + (index)])
#define SP (emulator->ram[SP_ADDR])
#define PC (emulator->ram[PC_ADDR])
#define EX (emulator->ram[EX_ADDR])
#define IA (emulator->ram[IA_ADDR])
#define LT (emulator->ram[LT_ADDR])
#define SKIP (emulator->skip)
#define HALT (emulator->halt)
#define CYCLE (emulator->cycle)

// Basic Opcodes
#define SET 0x01
#define ADD 0x02
#define SUB 0x03
#define MUL 0x04
#define MLI 0x05
#define DIV 0x06
#define DVI 0x07
#define MOD 0x08
#define AND 0x09
#define BOR 0x0a
#define XOR 0x0b
#define SHR 0x0c
#define ASR 0x0d
#define SHL 0x0e
#define IFB 0x10
#define IFC 0x11
#define IFE 0x12
#define IFN 0x13
#define IFG 0x14
#define IFA 0x15
#define IFL 0x16
#define IFU 0x17

// Non Basic Opcodes
#define JSR 0x01
#define INT 0x08
#define ING 0x09
#define INS 0x0a
#define HWN 0x10
#define HWQ 0x11
#define HWI 0x12

// Default Font Glyphs
unsigned short GLYPHS[] = {
    0x000f, 0x0808, 0x080f, 0x0808, 0x08f8, 0x0808, 0x00ff, 0x0808,
    0x0808, 0x0808, 0x08ff, 0x0808, 0x00ff, 0x1414, 0xff00, 0xff08,
    0x1f10, 0x1714, 0xfc04, 0xf414, 0x1710, 0x1714, 0xf404, 0xf414,
    0xff00, 0xf714, 0x1414, 0x1414, 0xf700, 0xf714, 0x1417, 0x1414,
    0x0f08, 0x0f08, 0x14f4, 0x1414, 0xf808, 0xf808, 0x0f08, 0x0f08,
    0x001f, 0x1414, 0x00fc, 0x1414, 0xf808, 0xf808, 0xff08, 0xff08,
    0x14ff, 0x1414, 0x080f, 0x0000, 0x00f8, 0x0808, 0xffff, 0xffff,
    0xf0f0, 0xf0f0, 0xffff, 0x0000, 0x0000, 0xffff, 0x0f0f, 0x0f0f,
    0x0000, 0x0000, 0x005f, 0x0000, 0x0300, 0x0300, 0x3e14, 0x3e00,
    0x266b, 0x3200, 0x611c, 0x4300, 0x3629, 0x7650, 0x0002, 0x0100,
    0x1c22, 0x4100, 0x4122, 0x1c00, 0x2a1c, 0x2a00, 0x083e, 0x0800,
    0x4020, 0x0000, 0x0808, 0x0800, 0x0040, 0x0000, 0x601c, 0x0300,
    0x3e41, 0x3e00, 0x427f, 0x4000, 0x6259, 0x4600, 0x2249, 0x3600,
    0x0f08, 0x7f00, 0x2745, 0x3900, 0x3e49, 0x3200, 0x6119, 0x0700,
    0x3649, 0x3600, 0x2649, 0x3e00, 0x0024, 0x0000, 0x4024, 0x0000,
    0x0814, 0x2241, 0x1414, 0x1400, 0x4122, 0x1408, 0x0259, 0x0600,
    0x3e59, 0x5e00, 0x7e09, 0x7e00, 0x7f49, 0x3600, 0x3e41, 0x2200,
    0x7f41, 0x3e00, 0x7f49, 0x4100, 0x7f09, 0x0100, 0x3e49, 0x3a00,
    0x7f08, 0x7f00, 0x417f, 0x4100, 0x2040, 0x3f00, 0x7f0c, 0x7300,
    0x7f40, 0x4000, 0x7f06, 0x7f00, 0x7f01, 0x7e00, 0x3e41, 0x3e00,
    0x7f09, 0x0600, 0x3e41, 0xbe00, 0x7f09, 0x7600, 0x2649, 0x3200,
    0x017f, 0x0100, 0x7f40, 0x7f00, 0x1f60, 0x1f00, 0x7f30, 0x7f00,
    0x7708, 0x7700, 0x0778, 0x0700, 0x7149, 0x4700, 0x007f, 0x4100,
    0x031c, 0x6000, 0x0041, 0x7f00, 0x0201, 0x0200, 0x8080, 0x8000,
    0x0001, 0x0200, 0x2454, 0x7800, 0x7f44, 0x3800, 0x3844, 0x2800,
    0x3844, 0x7f00, 0x3854, 0x5800, 0x087e, 0x0900, 0x4854, 0x3c00,
    0x7f04, 0x7800, 0x447d, 0x4000, 0x2040, 0x3d00, 0x7f10, 0x6c00,
    0x417f, 0x4000, 0x7c18, 0x7c00, 0x7c04, 0x7800, 0x3844, 0x3800,
    0x7c14, 0x0800, 0x0814, 0x7c00, 0x7c04, 0x0800, 0x4854, 0x2400,
    0x043e, 0x4400, 0x3c40, 0x7c00, 0x1c60, 0x1c00, 0x7c30, 0x7c00,
    0x6c10, 0x6c00, 0x4c50, 0x3c00, 0x6454, 0x4c00, 0x0836, 0x4100,
    0x0077, 0x0000, 0x4136, 0x0800, 0x0201, 0x0201, 0x704c, 0x7000,
};

// Emulator State
typedef struct {
    unsigned short *ram;
    unsigned int skip;
    unsigned int halt;
    unsigned int cycle;
} Emulator;

// Emulator Functions
void reset(Emulator *emulator) {
    SKIP = 0;
    HALT = 0;
    CYCLE = 0;
    for (unsigned int i = 0; i < EXT_SIZE; i++) {
        RAM(i) = 0;
    }
    for (unsigned int i = 0; i < 256; i++) {
        RAM(0x8180 + i) = GLYPHS[i];
    }
}

void load(Emulator *emulator, unsigned short *program, unsigned int length) {
    for (unsigned int i = 0; i < length; i++) {
        RAM(i) = program[i];
    }
}

unsigned int operand(Emulator *emulator, unsigned short x, 
    unsigned int dereference) {
    unsigned int result;
    unsigned int literal = 0;
    if (x < 0x08) {
        result = REG_ADDR + x;
    }
    else if (x >= 0x08 && x <= 0x0f) {
        result = REG(x - 0x08);
    }
    else if (x >= 0x10 && x <= 0x17) {
        result = REG(x - 0x10) + RAM(PC++);
        if (!SKIP) {
            CYCLES(1);
        }
    }
    else if (x == 0x18 && dereference) {
        result = SP;
        if (!SKIP) {
            SP++;
        }
    }
    else if (x == 0x18 && !dereference) {
        if (!SKIP) {
            SP--;
        }
        result = SP;
    }
    else if (x == 0x19) {
        result = SP;
    }
    else if (x == 0x1a) {
        result = SP + RAM(PC++);
        if (!SKIP) {
            CYCLES(1);
        }
    }
    else if (x == 0x1b) {
        result = SP_ADDR;
    }
    else if (x == 0x1c) {
        result = PC_ADDR;
    }
    else if (x == 0x1d) {
        result = EX_ADDR;
    }
    else if (x == 0x1e) {
        result = RAM(PC++);
        if (!SKIP) {
            CYCLES(1);
        }
    }
    else if (x == 0x1f) {
        literal = 1;
        result = RAM(PC++);
        if (!SKIP) {
            CYCLES(1);
        }
    }
    else if (x == 0x20) {
        literal = 1;
        result = MAX_VALUE;
    }
    else {
        literal = 1;
        result = x - 0x21;
    }
    if (literal && !dereference) {
        LT = result;
        result = LT_ADDR;
    }
    if (dereference && !literal) {
        result = RAM(result);
    }
    return result;
}

unsigned short divmod(unsigned int x, unsigned short *quo) {
    *quo = x / SIZE;
    return x % SIZE;
}

void basic_instruction(Emulator *emulator, unsigned short opcode, 
    unsigned short op_dst, unsigned short op_src) {
    unsigned int src = operand(emulator, op_src, 1);
    unsigned int dst = operand(emulator, op_dst, 0);
    unsigned int ram = RAM(dst);
    unsigned short quo;
    if (SKIP) {
        SKIP = 0;
        return;
    }
    switch (opcode) {
        case SET:
            RAM(dst) = src;
            CYCLES(1);
            break;
        case ADD:
            RAM(dst) = divmod(ram + src, &quo);
            EX = quo ? 1 : 0;
            CYCLES(2);
            break;
        case SUB:
            RAM(dst) = divmod(ram - src, &quo);
            EX = quo ? MAX_VALUE : 0;
            CYCLES(2);
            break;
        case MUL:
            RAM(dst) = divmod(ram * src, &quo);
            EX = quo % SIZE;
            CYCLES(2);
            break;
        case MLI:
            CYCLES(2);
            break;
        case DIV:
            if (src) {
                EX = ((ram << 16) / src) % SIZE;
                RAM(dst) /= src;
            }
            else {
                EX = 0;
                RAM(dst) = 0;
            }
            CYCLES(3);
            break;
        case DVI:
            CYCLES(3);
            break;
        case MOD:
            if (src) {
                RAM(dst) %= src;
            }
            else {
                RAM(dst) = 0;
            }
            CYCLES(3);
            break;
        case AND:
            RAM(dst) &= src;
            CYCLES(1);
            break;
        case BOR:
            RAM(dst) |= src;
            CYCLES(1);
            break;
        case XOR:
            RAM(dst) ^= src;
            CYCLES(1);
            break;
        case SHR:
            EX = ((ram << 16) >> src) % SIZE;
            RAM(dst) >>= src;
            CYCLES(2);
            break;
        case ASR:
            EX = ((ram << 16) >> src) % SIZE;
            RAM(dst) >>= src;
            CYCLES(2);
            break;
        case SHL:
            RAM(dst) = divmod(ram << src, &quo);
            EX = quo % SIZE;
            CYCLES(2);
            break;
        case IFB:
            SKIP = (ram & src) != 0 ? 0 : 1;
            CYCLES(2 + SKIP);
            break;
        case IFC:
            SKIP = (ram & src) == 0 ? 0 : 1;
            CYCLES(2 + SKIP);
            break;
        case IFE:
            SKIP = (ram == src) ? 0 : 1;
            CYCLES(2 + SKIP);
            break;
        case IFN:
            SKIP = (ram != src) ? 0 : 1;
            CYCLES(2 + SKIP);
            break;
        case IFG:
            SKIP = (ram > src) ? 0 : 1;
            CYCLES(2 + SKIP);
            break;
        case IFA:
            SKIP = ((int)ram > (int)src) ? 0 : 1;
            CYCLES(2 + SKIP);
            break;
        case IFL:
            SKIP = (ram < src) ? 0 : 1;
            CYCLES(2 + SKIP);
            break;
        case IFU:
            SKIP = ((int)ram < (int)src) ? 0 : 1;
            CYCLES(2 + SKIP);
            break;
    }
}

void special_instruction(Emulator *emulator, unsigned short opcode, 
    unsigned short op_src) {
    unsigned int dst = operand(emulator, op_src, 0);
    unsigned int src = RAM(dst);
    if (SKIP) {
        SKIP = 0;
        return;
    }
    switch (opcode) {
        case JSR:
            RAM(--SP) = PC;
            PC = src;
            CYCLES(3);
            break;
        case INT:
            CYCLES(4);
            break;
        case ING:
            RAM(dst) = IA;
            CYCLES(1);
            break;
        case INS:
            IA = src;
            CYCLES(1);
            break;
        case HWN:
            CYCLES(2);
            break;
        case HWQ:
            CYCLES(4);
            break;
        case HWI:
            CYCLES(4);
            break;
    }
}

void step(Emulator *emulator) {
    unsigned short word = RAM(PC++);
    unsigned short op = word & 0x1f;
    unsigned short dst = (word >> 5) & 0x1f;
    unsigned short src = (word >> 10) & 0x3f;
    if (op) {
        basic_instruction(emulator, op, dst, src);
    }
    else {
        special_instruction(emulator, dst, src);
    }
}

void n_steps(Emulator *emulator, unsigned int steps) {
    for (unsigned int i = 0; i < steps; i++) {
        step(emulator);
    }
}

void n_cycles(Emulator *emulator, unsigned int cycles) {
    unsigned int cycle = CYCLE + cycles;
    while (CYCLE < cycle) {
        step(emulator);
    }
}
