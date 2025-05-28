#include <iostream>
#include "order_book.hpp"
using namespace std;

int main() {
    OrderBook ob;

    ob.add_order(Order(1, Side::BUY, OrderType::LIMIT, 100.0, 10));
    ob.add_order(Order(2, Side::BUY, OrderType::LIMIT, 101.0, 5));
    ob.add_order(Order(3, Side::BUY, OrderType::LIMIT, 99.0, 20));

    ob.add_order(Order(4, Side::SELL, OrderType::LIMIT, 102.0, 15));
    ob.add_order(Order(5, Side::SELL, OrderType::LIMIT, 103.0, 10));

    ob.print_book();

    ob.add_order(Order(6, Side::BUY, OrderType::MARKET, 0.0, 10));

    ob.print_book();

    bool cancelled = ob.cancel_order(3);
    cout << "Cancel order 3: " << (cancelled ? "Success" : "Failure") << "\n";

    ob.print_book();

    bool modified = ob.modify_order(2, 98.0, 8);
    cout << "Modify order 2: " << (modified ? "Success" : "Failure") << "\n";

    ob.print_book();
    
    ob.add_order(Order(7, Side::SELL, OrderType::MARKET, 0.0, 12));

    ob.print_book();

    return 0;
}
