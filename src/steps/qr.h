#pragma once

#include <array>
#include <unordered_map>
#include <vector>

namespace qr {
enum ErrorCorrectionLevel { L, M, Q, H };
enum EncodingMode { NUMERIC, ALPHANUMERIC, BYTE };
constexpr int VERSIONS_AVAILBALE = 10;

// TODO: fill this, L and M only for now and up to 10 verions
const std::unordered_map<ErrorCorrectionLevel,
                         std::unordered_map<EncodingMode, std::vector<int>>>
    VERSION_CAPACITIES = {
        {ErrorCorrectionLevel::L,
         {{EncodingMode::NUMERIC,
           std::vector<int>{41, 77, 127, 187, 255, 322, 370, 461, 552, 652}},
          {EncodingMode::ALPHANUMERIC,
           std::vector<int>{25, 47, 77, 114, 154, 195, 224, 279, 335, 395}},
          {EncodingMode::BYTE,
           std::vector<int>{17, 32, 53, 78, 106, 134, 154, 192, 230, 271}}}},
        {ErrorCorrectionLevel::M,
         {{EncodingMode::NUMERIC,
           std::vector<int>{34, 63, 101, 149, 202, 255, 293, 365, 432, 514}},
          {EncodingMode::ALPHANUMERIC,
           std::vector<int>{20, 38, 61, 90, 122, 154, 178, 221, 262, 311}},
          {EncodingMode::BYTE,
           std::vector<int>{14, 26, 42, 62, 84, 106, 122, 152, 180, 213}}}}};

const std::unordered_map<char, int> ALPHANUMERIC_TO_VAL = {
    {'0', 0},  {'1', 1},  {'2', 2},  {'3', 3},  {'4', 4},  {'5', 5},  {'6', 6},
    {'7', 7},  {'8', 8},  {'9', 9},  {'A', 10}, {'B', 11}, {'C', 12}, {'D', 13},
    {'E', 14}, {'F', 15}, {'G', 16}, {'H', 17}, {'I', 18}, {'J', 19}, {'K', 20},
    {'L', 21}, {'M', 22}, {'N', 23}, {'O', 24}, {'P', 25}, {'Q', 26}, {'R', 27},
    {'S', 28}, {'T', 29}, {'U', 30}, {'V', 31}, {'W', 32}, {'X', 33}, {'Y', 34},
    {'Z', 35}, {' ', 36}, {'$', 37}, {'%', 38}, {'*', 39}, {'+', 40}, {'-', 41},
    {'.', 42}, {'/', 43}, {':', 44}};

constexpr std::array<int, VERSIONS_AVAILBALE + 1> VERSION_TO_TOTAL_CODEWORDS = {
    0, 26, 44, 70, 100, 134, 172, 196, 242, 292, 346};

constexpr std::array<std::array<int, 4>, VERSIONS_AVAILBALE + 1>
    VERSION_AND_EC_TO_DATA_CODEWORDS = {{{0, 0, 0, 0},
                                         {19, 16, 13, 9},
                                         {34, 28, 22, 16},
                                         {55, 44, 34, 26},
                                         {80, 64, 48, 36},
                                         {108, 86, 62, 46},
                                         {136, 108, 76, 60},
                                         {156, 124, 88, 66},
                                         {194, 154, 110, 86},
                                         {232, 102, 132, 100},
                                         {274, 216, 154, 122}}};

} // namespace qr
