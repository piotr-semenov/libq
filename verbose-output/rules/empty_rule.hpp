#ifndef INC_EMPTY_RULE_HPP_
#define INC_EMPTY_RULE_HPP_

#include "./../../../Common/verbose-output/rule.hpp"

namespace utils { namespace verbose_output {
    /// @brief rule that raises logging event for each changing for
    ///        state of observable
    template<typename T>
    class empty_rule
        :    public rule
    {
    public:

        /// @brief inherited from rule interface
        /// @see rule
        result_type operator()(args<0>::type) const{ return true; }
    };
}}

#endif
