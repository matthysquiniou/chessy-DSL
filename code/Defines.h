#pragma once

#ifndef DEFINES
#define DEFINES

// C++ built-in
#include <iostream>
#include <stdint.h>
#include <string>
#include <string_view>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <utility>
#include <cctype>
#include <source_location>
#include <filesystem>
#include <limits>

/// Using
using str_view      = std::string_view;
using str           = std::string;
using u8            = std::uint8_t;
using u16           = std::uint16_t;
using u32           = std::uint32_t;
using u64           = std::uint64_t;
using i8            = std::int8_t;
using i16           = std::int16_t;
using i32           = std::int32_t;
using i64           = std::int64_t;
using size          = size_t;
template<typename T>
using vector        = std::vector<T>;
template<typename K, typename V>
using un_map        = std::unordered_map<K, V>; 
template<typename T>
using un_set        = std::unordered_set<T>;
template<typename T, int N>
using array         = std::array<T, N>;

const std::filesystem::path data = std::filesystem::path(PROJECT_ROOT);

#endif