#pragma once
// ---------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
// ================================================DEFINITION===========================================================
#define SL_MACHINE_STACK_SIZE 8
#define SL_MACHINE_REGISTERS 4
#define SL_MACHINE_MEMORY_SIZE 16
// ---------------------------------------------------------------------------------------------------------------------
typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef unsigned int u32_t;
typedef unsigned long long u64_t;

typedef signed char s8_t;
typedef signed short s16_t;
typedef signed int s32_t;
typedef signed long long s64_t;

typedef float f32_t;
typedef double f64_t;
// Error Handling ------------------------------------------------------------------------------------------------------
typedef enum sl_error sl_error_t;
enum sl_error {
    SL_ERROR_NONE = 0x0,
    SL_ERROR_STACK_OVERFLOW = 0x1,
    SL_ERROR_STACK_UNDERFLOW = 0x2,
    SL_ERROR_INVALID_REGISTER = 0x3,
    SL_ERROR_BLOCK_SPLIT = 0x4,
    SL_ERROR_BLOCK_MERGE = 0x5,
    SL_ERROR_BLOCK_ALLOC = 0x6,
    SL_ERROR_BLOCK_FREE = 0x7,
};

#define sl_todo()                                                                                                      \
    {                                                                                                                  \
        printf("TODO: %s:%d\n", __FILE__, __LINE__);                                                                   \
        exit(0);                                                                                                       \
    }

