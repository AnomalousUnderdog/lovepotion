#include "common/debugger.h"
#include <switch.h>

using namespace love;

extern "C"
{
    Debugger::Debugger() : sockfd(-1)
    {
        if (this->IsInited())
            return;

        this->sockfd = nxlinkStdioForDebug();

        this->initialized = (this->sockfd != -1);
    }

    Debugger::~Debugger()
    {
        if (this->sockfd != -1)
            close(this->sockfd);
    }
}