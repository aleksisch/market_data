//
// Created by aleksey on 28.10.24.
//

#pragma once

#include <ostream>

namespace dumper {

namespace detail {
static constexpr char sep = '\n';
}

template <typename T, typename... Args>
void csv_dump(std::ostream& out, T arg1, Args&&... args) {
  out << arg1;
  ((out << ',' << std::forward<Args>(args)), ...);
  out << detail::sep;
}

}  // namespace dumper