#include "slim.h"

// Test ----------------------------------------------------------------------------------------------------------------
int main(void) {
    SlimMachine* machine = slim_machine_create();
    slim_machine_clear(machine);

    SlimBytecode* bytecode = slim_bytecode_load("test.slx");
    if (bytecode == NULL) {
        printf("Failed to load bytecode\n");
        return 1;
    }

    slim_machine_load(machine, bytecode->data, bytecode->bytesize);
    slim_machine_launch(machine);
    slim_machine_dump_stack(machine);
    slim_machine_dump_registers(machine);
    slim_machine_dump_memory(machine);
}