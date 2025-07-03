# Synapse Trading Simulator

A high-performance C++ trading simulator implementing a complete order book with matching engine functionality. Synapse provides realistic trading mechanics for backtesting, algorithm development, and educational purposes.

## Features

### Core Functionality
- **Order Book Management**: Maintains separate bid and ask order books with price-time priority
- **Order Matching Engine**: Implements realistic order matching logic for both market and limit orders
- **Real-time Order Operations**: Support for adding, canceling, and modifying orders
- **Market & Limit Orders**: Full support for both order types with proper execution semantics

### Order Types
- **Limit Orders**: Orders with specified price that wait in the book for matching
- **Market Orders**: Orders that execute immediately at best available prices

### Order Management
- **Add Orders**: Place new buy/sell orders in the book
- **Cancel Orders**: Remove existing orders from the book
- **Modify Orders**: Update price and quantity of existing orders
- **Order Tracking**: Unique order ID system for order lifecycle management

### Matching Engine
- **Price-Time Priority**: Orders matched based on price priority, then time priority
- **Partial Fills**: Support for partial order execution
- **Cross-Side Matching**: Automatic matching between buy and sell orders
- **Trade Execution**: Real-time trade reporting with execution details

## Architecture

### Core Components

#### Order Structure
```cpp
struct Order {
    uint64_t order_id;     // Unique identifier
    Side side;             // BUY or SELL
    OrderType type;        // MARKET or LIMIT
    double price;          // Order price
    uint64_t quantity;     // Order quantity
    timestamp;             // Order creation time
};
```

#### Order Book
- **Bids**: Buy orders stored in descending price order (highest first)
- **Asks**: Sell orders stored in ascending price order (lowest first)
- **Order Map**: Fast lookup for order cancellation and modification

### Data Structures
- `std::map` with custom comparators for price-ordered books
- `std::deque` for maintaining time priority within price levels
- `std::unordered_map` for O(1) order lookup by ID

## Installation & Build

### Prerequisites
- C++17 compatible compiler (GCC, Clang, MSVC)
- CMake (optional, for future builds)

### Building the Project
```bash
# Clone the repository
git clone <repository-url>
cd synapse

# Compile with g++
g++ -std=c++17 src/*.cpp -Iinclude -o trading_simulator

# Run the simulator
./trading_simulator
```

## Usage

### Basic Example
```cpp
#include "order_book.hpp"

int main() {
    OrderBook ob;
    
    // Add limit orders
    ob.add_order(Order(1, Side::BUY, OrderType::LIMIT, 100.0, 10));
    ob.add_order(Order(2, Side::SELL, OrderType::LIMIT, 102.0, 15));
    
    // View order book
    ob.print_book();
    
    // Execute market order
    ob.add_order(Order(3, Side::BUY, OrderType::MARKET, 0.0, 5));
    
    // Cancel order
    ob.cancel_order(1);
    
    // Modify order
    ob.modify_order(2, 101.0, 20);
    
    return 0;
}
```

### Order Book Operations

#### Adding Orders
```cpp
// Limit buy order at $100 for 10 shares
ob.add_order(Order(1, Side::BUY, OrderType::LIMIT, 100.0, 10));

// Market sell order for 5 shares
ob.add_order(Order(2, Side::SELL, OrderType::MARKET, 0.0, 5));
```

#### Order Management
```cpp
// Cancel order by ID
bool success = ob.cancel_order(order_id);

// Modify existing order
bool success = ob.modify_order(order_id, new_price, new_quantity);
```

## File Structure

```
synapse/
├── include/
│   ├── order.hpp          # Order structure and enums
│   └── order_book.hpp     # OrderBook class definition
├── src/
│   ├── main.cpp           # Demo application
│   ├── order.cpp          # Order implementation
│   └── order_book.cpp     # OrderBook implementation
├── CMakeLists.txt         # Build configuration (planned)
└── README.md              # This file
```

## Technical Details

### Matching Logic
1. **Market Orders**: Execute immediately against best available prices
2. **Limit Orders**: Match against opposing orders at same or better prices
3. **Price Priority**: Better prices get matched first
4. **Time Priority**: Earlier orders at same price get matched first

### Performance Characteristics
- **Order Addition**: O(log n) for limit orders, O(n) for market orders
- **Order Cancellation**: O(log n) average case
- **Order Modification**: O(log n) (cancel + add)
- **Book Display**: O(n) where n is number of price levels

### Memory Management
- Efficient use of STL containers
- Automatic cleanup of empty price levels
- Iterator-based order tracking for fast cancellation

## Example Output

```
Added limit order: ID=1 side=BUY price=100 qty=10
Added limit order: ID=2 side=SELL price=102 qty=15

===== ORDER BOOK =====

      --- Asks (Sell) ---
Price     Quantity  
--------------------------
102       15        

      --- Bids (Buy) ---
Price     Quantity  
--------------------------
100       10        
=========================

>> Trade executed: BUY 3 | SELL 2 | price 102 | qty 5
```

## Roadmap

### Planned Features
- [ ] **Historical Data Replay**: Load and replay historical market data
- [ ] **Strategy Framework**: Plugin system for trading strategies
- [ ] **Risk Management**: Position limits and risk controls
- [ ] **Market Data Feed**: Real-time market data integration
- [ ] **Performance Analytics**: Trade statistics and performance metrics
- [ ] **Web Interface**: Browser-based trading interface
- [ ] **Database Integration**: Order and trade persistence
- [ ] **Multi-Asset Support**: Support for different asset classes

### Technical Improvements
- [ ] **Multithreading**: Concurrent order processing
- [ ] **Network Interface**: TCP/UDP order entry
- [ ] **Configuration System**: JSON/YAML configuration files
- [ ] **Logging System**: Structured logging for debugging
- [ ] **Unit Tests**: Comprehensive test suite
- [ ] **Benchmarking**: Performance measurement tools

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Inspired by real-world electronic trading systems
- Built with modern C++ best practices
- Designed for educational and research purposes

---

**Note**: This is a simulation environment. Not intended for production trading without proper risk management and regulatory compliance.
