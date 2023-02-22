#include "my_string.hpp"

void SplitString(const std::string &s, std::vector<std::string> &v,
                 const std::string &c) {
  std::string::size_type pos1 = 0;
  std::string::size_type pos2 = s.find(c);

  while (std::string::npos != pos2) {
    if (pos2 != pos1) v.push_back(s.substr(pos1, pos2 - pos1));

    pos1 = pos2 + c.size();
    pos2 = s.find(c, pos1);
  }

  if (pos1 != s.length()) v.push_back(s.substr(pos1));
}
