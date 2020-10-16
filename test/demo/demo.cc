#include "io/Process/Process.h"
#include <iostream>
int main()
{
    try {
        XuanWu::Process process(1, false, 0);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}