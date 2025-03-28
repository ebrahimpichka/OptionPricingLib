#ifndef OPTIONS_PRICING_BLACK_SCHOLES_HPP
#define OPTIONS_PRICING_BLACK_SCHOLES_HPP

#include "Common.hpp"

namespace OptionsPricing {

class BlackScholesOption : public Option {
public:
    BlackScholesOption(double spot, double strike, double riskFreeRate, 
                       double volatility, double timeToMaturity, 
                       OptionType type)
        : Option(spot, strike, riskFreeRate, volatility, timeToMaturity, 
                 type, ExerciseType::European) {}
    
    // Price the option using Black-Scholes formula
    double price() const override {
        if (exerciseType_ != ExerciseType::European) {
            throw std::invalid_argument("Black-Scholes model only applicable for European options");
        }
        
        double d1 = (log(spot_ / strike_) + (riskFreeRate_ + volatility_ * volatility_ / 2.0) * timeToMaturity_) / 
                   (volatility_ * sqrt(timeToMaturity_));
        double d2 = d1 - volatility_ * sqrt(timeToMaturity_);
        
        if (type_ == OptionType::Call) {
            return spot_ * normalCDF(d1) - strike_ * exp(-riskFreeRate_ * timeToMaturity_) * normalCDF(d2);
        } else {  // Put
            return strike_ * exp(-riskFreeRate_ * timeToMaturity_) * normalCDF(-d2) - spot_ * normalCDF(-d1);
        }
    }
    
    // Calculate delta
    double delta() const {
        double d1 = (log(spot_ / strike_) + (riskFreeRate_ + volatility_ * volatility_ / 2.0) * timeToMaturity_) / 
                   (volatility_ * sqrt(timeToMaturity_));
        
        if (type_ == OptionType::Call) {
            return normalCDF(d1);
        } else {  // Put
            return normalCDF(d1) - 1.0;
        }
    }
    
    // Calculate gamma
    double gamma() const {
        double d1 = (log(spot_ / strike_) + (riskFreeRate_ + volatility_ * volatility_ / 2.0) * timeToMaturity_) / 
                   (volatility_ * sqrt(timeToMaturity_));
        
        return normalPDF(d1) / (spot_ * volatility_ * sqrt(timeToMaturity_));
    }
    
    // Calculate theta
    double theta() const {
        double d1 = (log(spot_ / strike_) + (riskFreeRate_ + volatility_ * volatility_ / 2.0) * timeToMaturity_) / 
                   (volatility_ * sqrt(timeToMaturity_));
        double d2 = d1 - volatility_ * sqrt(timeToMaturity_);
        
        if (type_ == OptionType::Call) {
            return -spot_ * normalPDF(d1) * volatility_ / (2.0 * sqrt(timeToMaturity_)) - 
                   riskFreeRate_ * strike_ * exp(-riskFreeRate_ * timeToMaturity_) * normalCDF(d2);
        } else {  // Put
            return -spot_ * normalPDF(d1) * volatility_ / (2.0 * sqrt(timeToMaturity_)) + 
                   riskFreeRate_ * strike_ * exp(-riskFreeRate_ * timeToMaturity_) * normalCDF(-d2);
        }
    }
    
    // Calculate vega
    double vega() const {
        double d1 = (log(spot_ / strike_) + (riskFreeRate_ + volatility_ * volatility_ / 2.0) * timeToMaturity_) / 
                   (volatility_ * sqrt(timeToMaturity_));
        
        return spot_ * sqrt(timeToMaturity_) * normalPDF(d1) / 100.0;  // Divided by 100 to represent 1% change
    }
    
    // Calculate rho
    double rho() const {
        double d1 = (log(spot_ / strike_) + (riskFreeRate_ + volatility_ * volatility_ / 2.0) * timeToMaturity_) / 
                   (volatility_ * sqrt(timeToMaturity_));
        double d2 = d1 - volatility_ * sqrt(timeToMaturity_);
        
        if (type_ == OptionType::Call) {
            return strike_ * timeToMaturity_ * exp(-riskFreeRate_ * timeToMaturity_) * normalCDF(d2) / 100.0;
        } else {  // Put
            return -strike_ * timeToMaturity_ * exp(-riskFreeRate_ * timeToMaturity_) * normalCDF(-d2) / 100.0;
        }
    }
    
    // Calculate all Greeks at once
    struct Greeks {
        double delta;
        double gamma;
        double theta;
        double vega;
        double rho;
    };
    
    Greeks calculateGreeks() const {
        return {delta(), gamma(), theta(), vega(), rho()};
    }
};

} // namespace OptionsPricing

#endif // OPTIONS_PRICING_BLACK_SCHOLES_HPP