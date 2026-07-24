#pragma once

#include <algorithm>
#include <cstdio>
#include <memory>
#include <stdexcept>

class String {
    static constexpr size_t SSO_CAPACITY = 15; 
    
    union {
        std::unique_ptr<char[]> long_data;
        char short_data[SSO_CAPACITY + 1];
    };
    
    size_t size_;
    bool is_short_;

    static const char* find_char(const char* s, char c);

public:
    class Iterator {
        String& str_;
        size_t index_;
        
    public:
        Iterator(String& str, size_t index);
        char& operator*();
        Iterator& operator++();
        bool operator!=(const Iterator& other) const;
    };

    // constructors
    String();
    String(const char* str);
    String(const String& other);
    String(String&& other) noexcept;
    ~String();

    // assignment operators
    String& operator=(const String& other);
    String& operator=(String&& other) noexcept;

    // it
    Iterator begin();
    Iterator end();

    // access methods
    const char* c_str() const;
    size_t size() const;
    bool empty() const;

    // el access
    char& operator[](size_t index);
    const char& operator[](size_t index) const;

 
    void clear();
    String operator+(const String& other) ;

    // functions 
    static void* memcpy(void* dest, const void* src, size_t n);
    static void* memmove(void* dest, const void* src, size_t n);
    static char* strcpy(char* dest, const char* src);
    static char* strncpy(char* dest, const char* src, size_t n);
    static char* strcat(char* dest, const char* src);
    static char* strncat(char* dest, const char* src, size_t n);
    static int memcmp(const void* s1, const void* s2, size_t n);
    static int strcmp(const char* s1, const char* s2);
    static int strcoll(const char* s1, const char* s2);
    static int strncmp(const char* s1, const char* s2, size_t n);
    static size_t strxfrm(char* dest, const char* src, size_t n);
    static void* memset(void* s, int c, size_t n);
    static char* strerror(int errnum);
    static size_t strlen(const char* s);
    static char* strtok(char* str, const char* delim);
};