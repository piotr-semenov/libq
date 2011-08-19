#include "./../../../Common/verbose-output/rule.hpp"

#include <boost/concept_check.hpp>

namespace utils { namespace verbose_output {
    /// @brief rule for events from integrals state changing
    /// @detailed 1. let x be monotonous increasing integral under observation
    ///           2. let we want to track x every time if it goes through 100
    ///           3. so integrals_rule will raise event if we observe x + y, where y >= 100
    template<typename T>
    class integrals_rule
        :    public rule
    {
        BOOST_CONCEPT_ASSERT((boost::IntegerConcept<T>));

        typedef integrals_rule<T> this_class;
        typedef rule base_class;

    public:
        /// @brief default constructor
        /// @param state observable
        /// @param stride stride we want to log observable through
        rule(T state, T stride)
            :    m_state(state), m_stride(stride){ };

        /// @brief inherited from rule interface
        /// @see rule
        result_type operator(args<0>::type) const;

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

#define _tmpl_head_T_ template<typename T>
#define _cls_name_ integrals_rule<T>

#include "./../../../Common/verbose-output/rules/details/integrals_rule.inl"

}}
