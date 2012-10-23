/// @brief provides a proxy class to deal with integer below fixed-point number

#include <boost/operators.hpp>

namespace utils {
    template<typename storage_type, size_t total, size_t fractionals, class op,
    class up>
    class number;

    /// @brief proxy class for dealing with fixed-point numbers as with integers
    template<typename T, size_t n, size_t f, class op, class up>
    class as_native_proxy
        :    boost::operators<as_native_proxy<T, n, f, op, up> >,
             boost::operators2<as_native_proxy<T, n, f, op, up>, T>,
             boost::shiftable<as_native_proxy<T, n, f, op, up> >,
             boost::shiftable2<as_native_proxy<T, n, f, op, up>, T>
    {
        typedef as_native_proxy<T, n, f, op, up> this_class;
        typedef number<T, n, f, op, up> fixed_point_class;

    public:
        /// @brief copy operator
        template<typename T>
        void operator =(T val){ this->m_value = val; }

        /// @brief less operator. It is the same as one for native integral types
        /// is
        bool operator <(this_class const& x) const{ return x.value() < this->value(); }
        bool operator <(T x) const{ return x < this->value(); }

        /// @brief equality comparison operator. It is the same as one for native
        /// integral types is
        bool operator ==(this_class const& x) const{ return x.value() == this->value(); }
        bool operator ==(T x) const{ return x == this->value(); }

        /// @brief addition operator. It checks if result fits the bounds drawn
        /// from n bits of length
        this_class& operator +=(this_class const& x)
        {
            fixed_point_class::bounds::check(this->value() += x.value());

            return *this;
        }
        this_class& operator +=(T x)
        {
            fixed_point_class::bounds::check(this->value() += x);

            return *this;
        }

        /// @brief subtraction operator. It checks if result fits the bounds drawn
        /// from n bits of length (signed/unsigned).
        this_class& operator -=(this_class const& x)
        {
            fixed_point_class::bounds::check(this->value() -= x.value());

            return *this;
        }
        this_class& operator -=(T x)
        {
            fixed_point_class::bounds::check(this->value() -= x);

            return *this;
        }

        /// @brief multiplication operator. It checks if result fits the bounds
        /// drawn from n bits of length.
        this_class& operator *=(this_class const& x)
        {
            fixed_point_class::bounds::check(this->value() *= x.value());

            return *this;
        }
        this_class& operator *=(T x)
        {
            fixed_point_class::bounds::check(this->value() *= x);

            return *this;
        }

        /// @brief division operator. It checks if result fits the bounds drawn
        /// from n bits of length.
        this_class& operator /=(this_class const& x)
        {
            fixed_point_class::bounds::check(this->value() /= x.value());

            return *this;
        }
        this_class& operator /=(T x)
        {
            fixed_point_class::bounds::check(this->value() /= x);

            return *this;
        }
        /// @brief mod operator. It is the same as one for native types is.
        this_class& operator %=(this_class const& x)
        {
            this->value() %= x.value();

            return *this;
        }
        this_class& operator %=(T x){ this->value() %= x; return *this; }

        /// @brief xor operator. It checks if result fits the bounds drawn from
        /// n bits of length
        this_class& operator ^=(this_class const& x)
        {
            fixed_point_class::bounds::check(this->value() ^= x.value());

            return *this;
        }
        this_class& operator ^=(T x)
        {
            fixed_point_class::bounds::check(this->value() ^= x);

            return *this;
        }

        /// @brief and operator. It is the same as one for native types is.
        this_class& operator &=(this_class const& x)
        {
            this->value() &= x.value();
            
            return *this;
        }
        this_class& operator &=(T x){ this->value() &= x; return *this; }

        /// @brief or operator. It checks if result fits the bounds drawn from
        /// n bits of length
        this_class& operator |=(this_class const& x)
        {
            fixed_point_class::bounds::check(this->value() |= x.value());

            return *this;
        }
        this_class& operator !=(T x)
        {
            fixed_point_class::bounds::check(this->value() |= x);

            return *this;
        }

        /// @brief increment operator. It checks if result fits the bounds drawn
        /// from n bits of length.
        this_class& operator ++()
        {
            fixed_point_class::bounds::check(this->value()++);

            return *this;
        }

        /// @brief decrement operator. It checks if result fits the bounds drawn
        /// from n bits of length.
        this_class& operator --()
        {
            fixed_point_class::bounds::check(this->value()--);

            return *this;
        }

        /// @brief left shift operator. It checks if result fits the bounds drawn
        /// from n bits of length.
        this_class& operator <<=(this_class const& x)
        {
            fixed_point_class::bounds::check(this_value() >>= x.value());

            return *this;
        }
        this_class& operator <<=(T x)
        {
            fixed_point_class::bounds::check(this_value() >>= x);

            return *this;
        }

        /// @brief right shift operator. It is the same as one for native integers
        /// is.
        this_class& operator >>=(this_class const& x)
        {
            this->value() >>= x.value();

            return *this;
        }
        this_class& operator >>=(T x){ this->value() >>= x; return x; }

    private:
        T& m_value;
        T& value(){ return this->m_value; }

        as_native_proxy(fixed_point_class& x)
            :    m_value(x.m_value){};

        template<>
        friend this_class as_native(fixed_point_class&);
    };
}
