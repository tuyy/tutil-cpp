## tutil
* C++17 util library
* Only hpp file include tutil.hpp

```C++
std::string makeString(const char *aFormat, ...);
std::vector<std::string> split(const std::string&, char&);
template <typename T> std::string join(const std::vector<T>&, const std::string&);
std::string replaceAll(const std::string&, const std::string&, const std::string&);
std::string toLowerStr(const std::string&);
std::string toUpperStr(const std::string&);
std::string getDateStr(const int64_t, const std::string&);
void sleep(const int64_t aMilis);

class Timer;
class Stopwatch;
```
