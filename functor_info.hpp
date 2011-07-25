// @brief provides info about functor

#ifndef INC_FUNCTOR_INFO_HPP_
#define INC_FUNCTOR_INFO_HPP_

#include <boost/function_types/function_arity.hpp>
#include <boost/function_types/components.hpp>

#include <boost/mpl/at.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>

#include <boost/mpl/int.hpp>

namespace utils {
    /// @brief retrieves info from function type
    template<typename T>
    class functor_info
    {
        typedef typename boost::function_types::components<T> type_info;
        typedef typename boost::mpl::pop_front<typename type_info::types>::type arguments_info;

    public:
        static const size_t arity = type_info::function_arity::value;     ///< function arity

        /// @brief function result type
        typedef typename boost::mpl::front<typename type_info::types>::type result_type;

        /// @brief function argument types
        template<size_t i>
        struct args
        {
        public:
            /// @brief type of function i-th argument
            typedef typename boost::mpl::at<arguments_info, boost::mpl::int_<i> >::type type;
        };
    };
}

#endif
