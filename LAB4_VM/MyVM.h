#ifndef MY_VM_H
#define MY_VM_H

#include <stdint.h>
#include <fstream>

// enumeration for commands supported by the virtual machine
// defines the opcodes for supported commands in the virtual machine
enum Commands : uint8_t
{
	NOP = 0xf0,		// no operation
	EXT = 0xf1,		// exit
	MOV = 0xf2,		// move: MOV R_destin R_source (R_destin <- R_source)
	INC = 0xf3,		// increment: INC R (R = R+1)
	MULT = 0xf4,	// multiplication: MULT R_dest R_multiplier (R_dest = R_dest*R_multiplier)
	CMP = 0xf5,		// comparison: COMP R_left, R_right (if R_left == R_right then R_flag = 0
													  // if R_left < R_right  then R_flag = 1
													  // if R_left > R_right  then R_flag = 2)
	JMP_E = 0xf6,	// jump if equals:  JMP_E c_addr (if R_flag == 0 then jump to command addr)
	JMP_L = 0xf7,	// jump if less:    JMP_L c_addr (if R_flag == 1 then jump to command addr)
	JMP_G = 0xf8,	// jump if greater: JMP_G c_addr (if R_flag == 2 then jump to command addr)
	JMP   = 0xf9,	// jump:			JMP   c_addr (jump to command addr)
	PRNT  = 0xfa	// print 1-byte value in new line:	PRNT R_source
};

// enumeration for the registers available in the virtual machine
// defines the register identifiers used in the virtual machine
enum Registers : uint8_t
{
	PC  = 0xe0,	// program counter register
	RF  = 0xe1,	// comparison register (flag register)
	R0  = 0xe2, // general purpose registers
	R1  = 0xe3,
	R2  = 0xe4,
	R3  = 0xe5,
	R4  = 0xe6,
	R5  = 0xe7,
	R6  = 0xe8,
	R7  = 0xe9,
	R8  = 0xea,
	R9  = 0xeb,
	R10 = 0xec,
	R11 = 0xed,
	R12 = 0xee,
	R13 = 0xef
};

// class representing the Virtual Machine (VM)
// represents a simple virtual machine that executes a set of commands.
// the virtual machine supports various operations such as moving values between registers,
// performing arithmetic operations, jumping to specific instructions, and printing values.
class MyVM
{
public:
	// constructor for initialising the virtual machine
	MyVM();
	// destructor for cleaning up resources
	~MyVM();

	// sets the number of general purpose registers
	bool SetRegistersNum(uint32_t reg_num);
	// gets the current number of general purpose registers
	uint32_t GetRegistersNum();
	// sets the command set using a program file
	bool SetCommandSet(std::ifstream &program_file);
	// sets the command set using a raw byte array
	bool SetCommandSet(uint8_t* command_set, uint32_t commands_num);
	// starts the execution of the loaded command set
	int32_t Start();

private:
	const uint8_t c_command_mask = 0xf0;	// mask for identifying commands
	const uint8_t c_register_mask = 0xe0;	// mask for identifying registers
	const uint32_t c_system_reg_num = 2;	// number of system registers (PC and RF)
	uint32_t m_reg_num;			// number of general purpose registers
	uint8_t* m_registers;		// array holding the registers values
	uint8_t* m_command_set;		// command set array
	uint32_t m_commands_num;	// number of commands in the command set

	// executes a jump instruction based on the comparison flag
	bool Jump(uint8_t equality);
	// checks if the provided index corresponds to a register
	bool IsRegister(uint8_t pc_index);
	// checks if the provided index corresponds to a command
	bool IsCommand(uint8_t pc_index);
};

#endif MY_VM_H