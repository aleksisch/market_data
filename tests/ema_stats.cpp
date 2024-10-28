//
// Created by aleksey on 28.10.24.
//
#include <fuzztest/fuzztest.h>
#include <gtest/gtest.h>

#include <rolling_stats/EMA.h>
#include <rolling_stats/rolling_buffer.h>

TEST(StatsSimple, ema) {}

void CheckEMACorrect(std::vector<double>& price, std::vector<uint64_t>& time) {
  const int TIME_DIFF = 5000;

  auto actual_size = std::min(price.size(), time.size());

  price.resize(actual_size);
  time.resize(actual_size);
  for (size_t i = 1; i < time.size(); i++) {
    time[i] += time[i - 1];
  }
  size_t l = 0;
  const auto buff_size = 100;
  rolling_stats::RollingBuffer buffer(buff_size, TIME_DIFF);

  auto ema = std::make_shared<rolling_stats::EMA>(TIME_DIFF);
  buffer.subscribe(ema);

  ASSERT_EQ(ema->get(), 0);
  double result = price.empty() ? 0. : price.at(0);
  for (size_t r = 0; r < time.size(); r++) {
    buffer.add(price[r], time[r]);
    value += ASSERT_EQ(result, ema.get());
  }
}

FUZZ_TEST(FuzzedStats, CheckEMACorrect)
    .WithDomains(
        /* price */ fuzztest::VectorOf(fuzztest::Positive<double>()),
        /* time delta */ fuzztest::VectorOf(
            fuzztest::InRange<uint64_t>(0, 10000)));
