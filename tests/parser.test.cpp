#include "CppUnitTest.h"

#include <string>
#include <parser.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test
{
	TEST_CLASS(MainClass)
	{
	public:
		TEST_METHOD(SaveFiveEmptyFrames) {
			unsigned char input[] = {
				0x00, 0x00, 0x00, 0x00,	//0 - 1	: 5
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x02, 0x00, 0x00,	//2		: 2
				0x00, 0x00, 0x00, 0x00, //3 - 4	: 5
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x02, 0x00, 0x00,	//5 - 7	: 2
				0x00, 0x02, 0x00, 0x00,
				0x00, 0x02, 0x00, 0x00,
				0x04, 0x02, 0x00, 0x00,	//8		: 2C
				0x00, 0x00, 0x00, 0x00, //9 - 10: 5
				0x00, 0x00, 0x00, 0x00,
			};

			std::string out = Parser::toFile(input, sizeof(input));
			std::string expected = "2: 2\n5-7: 2\n8: 2C\n10: 5\n";

			Assert::IsTrue(out.compare(expected) == 0);
		}

		TEST_METHOD(LoadInputStringIntoBytes) {
			std::string in = "2: 2\n5-7: 2\n8: 2C\n10: 5\n";
			unsigned char output[11 * 4];
			memset(output, 0, sizeof(output));
			size_t dummySize = 0;

			if (!Parser::toGameBytes(in, output, &dummySize, false)) {
				Assert::Fail(L"Parser failed.");
			}

			Assert::AreEqual((size_t)44, dummySize);

			unsigned char expected[44] = {
				0x00, 0x00, 0x00, 0x00,	//0 - 1	: 5
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x02, 0x00, 0x00,	//2		: 2
				0x00, 0x00, 0x00, 0x00, //3 - 4	: 5
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x02, 0x00, 0x00,	//5 - 7	: 2
				0x00, 0x02, 0x00, 0x00,
				0x00, 0x02, 0x00, 0x00,
				0x04, 0x02, 0x00, 0x00,	//8		: 2C
				0x00, 0x00, 0x00, 0x00, //9 - 10: 5
				0x00, 0x00, 0x00, 0x00
			};

			for (unsigned int i = 0; i < dummySize; i++) {
				Assert::AreEqual(expected[i], output[i]);
			}
		}

		TEST_METHOD(SaveAndLoadCycle) {
			unsigned char input[] = {
				0x00, 0x00, 0x00, 0x00,	//0 - 1	: 5
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x02, 0x00, 0x00,	//2		: 2
				0x00, 0x00, 0x00, 0x00, //3 - 4	: 5
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x02, 0x00, 0x00,	//5 - 7	: 2
				0x00, 0x02, 0x00, 0x00,
				0x00, 0x02, 0x00, 0x00,
				0x04, 0x02, 0x00, 0x00,	//8		: 2C
				0x00, 0x00, 0x00, 0x00, //9 - 10: 5
				0x00, 0x00, 0x00, 0x00
			};

			std::string out = Parser::toFile(input, sizeof(input));

			unsigned char result[1800 * 4];
			memset(result, 0, sizeof(result));

			size_t size = 0;

			if (!Parser::toGameBytes(out, result, &size, false)) {
				Assert::Fail(L"Parser failed.");
			}

			Assert::AreEqual(size, sizeof(input));

			for (unsigned int i = 0; i < size; i++) {
				Assert::AreEqual(input[i], result[i]);
			}
		}

		TEST_METHOD(LoadAsReversal) {
			std::string in = "2: 2\n5-7: 2\n8: 2C\n10: 5\n";
			unsigned char result[1800 * 4];
			memset(result, 0, sizeof(result));

			size_t size = 0;

			if (!Parser::toGameBytes(in, result, &size, true)) {
				Assert::Fail(L"Parser failed.");
			}

			unsigned char expected[] = {
				0x00, 0x02, 0x00, 0x00,	//2		: 2
				0x00, 0x00, 0x00, 0x00, //3 - 4	: 5
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x02, 0x00, 0x00,	//5 - 7	: 2
				0x00, 0x02, 0x00, 0x00,
				0x00, 0x02, 0x00, 0x00,
				0x04, 0x02, 0x00, 0x00,	//8		: 2C
				0x00, 0x00, 0x00, 0x00, //9 - 10: 5
				0x00, 0x00, 0x00, 0x00
			};

			constexpr size_t expectedLength = sizeof(expected);
			Assert::AreEqual(expectedLength, size);

			for (unsigned int i = 0; i < (size / 4); i++) {
				Assert::AreEqual(expected[i], result[i]);
			}
		}
	};
}
