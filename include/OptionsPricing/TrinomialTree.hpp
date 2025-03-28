#include "Common.hpp"

namespace BasicOptionsPricing {

class TrinomialTreeOption : public Option {
public:
    TrinomialTreeOption(double spot, double strike, double riskFreeRate, 
                        double volatility, double timeToMaturity, 
                        OptionType type, ExerciseType exerciseType,
                        unsigned int steps = 80)
        : Option(spot, strike, riskFreeRate, volatility, timeToMaturity, 
                 type, exerciseType), steps_(steps) {}
    
    // Price the option using trinomial tree method
    double price() const override {
        double dt = timeToMaturity_ / steps_;
        double dx = volatility_ * sqrt(3.0 * dt);
        double u = exp(dx);
        double d = exp(-dx);
        double m = 1.0; // Middle factor
        
        // Risk-neutral probabilities
        double discountFactor = exp(-riskFreeRate_ * dt);
        double pu = (exp(riskFreeRate_ * dt/2) - exp(-dx/2)) / (exp(dx/2) - exp(-dx/2)) * 
                    (exp(dx/2) - 1) / (exp(dx/2) - exp(-dx/2));
        double pd = (exp(dx/2) - exp(riskFreeRate_ * dt/2)) / (exp(dx/2) - exp(-dx/2)) * 
                    (1 - exp(-dx/2)) / (exp(dx/2) - exp(-dx/2));
        double pm = 1.0 - pu - pd;
        
        // Initialize the price lattice
        std::vector<std::vector<double>> stockPrices(steps_ + 1);
        for (unsigned int i = 0; i <= steps_; ++i) {
            stockPrices[i].resize(2 * i + 1);
            for (int j = -static_cast<int>(i); j <= static_cast<int>(i); ++j) {
                stockPrices[i][j + i] = spot_ * pow(u, j);
            }
        }
        
        // Initialize option values at maturity
        std::vector<double> optionValues(2 * steps_ + 1);
        for (int j = -static_cast<int>(steps_); j <= static_cast<int>(steps_); ++j) {
            double stockPrice = spot_ * pow(u, j);
            if (type_ == OptionType::Call) {
                optionValues[j + steps_] = std::max(0.0, stockPrice - strike_);
            } else {  // Put
                optionValues[j + steps_] = std::max(0.0, strike_ - stockPrice);
            }
        }
        
        // Work backwards through the tree
        for (int i = steps_ - 1; i >= 0; --i) {
            std::vector<double> newOptionValues(2 * i + 1);
            for (int j = -i; j <= i; ++j) {
                double stockPrice = spot_ * pow(u, j);
                
                // Calculate option value as discounted expected value
                double optionValue = discountFactor * (
                    pu * optionValues[j + 1 + steps_] +
                    pm * optionValues[j + steps_] +
                    pd * optionValues[j - 1 + steps_]
                );
                
                // For American options, check if early exercise is optimal
                if (exerciseType_ == ExerciseType::American) {
                    if (type_ == OptionType::Call) {
                        optionValue = std::max(optionValue, stockPrice - strike_);
                    } else {  // Put
                        optionValue = std::max(optionValue, strike_ - stockPrice);
                    }
                }
                
                newOptionValues[j + i] = optionValue;
            }
            optionValues = newOptionValues;
        }
        
        return optionValues[0];
    }
    
    // Calculate delta using finite difference method
    double delta() const {
        double h = spot_ * 0.01;  // Small price change
        
        TrinomialTreeOption optionUp(spot_ + h, strike_, riskFreeRate_, 
                                    volatility_, timeToMaturity_, 
                                    type_, exerciseType_, steps_);
        
        TrinomialTreeOption optionDown(spot_ - h, strike_, riskFreeRate_, 
                                      volatility_, timeToMaturity_, 
                                      type_, exerciseType_, steps_);
        
        return (optionUp.price() - optionDown.price()) / (2.0 * h);
    }
    
    // Calculate gamma using finite difference method
    double gamma() const {
        double h = spot_ * 0.01;  // Small price change
        
        TrinomialTreeOption optionUp(spot_ + h, strike_, riskFreeRate_, 
                                    volatility_, timeToMaturity_, 
                                    type_, exerciseType_, steps_);
        
        TrinomialTreeOption optionDown(spot_ - h, strike_, riskFreeRate_, 
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
        
        TrinomialTreeOption optionLess(spot_, strike_, riskFreeRate_, 
                                      volatility_, timeToMaturity_ - h, 
                                      type_, exerciseType_, steps_);
        
        return (optionLess.price() - price()) / h;
    }
    
    // Calculate vega using finite difference method
    double vega() const {
        double h = volatility_ * 0.01;  // Small volatility change
        
        TrinomialTreeOption optionUp(spot_, strike_, riskFreeRate_, 
                                    volatility_ + h, timeToMaturity_, 
                                    type_, exerciseType_, steps_);
        
        TrinomialTreeOption optionDown(spot_, strike_, riskFreeRate_, 
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

} // namespace BasicOptionsPricing