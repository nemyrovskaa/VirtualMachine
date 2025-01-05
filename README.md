# Virtual Machine

2024

This is a simple register-based virtual machine that processes binary input in the form of a bytecode array or a binary file. The bytecode array consists of a sequence of bytes representing commands and their arguments, which can be either registers or constant values.

## How It Works

At the start, the virtual machine reads the bytecode. Depending on the byte values, the corresponding commands are executed. After the program finishes execution, the virtual machine returns the following exit codes:

- 0  - The program successfully completed (special "exit" command).
- -1 - An error occurred during execution.
- 1  - The program completed but did not reach the "exit" command.

### Memory and Interpretation
The machine operates on 1-byte values, and a special rule is used to interpret each byte:

- 0x00 to 0xdf for data
- 0xe0 to 0xef for registers
- 0xf0 to 0xff for commands

This classification helps identify the type of byte using bitwise masks. The mask 0xe0 is used for identifying registers, while 0xf0 is used for identifying commands.

### Commands and Registers

The virtual machine supports 11 commands, with the option to add 5 more.
It also provides 16 registers, where 2 registers are system registers and other 14 are general purpose registers.

System registers are:
1. PC (Program Counter) - points to the current byte in the bytecode array being processed, including the data bytes.
2. RF (Register of Flags) - used for comparison operations (e.g., for equality, less than, greater than).

The registers are stored in an array. The user can define how many additional general-purpose registers are needed, beyond the 2 system registers. Access to the registers is done by applying masks. For instance, the PC register is always the first element in the register array, and it is accessed via the mask 0xe0 (0xe0 ^ 0xe0 = 0x00), which corresponds to the first element in the registers array.
