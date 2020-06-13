#include <pwn/assert.h>

#define CATCH_CONFIG_RUNNER
#include "catch/catch.hpp"

int main( int argc, char* argv[] )
{
    Catch::Session session;

    int returnCode = session.applyCommandLine( argc, argv );
    if( returnCode != 0 )
    {
        return returnCode;
    }

    pwn::assert::Init(pwn::assert::AlwaysThrow);

    int numFailed = session.run();
    return numFailed;
}
