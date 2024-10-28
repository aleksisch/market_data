//
// Created by aleksey on 28.10.24.
//

#pragma once
#include <daw/json/daw_json_link.h>
#include <string>
#include <vector>

struct CoinBaseSubscription {
    std::string type;
    std::vector<std::string> product_ids;
    std::vector<std::string> streams;
};

namespace daw::json {
template <>
struct json_data_contract<CoinBaseSubscription> {
    using type = json_member_list<json_string<"type">,
                                  json_array<"product_ids", std::string>,
                                  json_array<"channels", std::string>>;

    static constexpr auto to_json_data(CoinBaseSubscription const& p) {
        return std::forward_as_tuple(p.type, p.product_ids, p.streams);
    }
};
}  // namespace daw::json