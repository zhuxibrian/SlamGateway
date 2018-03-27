/*
 * String.h
 *
 *  Created on: 2018年3月27日
 *      Author: xd
 */

#ifndef STRING_H_
#define STRING_H_

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

class String;
typedef std::vector<String> StringList;
class String : public std::string{
public:
    inline String():std::string() {}
    inline String(std::string s):std::string(s) {}
    inline String(const char *s):std::string(s) {}
    virtual ~String(){}
    inline operator std::string(){return *this;}
    inline operator const char *(){return this->c_str();}
    template<typename _T>
    inline String operator+(_T v){
        std::stringstream ss;ss<<v;
        return this->append(ss.str());
    }
    template<typename _T>
    inline StringList split(const _T &delim){
        String &s = *this;
        StringList v;
        auto i = 0;
        auto pos = s.find(delim);
        while (pos != String::npos) {
            v.push_back(s.substr(i, pos - i));
            i = ++pos;
            pos = s.find(delim, pos);

            if (pos == String::npos)
                v.push_back(s.substr(i, s.length()));
        }
        return v;
    }
    inline String trim(const String &chars = "\t\n\v\f\r "){
        String str = *this;
        str.erase(0, str.find_first_not_of(chars));
        str.erase(str.find_last_not_of(chars) + 1);
        return str;
    }
    inline String trim_start(const String &chars = "\t\n\v\f\r "){
        String str = *this;
        str.erase(0, str.find_first_not_of(chars));
        return str;
    }
    inline String trim_end(const String &chars = "\t\n\v\f\r "){
        String str = *this;
        str.erase(str.find_last_not_of(chars) + 1);
        return str;
    }
    inline bool start_with(const String &s){
        return this->compare(0, s.size(), s) == 0;
    }
    inline bool end_with(const String &s){
        return this->compare(s.size() - s.size(), s.size(), s) == 0;
    }
    inline String skip(size_t n){
        if(n>=length())return "";
        return this->substr(n,this->length()-n);
    }
    inline String take(size_t n){
        if(n>=length())return *this;
        return this->substr(0,n);
    }
//    inline String &toupper(){
//        std::transform(begin(), end(), begin(), std::toupper);
//        return *this;
//    }
    template<typename _T>
    inline _T tonumber(){
        std::istringstream iss(*this);
        _T num;
        iss >> num;
        return num;
    }
    inline unsigned long int        toul(int base = 10)    {return strtoul(this->c_str(),NULL,base);}
    inline unsigned long long int     toull(int base = 10){return strtoull(this->c_str(),NULL,base);}
    inline unsigned int                 tol(int base = 10)    {return strtol(this->c_str(),NULL,base);}
    inline unsigned long int         toll(int base = 10) {return strtoll(this->c_str(),NULL,base);}
    inline float                     toul()                {return strtof(this->c_str(),NULL);}
    inline double                     toull()                {return strtod(this->c_str(),NULL);}
    template<typename... _Args>
    inline String& snprintf(int bufSize, const char *fmt, _Args... args){
        char *buf = new char[bufSize];
        ::snprintf(buf,bufSize,fmt,args...);
        this->assign(buf);
        delete buf;
        return *this;
    }
    inline String& vsnprintf(int bufSize, const char *fmt, va_list va){
        char *buf = new char[bufSize];
        ::vsnprintf(buf,bufSize,fmt,va);
        this->assign(buf);
        delete buf;
        return *this;
    }
};



#endif /* STRING_H_ */
