#include "order.hpp"
#include "order_book.hpp"

int main() {
    OrderBook ob;

    ob.add_order(Order(1, Side::BUY, OrderType::LIMIT, 100.5, 10));
    ob.add_order(Order(2, Side::SELL, OrderType::LIMIT, 101.0, 5));
    ob.add_order(Order(3, Side::BUY, OrderType::LIMIT, 100.0, 15));
    ob.add_order(Order(4, Side::SELL, OrderType::LIMIT, 99.0, 20));
    ob.add_order(Order(5, Side::BUY, OrderType::MARKET, 0.0, 12));

    ob.print_book();

    return 0;
}
