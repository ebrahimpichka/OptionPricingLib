#ifndef OPTIONS_PRICING_OPTION_FACTORY_HPP
#define OPTIONS_PRICING_OPTION_FACTORY_HPP

#include "BlackScholes.hpp"
#include "BinomialTree.hpp"
#include "TrinomialTree.hpp"
#include <memory>
#include <string>

namespace OptionsPricing {

// Option Factory class to create different types of options
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
        unsigned int steps = 100)
    {
        if (pricingMethod == "BlackScholes") {
            if (exerciseType == ExerciseType::American) {
                throw std::invalid_argument("Black-Scholes can only price European options");
            }
            return std::make_unique<BlackScholesOption>(
                spot, strike, riskFreeRate, volatility, timeToMaturity, type);
        }
        else if (pricingMethod == "BinomialTree") {
            return std::make_unique<BinomialTreeOption>(
                spot, strike, riskFreeRate, volatility, timeToMaturity, 
                type, exerciseType, steps);
        }
        else if (pricingMethod == "TrinomialTree") {
            return std::make_unique<TrinomialTreeOption>(
                spot, strike, riskFreeRate, volatility, timeToMaturity, 
                type, exerciseType, steps);
        }
        else {
            throw std::invalid_argument("Unknown pricing method: " + pricingMethod);
        }
    }
};


} // namespace OptionsPricing

#endif // OPTIONS_PRICING_OPTION_FACTORY_HPP