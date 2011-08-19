#include <boost/type_traits/make_signed.hpp>
#include <boost/mpl/eval_if.hpp>

#include <limits>
#include <cmath>

namespace utils { namespace verbose_output {
    namespace {
        // identity map for signed types
        template<typename T>
        struct signed_type_identity
        {
            typedef T type;
        };

        // map from unsigned type to signed analog
        template<typename T>
        struct unsigned_type_map
        {
            typename typedef boost::make_signed<T>::type type;
        };
    }

    _tmpl_head_T_
    typename _cls_name_::result_type _cls_name_::operator()(typename _cls_name_::args<0>::type x)
    {
        using boost::make_signed;
        using boost::mpl::eval_if_c;

        typedef eval_if_c<std::numeric_limits<T>::is_signed, signed_type_identity<T>,
            unsigned_type_map<T> >::type type;
        type const value = *boost::any_cast<T>(&x);

        type const pt = this->state() + this->stride();
        if (std::abs(pt) <= std::abs(value)) {
            this->state(static_cast<T>(pt));

            return true;
        }

        return false;
    }
}}
