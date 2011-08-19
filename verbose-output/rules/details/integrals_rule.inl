#include <boost/range/irange.hpp>
namespace utils { namespace verbose_output {
    _tmpl_head_T_
        typename _cls_name_::result_type _cls_name_::operator(typename _cls_name_::args<0>::type const& x) const
    {
        T const value = *boost::any_cast<T>(&x);

        bool status = (this->state() + this->stride() >= value) ? true : false;
        this->state(value);

        return status;
    }
}}
