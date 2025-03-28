#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <string>
#include "../include/OptionsPricing/basic_options_pricing.h"

using namespace BasicOptionsPricing;

// Forward declarations of example functions
void blackScholesExample();
void binomialTreeExample();
void trinomialTreeExample();
void impliedVolatilityExample();
void optionFactoryAndPortfolioExample();
void convergenceAnalysisExample();

// Helper function to print option parameters
void printOptionParams(const Option& option) {
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Option Parameters:\n";
    std::cout << "  Type: " << optionTypeToString(option.type()) << " " 
              << exerciseTypeToString(option.exerciseType()) << "\n";
    std::cout << "  Spot Price: " << option.spot() << "\n";
    std::cout << "  Strike Price: " << option.strike() << "\n";
    std::cout << "  Risk-Free Rate: " << option.riskFreeRate() * 100 << "%\n";
    std::cout << "  Volatility: " << option.volatility() * 100 << "%\n";
    std::cout << "  Time to Maturity: " << option.timeToMaturity() << " years\n";
    std::cout << std::endl;
}

// Example 5: Option Factory and Portfolio
void optionFactoryAndPortfolioExample() {
    std::cout << "=================================================\n";
    std::cout << "Example 5: Option Factory and Portfolio Management\n";
    std::cout << "=================================================\n";
    
    // Create a portfolio of options
    OptionPortfolio portfolio;
    
    // Add options using the factory
    // 1. ATM Call
    auto atmCall = OptionFactory::createOption(
        100.0, 100.0, 0.05, 0.2, 1.0, 
        OptionType::Call, ExerciseType::European, "BlackScholes");
    portfolio.addOption(std::move(atmCall), 1.0);
    
    // 2. ITM Put
    auto itmPut = OptionFactory::createOption(
        100.0, 90.0, 0.05, 0.2, 1.0, 
        OptionType::Put, ExerciseType::European, "BlackScholes");
    portfolio.addOption(std::move(itmPut), 2.0);
    
    // 3. OTM American Call
    auto otmAmCall = OptionFactory::createOption(
        100.0, 110.0, 0.05, 0.2, 1.0, 
        OptionType::Call, ExerciseType::American, "BinomialTree", 100);
    portfolio.addOption(std::move(otmAmCall), 1.0);
    
    // 4. ATM American Put
    auto atmAmPut = OptionFactory::createOption(
        100.0, 100.0, 0.05, 0.2, 1.0, 
        OptionType::Put, ExerciseType::American, "TrinomialTree", 80);
    portfolio.addOption(std::move(atmAmPut), 1.0);
    
    // Calculate portfolio value
    double portfolioValue = portfolio.totalValue();
    std::cout << "Portfolio Total Value: " << portfolioValue << "\n";
    
    // Calculate portfolio Greeks
    double portfolioDelta = portfolio.delta();
    double portfolioGamma = portfolio.gamma();
    
    std::cout << "Portfolio Delta: " << portfolioDelta << "\n";
    std::cout << "Portfolio Gamma: " << portfolioGamma << "\n";
    std::cout << std::endl;
}

// Example 6: Convergence Analysis
void convergenceAnalysisExample() {
    std::cout << "===================================\n";
    std::cout << "Example 6: Convergence Analysis\n";
    std::cout << "===================================\n";
    
    // Parameters
    double spot = 100.0;
    double strike = 100.0;
    double riskFreeRate = 0.05;
    double volatility = 0.2;
    double timeToMaturity = 1.0;
    
    // Black-Scholes price (analytical solution)
    BlackScholesOption bsOption(spot, strike, riskFreeRate, volatility, 
                              timeToMaturity, OptionType::Call);
    double bsPrice = bsOption.price();
    
    std::cout << "Black-Scholes Price (Analytical): " << bsPrice << "\n\n";
    
    // Convergence analysis for binomial tree
    std::cout << "Binomial Tree Convergence Analysis:\n";
    std::cout << "Steps\tPrice\t\tError\t\tRelative Error\n";
    
    for (unsigned int steps = 10; steps <= 1000; steps *= 2) {
        BinomialTreeOption binOption(spot, strike, riskFreeRate, volatility, 
                                   timeToMaturity, OptionType::Call, 
                                   ExerciseType::European, steps);
        double binPrice = binOption.price();
        double error = std::abs(binPrice - bsPrice);
        double relError = error / bsPrice * 100.0;
        
        std::cout << steps << "\t" << binPrice << "\t" << error << "\t" << relError << "%\n";
    }
    std::cout << std::endl;
    
    // Convergence analysis for trinomial tree
    std::cout << "Trinomial Tree Convergence Analysis:\n";
    std::cout << "Steps\tPrice\t\tError\t\tRelative Error\n";
    
    for (unsigned int steps = 10; steps <= 500; steps *= 2) {
        TrinomialTreeOption triOption(spot, strike, riskFreeRate, volatility, 
                                    timeToMaturity, OptionType::Call, 
                                    ExerciseType::European, steps);
        double triPrice = triOption.price();
        double error = std::abs(triPrice - bsPrice);
        double relError = error / bsPrice * 100.0;
        
        std::cout << steps << "\t" << triPrice << "\t" << error << "\t" << relError << "%\n";
    }
    std::cout << std::endl;
}

