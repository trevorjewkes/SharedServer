#ifndef GENERAL_MESSAGE
#define GENERAL_MESSAGE

#include <sstream>
#include <string>

class GeneralMessage
{
public:
  GeneralMessage();
  ~GeneralMessage();

  std::string msg;

  virtual std::string encode();
  GeneralMessage decode(std::string msg);

private:
};

#endif // !GENERAL_MESSAGE
