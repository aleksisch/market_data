//
// Created by aleksey on 28.10.24.
//

#pragma once
#include <market_data/utility.h>
#include <optional>

namespace rolling_stats {

using DataT = market_data::PriceT;
using TimeT = market_data::TimeT;
/**
     * Basic formula is: Y_n = x * alpha + Y_{n-1}*(1 - alpha) <=> Y += alpha * (X - Y)
     * However, we have case with irregular updates from exchange (on each match)
     * So we should adjust alpha in a `correct` way.
     * It can be proved, that `correct` constant in irregular case is
     * alpha = 1 - e^{-dt/T}, where dt is time elapsed from last update, and T is time window
     */
class EMA {
 public:
  EMA(TimeT window) : window_(window) {}

  DataT get() const { return value_; }

  /**
         * @param add new event
         * @param prev removed events
         */
  void add(DataT data, TimeT time) {
    if (last_ == 0) {
      value_ += data;
    } else {
      value_ += getMultiplier(time - last_) * (data - value_);
    }
    last_ = time;
  }

  DataT getMultiplier(TimeT delta) const {
    return 1 - std::exp(-(delta / static_cast<double>(window_)));
  }

 private:
  DataT value_{};
  TimeT last_{};
  const TimeT window_;
};
}  // namespace rolling_stats