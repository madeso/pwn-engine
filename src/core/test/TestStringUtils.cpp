#include <UnitTest++.h>

#include <pwn/core/stringutils>

#pragma comment(lib, "pwn-core.lib")

SUITE(testStringUtls)
{
	using namespace pwn;
	using namespace pwn::core;

	TEST(TestSpaceCharactersShouldContainSpace)
	{
		CHECK(string::npos != kSpaceCharacters().find(' '));
	}

	//

	TEST(TestSplitStringWithSpace)
	{
		std::vector<string> res = SplitString("ada c++ haskell ", " ");
		CHECK_EQUAL(std::size_t(3), res.size());
		CHECK_EQUAL("ada", res[0]);
		CHECK_EQUAL("c++", res[1]);
		CHECK_EQUAL("haskell", res[2]);
	}

	TEST(TestSplitStringWithSpaceAndDash)
	{
		std::vector<string> res = SplitString(" - medicine-doctor?", " -");
		CHECK_EQUAL(std::size_t(2), res.size());
		CHECK_EQUAL("medicine", res[0]);
		CHECK_EQUAL("doctor?", res[1]);
	}

	//

	TEST(TestTrimSeveralCharacters)
	{
		CHECK_EQUAL("beatles", Trim("beatles, the", "the, "));
		CHECK_EQUAL("beatles", Trim("The beatles", " ehT"));
	}

	TEST(TestBasicTrim)
	{
		CHECK_EQUAL("abc ", TrimLeft(" abc ", " "));
		CHECK_EQUAL(" abc", TrimRight(" abc ", " "));
		CHECK_EQUAL("abc", Trim("   abc   ", " "));
	}

	//

	TEST(TestStartsWith)
	{
		CHECK(StartsWith("abc", "a"));
		CHECK(!StartsWith("abc", "b"));
		CHECK(!StartsWith("abc", "A"));
		CHECK(StartsWith("abc", "abc"));
		CHECK(StartsWith("the Beatles", "the"));
	}

	//

	TEST(TestEndsWith)
	{
		CHECK(EndsWith("abc", "c"));
		CHECK(!EndsWith("abc", "a"));
		CHECK(!EndsWith("abc", "C"));
		CHECK(EndsWith("beatles, the", " the"));
	}

	//

	TEST(TestToLower)
	{
		CHECK_EQUAL("small", ToLower("SMALL"));
		CHECK_EQUAL("small", ToLower("small"));
		CHECK_EQUAL("the beatles", ToLower("THe BeatLeS"));
	}

	//

	TEST(TestReplaceObscenity)
	{
		CHECK_EQUAL("statementist", StringReplace("expressionist", "expression", "statement"));
		CHECK_EQUAL("picbutto", StringReplace("picasso", "ass", "butt"));
		CHECK_EQUAL("great breasts", StringReplace("great tits", "tit", "breast"));
	}
}