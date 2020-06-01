//
// Created by denis on 22.07.19.
//

#ifndef STRING_LIB_STRING_HPP
#define STRING_LIB_STRING_HPP


#include <iostream>
#include <cstdio>
#include <exception>
#include <cstring>
#include <cassert>
#include <stdexcept>
#include <cstdlib>


#define TOP_ASCII    127
#define BOTTOM_ASCII 0



/*This value represents the maximum number of char elements in the string
 * in case of reading from stdin*/
#define SIZE_ARRAY_FOR_STR 100


class String final{
public:
	explicit String(const char* str);
    String(String&& st)noexcept;
    ~String();
    String() = default;
    void MoveData(String&& st)noexcept;
    String& operator=(const String& s);
	String& operator=(String&& s) noexcept;
    String(const String& s);
    bool empty()const noexcept;

    /*This method will process the string and do such actions :
     * 1) Substitute long function names with just one letters
     * 2) Delete all the spaces in the string
     * 3) Report in the case of RUBBISH data in the string by throwing an exception
     *
     * As result this function will return new string which will represent
     * a short mathematical representation of the current string*/
    String ParseShortMathNotation()const;

    char* GetStr()const noexcept;
    int GetNumBytes()const noexcept;
    int size()const noexcept;
    bool IsThisByteAscii(char c) const;
	bool eng() const noexcept
	{
		return is_english;
	}

    friend std::ostream&operator<<(std::ostream& out, const String& s);
    friend std::istream&operator>>(std::istream& in, String& s);

private:
    char*  str_       = nullptr;
    int    size_      = 0;           //number of letters
    int    num_bytes_ = 0;          //including the last '\0'
    bool   is_english = false;     // this value will show if the string is ASCII or not


    int StrLen(const char* str)const;
    int CountBytes(const char* str) const;
};

#endif //STRING_LIB_STRING_HPP
