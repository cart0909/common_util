#include "HelloWorld.h"
#include "common.h"

int main(int argc, char** argv)
{
    FLAGS_logtostderr = 1;
    google::InitGoogleLogging(argv[0]);

    af::setDevice(0);
    af::info();

    HelloWorld::Foo();

    return 0;
}
