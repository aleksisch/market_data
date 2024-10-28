//
// Created by aleksey on 28.10.24.
//
#include <fuzztest/fuzztest.h>
#include <gtest/gtest.h>

#include <rolling_stats/EMA.h>

void CheckEMACorrect(std::vector<double> price, std::vector<uint64_t> time) {
  const int TIME_DIFF = 5000;

  auto actual_size = std::min(price.size(), time.size());

  price.resize(actual_size);
  time.resize(actual_size);

  for (size_t i = 1; i < time.size(); i++) {
    time[i] += time[i - 1];
  }

  auto ema = rolling_stats::EMA(TIME_DIFF);

  ASSERT_FLOAT_EQ(ema.get(), 0.);
  double result = 0;
  for (size_t r = 0; r < time.size(); r++) {
    ema.add(price[r], time[r]);
    if (r == 0) {
      result += price[r];
    } else {
      auto k =
          (1 -
           std::exp(-(static_cast<double>(time[r] - time[r - 1]) / TIME_DIFF)));
      result += k * (price[r] - result);
    }
    ASSERT_LT(std::abs(ema.get() - result),
              0.005);  // ASSERT_DOUBLE_EQ isn't enough due to high input range
  }
}

FUZZ_TEST(FuzzedStats, CheckEMACorrect)
    .WithDomains(
        /* price */ fuzztest::VectorOf(fuzztest::InRange<double>(0., 10000.)),
        /* time delta */ fuzztest::VectorOf(
            fuzztest::InRange<uint64_t>(1000, 10000)));
