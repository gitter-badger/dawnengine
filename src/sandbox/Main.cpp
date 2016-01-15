/*
 * Dawn Engine
 * Written by David Avedissian (c) 2012-2016 (git@davedissian.com)
 */
#include "DawnEngine.h"

int main(int argc, char** argv)
{
    dw::Engine* engine = new dw::Engine("Sandbox", "1.0.0");
    engine->Setup();
    engine->Run([](float) {});
    engine->Shutdown();
    return 0;
}
