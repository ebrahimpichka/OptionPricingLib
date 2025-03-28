#ifndef OPTIONS_PRICING_IMPLIED_VOLATILITY_HPP
#define OPTIONS_PRICING_IMPLIED_VOLATILITY_HPP

#include "BlackScholes.hpp"

namespace OptionsPricing {


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
        unsigned int maxIterations = 1000)
    {
        // Init guess for imp vo
        double volLow = 0.001;
        double volHigh = 2.0;  // 200% volatility as upper bound
        
        BlackScholesOption optionLow(spot, strike, riskFreeRate, volLow, timeToMaturity, type);
        BlackScholesOption optionHigh(spot, strike, riskFreeRate, volHigh, timeToMaturity, type);
        
        double priceLow = optionLow.price();
        double priceHigh = optionHigh.price();
        
        // Check if target price is within bounds
        if (targetPrice <= priceLow || targetPrice >= priceHigh) {
            throw std::runtime_error("Target price is outside the bounds of possible option prices");
        }

        // TODO: Implement Newton-Raphson method

        // Binary search for imp vol
        double vol = (volLow + volHigh) / 2.0;
        
        for (unsigned int i = 0; i < maxIterations; ++i) {
            BlackScholesOption option(spot, strike, riskFreeRate, vol, timeToMaturity, type);
            double price = option.price();
            
            if (std::abs(price - targetPrice) < tolerance) {
                return vol;
            }
            
            if (price < targetPrice) {
                volLow = vol;
            } else {
                volHigh = vol;
            }
            
            vol = (volLow + volHigh) / 2.0;
        }
        
        throw std::runtime_error("Failed to converge to implied volatility within tolerance");
    }
};


} // namespace OptionsPricing

#endif // OPTIONS_PRICING_IMPLIED_VOLATILITY_HPP