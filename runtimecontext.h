#ifndef RUNTIMECONTEXT_H
#define RUNTIMECONTEXT_H

#include <string>
#include <unordered_map>

class RuntimeContext
{
private:
    std::unordered_map<std::string, int> varValues;
    std::unordered_map<std::string, int> varUseCnts;

public:
    RuntimeContext();
    void clear();
    int getVarValue(const std::string& name);
    void setVarValue(const std::string& name, const int value);
    int getVarUseCnt(const std::string& name) const;
    void throwError(const char* errMsg) const;
};

#endif // RUNTIMECONTEXT_H
