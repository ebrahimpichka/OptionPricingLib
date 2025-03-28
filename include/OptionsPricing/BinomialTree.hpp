#ifndef OPTIONS_PRICING_BINOMIAL_TREE_HPP
#define OPTIONS_PRICING_BINOMIAL_TREE_HPP

#include "Common.hpp"
#include <vector>

namespace OptionsPricing {


class BinomialTreeOption : public Option {
public:
    BinomialTreeOption(double spot, double strike, double riskFreeRate, 
                       double volatility, double timeToMaturity, 
                       OptionType type, ExerciseType exerciseType, 
                       unsigned int steps = 1000)
        : Option(spot, strike, riskFreeRate, volatility, timeToMaturity, 
                 type, exerciseType), steps_(steps) {}
    
    // Price the option using binomial tree method
    double price() const override {
        double dt = timeToMaturity_ / steps_;
        double u = exp(volatility_ * sqrt(dt));
        double d = 1.0 / u;
        double p = (exp(riskFreeRate_ * dt) - d) / (u - d);
        
        // Initialize terminal stock prices
        std::vector<double> stockPrices(steps_ + 1);
        for (unsigned int i = 0; i <= steps_; ++i) {
            stockPrices[i] = spot_ * pow(u, steps_ - i) * pow(d, i);
        }
        
        // option vslue (payoffs) at maturity
        std::vector<double> optionValues(steps_ + 1);
        for (unsigned int i = 0; i <= steps_; ++i) {
            if (type_ == OptionType::Call) {
                optionValues[i] = std::max(0.0, stockPrices[i] - strike_);
            } else {  // Put
                optionValues[i] = std::max(0.0, strike_ - stockPrices[i]);
            }
        }
        
        // Work backwards through the tree
        for (int j = steps_ - 1; j >= 0; --j) {
            for (int i = 0; i <= j; ++i) {
                // calc stock price at this node
                double stockPrice = spot_ * pow(u, j - i) * pow(d, i);
                
                // calc option value (risk-neutral valuation formula)
                double optionValue = exp(-riskFreeRate_ * dt) * 
                                    (p * optionValues[i] + (1.0 - p) * optionValues[i + 1]);
                                    // (weighted average of up and down nodes)
                
                // For American options, check if early exercise is optimal
                if (exerciseType_ == ExerciseType::American) {
                    if (type_ == OptionType::Call) {
                        optionValue = std::max(optionValue, stockPrice - strike_);
                    } else {  // Put
                        optionValue = std::max(optionValue, strike_ - stockPrice);
                    }
                }
                
                optionValues[i] = optionValue;
            }
        }
        
        return optionValues[0];
    }
    
    // Calculate delta using finite difference method
    double delta() const {
        double h = spot_ * 0.001;  // Small price change
        
        BinomialTreeOption optionUp(spot_ + h, strike_, riskFreeRate_, 
                                   volatility_, timeToMaturity_, 
                                   type_, exerciseType_, steps_);
        
        BinomialTreeOption optionDown(spot_ - h, strike_, riskFreeRate_, 
                                     volatility_, timeToMaturity_, 
                                     type_, exerciseType_, steps_);
        
        return (optionUp.price() - optionDown.price()) / (2.0 * h);
    }
    
    // Calculate gamma using finite difference method
    double gamma() const {
        double h = spot_ * 0.001;  // Small price change
        
        BinomialTreeOption optionUp(spot_ + h, strike_, riskFreeRate_, 
                                   volatility_, timeToMaturity_, 
                                   type_, exerciseType_, steps_);
        
        BinomialTreeOption optionDown(spot_ - h, strike_, riskFreeRate_, 
                                     volatility_, timeToMaturity_, 
                                     type_, exerciseType_, steps_);
        
        return (optionUp.price() - 2.0 * price() + optionDown.price()) / (h * h);
    }
    
    // Calculate theta using finite difference method
    double theta() const {
        double h = timeToMaturity_ * 0.01;  // Small time change
        
        if (timeToMaturity_ <= h) {
            h = timeToMaturity_ / 10.0;  // Adjust for very short maturities
        }
        
        BinomialTreeOption optionLess(spot_, strike_, riskFreeRate_, 
                                     volatility_, timeToMaturity_ - h, 
                                     type_, exerciseType_, steps_);
        
        return (optionLess.price() - price()) / h;
    }
    
    // Calculate vega using finite difference method
    double vega() const {
        double h = volatility_ * 0.01;  // Small volatility change
        
        BinomialTreeOption optionUp(spot_, strike_, riskFreeRate_, 
                                   volatility_ + h, timeToMaturity_, 
                                   type_, exerciseType_, steps_);
        
        BinomialTreeOption optionDown(spot_, strike_, riskFreeRate_, 
                                     volatility_ - h, timeToMaturity_, 
                                     type_, exerciseType_, steps_);
        
        return (optionUp.price() - optionDown.price()) / (2.0 * h * 100.0);  // Divided by 100 for 1% change
    }
    
    // Calculate all Greeks at once
    struct Greeks {
        double delta;
        double gamma;
        double theta;
        double vega;
    };
    
    Greeks calculateGreeks() const {
        return {delta(), gamma(), theta(), vega()};
    }
    
private:
    unsigned int steps_;
};


} // namespace OptionsPricing

#endif // OPTIONS_PRICING_BINOMIAL_TREE_HPP
