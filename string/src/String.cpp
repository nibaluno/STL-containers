#include "/media/kate/diskE/QT/LR5/StringLib/include/String.h"

const char* String::find_char(const char* s, char c) {
    while (*s != '\0' && *s != c) {
        ++s;
    }
    return *s == c ? s : nullptr;
}

// it 
String::Iterator::Iterator(String& str, size_t index) : str_(str), index_(index) {}

char& String::Iterator::operator*() {
    return str_[index_];
}

String::Iterator& String::Iterator::operator++() {
    ++index_;
    return *this;
}

bool String::Iterator::operator!=(const Iterator& other) const {
    return index_ != other.index_;
}

// constructors
String::String() : short_data{0}, size_(0), is_short_(true) {}

String::String(const char* str) {
    if (!str) throw std::invalid_argument("nullptr");
    
    size_ = strlen(str);
    is_short_ = size_ <= SSO_CAPACITY;
    
    if (is_short_) {
        strcpy(short_data, str);
    } else {
        new (&long_data) std::unique_ptr<char[]>(new char[size_ + 1]);
        strcpy(long_data.get(), str);
    }
}

String::String(const String& other) : size_(other.size_), is_short_(other.is_short_) {
    if (is_short_) {
        strcpy(short_data, other.short_data);
    } else {
        new (&long_data) std::unique_ptr<char[]>(new char[size_ + 1]);
        strcpy(long_data.get(), other.long_data.get());
    }
}

String::String(String&& other) noexcept 
    : size_(other.size_), is_short_(other.is_short_) 
{
    if (is_short_) {
        strcpy(short_data, other.short_data);
    } else {
        new (&long_data) std::unique_ptr<char[]>(std::move(other.long_data));
    }
    
    other.size_ = 0;
    other.is_short_ = true;
    other.short_data[0] = '\0';
}

String::~String() {
    if (!is_short_) {
        long_data.~unique_ptr();
    }
}

// assignment operators
String& String::operator=(const String& other) {
    if (this != &other) {
        if (!is_short_) {
            long_data.~unique_ptr();
        }
        
        size_ = other.size_;
        is_short_ = other.is_short_;
        
        if (is_short_) {
            strcpy(short_data, other.short_data);
        } else {
            new (&long_data) std::unique_ptr<char[]>(new char[size_ + 1]);
            strcpy(long_data.get(), other.long_data.get());
        }
    }
    return *this;
}

String& String::operator=(String&& other) noexcept {
    if (this != &other) {
        if (!is_short_) {
            long_data.~unique_ptr();
        }
        
        size_ = other.size_;
        is_short_ = other.is_short_;
        
        if (is_short_) {
            strcpy(short_data, other.short_data);
        } else {
            new (&long_data) std::unique_ptr<char[]>(std::move(other.long_data));
        }
        
        other.size_ = 0;
        other.is_short_ = true;
        other.short_data[0] = '\0';
    }
    return *this;
}

// it
String::Iterator String::begin() { return Iterator(*this, 0); }
String::Iterator String::end() { return Iterator(*this, size_); }

// access methods
const char* String::c_str() const { 
    return is_short_ ? short_data : long_data.get(); 
}

size_t String::size() const { return size_; }
bool String::empty() const { return size_ == 0; }

// el access
char& String::operator[](size_t index) {
    if (index >= size_) throw std::out_of_range("String index out of range");
    return is_short_ ? short_data[index] : long_data.get()[index];
}

const char& String::operator[](size_t index) const {
    if (index >= size_) throw std::out_of_range("String index out of range");
    return is_short_ ? short_data[index] : long_data.get()[index];
}

// operations
void String::clear() {
    if (!is_short_) {
        long_data.~unique_ptr();
    }
    size_ = 0;
    is_short_ = true;
    short_data[0] = '\0';
}

String String::operator+(const String& other)  {
    String result;
    result.size_ = size_ + other.size_;
    result.is_short_ = (result.size_ <= SSO_CAPACITY);
    
    if (result.is_short_) {
        strcpy(result.short_data, c_str());
        strcat(result.short_data, other.c_str());
    } else {
        new (&result.long_data) std::unique_ptr<char[]>(new char[result.size_ + 1]);
        strcpy(result.long_data.get(), c_str());
        strcat(result.long_data.get(), other.c_str());
    }
    
    return result;
}

// functions 
void* String::memcpy(void* dest, const void* src, size_t n) {
    char* d = static_cast<char*>(dest);
    const char* s = static_cast<const char*>(src);
    for (size_t i = 0; i < n; ++i) {
        d[i] = s[i];
    }
    return dest;
}

