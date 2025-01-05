// LAB4_VM.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "MyVM.h"

int main()
{
	// write program (this program calculates factorial n! for 0 <= n <= 5)
	uint8_t command_set[] =
	   {MOV, R0, 0x05,	// R0 <- 2	(for n in n!)
		MOV, R1, 0x01,	// R1 <- 1	(for result)
		MOV, R2, 0x01,	// R2 <- 1	(for counter)

		CMP, R0, 0x05,	// compare n with 5
		JMP_G, 0x22,	// if n > 5 then exit (numbers can be from 0 to 223(0xdf) so 5! = 120 is limit)

		CMP, R0, 0x02,	// compare n with 2
		JMP_L, 0x1f,	// if n < 2 then print result (n! = 0! = 1! = 1 and R1 is already 1)

		CMP, R0, R2,	// compare n with counter
		JMP_L, 0x1f,	// if n < counter then print result else do next
		MULT, R1, R2,	// result *= counter
		INC, R2,		// counter++
		JMP, 0x13,		// jump to compare for new cycle iteration
		PRNT, R1,		// print result
		EXT};			// exit

	uint8_t commands_num = sizeof(command_set)/sizeof(uint8_t);

	// create binary file for the program
	std::ofstream file_to_write("program.bin", std::ios::binary);
	if (!file_to_write.is_open())
	{
		std::cout << "Something went wrong while creating the file.\n";
		return 1;
	}

	// write the program to the file
	file_to_write.write(reinterpret_cast<const char*>(command_set), commands_num);
	if (!file_to_write)
	{
		std::cout << "Something went wrong while writing to the file.\n";
		return 1;
	}

	// close file
	file_to_write.close();

	// trying to open file
	std::ifstream program_file("program.bin", std::ios::binary);
	if (!program_file.is_open())
	{
		std::cout << "Something went wrong while opening the file.\n";
		return 1;
	}

	// init virtual machine
	MyVM* vm = new MyVM();

	// execute program using VM
	if (vm->SetCommandSet(program_file))
	{
		int32_t vm_exit_code = vm->Start();
		printf("VM has exited with code %d.", vm_exit_code);
	}
}
