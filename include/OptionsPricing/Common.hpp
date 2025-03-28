#ifndef OPTIONS_PRICING_COMMON_HPP
#define OPTIONS_PRICING_COMMON_HPP

#include <cmath>
#include <stdexcept>

namespace OptionsPricing {

// Constants
const double PI = 3.14159265358979323846;


// Option types
enum class OptionType { Call, Put };

// Exercise types
enum class ExerciseType { European, American };

// Normal CDF
inline double normalCDF(double x) {
    return 0.5 * (1.0 + erf(x / sqrt(2.0)));
}

// Normal PDF
inline double normalPDF(double x) {
    return (1.0 / sqrt(2.0 * PI)) * exp(-0.5 * x * x);
}

// Base option class
// Base Option class
class Option {
public:
    Option(double spot, double strike, double riskFreeRate, 
           double volatility, double timeToMaturity, 
           OptionType type, ExerciseType exerciseType)
        : spot_(spot), strike_(strike), riskFreeRate_(riskFreeRate),
          volatility_(volatility), timeToMaturity_(timeToMaturity),
          type_(type), exerciseType_(exerciseType) {
        
        validateInputs();
    }
    
    virtual ~Option() = default;
    
    // Pure virtual method for pricing
    virtual double price() const = 0;
    
    // Getters
    double spot() const { return spot_; }
    double strike() const { return strike_; }
    double riskFreeRate() const { return riskFreeRate_; }
    double volatility() const { return volatility_; }
    double timeToMaturity() const { return timeToMaturity_; }
    OptionType type() const { return type_; }
    ExerciseType exerciseType() const { return exerciseType_; }
    
    
    // Validate inputs
    void validateInputs() const {
        if (spot_ <= 0.0) {
            throw std::invalid_argument("Spot price must be positive");
        }
        if (strike_ <= 0.0) {
            throw std::invalid_argument("Strike price must be positive");
        }
        if (volatility_ <= 0.0) {
            throw std::invalid_argument("Volatility must be positive");
        }
        if (timeToMaturity_ <= 0.0) {
            throw std::invalid_argument("Time to maturity must be positive");
        }
    }
    
protected:
    double spot_;
    double strike_;
    double riskFreeRate_;
    double volatility_;
    double timeToMaturity_;
    OptionType type_;
    ExerciseType exerciseType_;
};


// Helper function to convert option type to string
inline std::string optionTypeToString(OptionType type) {
    return (type == OptionType::Call) ? "Call" : "Put";
}

// Helper function to convert exercise type to string
inline std::string exerciseTypeToString(ExerciseType type) {
    return (type == ExerciseType::European) ? "European" : "American";
}

// Helper function to print option details
inline void printOptionDetails(const Option& option) {
    std::cout << "Option Type: " << optionTypeToString(option.type()) << std::endl;
    std::cout << "Exercise Type: " << exerciseTypeToString(option.exerciseType()) << std::endl;
    std::cout << "Spot Price: " << option.spot() << std::endl;
    std::cout << "Strike Price: " << option.strike() << std::endl;
    std::cout << "Risk-Free Rate: " << option.riskFreeRate() << std::endl;
    std::cout << "Volatility: " << option.volatility() << std::endl;
    std::cout << "Time to Maturity: " << option.timeToMaturity() << std::endl;
}

} // namespace OptionsPricing

#endif // OPTIONS_PRICING_COMMON_HPP