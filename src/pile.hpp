/*!
 * @file pile.hpp
 *
 * @brief Pile class header file
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <utility>

namespace ram {
    struct Overlap;
}

namespace raven {

class Pile;
std::unique_ptr<Pile> createPile(std::uint32_t id, std::uint32_t size);

class Pile {
public:
    ~Pile() = default;

    std::uint32_t begin() const {
        return begin_;
    }

    std::uint32_t end() const {
        return end_;
    }

    /*!
     * @brief Sets values of data_ outside the interval [begin, end> to zeroes,
     * and updates begin_ and end_ accordingly. If the region is shorter than
     * 1260 bp, the object is invalidated.
     */
    void set_valid_region(std::uint32_t begin, std::uint32_t end);

    /*!
     * @brief Finds region in data_ with values greater or equal to coverage and
     * sets the valid region.
     */
    void find_valid_region(std::uint32_t coverage);

    /*!
     * @brief Fills data_ inside interval [begin_, end_> with zeroes.
     */
    void clear_valid_region();

    /*!
     * @brief Fills data_ outside interval [begin_, end_> with zeroes.
     */
    void clear_invalid_region();

    std::uint32_t median() const {
        return median_;
    }

    /*!
     * @brief Finds median coverage value of data_ in interval [begin_, end_>.
     */
    void find_median();

    /*!
     * @brief Adds coverage to data_ from a set of overlaps.
     */
    void add_layers(std::vector<ram::Overlap>::const_iterator begin,
        std::vector<ram::Overlap>::const_iterator end);

    void set_invalid() {
        state_ |= 1U;
    }

    bool is_invalid() const {
        return state_ & 1U;
    }

    void set_contained() {
        state_ |= (1U << 1);
    }

    bool is_contained() {
        return state_ & (1U << 1);
    }

    bool is_chimeric() {
        return state_ & (1U << 2);
    }

    bool has_chimeric_region() const {
        return chimeric_regions_.size();
    }

    /*!
     * @brief Locates chimeric rifts (coverage drops) in data_.
     */
    void find_chimeric_regions();

    /*!
     * @brief Truncates data_ to longest non-chimeric region given the component
     * median.
     */
    void resolve_chimeric_regions(std::uint32_t median);

    std::string to_json() const;

    friend std::unique_ptr<Pile> createPile(std::uint32_t id, std::uint32_t size);
private:
    Pile(std::uint32_t id, std::uint32_t size);
    Pile(const Pile&) = delete;
    const Pile& operator=(const Pile&) = delete;

    std::vector<std::pair<std::uint32_t, std::uint32_t>> find_slopes(double q);

    std::uint32_t id_;
    std::vector<std::uint32_t> data_;
    std::uint32_t begin_;
    std::uint32_t end_;
    std::uint32_t median_;
    std::uint32_t state_;
    std::vector<std::pair<std::uint32_t, std::uint32_t>> chimeric_regions_;
};

}
