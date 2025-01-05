#include "MyVM.h"

// helper macro to simplify register access
#define GetRegister(comand_index) m_registers[m_command_set[comand_index] ^ c_register_mask]

// allocates memory for all registers and sets
// default number (3) of general purpose registers
MyVM::MyVM()
{
	m_reg_num = 3;
	m_registers = new uint8_t[c_system_reg_num + m_reg_num]{0};
	m_command_set = nullptr;
	m_commands_num = 0;
}

// frees the allocated memory for registers and command set
MyVM::~MyVM()
{
	delete[] m_registers;		// free memory for registers
	delete[] m_command_set;	// free memory for commands
}

// set the custom number of general purpose registers. note that
// all values will be reset to 0!
// returns true if the number is valid, false if more than 14
// registers are requested
bool MyVM::SetRegistersNum(uint32_t reg_num)
{
	// check is reg_num for general purpose registers is valid
	if (reg_num > 14)
		return false;
	
	m_reg_num = reg_num;
	m_registers = new uint8_t[c_system_reg_num + m_reg_num]{0};
	return true;
}

// get the current number of general purpose registers
// returns the number of general purpose registers allocated
uint32_t MyVM::GetRegistersNum()
{
	return m_reg_num;
}

// set the command set from a byte array
// returns true if the number is valid, false if more than memory capacity is requested
bool MyVM::SetCommandSet(uint8_t* command_set, uint32_t commands_num)
{
	// check if commands_num is less than memory capacity
	if (commands_num > 0xdf || commands_num == 0x00)
	{
		delete[] command_set;
		return false;
	}

	m_command_set = command_set;
	m_commands_num = commands_num;
	m_registers[PC ^ c_register_mask] = 0; // set PC to 0

	return true;
}

// set the command set from a file
// returns true if the file was loaded successfully, otherwise false
bool MyVM::SetCommandSet(std::ifstream &program_file)
{
	program_file.seekg(0, std::ios::end);
	uint8_t commands_num = program_file.tellg();

	program_file.seekg(0, std::ios::beg);
	uint8_t* command_set = new uint8_t[commands_num];
	program_file.read(reinterpret_cast<char*>(command_set), commands_num);

	if (!program_file)
	{
		delete[] command_set;
		return false;
	}

	program_file.close();
	return SetCommandSet(command_set, commands_num);
}

