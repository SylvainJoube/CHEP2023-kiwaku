#pragma once

#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>

inline void log(const std::string& s)
{
  std::cout << s << "\n";
}


struct printer_t
{
  inline static void head(const std::string& str)
  {
    std::string hd = "===================================================";
    std::string hp = "-";
    
    std::string header = hp + " " + str + " " + hp;
    uint rpad_len = (hd.size() - header.size()) / 2;
    std::string rpad(rpad_len, '=');
    uint lpad_len = hd.size() - (rpad_len + header.length());
    std::string lpad(lpad_len, '=');

    std::cout << hd << "\n";
    std::cout << hd << "\n";
    std::cout << rpad << header << lpad << "\n";
    std::cout << hd << "\n";
    std::cout << hd << "\n\n";
  }

  inline static void err(const std::string& str) {
    error(str);
  }
  inline static void warn(const std::string& str) {
    warning(str);
  }


  inline static void line(const std::string& str) {
    std::cout << str << "\n";
  }
  inline static void str(const std::string& str) {
    std::cout << str;
  }
  inline static void error(const std::string& str) {
    std::cout << "!!!!!!!       ERROR   : " << str << "\n";
  }
  inline static void ok(const std::string& str) {
    std::cout << "OK      : " << str << "\n";
  }
  inline static void warning(const std::string& str) {
    std::cout << "!!!!!!!       WARNING : " << str << "\n";
  }

  inline static std::string pad_right(std::string str, const std::size_t len, const char paddingChar = ' ') {
    if (len <= str.size()) return str;
    str.insert(0, len - str.size(), paddingChar);
    return str;
  }

  inline static std::string pad_right(int value, const std::size_t len, const char paddingChar = ' ') {
    std::string str = std::to_string(value);
    return pad_right(str, len, paddingChar);
  }
  
};

using print = printer_t;

inline std::string pad_right(std::string str, const std::size_t len, const char paddingChar = ' ')
{
  return print::pad_right(str, len, paddingChar);
}

inline std::string pad_right(int value, const std::size_t len, const char paddingChar = ' ')
{
  return print::pad_right(value, len, paddingChar);
}