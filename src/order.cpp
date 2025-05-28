#include "order.hpp"

Order::Order(uint64_t id, Side s, OrderType t, double p, uint64_t q)
    : order_id(id), side(s), type(t), price(p), quantity(q),
      timestamp(std::chrono::steady_clock::now()) {}