void* String::memmove(void* dest, const void* src, size_t n) {
    char* d = static_cast<char*>(dest);
    const char* s = static_cast<const char*>(src);

    if (d < s) {
        for (size_t i = 0; i < n; ++i) {
            d[i] = s[i];
        }
    } else {
        for (size_t i = n; i > 0; --i) {
            d[i-1] = s[i-1];
        }
    }
    return dest;
}

char* String::strcpy(char* dest, const char* src) {
    char* d = dest;
    while ((*d++ = *src++) != '\0');
    return dest;
}

char* String::strncpy(char* dest, const char* src, size_t n) {
    char* d = dest;
    size_t i = 0;
    
    for (; i < n && src[i] != '\0'; ++i) {
        d[i] = src[i];
    }
    
    for (; i < n; ++i) {
        d[i] = '\0';
    }
    
    return dest;
}

char* String::strcat(char* dest, const char* src) {
    char* d = dest;
    while (*d != '\0') d++;
    while ((*d++ = *src++) != '\0');
    return dest;
}

char* String::strncat(char* dest, const char* src, size_t n) {
    char* d = dest;
    while (*d != '\0') d++;
    
    size_t i = 0;
    while (i < n && *src != '\0') {
        *d++ = *src++;
        i++;
    }
    
    *d = '\0';
    return dest;
}

int String::memcmp(const void* s1, const void* s2, size_t n) {
    const unsigned char* p1 = static_cast<const unsigned char*>(s1);
    const unsigned char* p2 = static_cast<const unsigned char*>(s2);

    for (size_t i = 0; i < n; ++i) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }
    return 0;
}

int String::strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        ++s1;
        ++s2;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int String::strcoll(const char* s1, const char* s2) {
    return strcmp(s1, s2);
}

int String::strncmp(const char* s1, const char* s2, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if (s1[i] != s2[i]) {
            return static_cast<unsigned char>(s1[i]) - static_cast<unsigned char>(s2[i]);
        }
        if (s1[i] == '\0') {
            return 0;
        }
    }
    return 0;
}

size_t String::strxfrm(char* dest, const char* src, size_t n) {
    size_t len = strlen(src);
    
    if (dest && n > 0) {
        if (len + 1 <= n) {
            strcpy(dest, src);
        } else {
            strncpy(dest, src, n - 1);
            dest[n - 1] = '\0';
        }
    }
    
    return len;
}

void* String::memset(void* s, int c, size_t n) {
    unsigned char* p = static_cast<unsigned char*>(s);
    for (size_t i = 0; i < n; ++i) {
        p[i] = static_cast<unsigned char>(c);
    }
    return s;
}

char* String::strerror(int errnum) {
    static char buffer[256];
    
    const char* msg = nullptr;
    switch (errnum) {
        case 0:  msg = "No error"; break;
        case 1:  msg = "Operation not permitted"; break;
        case 2:  msg = "No such file or directory"; break;
        case 3:  msg = "No such process"; break;
        case 4:  msg = "Interrupted system call"; break;
        case 5:  msg = "I/O error"; break;
        default:
            char* p = buffer;
            const char* unknown = "Unknown error: ";
            while (*unknown) *p++ = *unknown++;
            
            int num = errnum;
            if (num < 0) {
                *p++ = '-';
                num = -num;
            }
            
            char temp[20];
            char* t = temp;
            do {
                *t++ = '0' + num % 10;
                num /= 10;
            } while (num > 0);
            
            while (t > temp) {
                *p++ = *--t;
            }
            *p = '\0';
            return buffer;
    }
    
    strncpy(buffer, msg, sizeof(buffer));
    buffer[sizeof(buffer)-1] = '\0';
    return buffer;
}

size_t String::strlen(const char* s) {
    size_t len = 0;
    while (s[len] != '\0') len++;
    return len;
}


char* String::strtok(char* str, const char* delim) {
    static char* last = nullptr;
    
    if (!str && !(str = last)) return nullptr;
    
   
    while (*str) {
        const char* d = delim;
        while (*d && *str != *d) ++d;
        if (!*d) break; 
        ++str;
    }
    
    if (!*str) {
        last = nullptr;
        return nullptr;
    }
    
    char* token_start = str;
    
 
    while (*str) {
        const char* d = delim;
        while (*d && *str != *d) ++d;
        if (*d) break; 
        ++str;
    }
    
    if (*str) {
        *str = '\0';
        last = str + 1;
    } else {
        last = nullptr;
    }
    
    return token_start;
}