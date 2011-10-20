#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <pwn/assert.h>

#include <iostream>

class QuietConsolePrinter : public ::testing::TestEventListener
{
public:
	virtual void OnTestProgramStart(const ::testing::UnitTest&) { }

	// Fired before each iteration of tests starts.  There may be more than
	// one iteration if GTEST_FLAG(repeat) is set. iteration is the iteration
	// index, starting from 0.
	virtual void OnTestIterationStart(const ::testing::UnitTest&, int) { }

	// Fired before environment set-up for each iteration of tests starts.
	virtual void OnEnvironmentsSetUpStart(const ::testing::UnitTest&) { }

	// Fired after environment set-up for each iteration of tests ends.
	virtual void OnEnvironmentsSetUpEnd(const ::testing::UnitTest&) { }

	// Fired before the test case starts.
	virtual void OnTestCaseStart(const ::testing::TestCase&) { }

	// Fired before the test starts.
	virtual void OnTestStart(const ::testing::TestInfo&) { }

	// Fired after a failed assertion or a SUCCESS().
	virtual void OnTestPartResult(const ::testing::TestPartResult& r)
	{
		if(r.passed())
		{
			return;
		}
		const std::string errname = "error"; //r.nonfatally_failed() ? "warning" : "error";
		::std::cout << r.file_name() << "(" << r.line_number() << ") : " << errname << " C4242: " << r.summary() << std::endl;// << r.message() << std::endl;
		// message seems to be the same as summary()
	}

	// Fired after the test ends.
	virtual void OnTestEnd(const ::testing::TestInfo&)
	{
	}

	// Fired after the test case ends.
	virtual void OnTestCaseEnd(const ::testing::TestCase&) { }

	// Fired before environment tear-down for each iteration of tests starts.
	virtual void OnEnvironmentsTearDownStart(const ::testing::UnitTest&) { }

	// Fired after environment tear-down for each iteration of tests ends.
	virtual void OnEnvironmentsTearDownEnd(const ::testing::UnitTest&) { }

	// Fired after each iteration of tests finishes.
	virtual void OnTestIterationEnd(const ::testing::UnitTest&,
	                                int) { }

	// Fired after all test activities have ended.
	virtual void OnTestProgramEnd(const ::testing::UnitTest& unit_test)
	{
		if(unit_test.failed_test_case_count() == 0)
		{
			::std::cout << "All " << unit_test.successful_test_count() <<  " tests PASSED." << ::std::endl;
		}
		else
		{
			::std::cout << unit_test.failed_test_case_count() << " test(s) failed." << ::std::endl;
		}
	}

	static void Setup()
	{
		::testing::TestEventListeners& l = ::testing::UnitTest::GetInstance()->listeners();
		delete l.Release(l.default_result_printer());
		l.Append(new QuietConsolePrinter());
	}
};


int main(int argc, char** argv)
{
	pwn::assert::Init(pwn::assert::AlwaysThrow);
	QuietConsolePrinter::Setup();
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}
