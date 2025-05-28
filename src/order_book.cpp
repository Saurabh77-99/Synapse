#include "order_book.hpp"
using namespace std;

void OrderBook::add_order(const Order& order) {
    if (order.type == OrderType::LIMIT) {
        if (order.side == Side::BUY) {
            bids[order.price].push_back(order);
        } else {
            asks[order.price].push_back(order);
        }
        cout << "Added limit order: ID=" << order.order_id << " side=" 
                  << (order.side == Side::BUY ? "BUY" : "SELL") << " price=" << order.price 
                  << " qty=" << order.quantity << "\n";
    } else {
        cout << "Market orders not handled yet\n";
    }
}

void OrderBook::print_book() const {
    cout << "--- Order Book ---\nAsks:\n";
    for (const auto& [price, orders] : asks) {
        uint64_t qty = 0;
        for (const auto& o : orders) qty += o.quantity;
        cout << "Price: " << price << ", Qty: " << qty << "\n";
    }

    cout << "Bids:\n";
    for (const auto& [price, orders] : bids) {
        uint64_t qty = 0;
        for (const auto& o : orders) qty += o.quantity;
        cout << "Price: " << price << ", Qty: " << qty << "\n";
    }
}
