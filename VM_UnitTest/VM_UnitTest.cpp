#include "pch.h"
#include "CppUnitTest.h"
#include "../LAB4_VM/MyVM.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace VMUnitTest
{
	TEST_CLASS(VMUnitTest)
	{
	public:
		
		TEST_METHOD(TestSetRegistersNum)
		{
			MyVM* vm = new MyVM();

			uint32_t reg_num = 5;
			bool set_res = vm->SetRegistersNum(reg_num);
			Assert::AreEqual(reg_num, vm->GetRegistersNum());
			Assert::AreEqual(true, set_res);

			reg_num = 15;
			set_res = vm->SetRegistersNum(reg_num);
			Assert::AreEqual(false, set_res);
		}

		TEST_METHOD(TestNOP)
		{
			MyVM* vm = new MyVM();

			uint8_t commands_num = 1;
			uint8_t* command_set = new uint8_t[commands_num]{NOP};
			vm->SetCommandSet(command_set, commands_num);
			int32_t vm_exit_code = vm->Start();
			Assert::AreEqual(1, vm_exit_code);

			delete[] command_set;
		}

		TEST_METHOD(TestEXT)
		{
			MyVM* vm = new MyVM();

			uint8_t commands_num = 2;
			uint8_t* command_set = new uint8_t[commands_num]{ EXT, NOP };
			vm->SetCommandSet(command_set, commands_num);
			int32_t vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			delete[] command_set;
		}

		TEST_METHOD(TestMOV)
		{
			MyVM* vm = new MyVM();

			uint8_t commands_num = 2;
			uint8_t* command_set = new uint8_t[commands_num]{ MOV, 0x01};
			vm->SetCommandSet(command_set, commands_num);
			int32_t vm_exit_code = vm->Start();
			Assert::AreEqual(-1, vm_exit_code);

			commands_num = 3;
			command_set = new uint8_t[commands_num]{ MOV, 0x01, 0x01 };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(-1, vm_exit_code);

			commands_num = 3;
			command_set = new uint8_t[commands_num]{ MOV, 0x01, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(-1, vm_exit_code);

			commands_num = 4;
			command_set = new uint8_t[commands_num]{ MOV, R0, 0x01, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			commands_num = 7;
			command_set = new uint8_t[commands_num]{ MOV, R0, 0x01, MOV, R1, R0, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			delete[] command_set;
		}

		TEST_METHOD(TestINC)
		{
			MyVM* vm = new MyVM();

			uint8_t commands_num = 2;
			uint8_t* command_set = new uint8_t[commands_num]{ INC, 0x01};
			vm->SetCommandSet(command_set, commands_num);
			int32_t vm_exit_code = vm->Start();
			Assert::AreEqual(-1, vm_exit_code);

			commands_num = 2;
			command_set = new uint8_t[commands_num]{ INC, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(-1, vm_exit_code);

			commands_num = 6;
			command_set = new uint8_t[commands_num]{ MOV, R0, 0x01, INC, R0, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			delete[] command_set;
		}

		TEST_METHOD(TestMULT)
		{
			MyVM* vm = new MyVM();

			uint8_t commands_num = 3;
			uint8_t* command_set = new uint8_t[commands_num]{ MULT, 0x01, 0x02 };
			vm->SetCommandSet(command_set, commands_num);
			int32_t vm_exit_code = vm->Start();
			Assert::AreEqual(-1, vm_exit_code);

			commands_num = 2;
			command_set = new uint8_t[commands_num]{ MULT, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(-1, vm_exit_code);

			commands_num = 6;
			command_set = new uint8_t[commands_num]{ MOV, R0, 0x01, MULT, R0, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(-1, vm_exit_code);

			commands_num = 7;
			command_set = new uint8_t[commands_num]{ MOV, R0, 0x01, MULT, R0, 0x02, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			commands_num = 10;
			command_set = new uint8_t[commands_num]{ MOV, R0, 0x01, MOV, R1, 0x02, MULT, R0, R1, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			delete[] command_set;
		}

		TEST_METHOD(TestCMP)
		{
			MyVM* vm = new MyVM();

			uint8_t commands_num = 2;
			uint8_t* command_set = new uint8_t[commands_num]{ CMP, EXT };
			vm->SetCommandSet(command_set, commands_num);
			int32_t vm_exit_code = vm->Start();
			Assert::AreEqual(-1, vm_exit_code);

			commands_num = 3;
			command_set = new uint8_t[commands_num]{ CMP, 0x01, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(-1, vm_exit_code);

			commands_num = 4;
			command_set = new uint8_t[commands_num]{ CMP, 0x01, 0x02, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			commands_num = 7;
			command_set = new uint8_t[commands_num]{ MOV, R0, 0x01, CMP, R0, 0x02, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			commands_num = 7;
			command_set = new uint8_t[commands_num]{ MOV, R0, 0x01, CMP, 0x02, R0, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			commands_num = 10;
			command_set = new uint8_t[commands_num]{ MOV, R0, 0x01, MOV, R1, 0x02, CMP, R0, R1, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			delete[] command_set;
		}

		TEST_METHOD(TestJMP_E)
		{
			MyVM* vm = new MyVM();

			uint8_t commands_num = 1;
			uint8_t* command_set = new uint8_t[commands_num]{ JMP_E };
			vm->SetCommandSet(command_set, commands_num);
			int32_t vm_exit_code = vm->Start();
			Assert::AreEqual(-1, vm_exit_code);

			commands_num = 2;
			command_set = new uint8_t[commands_num]{ JMP_E, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(-1, vm_exit_code);

			commands_num = 8;
			command_set = new uint8_t[commands_num]{ CMP, 0x01, 0x01, JMP_E, 0x07, INC, 0x01, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			commands_num = 8;
			command_set = new uint8_t[commands_num]{ CMP, 0x01, 0x02, JMP_E, 0x06, EXT, INC, 0x01 };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			commands_num = 8;
			command_set = new uint8_t[commands_num]{ CMP, 0x02, 0x01, JMP_E, 0x06, EXT, INC, 0x01 };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			commands_num = 11;
			command_set = new uint8_t[commands_num]{ MOV, R0, 0x0a, CMP, 0x01, 0x01, JMP_E, R0, INC, 0x01, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			commands_num = 11;
			command_set = new uint8_t[commands_num]{ MOV, R0, 0x01, CMP, R0, R0, JMP_E, 0x0a, INC, 0x01, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			delete[] command_set;
		}

		TEST_METHOD(TestJMP_L)
		{
			MyVM* vm = new MyVM();

			uint8_t commands_num = 1;
			uint8_t* command_set = new uint8_t[commands_num]{ JMP_L };
			vm->SetCommandSet(command_set, commands_num);
			int32_t vm_exit_code = vm->Start();
			Assert::AreEqual(-1, vm_exit_code);

			commands_num = 2;
			command_set = new uint8_t[commands_num]{ JMP_L, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(-1, vm_exit_code);

			commands_num = 8;
			command_set = new uint8_t[commands_num]{ CMP, 0x01, 0x02, JMP_L, 0x07, INC, 0x01, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			commands_num = 8;
			command_set = new uint8_t[commands_num]{ CMP, 0x01, 0x01, JMP_L, 0x07, EXT, INC, 0x01 };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			commands_num = 8;
			command_set = new uint8_t[commands_num]{ CMP, 0x02, 0x01, JMP_L, 0x07, EXT, INC, 0x01 };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			commands_num = 11;
			command_set = new uint8_t[commands_num]{ MOV, R0, 0x01, CMP, R0, 0x02, JMP_L, 0x0a, INC, 0x01, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			commands_num = 14;
			command_set = new uint8_t[commands_num]{ MOV, R0, 0x01, MOV, R1, 0x02, CMP, R0, R1, JMP_L, 0x0d, INC, 0x01, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			delete[] command_set;
		}

		TEST_METHOD(TestJMP_G)
		{
			MyVM* vm = new MyVM();

			uint8_t commands_num = 1;
			uint8_t* command_set = new uint8_t[commands_num]{ JMP_G };
			vm->SetCommandSet(command_set, commands_num);
			int32_t vm_exit_code = vm->Start();
			Assert::AreEqual(-1, vm_exit_code);

			commands_num = 2;
			command_set = new uint8_t[commands_num]{ JMP_G, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(-1, vm_exit_code);

			commands_num = 8;
			command_set = new uint8_t[commands_num]{ CMP, 0x02, 0x01, JMP_G, 0x07, INC, 0x01, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			commands_num = 8;
			command_set = new uint8_t[commands_num]{ CMP, 0x01, 0x01, JMP_G, 0x07, EXT, INC, 0x01 };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			commands_num = 8;
			command_set = new uint8_t[commands_num]{ CMP, 0x01, 0x02, JMP_G, 0x07, EXT, INC, 0x01 };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			commands_num = 11;
			command_set = new uint8_t[commands_num]{ MOV, R0, 0x02, CMP, R0, 0x01, JMP_G, 0x0a, INC, 0x01, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			commands_num = 14;
			command_set = new uint8_t[commands_num]{ MOV, R0, 0x02, MOV, R1, 0x01, CMP, R0, R1, JMP_G, 0x0d, INC, 0x01, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			delete[] command_set;
		}

		TEST_METHOD(TestJMP)
		{
			MyVM* vm = new MyVM();

			uint8_t commands_num = 1;
			uint8_t* command_set = new uint8_t[commands_num]{ JMP };
			vm->SetCommandSet(command_set, commands_num);
			int32_t vm_exit_code = vm->Start();
			Assert::AreEqual(-1, vm_exit_code);

			commands_num = 2;
			command_set = new uint8_t[commands_num]{ JMP, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(-1, vm_exit_code);

			commands_num = 5;
			command_set = new uint8_t[commands_num]{ JMP, 0x04, INC, 0x01, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			commands_num = 8;
			command_set = new uint8_t[commands_num]{ MOV, R0, 0x07, JMP, R0, INC, 0x01, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			delete[] command_set;
		}

		TEST_METHOD(TestPRNT)
		{
			MyVM* vm = new MyVM();

			uint8_t commands_num = 1;
			uint8_t* command_set = new uint8_t[commands_num]{ PRNT };
			vm->SetCommandSet(command_set, commands_num);
			int32_t vm_exit_code = vm->Start();
			Assert::AreEqual(-1, vm_exit_code);

			commands_num = 2;
			command_set = new uint8_t[commands_num]{ PRNT, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(-1, vm_exit_code);

			commands_num = 3;
			command_set = new uint8_t[commands_num]{ PRNT, 0x01, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			commands_num = 6;
			command_set = new uint8_t[commands_num]{ MOV, R0, 0x01, PRNT, R0, EXT };
			vm->SetCommandSet(command_set, commands_num);
			vm_exit_code = vm->Start();
			Assert::AreEqual(0, vm_exit_code);

			delete[] command_set;
		}
	};
}
