//
// Created by aleksey on 28.10.24.
//

#pragma once
#include <rolling_stats/rolling_buffer.h>
#include <optional>

namespace rolling_stats {

/**
     * Basic formula is: Y_n = x * alpha + Y_{n-1}*(1 - alpha) <=> Y += alpha * (X - Y)
     * However, we have case with irregular updates from exchange (on each match)
     * So we should adjust alpha in a `correct` way.
     * It can be proved, that `correct` constant in irregular case is
     * alpha = e^{-dt/T}, where dt is time elapsed from last update, and T is time window
     */
class EMA : public IRollingStat {
  friend class RollingBuffer;

 public:
  EMA(TimeT window) : window_(window) {}

  DataT get() const override { return value_; }

 protected:
  /**
         * @param add new event
         * @param prev removed events
         */
  void update(Update add, const std::vector<Update>& prev) override {
    value_ += getMultiplier(add.time - last_) * (add.data - value_);
    last_ = add.time;
  }

  DataT getMultiplier(TimeT delta) const {
    return std::exp(-delta / static_cast<double>(last_));
  }

 private:
  DataT value_;
  TimeT last_;
  const DataT multiplier_;
  const TimeT window_;
};
}  // namespace rolling_stats