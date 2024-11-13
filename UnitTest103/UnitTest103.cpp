#include "pch.h"
#include "CppUnitTest.h"
#include "../10.3/10.3.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest103
{
	TEST_CLASS(UnitTest103)
	{
	public:

		TEST_METHOD(TestIsValidPhoneNumber)
		{
			Assert::IsTrue(isValidPhoneNumber("1234567890"));
			Assert::IsFalse(isValidPhoneNumber("12345"));
			Assert::IsFalse(isValidPhoneNumber("1234abcd90"));
			Assert::IsTrue(isValidPhoneNumber("0987654321"));
		}

		TEST_METHOD(TestIsValidDate)
		{
			Assert::IsTrue(isValidDate(15, 5, 2000));  
			Assert::IsFalse(isValidDate(31, 2, 2020)); 
			Assert::IsTrue(isValidDate(29, 2, 2020));  
			Assert::IsFalse(isValidDate(32, 1, 2021)); 
			Assert::IsFalse(isValidDate(15, 13, 2021));
		}


	};
}