int main() {
    try {
        // Run all examples
        blackScholesExample();
        binomialTreeExample();
        trinomialTreeExample();
        impliedVolatilityExample();
        optionFactoryAndPortfolioExample();
        convergenceAnalysisExample();
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}

// Example 1: Black-Scholes pricing for European options
void blackScholesExample() {
    std::cout << "==========================================\n";
    std::cout << "Example 1: Black-Scholes European Options\n";
    std::cout << "==========================================\n";
    
    // Parameters
    double spot = 100.0;
    double strike = 100.0;
    double riskFreeRate = 0.05;
    double volatility = 0.2;
    double timeToMaturity = 1.0;
    
    // Create call option
    BlackScholesOption callOption(spot, strike, riskFreeRate, volatility, timeToMaturity, OptionType::Call);
    std::cout << "European Call Option:\n";
    printOptionParams(callOption);
    
    // Price and Greeks
    double callPrice = callOption.price();
    BlackScholesOption::Greeks callGreeks = callOption.calculateGreeks();
    
    std::cout << "Call Option Price: " << callPrice << "\n";
    std::cout << "Call Option Greeks:\n";
    std::cout << "  Delta: " << callGreeks.delta << "\n";
    std::cout << "  Gamma: " << callGreeks.gamma << "\n";
    std::cout << "  Theta: " << callGreeks.theta << " (per day: " << callGreeks.theta / 365.0 << ")\n";
    std::cout << "  Vega: " << callGreeks.vega << " (for 1% change in volatility)\n";
    std::cout << "  Rho: " << callGreeks.rho << " (for 1% change in interest rate)\n";
    std::cout << std::endl;
    
    // Create put option
    BlackScholesOption putOption(spot, strike, riskFreeRate, volatility, timeToMaturity, OptionType::Put);
    std::cout << "European Put Option:\n";
    printOptionParams(putOption);
    
    // Price and Greeks
    double putPrice = putOption.price();
    BlackScholesOption::Greeks putGreeks = putOption.calculateGreeks();
    
    std::cout << "Put Option Price: " << putPrice << "\n";
    std::cout << "Put Option Greeks:\n";
    std::cout << "  Delta: " << putGreeks.delta << "\n";
    std::cout << "  Gamma: " << putGreeks.gamma << "\n";
    std::cout << "  Theta: " << putGreeks.theta << " (per day: " << putGreeks.theta / 365.0 << ")\n";
    std::cout << "  Vega: " << putGreeks.vega << " (for 1% change in volatility)\n";
    std::cout << "  Rho: " << putGreeks.rho << " (for 1% change in interest rate)\n";
    std::cout << std::endl;
    
    // Verify put-call parity
    double putCallParity = callPrice - putPrice - spot + strike * exp(-riskFreeRate * timeToMaturity);
    std::cout << "Put-Call Parity Check: " << putCallParity << " (should be close to 0)\n";
    std::cout << std::endl;
}   

// Example 2: Binomial Tree for American options
void binomialTreeExample() {
    std::cout << "==========================================\n";
    std::cout << "Example 2: Binomial Tree American Options\n";
    std::cout << "==========================================\n";
    
    // Parameters
    double spot = 100.0;
    double strike = 100.0;
    double riskFreeRate = 0.05;
    double volatility = 0.2;
    double timeToMaturity = 1.0;
    unsigned int steps = 100;
    
    // Create American call option
    BinomialTreeOption amCallOption(spot, strike, riskFreeRate, volatility, 
                                  timeToMaturity, OptionType::Call, 
                                  ExerciseType::American, steps);
    std::cout << "American Call Option (Binomial Tree with " << steps << " steps):\n";
    printOptionParams(amCallOption);
    
    // Price and Greeks
    double amCallPrice = amCallOption.price();
    BinomialTreeOption::Greeks amCallGreeks = amCallOption.calculateGreeks();
    
    std::cout << "American Call Option Price: " << amCallPrice << "\n";
    std::cout << "American Call Option Greeks:\n";
    std::cout << "  Delta: " << amCallGreeks.delta << "\n";
    std::cout << "  Gamma: " << amCallGreeks.gamma << "\n";
    std::cout << "  Theta: " << amCallGreeks.theta << " (per day: " << amCallGreeks.theta / 365.0 << ")\n";
    std::cout << "  Vega: " << amCallGreeks.vega << " (for 1% change in volatility)\n";
    std::cout << std::endl;
    
    // Create American put option
    BinomialTreeOption amPutOption(spot, strike, riskFreeRate, volatility, 
                                 timeToMaturity, OptionType::Put, 
                                 ExerciseType::American, steps);
    std::cout << "American Put Option (Binomial Tree with " << steps << " steps):\n";
    printOptionParams(amPutOption);
    
    // Price and Greeks
    double amPutPrice = amPutOption.price();
    BinomialTreeOption::Greeks amPutGreeks = amPutOption.calculateGreeks();
    
    std::cout << "American Put Option Price: " << amPutPrice << "\n";
    std::cout << "American Put Option Greeks:\n";
    std::cout << "  Delta: " << amPutGreeks.delta << "\n";
    std::cout << "  Gamma: " << amPutGreeks.gamma << "\n";
    std::cout << "  Theta: " << amPutGreeks.theta << " (per day: " << amPutGreeks.theta / 365.0 << ")\n";
    std::cout << "  Vega: " << amPutGreeks.vega << " (for 1% change in volatility)\n";
    std::cout << std::endl;
    
    // Create European options with binomial tree for comparison
    BinomialTreeOption euCallOption(spot, strike, riskFreeRate, volatility, 
                                  timeToMaturity, OptionType::Call, 
                                  ExerciseType::European, steps);
    BinomialTreeOption euPutOption(spot, strike, riskFreeRate, volatility, 
                                 timeToMaturity, OptionType::Put, 
                                 ExerciseType::European, steps);
    
    double euCallPrice = euCallOption.price();
    double euPutPrice = euPutOption.price();
    
    std::cout << "European Call Option Price (Binomial Tree): " << euCallPrice << "\n";
    std::cout << "European Put Option Price (Binomial Tree): " << euPutPrice << "\n";
    std::cout << std::endl;
    
    // Create Black-Scholes options for comparison
    BlackScholesOption bsCallOption(spot, strike, riskFreeRate, volatility, 
                                  timeToMaturity, OptionType::Call);
    BlackScholesOption bsPutOption(spot, strike, riskFreeRate, volatility, 
                                 timeToMaturity, OptionType::Put);
    
    double bsCallPrice = bsCallOption.price();
    double bsPutPrice = bsPutOption.price();
    
    std::cout << "European Call Option Price (Black-Scholes): " << bsCallPrice << "\n";
    std::cout << "European Put Option Price (Black-Scholes): " << bsPutPrice << "\n";
    std::cout << std::endl;
    
    // Calculate early exercise premium
    double callPremium = amCallPrice - bsCallPrice;
    double putPremium = amPutPrice - bsPutPrice;
    
    std::cout << "Early Exercise Premium (Call): " << callPremium << "\n";
    std::cout << "Early Exercise Premium (Put): " << putPremium << "\n";
    std::cout << std::endl;
    };

// Example 3: Trinomial Tree for American options
void trinomialTreeExample() {
    std::cout << "===========================================\n";
    std::cout << "Example 3: Trinomial Tree American Options\n";
    std::cout << "===========================================\n";
    
    // Parameters
    double spot = 100.0;
    double strike = 100.0;
    double riskFreeRate = 0.05;
    double volatility = 0.2;
    double timeToMaturity = 1.0;
    unsigned int steps = 80;  // Fewer steps needed due to improved convergence
    
    // Create American call option
    TrinomialTreeOption amCallOption(spot, strike, riskFreeRate, volatility, 
                                   timeToMaturity, OptionType::Call, 
                                   ExerciseType::American, steps);
    std::cout << "American Call Option (Trinomial Tree with " << steps << " steps):\n";
    printOptionParams(amCallOption);
    
    // Price and Greeks
    double amCallPrice = amCallOption.price();
    TrinomialTreeOption::Greeks amCallGreeks = amCallOption.calculateGreeks();
    
    std::cout << "American Call Option Price: " << amCallPrice << "\n";
    std::cout << "American Call Option Greeks:\n";
    std::cout << "  Delta: " << amCallGreeks.delta << "\n";
    std::cout << "  Gamma: " << amCallGreeks.gamma << "\n";
    std::cout << "  Theta: " << amCallGreeks.theta << " (per day: " << amCallGreeks.theta / 365.0 << ")\n";
    std::cout << "  Vega: " << amCallGreeks.vega << " (for 1% change in volatility)\n";
    std::cout << std::endl;
    
    // Create American put option
    TrinomialTreeOption amPutOption(spot, strike, riskFreeRate, volatility, 
                                  timeToMaturity, OptionType::Put, 
                                  ExerciseType::American, steps);
    std::cout << "American Put Option (Trinomial Tree with " << steps << " steps):\n";
    printOptionParams(amPutOption);
    
    // Price and Greeks
    double amPutPrice = amPutOption.price();
    TrinomialTreeOption::Greeks amPutGreeks = amPutOption.calculateGreeks();
    
    std::cout << "American Put Option Price: " << amPutPrice << "\n";
    std::cout << "American Put Option Greeks:\n";
    std::cout << "  Delta: " << amPutGreeks.delta << "\n";
    std::cout << "  Gamma: " << amPutGreeks.gamma << "\n";
    std::cout << "  Theta: " << amPutGreeks.theta << " (per day: " << amPutGreeks.theta / 365.0 << ")\n";
    std::cout << "  Vega: " << amPutGreeks.vega << " (for 1% change in volatility)\n";
    std::cout << std::endl;
    
    // Compare with Binomial Tree
    BinomialTreeOption binCallOption(spot, strike, riskFreeRate, volatility, 
                                   timeToMaturity, OptionType::Call, 
                                   ExerciseType::American, 100);
    BinomialTreeOption binPutOption(spot, strike, riskFreeRate, volatility, 
                                  timeToMaturity, OptionType::Put, 
                                  ExerciseType::American, 100);
    
    double binCallPrice = binCallOption.price();
    double binPutPrice = binPutOption.price();
    
    std::cout << "Comparison with Binomial Tree (100 steps):\n";
    std::cout << "  American Call Price (Binomial): " << binCallPrice << "\n";
    std::cout << "  American Put Price (Binomial): " << binPutPrice << "\n";
    std::cout << "  Call Price Difference: " << amCallPrice - binCallPrice << "\n";
    std::cout << "  Put Price Difference: " << amPutPrice - binPutPrice << "\n";
    std::cout << std::endl;
};

// Example 4: Implied Volatility
void impliedVolatilityExample() {
    std::cout << "==========================================\n";
    std::cout << "Example 4: Implied Volatility Calculation\n";
    std::cout << "==========================================\n";
    
    // Parameters
    double spot = 100.0;
    double strike = 100.0;
    double riskFreeRate = 0.05;
    double actualVolatility = 0.2;
    double timeToMaturity = 1.0;
    
    // Create options with known volatility
    BlackScholesOption callOption(spot, strike, riskFreeRate, actualVolatility, 
                               timeToMaturity, OptionType::Call);
    BlackScholesOption putOption(spot, strike, riskFreeRate, actualVolatility, 
                              timeToMaturity, OptionType::Put);
    
    // Get option prices
    double callPrice = callOption.price();
    double putPrice = putOption.price();
    
    std::cout << "Original Options:\n";
    std::cout << "  Call Option Price: " << callPrice << " (volatility = " << actualVolatility * 100 << "%)\n";
    std::cout << "  Put Option Price: " << putPrice << " (volatility = " << actualVolatility * 100 << "%)\n";
    std::cout << std::endl;
    
    // Calculate implied volatility
    double impliedCallVol = calculateImpliedVolatility(
        callPrice, spot, strike, riskFreeRate, timeToMaturity, OptionType::Call);
    
    double impliedPutVol = calculateImpliedVolatility(
        putPrice, spot, strike, riskFreeRate, timeToMaturity, OptionType::Put);
    
    std::cout << "Implied Volatility Calculation:\n";
    std::cout << "  Call Option Implied Volatility: " << impliedCallVol * 100 << "%\n";
    std::cout << "  Put Option Implied Volatility: " << impliedPutVol * 100 << "%\n";
    std::cout << "  Actual Volatility: " << actualVolatility * 100 << "%\n";
    std::cout << std::endl;
    
    // Test with different market prices
    double marketCallPrice = callPrice * 1.1;  // 10% higher price
    double marketPutPrice = putPrice * 0.9;   // 10% lower price
    
    double marketCallVol = calculateImpliedVolatility(
        marketCallPrice, spot, strike, riskFreeRate, timeToMaturity, OptionType::Call);
    
    double marketPutVol = calculateImpliedVolatility(
        marketPutPrice, spot, strike, riskFreeRate, timeToMaturity, OptionType::Put);
    
    std::cout << "Market Price Scenarios:\n";
    std::cout << "  Market Call Price: " << marketCallPrice << " (10% higher)\n";
    std::cout << "  Implied Call Volatility: " << marketCallVol * 100 << "%\n";
    std::cout << "  Market Put Price: " << marketPutPrice << " (10% lower)\n";
    std::cout << "  Implied Put Volatility: " << marketPutVol * 100 << "%\n";
    std::cout << std::endl;
};