/*
 * vim 설정
 * vim:ts=4:shiftwidth=4:sts=4:et:cindent:fileencoding=utf-8:
 */

#include <tutil.hpp>

#include <chrono>
#include <thread>
#include <string>

static void doWork(void)
{
    tutil::Timer sTimer(tutil::makeString("doWork(void) %s", "Hello world!"));

    // TODO
    // ..

    tutil::sleep(1000);
}

int32_t main(void)
{
    doWork();

    return 0;
}
