#include <gtest/gtest.h>

#include <pwn/core/stringutils.h>

#define GTEST(x) TEST(StringUtils, x)

namespace test
{
	using namespace pwn;
	using namespace pwn::core;

	GTEST(TestSpaceCharactersShouldContainSpace)
	{
		EXPECT_NE(string::npos, kSpaceCharacters().find(' '));
	}

	//

	GTEST(TestSplitStringWithSpace)
	{
		std::vector<string> res = SplitString("ada c++ haskell ", " ");
		EXPECT_EQ(std::size_t(3), res.size());
		EXPECT_EQ("ada", res[0]);
		EXPECT_EQ("c++", res[1]);
		EXPECT_EQ("haskell", res[2]);
	}

	GTEST(TestSplitStringWithSpaceAndDash)
	{
		std::vector<string> res = SplitString(" - medicine-doctor?", " -");
		EXPECT_EQ(std::size_t(2), res.size());
		EXPECT_EQ("medicine", res[0]);
		EXPECT_EQ("doctor?", res[1]);
	}

	//

	GTEST(TestTrimSeveralCharacters)
	{
		EXPECT_EQ("beatles", Trim("beatles, the", "the, "));
		EXPECT_EQ("beatles", Trim("The beatles", " ehT"));
	}

	GTEST(TestBasicTrim)
	{
		EXPECT_EQ("abc ", TrimLeft(" abc ", " "));
		EXPECT_EQ(" abc", TrimRight(" abc ", " "));
		EXPECT_EQ("abc", Trim("   abc   ", " "));
	}

	//

	GTEST(TestStartsWith)
	{
		EXPECT_TRUE(StartsWith("abc", "a"));
		EXPECT_FALSE(StartsWith("abc", "b"));
		EXPECT_FALSE(StartsWith("abc", "A"));
		EXPECT_TRUE(StartsWith("abc", "abc"));
		EXPECT_TRUE(StartsWith("the Beatles", "the"));
	}

	//

	GTEST(TestEndsWith)
	{
		EXPECT_TRUE(EndsWith("abc", "c"));
		EXPECT_FALSE(EndsWith("abc", "a"));
		EXPECT_FALSE(EndsWith("abc", "C"));
		EXPECT_TRUE(EndsWith("beatles, the", " the"));
	}

	//

	GTEST(TestToLower)
	{
		EXPECT_EQ("small", ToLower("SMALL"));
		EXPECT_EQ("small", ToLower("small"));
		EXPECT_EQ("the beatles", ToLower("THe BeatLeS"));
	}

	//

	GTEST(TestReplaceObscenity)
	{
		EXPECT_EQ("statementist", StringReplace("expressionist", "expression", "statement"));
		EXPECT_EQ("picbutto", StringReplace("picasso", "ass", "butt"));
		EXPECT_EQ("great breasts", StringReplace("great tits", "tit", "breast"));
	}
}
