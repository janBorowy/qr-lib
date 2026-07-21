#include "codewords_assembly.h"
#include "../qr.h"
#include "error_correction/ec_generation.h"
#include <algorithm>
#include <cassert>
#include <vector>

struct Block {
    std::vector<qr::Codeword> data;
    std::vector<qr::Codeword> error_correction;
    Block()
        : data(std::vector<qr::Codeword>()),
          error_correction(std::vector<qr::Codeword>()) {}
};

void group_and_append(std::vector<qr::Codeword>::const_iterator& it,
                      int blocks_num, int codewords_num,
                      std::vector<Block>& blocks) {
    for (int i = 0; i < blocks_num; i++) {
        Block b;
        for (int j = 0; j < codewords_num; j++) {
            b.data.push_back(*it++);
        }
        blocks.push_back(b);
    }
}

void fill_error_codewords(std::vector<Block>& blocks, int ec_codewords_num) {
    for (auto& b : blocks) {
        b.error_correction = generate_ec_codewords(b.data, ec_codewords_num);
    }
}

std::vector<qr::Codeword> interleave(const std::vector<Block>& blocks) {
    std::vector<qr::Codeword> result;
    std::vector<std::pair<std::vector<qr::Codeword>::const_iterator,
                          std::vector<qr::Codeword>::const_iterator>>
        data_iterators;

    std::vector<std::pair<std::vector<qr::Codeword>::const_iterator,
                          std::vector<qr::Codeword>::const_iterator>>
        ec_iterators;
    for (const auto& b : blocks) {
        data_iterators.push_back({b.data.begin(), b.data.end()});
        ec_iterators.push_back(
            {b.error_correction.begin(), b.error_correction.end()});
    }

    while (std::any_of(data_iterators.begin(), data_iterators.end(),
                       [](auto its) { return its.first != its.second; })) {
        for (auto& [it, end] : data_iterators) {
            if (it != end) {
                result.push_back(*it++);
            } }
    }

    while (std::any_of(ec_iterators.begin(), ec_iterators.end(),
                       [](auto its) { return its.first != its.second; })) {
        for (auto& [it, end] : ec_iterators) {
            if (it != end) {
                result.push_back(*it++);
            }
        }
    }

    assert(all_of(data_iterators.begin(), data_iterators.end(),
                  [](auto its) { return its.first == its.second; }));
    assert(all_of(ec_iterators.begin(), ec_iterators.end(),
                  [](auto its) { return its.first == its.second; }));

    return result;
}

std::vector<qr::Codeword>
assemble_data_codewords(const std::vector<qr::Codeword>& data_codewords,
                        int version, qr::ErrorCorrectionLevel ec_level) {
    auto it = data_codewords.begin();
    auto group_one_blocks =
        qr::VERSION_AND_EC_TO_BLOCKS_NUM_GROUP_1[version][ec_level];
    auto group_one_data =
        qr::VERSION_AND_EC_TO_DATA_NUM_GROUP_1[version][ec_level];
    std::vector<Block> blocks;
    group_and_append(it, group_one_blocks, group_one_data, blocks);

    auto group_two_blocks =
        qr::VERSION_AND_EC_TO_BLOCKS_NUM_GROUP_2[version][ec_level];
    auto group_two_data =
        qr::VERSION_AND_EC_TO_DATA_NUM_GROUP_2[version][ec_level];
    group_and_append(it, group_two_blocks, group_two_data, blocks);

    assert(it == data_codewords.end());
    auto ec_codewords_num =
        qr::VERSION_AND_EC_TO_EC_CODEWORDS[version][ec_level];
    fill_error_codewords(blocks, ec_codewords_num);

    return interleave(blocks);
}
