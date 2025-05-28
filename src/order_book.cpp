#include "order_book.hpp"
#include <algorithm>
#include <iostream>
#include <iomanip>

void OrderBook::add_order(const Order& order) {
    std::cout << "Added " << (order.type == OrderType::MARKET ? "market" : "limit")
              << " order: ID=" << order.order_id
              << " side=" << (order.side == Side::BUY ? "BUY" : "SELL")
              << " price=" << order.price
              << " qty=" << order.quantity << "\n";

    uint64_t remaining = order.quantity;

    // Get reference to the opposite book
    std::map<double, std::deque<Order>>* opposing_book = nullptr;
    if (order.side == Side::BUY) {
        opposing_book = &asks;
    } else {
        opposing_book = reinterpret_cast<std::map<double, std::deque<Order>>*>(&bids);  // safe as long as logic is separate
    }

    auto match = [&](auto it) {
        auto& queue = it->second;
        while (!queue.empty() && remaining > 0) {
            Order& match_order = queue.front();
            uint64_t trade_qty = std::min(remaining, match_order.quantity);

            std::cout << ">> Trade executed: "
                      << "BUY " << (order.side == Side::BUY ? order.order_id : match_order.order_id)
                      << " | SELL " << (order.side == Side::SELL ? order.order_id : match_order.order_id)
                      << " | price " << match_order.price
                      << " | qty " << trade_qty << "\n";

            match_order.quantity -= trade_qty;
            remaining -= trade_qty;

            if (match_order.quantity == 0)
                queue.pop_front();
        }

        if (queue.empty()) {
            opposing_book->erase(it);
        }
    };

    // MARKET ORDER
    if (order.type == OrderType::MARKET) {
        while (!opposing_book->empty() && remaining > 0) {
            match(opposing_book->begin());
        }
        return;
    }

    // LIMIT ORDER matching
    if (order.side == Side::BUY) {
        auto it = asks.begin();
        while (it != asks.end() && it->first <= order.price && remaining > 0) {
            match(it++);
        }
    } else {
        auto it = bids.begin();
        while (it != bids.end() && it->first >= order.price && remaining > 0) {
            match(it++);
        }
    }

    // Add remaining to same side book
    if (remaining > 0) {
        Order leftover = order;
        leftover.quantity = remaining;

        if (order.side == Side::BUY) {
            bids[order.price].push_back(leftover);
        } else {
            asks[order.price].push_back(leftover);
        }
    }
}

void OrderBook::print_book() const {
    std::cout << "\n===== ORDER BOOK =====\n";

    // Show asks (sells) - sorted ascending
    std::cout << "\n      --- Asks (Sell) ---\n";
    std::cout << std::left << std::setw(10) << "Price" 
              << std::setw(10) << "Quantity" << "\n";
    std::cout << "--------------------------\n";
    for (const auto& [price, orders] : asks) {
        uint64_t qty = 0;
        for (const auto& o : orders) qty += o.quantity;
        std::cout << std::left << std::setw(10) << price 
                  << std::setw(10) << qty << "\n";
    }
    if (asks.empty()) std::cout << "(Empty)\n";

    // Show bids (buys) - sorted descending
    std::cout << "\n      --- Bids (Buy) ---\n";
    std::cout << std::left << std::setw(10) << "Price" 
              << std::setw(10) << "Quantity" << "\n";
    std::cout << "--------------------------\n";
    for (const auto& [price, orders] : bids) {
        uint64_t qty = 0;
        for (const auto& o : orders) qty += o.quantity;
        std::cout << std::left << std::setw(10) << price 
                  << std::setw(10) << qty << "\n";
    }
    if (bids.empty()) std::cout << "(Empty)\n";

    std::cout << "=========================\n";
}


