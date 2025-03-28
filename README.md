# BasicOptionsPricing Library

A modern C++ library for pricing options using various numerical methods, designed for quantitative finance applications.

## Features

- **Black-Scholes Model**: Analytical pricing for European options
- **Binomial Tree**: Numerical method for pricing American and European options
- **Trinomial Tree**: Enhanced numerical method with better convergence
- **Greeks Calculation**: Delta, Gamma, Theta, Vega, Rho
- **Implied Volatility**: Calculate implied volatility from option prices
- **Portfolio Management**: Tools for managing options portfolios

## Future Enhancements - TODO

1. **Additional Option Types**
   - Digital/Binary options
   - Barrier options (knock-in, knock-out)
   - Asian options (arithmetic and geometric averages)
   - Lookback options

2. **Improved Numerical Methods**
   - Finite difference methods for PDE solutions
   - Monte Carlo simulations for path-dependent options
   - Control variate techniques for variance reduction
   - Antithetic variates for Monte Carlo

3. **Volatility Models**
   - Term structure of volatility
   - SABR model implementation
   - Local volatility models
   - Heston stochastic volatility model

4. **Market Data Integration**
   - Yield curve bootstrapping
   - Volatility surface calibration
   - Historical data analysis tools

5. **Risk Management**
   - VaR (Value at Risk) calculations
   - Historical and Monte Carlo VaR
   - Portfolio stress testing

6. **Advanced Models and Methods**
   - Jump-diffusion models (Merton model)
   - Rough volatility models

7. **Exotic Options**
   - Bermudan options
   - Compound options
   - Chooser options
   - Basket options on multiple underlyings
   - Quanto options with foreign exchange features

8. **Calibration Techniques**
   - Advanced optimization methods for model calibration
   - Global optimization techniques (genetic algorithms, simulated annealing)
   - Multi-curve calibration framework

9. **Regulatory and Compliance Features**
   - FRTB (Fundamental Review of the Trading Book) calculations
   - XVA suite (CVA, DVA, FVA, MVA)
   - Back-testing framework for model validation



## Requirements

