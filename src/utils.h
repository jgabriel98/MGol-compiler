#ifndef UTILS_HEADER
#define UTILS_HEADER

#include <stddef.h>
#include <utility>
#include <vector>
#include <sstream>
#include <algorithm>

struct hash_pair {
    template <class T1, class T2> 
    inline size_t operator()(const std::pair<T1, T2>& p) const { 
        auto hash1 = std::hash<T1>{}(p.first); 
        auto hash2 = std::hash<T2>{}(p.second); 
        return hash1 ^ hash2; 
    } 
};


class string_utils {

public:
    static std::string wrap(std::string const &input, size_t max_width, size_t min_width = 0, size_t indent = 0) { 
        std::string out;

        out = std::string(indent, ' '); 
        size_t current = indent;
        std::string word;


        for(auto &c: input){
            if(c == '\n') current = 0;
            if(++current > max_width){
                out.push_back('\n');
                current=0;
            }
            out.push_back(c);
        }

        size_t remaining = out.find_last_of('\n');
        remaining = (remaining == std::string::npos) ? out.size() :  out.size() - (remaining+1);
        remaining = min_width - remaining;
        out += std::string(remaining, ' ');
        
        return out;
    }
};

#endif