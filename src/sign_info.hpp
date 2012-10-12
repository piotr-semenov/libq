/// @brief provides info abount sign of result in fixed-point arithmetical operations

#ifndef INC_SIGN_INFO_HPP_
#define INC_SIGN_INFO_HPP_

#include "./number.hpp"

#include <boost/integer.hpp>
#include <boost/static_assert.hpp>
#include <boost/concept_check.hpp>

#include <limits>

namespace utils {
    // fixed-point number class
    template<typename T, size_t total, size_t fractionals>
    class number;

    /// @brief meta-class to inference sign of some arithmetical operation result.
    ///        In case of float-point built-in types
    template<typename value_type, typename value_type1>
    class sign_info
    {
        BOOST_STATIC_ASSERT((boost::is_float<value_type>::value));
        BOOST_STATIC_ASSERT((boost::is_float<value_type1>::value));

    public:
        enum { sign = true };
    };

    /// @brief in case of fixed-point type
    template<typename T, size_t total, size_t fractionals,
             typename T1, size_t total1, size_t fractionals1>
    class sign_info<number<T, total, fractionals>, number<T1, total1, fractionals1> >
    {
        BOOST_CONCEPT_ASSERT((boost::IntegerConcept<T>));
        BOOST_CONCEPT_ASSERT((boost::IntegerConcept<T1>));

        typedef number<T, total, fractionals> operand_type1;
        typedef number<T1, total1, fractionals1> operand_type2;

    public:
        enum { sign = std::numeric_limits<T>::is_signed || std::numeric_limits<T1>::is_signed };
    };
}

#endif