- C++17 compatible compiler
- [Catch2](https://github.com/catchorg/Catch2) (for testing only)

## Installation

This is a header-only library, so no installation is required. Simply include the header file in your project:


```cpp

// Managing a Portfolio of Options

#include "basic_options_pricing.h"
#include <iostream>

using namespace BasicOptionsPricing;

int main() {
    // Create a portfolio
    OptionPortfolio portfolio;
    
    // Add different options to the portfolio
    auto call = OptionFactory::createOption(
        100.0, 100.0, 0.05, 0.2, 1.0, OptionType::Call, 
        ExerciseType::European, "BlackScholes");
    
    auto put = OptionFactory::createOption(
        100.0, 100.0, 0.05, 0.2, 1.0, OptionType::Put, 
        ExerciseType::American, "BinomialTree", 100);
    
    // Add options with quantities
    portfolio.addOption(std::move(call), 1.0);
    portfolio.addOption(std::move(put), 2.0);
    
    // Calculate portfolio value and greeks
    double value = portfolio.totalValue();
    double delta = portfolio.delta();
    double gamma = portfolio.gamma();
    
    std::cout << "Portfolio Value: " << value << std::endl;
    std::cout << "Portfolio Delta: " << delta << std::endl;
    std::cout << "Portfolio Gamma: " << gamma << std::endl;
    
    return 0;
}
```

## API Reference

### Option Types

```cpp
enum class OptionType { Call, Put };
enum class ExerciseType { European, American };
```

### Base Option Class

```cpp
class Option {
public:
    Option(double spot, double strike, double riskFreeRate, 
           double volatility, double timeToMaturity, 
           OptionType type, ExerciseType exerciseType);
    
    virtual double price() const = 0;
    
    // Getters
    double spot() const;
    double strike() const;
    double riskFreeRate() const;
    double volatility() const;
    double timeToMaturity() const;
    OptionType type() const;
    ExerciseType exerciseType() const;
};
```

### Black-Scholes Option

```cpp
class BlackScholesOption : public Option {
public:
    BlackScholesOption(double spot, double strike, double riskFreeRate, 
                      double volatility, double timeToMaturity, 
                      OptionType type);
    
    double price() const override;
    double delta() const;
    double gamma() const;
    double theta() const;
    double vega() const;
    double rho() const;
    
    struct Greeks {
        double delta;
        double gamma;
        double theta;
        double vega;
        double rho;
    };
    
    Greeks calculateGreeks() const;
};
```

### Binomial Tree Option

```cpp
class BinomialTreeOption : public Option {
public:
    BinomialTreeOption(double spot, double strike, double riskFreeRate, 
                      double volatility, double timeToMaturity, 
                      OptionType type, ExerciseType exerciseType, 
                      unsigned int steps = 100);
    
    double price() const override;
    double delta() const;
    double gamma() const;
    double theta() const;
    double vega() const;
    
    struct Greeks {
        double delta;
        double gamma;
        double theta;
        double vega;
    };
    
    Greeks calculateGreeks() const;
};
```

### Trinomial Tree Option

```cpp
class TrinomialTreeOption : public Option {
public:
    TrinomialTreeOption(double spot, double strike, double riskFreeRate, 
                       double volatility, double timeToMaturity, 
                       OptionType type, ExerciseType exerciseType,
                       unsigned int steps = 80);
    
    double price() const override;
    double delta() const;
    double gamma() const;
    double theta() const;
    double vega() const;
    
    struct Greeks {
        double delta;
        double gamma;
        double theta;
        double vega;
    };
    
    Greeks calculateGreeks() const;
};
```

### Option Factory

```cpp
class OptionFactory {
public:
    static std::unique_ptr<Option> createOption(
        double spot,
        double strike,
        double riskFreeRate,
        double volatility,
        double timeToMaturity,
        OptionType type,
        ExerciseType exerciseType,
        const std::string& pricingMethod,
        unsigned int steps = 100);
};
```

### Implied Volatility Calculator

```cpp
class ImpliedVolatilityCalculator {
public:
    static double calculateImpliedVolatility(
        double targetPrice,
        double spot,
        double strike,
        double riskFreeRate,
        double timeToMaturity,
        OptionType type,
        double tolerance = 1e-6,
        unsigned int maxIterations = 100);
};
```

### Option Portfolio

```cpp
class OptionPortfolio {
public:
    void addOption(std::unique_ptr<Option> option, double quantity = 1.0);
    double totalValue() const;
    double delta() const;
    double gamma() const;
};
```

## Building and Testing

To build and run the examples:

```bash
g++ -std=c++17 -o option_pricing_examples main.cpp
./option_pricing_examples
```

To run the tests (requires Catch2):

```bash
g++ -std=c++17 -o option_pricing_tests tests.cpp
./option_pricing_tests
```

## Performance Considerations

- For European options, the Black-Scholes model provides exact analytical solutions and is significantly faster than tree-based methods.
- For American options, tree-based methods are necessary. The trinomial tree generally provides better accuracy than the binomial tree with fewer steps, but at a higher computational cost per step.
- For large portfolios or high-frequency applications, consider using optimized numerical libraries or GPU acceleration for the tree-based methods.

## References

- Hull, J. (2015). Options, Futures, and Other Derivatives. Pearson.
- Wilmott, P. (2007). Paul Wilmott Introduces Quantitative Finance. Wiley.
- QuantLib: The Quantitative Finance Library (https://www.quantlib.org/)

<!-- ## License

This library is provided under the MIT License.

## Author

[Your Name]

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
cpp
#include "basic_options_pricing.h"
```

## Usage Examples

### Basic Black-Scholes Pricing

```cpp
#include "basic_options_pricing.h"
#include <iostream>

using namespace BasicOptionsPricing;

int main() {
    // Create a European call option
    BlackScholesOption callOption(
        100.0,     // Spot price
        100.0,     // Strike price
        0.05,      // Risk-free rate (5%)
        0.2,       // Volatility (20%)
        1.0,       // Time to maturity (1 year)
        OptionType::Call
    );
    
    // Calculate option price and Greeks
    double price = callOption.price();
    auto greeks = callOption.calculateGreeks();
    
    std::cout << "Call Option Price: " << price << std::endl;
    std::cout << "Delta: " << greeks.delta << std::endl;
    std::cout << "Gamma: " << greeks.gamma << std::endl;
    std::cout << "Theta: " << greeks.theta << std::endl;
    std::cout << "Vega: " << greeks.vega << std::endl;
    std::cout << "Rho: " << greeks.rho << std::endl;
    
    return 0;
}
```

### American Option Pricing with Binomial Tree

```cpp
#include "basic_options_pricing.h"
#include <iostream>

using namespace BasicOptionsPricing;

int main() {
    // Create an American put option
    BinomialTreeOption putOption(
        100.0,     // Spot price
        100.0,     // Strike price
        0.05,      // Risk-free rate (5%)
        0.2,       // Volatility (20%)
        1.0,       // Time to maturity (1 year)
        OptionType::Put,
        ExerciseType::American,
        100        // Number of steps
    );
    
    // Calculate option price and Greeks
    double price = putOption.price();
    auto greeks = putOption.calculateGreeks();
    
    std::cout << "American Put Option Price: " << price << std::endl;
    std::cout << "Delta: " << greeks.delta << std::endl;
    std::cout << "Gamma: " << greeks.gamma << std::endl;
    std::cout << "Theta: " << greeks.theta << std::endl;
    std::cout << "Vega: " << greeks.vega << std::endl;
    
    return 0;
}
```

### Using the Option Factory

```cpp
#include "basic_options_pricing.h"
#include <iostream>

using namespace BasicOptionsPricing;

int main() {
    // Create an option using the factory
    auto option = OptionFactory::createOption(
        100.0,               // Spot price
        100.0,               // Strike price
        0.05,                // Risk-free rate
        0.2,                 // Volatility
        1.0,                 // Time to maturity
        OptionType::Call,
        ExerciseType::American,
        "TrinomialTree",     // Pricing method
        80                   // Number of steps
    );
    
    // Calculate option price
    double price = option->price();
    std::cout << "Option Price: " << price << std::endl;
    
    return 0;
}
```

### Calculating Implied Volatility

```cpp
#include "basic_options_pricing.h"
#include <iostream>

using namespace BasicOptionsPricing;

int main() {
    double marketPrice = 10.5;  // Observed market price
    
    double impliedVol = ImpliedVolatilityCalculator::calculateImpliedVolatility(
        marketPrice,         // Market price
        100.0,               // Spot price
        100.0,               // Strike price
        0.05,                // Risk-free rate
        1.0,                 // Time to maturity
        OptionType::Call     // Option type
    );
    
    std::cout << "Implied Volatility: " << impliedVol * 100 << "%" << std::endl;
    
    return 0;
} -->