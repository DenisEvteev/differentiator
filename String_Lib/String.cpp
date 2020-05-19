//
// Created by denis on 22.07.19.
//

#include "String.hpp"

String::String(const char* str) {
    size_ = StrLen(str);
    num_bytes_ = CountBytes(str);
    str_ = new char[num_bytes_];
    for(int i = 0; i < num_bytes_; ++i)
        str_[i] = str[i];
}

String::String(String&& st)noexcept{
    MoveData(std::move(st));
}

void String::MoveData(String&& st)noexcept {
    str_ = st.str_;
    size_ = st.size_;
    num_bytes_ = st.num_bytes_;
    is_english = st.is_english;
    st.size_ = 0;
    st.num_bytes_ = 0;
    st.str_ = nullptr;
}

String& String::operator=(String&& st) noexcept{
	if(this == &st)
		return *this;
	delete[] str_;
    MoveData(std::move(st));
    return *this;
}

String::String(const String& s){
    size_ = s.size_;
    num_bytes_ = s.num_bytes_;
    str_ = new char[num_bytes_]{};
    for(int i = 0; i < num_bytes_; ++i)
        str_[i] = s.str_[i];

    is_english = s.is_english;
}

String& String::operator=(const String& s){
    if(this == &s)
        return *this;
    if(num_bytes_ != 0){
        delete[] str_;
        size_ = 0;
        num_bytes_ = 0;
    }
    if(s.num_bytes_ != 0){
        size_ = s.size_;
        num_bytes_ = s.num_bytes_;

        str_ = new char[num_bytes_]{};

        for(int i = 0; i < num_bytes_; ++i)
            str_[i] = s.str_[i];
    }
    is_english = s.is_english;
    return *this;
}

String::~String() { delete[] str_; }

int String::CountBytes(const char* str) const{
    int bytes{0};
	assert(str);
    while(*str != '\0'){
        ++bytes;
        ++str;
    }
    return ++bytes; // including the last '\0'
}

int String::StrLen(const char *str) const {
    int len{0};
    assert(str);
    while(*str != '\0'){
        ++len;
        IsThisByteAscii(*str) ? ++str : str += 2;
    }
    return len;
}

bool String::IsThisByteAscii(char c) const {
    return (c >= BOTTOM_ASCII && c <= TOP_ASCII);
}

std::istream& operator>>(std::istream& in, String& s){

    int flag{0};
    char c{'\0'};

    char static_array[SIZE_ARRAY_FOR_STR];
    for(int i = 0; i < SIZE_ARRAY_FOR_STR; ++i)
        static_array[i] = '\0';

    std::cout << "Enter the str : " << std::endl;

    //--------------------------------------------------------//
    int i{0};
    errno = 0;
    while(c != '\n'){
        scanf("%c", &c);
        assert(errno == 0);
        if(c == '\n' && flag == 0){
            flag = 1;
            continue;
        }
        static_array[i] = c;
        ++i;
    }
    //______________________________________________________//

    s = String(static_array);

    return in;
}

std::ostream& operator<<(std::ostream& out, const String& s){
    out << "Size of str_ is : " << s.size_ << std::endl;
    out << "Number of bytes is : " << s.num_bytes_ << std::endl;
    if(s.num_bytes_ == 0)
        out << "{}" << std::endl;
    else{
        out << "{" << s.str_ << "}" << std::endl;
    }
    return out;
}

char* String::GetStr() const noexcept {
    return str_;
}

int String::GetNumBytes() const noexcept {
    return num_bytes_;
}

int String::size() const noexcept { return size_; }

bool String::empty() const noexcept {
    return num_bytes_;
}

String String::ParseShortMathNotation() const {
	if(!size_)
		return String();

	if(size_ != num_bytes_ - 1)
		throw std::runtime_error("this string contain not ASCII symbols");

	/*Create new string for filling it with short representation of the initial expression*/
	char * shortrep = new char[num_bytes_];
	std::memset(shortrep, '\0', num_bytes_);
	char* cur = str_;
	char* copy = shortrep;
	assert(cur);

	while(*cur != '\0')
	{
		switch(*cur)
		{
		case 'e' :
		case 'x' : // the only available variable
		case '*' :
		case '/' :
		case '^' :
		case '+' :
		case '-' :
		case '(' :
		case ')' :
		case '0' :
		case '1' :
		case '2' :
		case '3' :
		case '4' :
		case '5' :
		case '6' :
		case '7' :
		case '8' :
		case '9' :
			/* this symbol can appear only in case of arguments to logarithm, so
			 * You should use it only in the right way!!!*/
		case ',' :
			*copy = *cur;
				++copy;
				break;
			case ' ' :
				break;
			case '.' : // only letter of number can preceed point otherwise we generate an exception
				if( cur != str_ && *(cur - 1) >= '0' && *(cur - 1) <= '9'){
					*copy = *cur;
					++copy;
					break;
				}
				else {
					std::cerr << "ERROR data in given string : " << str_ << std::endl;
					delete [] shortrep;
					throw std::runtime_error("Bad result with point in the given string");
				}

			/*This moment I've decided not to write a lot of checks at the cases of space between letters in functions
			 * name and different types of errors in it due to in that case the code will be so huge but not very cute*/
			case 'c' : // avaliable tokens are : [cos] and [ctg]
			{
				++cur;
				if(*cur == 'o') // cos
					*copy = 'c';
				else          // ctg
					*copy = 'g';
				++copy;
				++cur; // one more increment in the instruction after switch
				break;
			}
			case 's' :
				*copy = 's';
				++copy;
				cur += 2;
				break;

		case 't' :
			*copy = 't';
			++cur;
			++copy;
			break;

		case 'l' :
			*copy = 'l';
			++copy;
			cur += 2;
			break;
		case 'a' :
			cur += 3;
			if (*cur == 'c') // arccos
				*copy = 'a';
			else *copy = 'n'; //arcsin
			++copy;
			cur += 2;
			break;

			default :  //this branch for the case of RUBBISH letters in the given string
				std::cerr << "ERROR data in given string\n" << "The element is : " << *cur << std::endl;
				delete [] shortrep;
				throw std::runtime_error("Bad letter in given string");
		}
		++cur;
	}

	String str(shortrep);
	delete [] shortrep;
	return str;
}