#define sl_assert(condition, error)                                                                                    \
    if (!condition) {                                                                                                  \
        printf("Assertion failed: %s\n", #condition);                                                                  \
        return error                                                                                                   \
    }

#define sl_machine_except(machine, error)                                                                              \
    {                                                                                                                  \
        if (error != SL_ERROR_NONE) {                                                                                  \
            machine->flags.error = 1;                                                                                  \
            return;                                                                                                    \
        }                                                                                                              \
    }
// ---------------------------------------------------------------------------------------------------------------------
typedef struct sl_machine sl_machine_t;
typedef struct sl_machine_flags sl_machine_flags_t;
typedef struct sl_instruction sl_instruction_t;
typedef struct sl_bytecode sl_bytecode_t;
typedef enum sl_opcode sl_opcode_t;
typedef struct sl_block sl_block_t;
// Logic and Control Flow - Instructions, Routines, and Opcodes --------------------------------------------------------
enum sl_opcode {
    // clang-format off
    SL_OPCODE_NOOP      = 0x00,     // No operation                                             NOOP                                     
    SL_OPCODE_HALT      = 0x01,     // Halt the machine                                         HALT 

    SL_OPCODE_LOADI     = 0x10,     // Load onto stack using Immediate Mode                     LOADI VALUE
    SL_OPCODE_LOADR     = 0x11,     // Load onto stack using Register Mode                      LOADR REG
    SL_OPCODE_LOADM     = 0x12,     // Load onto stack using Memory Mode                        LOADM ADDR FIELD_OFFSET
    SL_OPCODE_DROP      = 0x13,     // Drop the top of the stack                                DROP
    SL_OPCODE_STORER    = 0x14,     // Store the 2nd of the stack in the register from 1st      STORER [0] [1]
    SL_OPCODE_STOREM    = 0x15,     // Store the 2nd of the stack in the address from 1st       STOREM [0] [1] FIELD_OFFSET

    SL_OPCODE_DUP       = 0x20,     // Duplicate the top of the stack                           DUP
    SL_OPCODE_SWAP      = 0x21,     // Swap the top two values on the stack                     SWAP
    SL_OPCODE_ROT       = 0x22,     // Rotate the top three values on the stack                 ROT3


    SL_OPCODE_ADD       = 0x30,     // Add the top two values on the stack as integers          ADD
    SL_OPCODE_SUB       = 0x31,     // Subtract the top two values on the stack as integers     SUBI
    SL_OPCODE_MUL       = 0x32,     // Multiply the top two values on the stack as integers     MULI
    SL_OPCODE_DIV       = 0x33,     // Divide the top two values on the stack as integers       DIVI
    SL_OPCODE_MODI      = 0x34,     // Modulo the top two values on the stack as integers       MODI
    
    SL_OPCODE_ADDF      = 0x35,     // Add the top two values on the stack as floats            ADDF
    SL_OPCODE_SUBF      = 0x36,     // Subtract the top two values on the stack as floats       SUBF
    SL_OPCODE_MULF      = 0x37,     // Multiply the top two values on the stack as floats       MULF
    SL_OPCODE_DIVF      = 0x38,     // Divide the top two values on the stack as floats         DIVF
    SL_OPCODE_MODF      = 0x39,     // Modulo the top two values on the stack as floats         MODF

    SL_OPCODE_ALLOC     = 0x40,     // Allocate memory, return address to top of stack          ALLOC SIZE
    SL_OPCODE_FREE      = 0x41,     // Free memory at address on top of stack                   FREE 
    // clang-format on
};

struct sl_bytecode {
    u8_t* data;
    u8_t size;
    u16_t bytesize;
};

struct sl_instruction {
    u8_t opcode;
    u32_t arg1;
    u32_t arg2;
};

typedef void (*sl_routine_t)(sl_machine_t* machine, sl_instruction_t instruction);

sl_bytecode_t* sl_bytecode_load(const char* filename);
void sl_bytecode_destroy(sl_bytecode_t* bytecode);

void sl_routine_nop(sl_machine_t* machine, sl_instruction_t instruction);
void sl_routine_halt(sl_machine_t* machine, sl_instruction_t instruction);

void sl_routine_loadi(sl_machine_t* machine, sl_instruction_t instruction);
void sl_routine_loadr(sl_machine_t* machine, sl_instruction_t instruction);
void sl_routine_loadm(sl_machine_t* machine, sl_instruction_t instruction);

void sl_routine_drop(sl_machine_t* machine, sl_instruction_t instruction);
void sl_routine_storer(sl_machine_t* machine, sl_instruction_t instruction);
void sl_routine_storem(sl_machine_t* machine, sl_instruction_t instruction);

void sl_routine_dup(sl_machine_t* machine, sl_instruction_t instruction);
void sl_routine_swap(sl_machine_t* machine, sl_instruction_t instruction);
void sl_routine_rot(sl_machine_t* machine, sl_instruction_t instruction);

void sl_routine_add(sl_machine_t* machine, sl_instruction_t instruction);
void sl_routine_sub(sl_machine_t* machine, sl_instruction_t instruction);
void sl_routine_mul(sl_machine_t* machine, sl_instruction_t instruction);
void sl_routine_div(sl_machine_t* machine, sl_instruction_t instruction);

void sl_routine_addf(sl_machine_t* machine, sl_instruction_t instruction);
void sl_routine_subf(sl_machine_t* machine, sl_instruction_t instruction);
void sl_routine_mulf(sl_machine_t* machine, sl_instruction_t instruction);
void sl_routine_divf(sl_machine_t* machine, sl_instruction_t instruction);

void sl_routine_alloc(sl_machine_t* machine, sl_instruction_t instruction);
void sl_routine_free(sl_machine_t* machine, sl_instruction_t instruction);
// State and Data - Machine, Errors, and Memory ------------------------------------------------------------------------
struct sl_machine_flags {
    u16_t zero : 1;
    u16_t carry : 1;
    u16_t overflow : 1;
    u16_t negative : 1;
    u16_t interrupt : 1;
    u16_t decimal : 1;
    u16_t error : 1;
    u16_t halt : 1;
};

struct sl_machine {
    sl_machine_flags_t flags;

    // We will use a 32-bit address space
    // Really, even this is too large because
    // we can't use the full 32-bits for addressing
    // without a page table
    u32_t stack_pointer;
    u32_t instruction_pointer;

    // 64-bits chosen for simplicity
    // Everything is stored as a 64-bit value
    u64_t stack[SL_MACHINE_STACK_SIZE];
    u64_t registers[SL_MACHINE_REGISTERS];
    
    sl_block_t *blocks;
    u64_t memory[SL_MACHINE_MEMORY_SIZE];

    u8_t* bytecode;
    u32_t bytecode_size;
};

// Fetch, Decode, Execute
sl_instruction_t sl_machine_fetch(sl_machine_t* machine);
sl_routine_t sl_machine_decode(sl_machine_t* machine, sl_instruction_t instruction);
void sl_machine_execute(sl_machine_t* machine, sl_routine_t routine, sl_instruction_t instruction);

// External API
sl_machine_t* sl_machine_create();
void sl_machine_destroy(sl_machine_t* machine);
void sl_machine_clear(sl_machine_t* machine);
void sl_machine_load(sl_machine_t* machine, u8_t* data, u32_t size);
void sl_machine_launch(sl_machine_t* machine);

// Internal API - Called by routines to manipulate the machine
sl_error_t ___sl_machine_push(sl_machine_t* machine, u64_t value);
sl_error_t ___sl_machine_pop(sl_machine_t* machine, u64_t* value);
sl_error_t ___sl_machine_load(sl_machine_t* machine, u32_t register);
sl_error_t ___sl_machine_store(sl_machine_t* machine, u32_t register);
sl_error_t ___sl_machine_read(sl_machine_t* machine, u32_t address, u32_t offset);
sl_error_t ___sl_machine_write(sl_machine_t* machine, u32_t address, u32_t offset);
sl_error_t ___sl_machine_alloc(sl_machine_t* machine, u32_t size, u32_t* address);
sl_error_t ___sl_machine_free(sl_machine_t* machine, u32_t address);

// Block Management
struct sl_block {
    u8_t allocated;
    u32_t start;
    u32_t end;
    sl_block_t *next;
};

sl_block_t* sl_block_create(u32_t start, u32_t end);
void sl_block_destroy(sl_block_t *block);
sl_error_t sl_block_split(sl_block_t* block, u32_t size);
sl_error_t sl_block_merge(sl_block_t* block);
// Debugging
void sl_machine_dump_stack(sl_machine_t* machine);
void sl_machine_dump_registers(sl_machine_t* machine);
void sl_machine_dump_memory(sl_machine_t* machine);
// ==============================================IMPLEMENTATION=========================================================
sl_bytecode_t* sl_bytecode_load(const char* filename) {
    // Open the file

    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Failed to open file\n");
        return NULL;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    u32_t size = ftell(file);
    if (size % 9 != 0) {
        printf("Invalid bytecode file\n");
        return NULL;
    }

    // Allocate memory for the bytecode
    u8_t* data = malloc(size);
    if (data == NULL) {
        printf("Failed to allocate memory\n");
        return NULL;
    }

    // Read the file into memory
    fseek(file, 0, SEEK_SET);
    fread(data, size, 1, file);
    fclose(file);

    // Create the bytecode struct
    sl_bytecode_t* bytecode = malloc(sizeof(sl_bytecode_t));
    bytecode->data = data;
    bytecode->size = size / 9;
    bytecode->bytesize = size;

    return bytecode;
}

void sl_bytecode_destroy(sl_bytecode_t* bytecode) {
    free(bytecode->data);
    free(bytecode);
}

sl_error_t ___sl_machine_push(sl_machine_t* machine, u64_t value) {
    if (machine->stack_pointer >= SL_MACHINE_STACK_SIZE) {
        return SL_ERROR_STACK_OVERFLOW;
    }

    machine->stack[machine->stack_pointer++] = value;

    return SL_ERROR_NONE;
}

sl_error_t ___sl_machine_pop(sl_machine_t* machine, u64_t* value) {
    if (machine->stack_pointer == 0) {
        return SL_ERROR_STACK_UNDERFLOW;
    }

    u64_t top = machine->stack[machine->stack_pointer - 1];
    machine->stack[machine->stack_pointer - 1] = 0;
    machine->stack_pointer--;
    *value = top;

    return SL_ERROR_NONE;
}

sl_error_t ___sl_machine_load(sl_machine_t* machine, u32_t index) {
    if (index >= SL_MACHINE_REGISTERS) {
        return SL_ERROR_INVALID_REGISTER;
    }

    u64_t value = machine->registers[index];
    sl_error_t error = ___sl_machine_push(machine, value);
    if (error != SL_ERROR_NONE) {
        return error;
    }

    return SL_ERROR_NONE;
}

sl_error_t ___sl_machine_store(sl_machine_t* machine, u32_t index) {
    if (index >= SL_MACHINE_REGISTERS) {
        return SL_ERROR_INVALID_REGISTER;
    }

    u64_t value;
    sl_error_t error = ___sl_machine_pop(machine, &value);
    if (error != SL_ERROR_NONE) {
        return error;
    }

    machine->registers[index] = value;

    return SL_ERROR_NONE;
}

sl_error_t ___sl_machine_read(sl_machine_t* machine, u32_t address, u32_t offset) { 
    u64_t value;
    sl_error_t error;

    u64_t* ptr = (u64_t*)(machine->memory + address + offset);
    value = *ptr;

    error = ___sl_machine_push(machine, value);
    if (error != SL_ERROR_NONE) {
        return error;
    }

    return SL_ERROR_NONE;
}

sl_error_t ___sl_machine_write(sl_machine_t* machine, u32_t address, u32_t offset) {
    u64_t value;
    sl_error_t error;

    error = ___sl_machine_pop(machine, &value);
    if (error != SL_ERROR_NONE) {
        return error;
    }

    u64_t* ptr = (u64_t*)(machine->memory + address + offset);
    *ptr = value;

    return SL_ERROR_NONE;
}

// TODO: Validate Me
sl_error_t ___sl_machine_alloc(sl_machine_t* machine, u32_t size, u32_t* address) { 
    sl_block_t* block = machine->blocks;
    while (block != NULL) {
        if (block->allocated == 0 && block->end - block->start >= size) {
            sl_error_t error = sl_block_split(block, size);
            if (error != SL_ERROR_NONE) {
                return error;
            }

            block->allocated = 1;
            *address = block->start;
            return SL_ERROR_NONE;
        }

        block = block->next;
    }

    return SL_ERROR_BLOCK_ALLOC;
}

// TODO: Validate Me
sl_error_t ___sl_machine_free(sl_machine_t* machine, u32_t address) { 
    sl_block_t* block = machine->memory;
    while (block != NULL) {
        if (block->start == address) {
            block->allocated = 0;
            sl_error_t error = sl_block_merge(block);
            if (error != SL_ERROR_NONE) {
                return error;
            }

            return SL_ERROR_NONE;
        }

        block = block->next;
    }

    return SL_ERROR_BLOCK_FREE;
}
// Routines and Operations ---------------------------------------------------------------------------------------------
void sl_routine_nop(sl_machine_t* machine, sl_instruction_t instruction) {
    printf("NOP\n");
    return;
}

void sl_routine_halt(sl_machine_t* machine, sl_instruction_t instruction) {
    printf("HALT\n");
    machine->flags.halt = 1;
    return;
}

void sl_routine_loadi(sl_machine_t* machine, sl_instruction_t instruction) {
    sl_error_t error;

    u64_t value = (u64_t)instruction.arg1 << 32 | instruction.arg2;
    printf("LOADI %lld\n", value);

    error = ___sl_machine_push(machine, value);

    if (error != SL_ERROR_NONE) {
        printf("ERROR: %d\n", error);
    }

    sl_machine_except(machine, error);
}

void sl_routine_loadr(sl_machine_t* machine, sl_instruction_t instruction) {
    printf("LOADR %d\n", instruction.arg1);

    u32_t index = instruction.arg1;

    sl_error_t error = ___sl_machine_load(machine, index);
    sl_machine_except(machine, error);

    return;
}

void sl_routine_loadm(sl_machine_t* machine, sl_instruction_t instruction) {
    printf("LOADM %x, %x\n", instruction.arg1, instruction.arg2);

    u32_t address;
    u32_t offset;
    sl_error_t error;

    error = ___sl_machine_pop(machine, &address);
    sl_machine_except(machine, error);

    error = ___sl_machine_read(machine, address, offset);
    sl_machine_except(machine, error);

    return;
}

void sl_routine_drop(sl_machine_t* machine, sl_instruction_t instruction) {
    printf("DROP\n");

    sl_error_t error = ___sl_machine_pop(machine, NULL);
    sl_machine_except(machine, error);

    return;
}

void sl_routine_storer(sl_machine_t* machine, sl_instruction_t instruction) {
    printf("STORER %d\n", instruction.arg1);

    u32_t index = instruction.arg1;
    sl_error_t error;

    error = ___sl_machine_store(machine, index);
    sl_machine_except(machine, error);

    return;
}

void sl_routine_storem(sl_machine_t* machine, sl_instruction_t instruction) {
    printf("STOREM %d\n", instruction.arg1);    

    u64_t value;
    u64_t address;
    u64_t offset;
    sl_error_t error;

    error = ___sl_machine_pop(machine, &address);
    sl_machine_except(machine, error);

    sl_machine_except(machine, error);

    offset = instruction.arg1;
    error = ___sl_machine_write(machine, address, offset);

    return;
}

void sl_routine_dup(sl_machine_t* machine, sl_instruction_t instruction) {
    printf("DUP\n");

    u64_t value;
    sl_error_t error;

    error = ___sl_machine_pop(machine, &value);
    sl_machine_except(machine, error);

    error = ___sl_machine_push(machine, value);
    sl_machine_except(machine, error);

    error = ___sl_machine_push(machine, value);
    sl_machine_except(machine, error);

    return;
}

void sl_routine_swap(sl_machine_t* machine, sl_instruction_t instruction) {
    printf("SWAP\n");
    u64_t a;
    u64_t b;
    sl_error_t error;

    error = ___sl_machine_pop(machine, &a);
    sl_machine_except(machine, error);

    error = ___sl_machine_pop(machine, &b);
    sl_machine_except(machine, error);

    error = ___sl_machine_push(machine, a);
    sl_machine_except(machine, error);

    error = ___sl_machine_push(machine, b);
    sl_machine_except(machine, error);

    return;
}

void sl_routine_rot(sl_machine_t* machine, sl_instruction_t instruction) {
    printf("ROT\n");

    u64_t a;
    u64_t b;
    u64_t c;
    sl_error_t error;

    error = ___sl_machine_pop(machine, &a);
    sl_machine_except(machine, error);

    error = ___sl_machine_pop(machine, &b);
    sl_machine_except(machine, error);

    error = ___sl_machine_pop(machine, &c);
    sl_machine_except(machine, error);

    error = ___sl_machine_push(machine, b);
    sl_machine_except(machine, error);

    error = ___sl_machine_push(machine, a);
    sl_machine_except(machine, error);

    error = ___sl_machine_push(machine, c);
    sl_machine_except(machine, error);

    return;
}

void sl_routine_add(sl_machine_t* machine, sl_instruction_t instruction) {
    printf("ADD\n");

    u64_t a;
    u64_t b;
    sl_error_t error;

    error = ___sl_machine_pop(machine, &a);
    sl_machine_except(machine, error);

    error = ___sl_machine_pop(machine, &b);
    sl_machine_except(machine, error);

    u64_t result = a + b;

    error = ___sl_machine_push(machine, result);
    sl_machine_except(machine, error);

    return;
}

void sl_routine_sub(sl_machine_t* machine, sl_instruction_t instruction) {
    printf("SUB\n");

    u64_t a;
    u64_t b;
    sl_error_t error;

    error = ___sl_machine_pop(machine, &a);
    sl_machine_except(machine, error);

    error = ___sl_machine_pop(machine, &b);
    sl_machine_except(machine, error);

    u64_t result = a - b;

    error = ___sl_machine_push(machine, result);
    sl_machine_except(machine, error);

    return;
}

void sl_routine_mul(sl_machine_t* machine, sl_instruction_t instruction) {
    printf("MUL\n");

    u64_t a;
    u64_t b;
    sl_error_t error;

    error = ___sl_machine_pop(machine, &a);
    sl_machine_except(machine, error);

    error = ___sl_machine_pop(machine, &b);
    sl_machine_except(machine, error);

    u64_t result = a * b;

    error = ___sl_machine_push(machine, result);
    sl_machine_except(machine, error);

    return;
}

void sl_routine_div(sl_machine_t* machine, sl_instruction_t instruction) {
    printf("DIV\n");

    u64_t a;
    u64_t b;
    sl_error_t error;

    error = ___sl_machine_pop(machine, &a);
    sl_machine_except(machine, error);

    error = ___sl_machine_pop(machine, &b);
    sl_machine_except(machine, error);

    u64_t result = a / b;

    error = ___sl_machine_push(machine, result);
    sl_machine_except(machine, error);

    return;
}

void sl_routine_addf(sl_machine_t* machine, sl_instruction_t instruction) {
    sl_todo();
    return;
}

void sl_routine_subf(sl_machine_t* machine, sl_instruction_t instruction) {
    sl_todo();
    return;
}

void sl_routine_mulf(sl_machine_t* machine, sl_instruction_t instruction) {
    sl_todo();
    return;
}

void sl_routine_divf(sl_machine_t* machine, sl_instruction_t instruction) {
    sl_todo();
    return;
}

void sl_routine_alloc(sl_machine_t* machine, sl_instruction_t instruction) {
    printf("ALLOC %d\n", instruction.arg1);

    u32_t size = instruction.arg1;
    sl_error_t error;

    u32_t address;

    error = ___sl_machine_alloc(machine, size, &address);
    sl_machine_except(machine, error);

    error = ___sl_machine_push(machine, address);
    sl_machine_except(machine, error);

    return;
}

void sl_routine_free(sl_machine_t* machine, sl_instruction_t instruction) {
    printf("FREE %d\n", instruction.arg1);

    sl_error_t error = ___sl_machine_free(machine, instruction.arg1);
    sl_machine_except(machine, error);

    return;
}

// Fetch, Decode, Execute ----------------------------------------------------------------------------------------------
sl_instruction_t sl_machine_fetch(sl_machine_t* machine) {
    sl_instruction_t instruction;
    instruction.opcode = machine->bytecode[machine->instruction_pointer];

    // Read in the next 4 bytes as the first argument
    u32_t arg1 = 0;
    for (int i = 0; i < 4; i++) {
        arg1 |= machine->bytecode[machine->instruction_pointer + 1 + i] << (i * 8);
    }
    // Reverse the bits
    arg1 = ((arg1 & 0x000000FF) << 24) | ((arg1 & 0x0000FF00) << 8) | ((arg1 & 0x00FF0000) >> 8) |
           ((arg1 & 0xFF000000) >> 24);

    // Read in the next 4 bytes as the second argument
    u32_t arg2 = 0;
    for (int i = 0; i < 4; i++) {
        arg2 |= machine->bytecode[machine->instruction_pointer + 5 + i] << (i * 8);
    }
    // Reverse the bits
    arg2 = ((arg2 & 0x000000FF) << 24) | ((arg2 & 0x0000FF00) << 8) | ((arg2 & 0x00FF0000) >> 8) |
           ((arg2 & 0xFF000000) >> 24);

    instruction.arg1 = arg1;
    instruction.arg2 = arg2;

    printf("Fetch: 0x%x 0x%x 0x%x\n", instruction.opcode, instruction.arg1, instruction.arg2);

    machine->instruction_pointer += 9;

    return instruction;
}

sl_routine_t sl_machine_decode(sl_machine_t* machine, sl_instruction_t instruction) {
    switch (instruction.opcode) {
    case SL_OPCODE_NOOP: return sl_routine_nop; break;
    case SL_OPCODE_HALT: return sl_routine_halt; break;
    case SL_OPCODE_LOADI: return sl_routine_loadi; break;
    case SL_OPCODE_LOADR: return sl_routine_loadr; break;
    case SL_OPCODE_LOADM: return sl_routine_loadm; break;
    case SL_OPCODE_DROP: return sl_routine_drop; break;
    case SL_OPCODE_STORER: return sl_routine_storer; break;
    case SL_OPCODE_STOREM: return sl_routine_storem; break;
    case SL_OPCODE_DUP: return sl_routine_dup; break;
    case SL_OPCODE_SWAP: return sl_routine_swap; break;
    case SL_OPCODE_ROT: return sl_routine_rot; break;
    case SL_OPCODE_ADD: return sl_routine_add; break;
    case SL_OPCODE_SUB: return sl_routine_sub; break;
    case SL_OPCODE_MUL: return sl_routine_mul; break;
    case SL_OPCODE_DIV: return sl_routine_div; break;
    case SL_OPCODE_ADDF: return sl_routine_addf; break;
    case SL_OPCODE_SUBF: return sl_routine_subf; break;
    case SL_OPCODE_MULF: return sl_routine_mulf; break;
    case SL_OPCODE_DIVF: return sl_routine_divf; break;
    case SL_OPCODE_ALLOC: return sl_routine_alloc; break;
    case SL_OPCODE_FREE: return sl_routine_free; break;
    default: return NULL; break;
    }
}

void sl_machine_execute(sl_machine_t* machine, sl_routine_t routine, sl_instruction_t instruction) {
    if (routine) {
        routine(machine, instruction);
    } else {
        printf("Invalid instruction\n");
        machine->flags.error = 1;
    }
}
// External API --------------------------------------------------------------------------------------------------------
sl_machine_t* sl_machine_create() {
    sl_machine_t* machine = malloc(sizeof(sl_machine_t));
    machine->bytecode = NULL;
    machine->bytecode_size = 0;
    machine->blocks = sl_block_create(0, SL_MACHINE_MEMORY_SIZE);
    return machine;
}

void sl_machine_destroy(sl_machine_t* machine) {
    if (machine->bytecode) {
        free(machine->bytecode);
    }

    sl_block_destroy(machine->blocks);

    free(machine);
}

void sl_machine_clear(sl_machine_t* machine) {
    for (u32_t i = 0; i < SL_MACHINE_STACK_SIZE; i++) {
        machine->stack[i] = 0;
    }

    for (u32_t i = 0; i < SL_MACHINE_REGISTERS; i++) {
        machine->registers[i] = 0;
    }

    for (u32_t i = 0; i < SL_MACHINE_MEMORY_SIZE; i++) {
        machine->memory[i] = 0;
    }

    // Reset Flags
    machine->flags.zero = 0;
    machine->flags.carry = 0;
    machine->flags.overflow = 0;
    machine->flags.negative = 0;
    machine->flags.interrupt = 0;
    machine->flags.decimal = 0;
    machine->flags.error = 0;
    machine->flags.halt = 0;

    // Reset Pointers
    machine->stack_pointer = 0;
    machine->instruction_pointer = 0;

    // Reset Blocks
    sl_block_destroy(machine->blocks);
    machine->blocks = sl_block_create(0, SL_MACHINE_MEMORY_SIZE);
}

void sl_machine_load(sl_machine_t* machine, u8_t* data, u32_t size) {
    machine->bytecode = data;
    machine->bytecode_size = size;
}

void sl_machine_launch(sl_machine_t* machine) {
    while (machine->flags.halt == 0) {
        sl_instruction_t instruction = sl_machine_fetch(machine);
        sl_routine_t routine = sl_machine_decode(machine, instruction);
        sl_machine_execute(machine, routine, instruction);
    }
    printf("Machine halted\n");
}
// Block Management ----------------------------------------------------------------------------------------------------
sl_block_t* sl_block_create(u32_t start, u32_t end) {
    sl_block_t *block = malloc(sizeof(sl_block_t));
    if (block == NULL) {
        return NULL;
    }
    block->allocated = 0;
    block->start = start;
    block->end = end;
    block->next = NULL;
    return block;
}

void sl_block_destroy(sl_block_t *block) {
    if (block->next != NULL) {
        sl_block_destroy(block->next);
    }
    free(block);
}

sl_error_t sl_block_split(sl_block_t* block, u32_t size) {
    if (block->allocated) {
        return SL_ERROR_BLOCK_SPLIT;
    }

    if (block->end - block->start < size) {
        return SL_ERROR_BLOCK_SPLIT;
    }

    sl_block_t *new_block = sl_block_create(block->start + size, block->end);
    if (new_block == NULL) {
        return SL_ERROR_BLOCK_SPLIT;
    }

    block->end = block->start + size;
    new_block->next = block->next;
    block->next = new_block;
    return SL_ERROR_NONE;
}

sl_error_t sl_block_merge(sl_block_t* block) {
    if (block->allocated) {
        return SL_ERROR_BLOCK_MERGE;
    }

    if (block->next == NULL) {
        return SL_ERROR_BLOCK_MERGE;
    }

    if (block->next->allocated) {
        return SL_ERROR_BLOCK_MERGE;
    }

    block->end = block->next->end;
    sl_block_t *next = block->next;
    block->next = next->next;
    free(next);

    return SL_ERROR_NONE;
}
// Debugging -----------------------------------------------------------------------------------------------------------
void sl_machine_dump_stack(sl_machine_t* machine) {
    printf("Stack:\n");
    for (u32_t i = 0; i < SL_MACHINE_STACK_SIZE; i++) {
        printf("%d: %d\n", i, machine->stack[i]);
    }

    printf("Stack Pointer: %lld\n", (s64_t)machine->stack_pointer);

    printf("\n");
}

void sl_machine_dump_registers(sl_machine_t* machine) {
    printf("Registers:\n");
    for (u32_t i = 0; i < SL_MACHINE_REGISTERS; i++) {
        printf("%d: %d\n", i, machine->registers[i]);
    }

    printf("\n");
}

void sl_machine_dump_memory(sl_machine_t* machine) {
    printf("Memory:\n");
    for (u32_t i = 0; i < SL_MACHINE_MEMORY_SIZE; i++) {
        printf("%d: %d\n", i, machine->memory[i]);
    }

    printf("\n");
}
// Test ----------------------------------------------------------------------------------------------------------------
int main(void) {
    sl_machine_t* machine = sl_machine_create();
    sl_machine_clear(machine);

    sl_bytecode_t* bytecode = sl_bytecode_load("test.slx");
    if (bytecode == NULL) {
        printf("Failed to load bytecode\n");
        return 1;
    }

    sl_machine_load(machine, bytecode->data, bytecode->bytesize);
    sl_machine_launch(machine);
    sl_machine_dump_stack(machine);
    sl_machine_dump_registers(machine);
    sl_machine_dump_memory(machine);    
}   