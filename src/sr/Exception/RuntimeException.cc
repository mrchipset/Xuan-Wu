#include "RuntimeException.h"


using namespace std;
using namespace XuanWu;

ConstructorException::ConstructorException(std::string const& _Message) : RuntimeException(_Message)
{}

NotImplementedException::NotImplementedException(std::string const& _Message) : RuntimeException(_Message)
{}