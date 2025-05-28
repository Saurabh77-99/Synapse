#ifndef ORDER_BOOK_HPP
#define ORDER_BOOK_HPP

#include "order.hpp"
#include <map>
#include <deque>
#include <unordered_map>
#include <iostream>
using namespace std;

class OrderBook {
public:
    void add_order(const Order& order);
    void print_book() const;
    bool cancel_order(uint64_t order_id);
    bool modify_order(uint64_t order_id, double new_price, uint64_t new_quantity);

private:
    map<double, deque<Order>, greater<double>> bids;
    map<double, deque<Order>> asks;
    unordered_map<uint64_t, tuple<Side, deque<Order>::iterator>> order_map;
};

#endif
