#ifndef ORDER_BOOK_HPP
#define ORDER_BOOK_HPP

#include "order.hpp"
#include <map>
#include <deque>
#include <unordered_map>
#include <iostream>

class OrderBook {
public:
    void add_order(const Order& order);
    void print_book() const;

private:
    std::map<double, std::deque<Order>, std::greater<double>> bids;
    std::map<double, std::deque<Order>> asks;
    std::unordered_map<uint64_t, std::pair<Side, std::deque<Order>::iterator>> order_map;
};

#endif
