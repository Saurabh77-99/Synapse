#pragma once

#include <chrono>
#include <cstdint>
using namespace std;

enum class Side { BUY, SELL };
enum class OrderType { MARKET, LIMIT };

struct Order {
    uint64_t order_id;
    Side side;
    OrderType type;
    double price;
    uint64_t quantity;
    chrono::steady_clock::time_point timestamp;

    Order(uint64_t id, Side s, OrderType t, double p, uint64_t q);
};
