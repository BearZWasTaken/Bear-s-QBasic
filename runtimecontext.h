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
    int getIdentifierValue(const std::string& name) const;
    void setIdentifierValue(const std::string& name, const int value);
    void throwError(const char* errMsg);
};

#endif // RUNTIMECONTEXT_H
