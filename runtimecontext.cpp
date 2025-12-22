#include "runtimecontext.h"

RuntimeContext::RuntimeContext() {}

void RuntimeContext::clear()
{
    varValues.clear();
    varUseCnts.clear();
}

int RuntimeContext::getVarValue(const std::string& name)
{
    varUseCnts[name]++;
    auto it = varValues.find(name);
    if (it == varValues.end())
    {
        throwError(("Variable '" + name + "' not found").c_str());
    }
    return it->second;
}

void RuntimeContext::setVarValue(const std::string& name, const int value)
{
    varValues[name] = value;
}

int RuntimeContext::getVarUseCnt(const std::string& name) const
{
    auto it = varUseCnts.find(name);
    if (it == varUseCnts.end())
        return 0;
    return it->second;
}

void RuntimeContext::throwError(const char* errMsg) const
{
    throw errMsg;
}
