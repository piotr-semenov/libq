#ifndef INC_NUMERALS_RULE_HPP_
#define INC_NUMERALS_RULE_HPP_

#include "./../../../Common/verbose-output/rule.hpp"

#include <boost/static_assert.hpp>

namespace utils { namespace verbose_output {
    /// @brief rule for events from integrals state changing
    /// @detailed 1. let x be monotonous increasing integral under observation
    ///           2. let we want to track x every time if it goes through 100
    ///           3. so numerals_rule will raise event if we observe x + y, where y >= 100
    template<typename T>
    class numerals_rule
        :    public rule
    {
        BOOST_STATIC_ASSERT((boost::is_arithmetic<T>::value));

        typedef numerals_rule<T> this_class;
        typedef rule base_class;

    public:
        /// @brief default constructor
        /// @param state observable
        /// @param stride stride we want to log observable through
        numerals_rule(T state, T stride)
            :    m_state(state), m_stride(stride){ };

        /// @brief inherited from rule interface
        /// @see rule
        result_type operator()(args<0>::type);

    private:
        // logics: we could not adaptively change rule behavior.
        //         Correctly, it should be done by changing rule
        // private setter and getter for observable state
        T state() const{ return m_state; }
        this_class& state(T val){ m_state = val; return *this; }

        // just getter for observations stride
        T stride() const{ return m_stride; }

        T m_state;
        T m_stride;
    };
}}

#define _tmpl_head_T_ template<typename T>
#define _cls_name_ numerals_rule<T>

#include "./../../../Common/verbose-output/rules/details/numerals_rule.inl"

#endif