// executes the loaded commands from the command set by
// incrementing the program counter
// returns 0 if execution finishes successfully, -1 on error,
// and 1 in other cases
int32_t MyVM::Start()
{
	// init program counter
	uint8_t* program_counter = &m_registers[PC ^ c_register_mask];
	while (*program_counter < m_commands_num)
	{
		// get current comend from program set using program counter
		uint8_t current_command = m_command_set[*program_counter];

		switch (current_command)
		{
		case NOP:
			break;

		case EXT:
			return 0;
			break;

		case MOV:
		{
			// check if there are two operands in command set
			if (*program_counter >= m_commands_num - 2)
				return -1;

			// error if 1st operand isn't a register or one of them is command
			if (!IsRegister(*program_counter + 1) ||
				IsCommand(*program_counter + 1) ||
				IsCommand(*program_counter + 2))
				return -1;

			// move to 1st operand (reg) value from reg or literal
			if (IsRegister(*program_counter + 2))
				GetRegister(*program_counter + 1) = GetRegister(*program_counter + 2);
			else
				GetRegister(*program_counter + 1) = m_command_set[*program_counter + 2];

			// increase program counter (2 operands further)
			(*program_counter) += 2;
			break;
		}
		case INC:
		{
			// check if there is register operand
			if ((*program_counter >= m_commands_num - 1) ||
				!IsRegister(*program_counter + 1) ||
				IsCommand(*program_counter + 1))
				return -1;

			// increment register value
			GetRegister(*program_counter + 1)++;

			// increase program counter (1 operand further)
			(*program_counter)++;
			break;
		}
		case MULT:
		{
			// check if there are two operands in command set
			if (*program_counter >= m_commands_num - 2)
				return -1;

			// error if 1st operand isn't a register or one of them is command
			if(!IsRegister(*program_counter + 1) ||
				IsCommand(*program_counter + 1) ||
				IsCommand(*program_counter + 2))
				return -1;

			// multiply to 1st operand (reg) value from reg or literal
			if (IsRegister(*program_counter + 2))
				GetRegister(*program_counter + 1) *= GetRegister(*program_counter + 2);
			else
				GetRegister(*program_counter + 1) *= m_command_set[*program_counter + 2];

			// increase program counter (2 operands further)
			(*program_counter) += 2;
			break;
		}
		case CMP:
		{
			// check if there are two operands in command set
			// error if one of operands is command
			if ((*program_counter >= m_commands_num - 2) ||
				IsCommand(*program_counter + 1) ||
				IsCommand(*program_counter + 2))
				return -1;

			// get left val from register or as literal
			uint8_t left_val;
			if (IsRegister(*program_counter + 1))
				left_val = GetRegister(*program_counter + 1);
			else
				left_val = m_command_set[*program_counter + 1];

			// get right val from register or as literal
			uint8_t right_val;
			if (IsRegister(*program_counter + 2))
				right_val = GetRegister(*program_counter + 2);
			else
				right_val = m_command_set[*program_counter + 2];

			// compare values and set comparison register (flag register)
			if (left_val == right_val)
				m_registers[RF ^ c_register_mask] = 0;
			else if (left_val < right_val)
				m_registers[RF ^ c_register_mask] = 1;
			else
				m_registers[RF ^ c_register_mask] = 2;

			// increase program counter (2 operands further)
			(*program_counter) += 2;
			break;
		}
		case JMP_E:
		{
			// check if there is register operand or literal with c_addr value
			if ((*program_counter >= m_commands_num - 1) ||
				(IsCommand(*program_counter + 1)))
				return -1;

			// perform jump if RF == 0 (equal)
			if (Jump(0))
				continue;
			break;
		}
		case JMP_L:
		{
			// check if there is register operand or literal with c_addr value
			if ((*program_counter >= m_commands_num - 1) ||
				(IsCommand(*program_counter + 1)))
				return -1;

			// perform jump if RF == 1 (less)
			if (Jump(1))
				continue;
			break;
		}
		case JMP_G:
		{
			// check if there is register operand or literal with c_addr value
			if ((*program_counter >= m_commands_num - 1) ||
				(IsCommand(*program_counter + 1)))
				return -1;

			// perform jump if RF == 2 (greater)
			if (Jump(2))
				continue;
			break;
		}
		case JMP:
		{
			// check if there is register operand or literal with c_addr value
			if ((*program_counter >= m_commands_num - 1) ||
				(IsCommand(*program_counter + 1)))
				return -1;

			// set program counter to c_addr value from register operand or literal
			if (IsRegister(*program_counter + 1))
				*program_counter = GetRegister(*program_counter + 1);
			else
				*program_counter = m_command_set[*program_counter + 1];

			continue;
		}
		case PRNT:
		{
			// check if there is register operand or literal with print value
			if ((*program_counter >= m_commands_num - 1) ||
				(IsCommand(*program_counter + 1)))
				return -1;

			// get value from register or as literal
			uint8_t val_to_print;
			if (IsRegister(*program_counter + 1))
				val_to_print = GetRegister(*program_counter + 1);
			else
				val_to_print = m_command_set[*program_counter + 1];

			// print value
			printf("%d\n", val_to_print);

			// write value to the "result.txt" file
			std::ofstream result_file("result.txt");
			if (!result_file.is_open())
				return -1;

			result_file << uint16_t(val_to_print);

			result_file.close();

			// increase program counter (1 operand further)
			(*program_counter)++;
			break;
		}
		default:
			return -1;
		}

		// increase program counter (next command)
		(*program_counter)++;
	}

	return 1;
}

// helper function for handling jumps. performs a jump if the specified
// condition (equality) matches the value of the flag register (RF).
// returns true if the jump occurred, false otherwise
bool MyVM::Jump(uint8_t equality)
{
	uint8_t* program_counter = &m_registers[PC ^ c_register_mask];

	// if RF is not equal to specified equality value
	if (m_registers[RF ^ c_register_mask] != equality)
	{
		// no jump, increase program counter (1 operand further)
		(*program_counter)++;
		return false;
	}

	// otherwise, set program counter to c_addr value from register operand or literal
	if (IsRegister(*program_counter + 1))
		*program_counter = GetRegister(*program_counter + 1);
	else
		*program_counter = m_command_set[*program_counter + 1];

	return true;
}

// checks if the value specified under pc_index in command set is register
bool MyVM::IsRegister(uint8_t pc_index)
{
	if (!IsCommand(pc_index) && (m_command_set[pc_index] & c_register_mask) == c_register_mask)
		return true;
	return false;
}

// checks if the value specified under pc_index in command set is command
bool MyVM::IsCommand(uint8_t pc_index)
{
	if ((m_command_set[pc_index] & c_command_mask) == c_command_mask)
		return true;
	return false;
}