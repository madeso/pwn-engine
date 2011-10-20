#include <gtest/gtest.h>
#include <pwn/core/enum.h>

#define GTEST(x) TEST(Enum, x)

namespace test
{
	using namespace pwn;
	using namespace pwn::core;

	GTEST(testEnumUsageBeforeAdd)
	{
		EnumType t;
		EnumValue a1 = t.toEnum("a");
		EnumValue b = t.toEnum("b");
		EnumValue a2 = t.toEnum("a");
		EXPECT_EQ(a1, a2);
		EXPECT_NE(a1, b);
		t.addEnum("a");
		t.addEnum("b");
		t.addEnum("c");
		t.stopAdding();
		EnumValue c = t.toEnum("c");
		EXPECT_EQ(0, a1.toValue());
		EXPECT_EQ(1, b.toValue());
		EXPECT_EQ(2, c.toValue());
	}
}
