#include "order_book.hpp"
#include <algorithm>
#include <iostream>
#include <iomanip>
using namespace std;

void OrderBook::add_order(const Order &order) {
    cout << "Added " << (order.type == OrderType::MARKET ? "market" : "limit")
         << " order: ID=" << order.order_id
         << " side=" << (order.side == Side::BUY ? "BUY" : "SELL")
         << " price=" << order.price
         << " qty=" << order.quantity << "\n";

    uint64_t remaining = order.quantity;

    if (order.type == OrderType::MARKET) {
        // Market order: match fully on opposite side
        if (order.side == Side::BUY) {
            // Match against asks (ascending order)
            while (!asks.empty() && remaining > 0) {
                auto it = asks.begin();
                auto &queue = it->second;
                while (!queue.empty() && remaining > 0) {
                    Order &match_order = queue.front();
                    uint64_t trade_qty = std::min(remaining, match_order.quantity);

                    cout << ">> Trade executed: "
                         << "BUY " << order.order_id
                         << " | SELL " << match_order.order_id
                         << " | price " << match_order.price
                         << " | qty " << trade_qty << "\n";

                    match_order.quantity -= trade_qty;
                    remaining -= trade_qty;

                    if (match_order.quantity == 0)
                        queue.pop_front();
                }
                if (queue.empty())
                    asks.erase(it);
            }
        } else {
            // Match against bids (descending order)
            while (!bids.empty() && remaining > 0) {
                auto it = bids.begin();
                auto &queue = it->second;
                while (!queue.empty() && remaining > 0) {
                    Order &match_order = queue.front();
                    uint64_t trade_qty = std::min(remaining, match_order.quantity);

                    cout << ">> Trade executed: "
                         << "BUY " << match_order.order_id
                         << " | SELL " << order.order_id
                         << " | price " << match_order.price
                         << " | qty " << trade_qty << "\n";

                    match_order.quantity -= trade_qty;
                    remaining -= trade_qty;

                    if (match_order.quantity == 0)
                        queue.pop_front();
                }
                if (queue.empty())
                    bids.erase(it);
            }
        }
        return;
    }

    // LIMIT ORDER matching
    if (order.side == Side::BUY) {
        // Match against asks where price <= order.price
        auto it = asks.begin();
        while (it != asks.end() && it->first <= order.price && remaining > 0) {
            auto &queue = it->second;
            while (!queue.empty() && remaining > 0) {
                Order &match_order = queue.front();
                uint64_t trade_qty = std::min(remaining, match_order.quantity);

                cout << ">> Trade executed: "
                     << "BUY " << order.order_id
                     << " | SELL " << match_order.order_id
                     << " | price " << match_order.price
                     << " | qty " << trade_qty << "\n";

                match_order.quantity -= trade_qty;
                remaining -= trade_qty;

                if (match_order.quantity == 0)
                    queue.pop_front();
            }
            if (queue.empty()) {
                it = asks.erase(it);
            } else {
                ++it;
            }
        }
    } else {
        // SELL limit order: match bids where price >= order.price
        auto it = bids.begin();
        while (it != bids.end() && it->first >= order.price && remaining > 0) {
            auto &queue = it->second;
            while (!queue.empty() && remaining > 0) {
                Order &match_order = queue.front();
                uint64_t trade_qty = std::min(remaining, match_order.quantity);

                cout << ">> Trade executed: "
                     << "BUY " << match_order.order_id
                     << " | SELL " << order.order_id
                     << " | price " << match_order.price
                     << " | qty " << trade_qty << "\n";

                match_order.quantity -= trade_qty;
                remaining -= trade_qty;

                if (match_order.quantity == 0)
                    queue.pop_front();
            }
            if (queue.empty()) {
                it = bids.erase(it);
            } else {
                ++it;
            }
        }
    }

    // Add remaining to own book
    if (remaining > 0) {
        Order remaining_order = order;
        remaining_order.quantity = remaining;

        if (order.side == Side::BUY) {
            auto &queue = bids[remaining_order.price];
            queue.push_back(remaining_order);
            order_map[remaining_order.order_id] = std::make_tuple(Side::BUY, std::prev(queue.end()));
        } else {
            auto &queue = asks[remaining_order.price];
            queue.push_back(remaining_order);
            order_map[remaining_order.order_id] = std::make_tuple(Side::SELL, std::prev(queue.end()));
        }
    }
}


bool OrderBook::cancel_order(uint64_t order_id) {
    auto it = order_map.find(order_id);
    if (it == order_map.end()) return false;

    auto [side, deque_it] = it->second;

    // Declare reference to book map before if-else
    std::map<double, std::deque<Order>, std::greater<double>>& book_bids = bids;
    std::map<double, std::deque<Order>>& book_asks = asks;

    if (side == Side::BUY) {
        // Reference to bids
        auto& book = bids;
        double price = deque_it->price;
        auto price_level_it = book.find(price);
        if (price_level_it == book.end()) return false;

        price_level_it->second.erase(deque_it);
        if (price_level_it->second.empty()) {
            book.erase(price_level_it);
        }
        order_map.erase(it);
        std::cout << "Cancelled order ID=" << order_id << "\n";
        return true;
    } else {
        // Reference to asks
        auto& book = asks;
        double price = deque_it->price;
        auto price_level_it = book.find(price);
        if (price_level_it == book.end()) return false;

        price_level_it->second.erase(deque_it);
        if (price_level_it->second.empty()) {
            book.erase(price_level_it);
        }
        order_map.erase(it);
        std::cout << "Cancelled order ID=" << order_id << "\n";
        return true;
    }
}

bool OrderBook::modify_order(uint64_t order_id, double new_price, uint64_t new_quantity) {
    auto it = order_map.find(order_id);
    if (it == order_map.end()) return false;

    auto [side, deque_it] = it->second;

    if (!cancel_order(order_id)) return false;

    Order modified(order_id, side, OrderType::LIMIT, new_price, new_quantity);
    add_order(modified);
    std::cout << "Modified order ID=" << order_id << " to price=" << new_price << " qty=" << new_quantity << "\n";
    return true;
}


void OrderBook::print_book() const {
    cout << "\n===== ORDER BOOK =====\n";

    // Asks (sells) - ascending order of price
    cout << "\n      --- Asks (Sell) ---\n";
    cout << left << setw(10) << "Price"
         << setw(10) << "Quantity" << "\n";
    cout << "--------------------------\n";
    for (const auto &[price, orders] : asks) {
        uint64_t qty = 0;
        for (const auto &o : orders)
            qty += o.quantity;
        cout << left << setw(10) << price
             << setw(10) << qty << "\n";
    }
    if (asks.empty())
        cout << "(Empty)\n";

    // Bids (buys) - descending order of price
    cout << "\n      --- Bids (Buy) ---\n";
    cout << left << setw(10) << "Price"
         << setw(10) << "Quantity" << "\n";
    cout << "--------------------------\n";
    for (auto it = bids.begin(); it != bids.end(); ++it) {
        const auto &price = it->first;
        const auto &orders = it->second;

        uint64_t qty = 0;
        for (const auto &o : orders)
            qty += o.quantity;
        cout << left << setw(10) << price
             << setw(10) << qty << "\n";
    }
    if (bids.empty())
        cout << "(Empty)\n";

    cout << "=========================\n";
}
