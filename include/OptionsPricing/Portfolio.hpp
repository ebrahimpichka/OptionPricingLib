#ifndef OPTIONS_PRICING_PORTFOLIO_HPP
#define OPTIONS_PRICING_PORTFOLIO_HPP

#include "BlackScholes.hpp"
#include "BinomialTree.hpp"
#include "TrinomialTree.hpp"
#include <memory>
#include <vector>
#include <utility>

namespace OptionsPricing {

class OptionPortfolio{
public:
    void addOption(std::unique_ptr<Option> option, double quantity = 1.0) {
        options_.push_back(std::make_pair(std::move(option), quantity));
    }
    
    double totalValue() const {
        double total = 0.0;
        for (const auto& [option, quantity] : options_) {
            total += option->price() * quantity;
        }
        return total;
    }
    
    // Calculate portfolio delta
    double delta() const {
        double total = 0.0;
        for (const auto& [option, quantity] : options_) {
            if (auto* bs = dynamic_cast<const BlackScholesOption*>(option.get())) {
                total += bs->delta() * quantity;
            } else if (auto* bt = dynamic_cast<const BinomialTreeOption*>(option.get())) {
                total += bt->delta() * quantity;
            } else if (auto* tt = dynamic_cast<const TrinomialTreeOption*>(option.get())) {
                total += tt->delta() * quantity;
            }
        }
        return total;
    }
    
    // Calculate portfolio gamma
    double gamma() const {
        double total = 0.0;
        for (const auto& [option, quantity] : options_) {
            if (auto* bs = dynamic_cast<const BlackScholesOption*>(option.get())) {
                total += bs->gamma() * quantity;
            } else if (auto* bt = dynamic_cast<const BinomialTreeOption*>(option.get())) {
                total += bt->gamma() * quantity;
            } else if (auto* tt = dynamic_cast<const TrinomialTreeOption*>(option.get())) {
                total += tt->gamma() * quantity;
            }
        }
        return total;
    }
    
private:
    std::vector<std::pair<std::unique_ptr<Option>, double>> options_;
};

} // namespace OptionsPricing

#endif // OPTIONS_PRICING_PORTFOLIO_HPP