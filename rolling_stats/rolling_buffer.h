//
// Created by aleksey on 28.10.24.
//

#pragma once

#include <optional>
#include <vector>

#include <market_data/utility.h>

namespace rolling_stats {

using DataT = market_data::PriceT;
using TimeT = uint64_t;

struct Update {
  DataT data;
  TimeT time;
};

// If we care about performance we can use static inheritance + concepts + variadic templates
// although code will be awful
class IRollingStat {
 public:
  virtual void update(Update add, const std::vector<Update>& remove) = 0;
  virtual DataT get() const = 0;
};

class RollingBuffer {
 public:
  explicit RollingBuffer(size_t size, TimeT window)
      : max_size_(size), window_(window) {
    data_.resize(size);
  }

  void subscribe(std::shared_ptr<IRollingStat> stat) {
    subscribers_.emplace_back(stat);
  }

  void add(DataT value, uint64_t timestamp) {
    std::vector<Update> prev = removeStale(timestamp - window_);
    auto [slot, val] = nextSlot();
    size_ += 1;
    if (val) {
      prev.emplace_back(val.value());
    }

    auto upd = Update{.data = value, .time = timestamp};
    data_.at(slot) = upd;
    for (auto sub : subscribers_) {
      sub->update(upd, prev);
    }
  }

 private:
  /**
         * @return next available slot and value, if there are too many events in time window
         */
  std::pair<size_t, std::optional<Update>> nextSlot() {
    auto res = top_index_;
    top_index_ = nextId(top_index_);
    if (res == last_index_) {
      nextId(last_index_);
      return std::make_pair(res, data_.at(res));
    }
    return std::make_pair(res, std::nullopt);
  }

  /**
         * Removes stale slot
         * @param time slots before this time will be cleared
         * @return all removed slots
         */
  std::vector<Update> removeStale(TimeT time) {
    std::vector<Update> res;
    while (last_index_ != top_index_ && data_.at(last_index_).time < time) {
      res.emplace_back(data_.at(last_index_));
      last_index_ = nextId(last_index_);
    }
    return res;
  }

  size_t nextId(size_t id) { return (id + 1) % max_size_; }

 private:
  TimeT last_timestamp_{};
  const size_t max_size_;  // always equal to data_.size()
  uint64_t size_{0};
  std::vector<Update> data_;
  size_t top_index_;   // not including
  size_t last_index_;  // including
  TimeT window_;
  std::vector<std::shared_ptr<IRollingStat>> subscribers_;
};

}  // namespace rolling_stats