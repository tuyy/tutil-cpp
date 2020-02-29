/*
 * vim 설정
 * vim:ts=4:shiftwidth=4:sts=4:et:cindent:fileencoding=utf-8:
 */

#pragma once

#include <iostream>
#include <functional>
#include <fstream>
#include <algorithm>
#include <string>
#include <thread>
#include <sstream>
#include <chrono>
#include <vector>
#include <locale>
#include <ctime>
#include <string>
#include <chrono>

namespace tutil
{

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

std::string makeString(const char *aFormat, ...)
{
    if (aFormat == nullptr)
    {
        return std::string();
    }
    else
    {
        va_list sAp;
        int32_t sLen;
        va_start(sAp, aFormat);
        sLen = std::vsnprintf(nullptr, 0, aFormat, sAp);
        va_end(sAp);

        std::unique_ptr<char[]> sBuffer(new char[sLen + 1]);

        va_start(sAp, aFormat);
        (void)std::vsnprintf(sBuffer.get(), sLen + 1, aFormat, sAp);
        va_end(sAp);

        return std::string(sBuffer.get(), sLen);
    }
}

std::vector<std::string> split(const std::string &aTarget, char aDelim)
{
    std::vector<std::string> sResult;
    std::stringstream sStream(aTarget);
    std::string sItem;

    while (std::getline(sStream, sItem, aDelim))
    {
        sResult.push_back(sItem);
    }

    return sResult;
}

template <typename T>
std::string join(const std::vector<T> &aVector, const std::string &aDelim)
{
    std::ostringstream sResult;

    bool sIsFirst = true;
    for (const auto &sVal: aVector)
    {
        if (sIsFirst == false)
        {
            sResult << aDelim << sVal;
        }
        else
        {
            sResult << sVal;
            sIsFirst = false;
        }
    }

    return std::string(sResult.str());
}

static size_t calcNecessaryBufferSize(const std::string &aSourceStr, const std::string &aFrom, const std::string &aTo)
{
    size_t sFromSize = aFrom.size();
    size_t sToSize = aTo.size();

    if (sFromSize == 0)
    {
        return aSourceStr.size();
    }

    if (sToSize > sFromSize)
    {
        size_t sPossibleOccurenceCnt = aSourceStr.size() / sFromSize + 1;
        return sPossibleOccurenceCnt * sToSize + sFromSize;
    }
    else
    {
        return aSourceStr.size();
    }
}

std::string replaceAll(const std::string &aSourceStr, const std::string &aFrom, const std::string &aTo)
{
    if (aFrom.empty() == true)
    {
        return aSourceStr;
    }

    std::string sResultStr;

    sResultStr.reserve(calcNecessaryBufferSize(aSourceStr, aFrom, aTo));

    std::string::size_type sPos = 0;
    std::string::size_type sFound = 0;

    while ((sFound = aSourceStr.find(aFrom, sPos)) != std::string::npos)
    {
        sResultStr.append(aSourceStr, sPos, sFound - sPos);
        sResultStr.append(aTo);

        sPos = sFound + aFrom.size();
    }

    sResultStr.append(aSourceStr, sPos, std::string::npos);

    return sResultStr;
}

std::string toLowerStr(const std::string &aSourceStr)
{
    std::string sResultStr(aSourceStr);

    std::transform(sResultStr.begin(), sResultStr.end(), sResultStr.begin(), ::tolower);

    return sResultStr;
}

std::string toUpperStr(const std::string &aSourceStr)
{
    std::string sResultStr(aSourceStr);

    std::transform(sResultStr.begin(), sResultStr.end(), sResultStr.begin(), ::toupper);

    return sResultStr;
}

std::string getDateStr(const int64_t aSeconds, const std::string &aFormat)
{
    char sBuffer[32] = {0x00};
    struct tm sTm;
    time_t sTime = aSeconds;

    if (localtime_r(&sTime, &sTm) == nullptr)
    {
        throw std::runtime_error("localtime_r return nullptr.");
    }

    if (strftime(sBuffer, sizeof(sBuffer), aFormat.c_str(), &sTm) == 0)
    {
        throw std::runtime_error("strftime return zero.");
    }

    return std::string(sBuffer);
}

void sleep(const int64_t aMilis)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(aMilis));
}

template <typename T>
void processWithFile(const std::string &aFileName,
                     const std::function<T(const std::string& aLine)> &aParseLineFunc,
                     const std::function<void(T aParam)> &aProcessFunc)
{
    std::ifstream sInput(aFileName);

    size_t sCount = 1;
    std::string sLine;
    if (sInput.is_open())
    {
        while (getline(sInput, sLine))
        {
            if (sLine.size() == 0 || sLine[0] == '#')
            {
                continue;
            }

            std::cout << makeString("NO:%ld LINE:%s", sCount++, sLine.c_str()) << "\n";

            T sParam = aParseLineFunc(sLine);

            aProcessFunc(sParam);
        }
    }
}

class Timer
{
    public:
        Timer(const std::string &aName="")
            : mName(aName), mStart(std::chrono::system_clock::now())
        {
        }

        ~Timer(void)
        {
            std::chrono::system_clock::duration sDiff = std::chrono::system_clock::now() - mStart;

            auto sResult = std::chrono::duration_cast<std::chrono::milliseconds>(sDiff).count();

            if (mName.size() == 0)
            {
                std::cout << makeString("ELAPSED:%ld.%.3ld", sResult / 1000, (sResult % 1000)) << "\n";
            }
            else
            {
                std::cout << makeString("ELAPSED:%ld.%.3ld NAME:%s",
                                        sResult / 1000,
                                        (sResult % 1000),
                                        mName.c_str()) << "\n";
            }
        }

    private:
        std::string mName;
        std::chrono::system_clock::time_point mStart;
};

class Stopwatch final
{
    public:
        Stopwatch(void)
            : mStart(std::chrono::system_clock::time_point::min())
        {
        }

        ~Stopwatch(void) = default;

        void clear(void)
        {
            mStart = std::chrono::system_clock::time_point::min();
        }

        bool isStarted(void) const
        {
            return (mStart.time_since_epoch() != std::chrono::system_clock::duration(0)); 
        }

        void start(void)
        {
            mStart = std::chrono::system_clock::now(); 
        }

        std::string stopAndGetMsStr(void) const
        {
            if (isStarted() == true)
            {
                std::chrono::system_clock::duration sDiff = std::chrono::system_clock::now() - mStart;

                auto sResult = std::chrono::duration_cast<std::chrono::milliseconds>(sDiff).count();

                return makeString("%ld.%.3ld", sResult / 1000, (sResult % 1000));
            }
            else
            {
                return 0;
            }
        }

    private:
        std::chrono::system_clock::time_point mStart;
};

inline Stopwatch gStopwatch;

}
