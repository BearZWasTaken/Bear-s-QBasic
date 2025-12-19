#include "runtimecontext.h"

RuntimeContext::RuntimeContext() {}

int RuntimeContext::getIdentifierValue(const std::string& name) const
{
    auto it = varValues.find(name);
    if (it == varValues.end())
    {
        //// errrrr
    }
    return it->second;
}

void RuntimeContext::setIdentifierValue(const std::string& name, const int value)
{
    varValues[name] = value;
}

void RuntimeContext::throwError(const char* errMsg)
{
    throw errMsg;
}
