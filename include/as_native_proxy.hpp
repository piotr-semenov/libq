/// @brief provides a proxy class to deal with integer below fixed-point number

#ifndef INC_CORE_AS_NATIVE_PROXY_HPP_
#define INC_CORE_AS_NATIVE_PROXY_HPP_

#include <boost/integer/integer_mask.hpp>
#include <boost/noncopyable.hpp>

namespace core {
    template<typename T, size_t n, size_t f, class op, class up>
    class fixed_point;

    /// @brief proxy class for dealing with fixed-point numbers as with integers
    template<typename T, size_t n, size_t f, class op, class up>
    class as_native_proxy
        :    public boost::noncopyable
    {
        typedef as_native_proxy<T, n, f, op, up> this_class;
        typedef fixed_point<T, n, f, op, up> fixed_point_class;

    public:
        template<typename U>
        operator U() const
        {
            BOOST_STATIC_ASSERT((boost::is_arithmetic<U>::value));

            return U(this->value()); 
        }

        /// @brief copy operator
        template<typename U>
        void operator =(U val)
        {
            BOOST_STATIC_ASSERT((boost::is_integral<U>::value));

            this->m_value = val;
        }

        // motivation: boost ordered operators from operators.hpp are not the option
        // because it need the copy operator
#define OPERATOR(f) \
    template<typename U> \
    bool operator ##f(U const& x) const \
    { \
        BOOST_STATIC_ASSERT((boost::is_integral<U>::value)); \
        return x ##f this->value(); \
    } \
    template<> \
    bool operator ##f(this_class const& x) const{ return x.value() ##f this->value(); }

        OPERATOR(<);
        OPERATOR(>);
        OPERATOR(==);
        OPERATOR(<=);
        OPERATOR(>=);
#undef OPERATOR

#define CHECKED_ARITHMETICS(f, val) \
    fixed_point_class::handle_underflow( \
        this->value(), \
        fixed_point_class::handle_overflow(this->value() f val, op()), \
        up() \
    )
#define OPERATOR(f) \
    template<typename U> \
    this_class& operator ##f(U const& x) \
    { \
        CHECKED_ARITHMETICS(##f, x); \
        return *this; \
    } \
    template<> \
    this_class& operator ##f(this_class const& x) \
    { \
        CHECKED_ARITHMETICS(##f, x.value()); \
        return *this; \
    }

        OPERATOR(+=);
        OPERATOR(-=);
        OPERATOR(*=);
        OPERATOR(/=);
        OPERATOR(%=);
        OPERATOR(^=);
        OPERATOR(&=);
        OPERATOR(|=);
        OPERATOR(<<=);
        OPERATOR(>>=);
#undef OPERATOR
#undef CHECKED_ARITHMETICS

        /// @brief increment operator. It checks if result fits the bounds drawn
        /// from n bits of length.
        this_class& operator ++()
        {
            fixed_point_class::handle_overflow(this->value()++, op());

            return *this;
        }

        /// @brief decrement operator. It checks if result fits the bounds drawn
        /// from n bits of length.
        this_class& operator --()
        {
            fixed_point_class::handle_overflow(this->value()--, op());

            return *this;
        }

#define OPERATOR(f) \
    template<typename U> \
    friend T operator ##f(this_class const& x, U const& y) \
    { \
        BOOST_STATIC_ASSERT((boost::is_integral<U>::value)); \
        return x.value() + y; \
    } \
    template<typename U> \
    friend U operator ##f(U const& x, this_class const& y) \
    { \
        BOOST_STATIC_ASSERT((boost::is_integral<U>::value)); \
        return y.value() + x; \
    }

        OPERATOR(+);
        OPERATOR(-);
        OPERATOR(*);
        OPERATOR(/);
        OPERATOR(%);
        OPERATOR(^);
        OPERATOR(|);
        OPERATOR(&);
#undef OPERATOR

        T operator ~() const{ return this->m_value ^ T(boost::low_bits_mask_t<n>::sig_bits); }

    private:
        T& m_value;
        T& value(){ return this->m_value; }
        T value() const{ return this->m_value; }

        as_native_proxy(fixed_point_class& x)
            :    m_value(x.m_value){};
        as_native_proxy(this_class const&){};

        template<typename T1, size_t n1, size_t f1, class op1, class up1>
        friend as_native_proxy<T1, n1, f1, op1, up1> as_native(fixed_point<T1, n1, f1, op1, up1>&);

        template<typename T1, size_t n1, size_t f1, class op1, class up1>
        friend class fixed_point;
    };
}

#endif
