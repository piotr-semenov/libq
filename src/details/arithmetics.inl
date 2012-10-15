namespace utils {
    namespace {
        using boost::mpl::bool_;
    }

    // SUMMATION
    //////////////////////////////////////////////////////////////////////////
    // 1. if summation is non-closed operation
    _tmpl_head_ template<typename Other_type>
    typename _cls_name_::sum_type _cls_name_::add(_cls_name_ const& a, Other_type const& b, bool_<true>::type) const
    {
        typedef sum_type::value_type sum_value_type;

        sum_value_type const val = boost::numeric_cast<sum_value_type>(a.value()) + this_class(b).value();
        return sum_type::wrap(val);
    }

    // 2. if summation is closed operation
    _tmpl_head_ template<typename Other_type>
    _cls_name_ _cls_name_::add(_cls_name_ const& a, Other_type const& b, bool_<false>::type) const
    {
        value_type const sum = a.value() + this_class(b).value();

        value_type const val = static_cast<value_type>(bounds::check(sum) % this_class::mod);
        return this_class::wrap(val);
    }

    // 3. summation operator
    _tmpl_head_ template<typename Other_type>
    typename _cls_name_::sum_type _cls_name_::operator +(Other_type const& x) const
    { 
        sum_type const val = add(*this, x, bool_<!(sum_info<this_class>::is_closed::value)>::type());

        return val;
    }

    // SUBTRACTION
    //////////////////////////////////////////////////////////////////////////
    // 1. if subtraction is non-closed operation
    _tmpl_head_ template<typename Other_type>
    typename _cls_name_::diff_type _cls_name_::subtract(_cls_name_ const& a, Other_type const& b, bool_<true>::type) const
    {
        typedef diff_type::value_type diff_value_type;

        diff_value_type const val = boost::numeric_cast<diff_value_type>(a.value()) - this_class(b).value();
        return diff_type::wrap(val);
    }

    // 2. if subtraction is closed operation
    _tmpl_head_ template<typename Other_type>
    _cls_name_ _cls_name_::subtract(_cls_name_ const& a, Other_type const& b, bool_<false>::type) const
    {
        value_type const diff = a.value() - this_class(b).value();

        value_type const val = static_cast<value_type>(diff % this_class::mod);
        return this_class::wrap(val);
    }

    // 3. subtraction operator
    _tmpl_head_ template<typename Other_type>
    typename _cls_name_::diff_type _cls_name_::operator -(Other_type const& x) const
    {
        diff_type const val = subtract(*this, x, mpl::bool_<!(diff_info<this_class>::is_closed::value)>::type());
        return val;
    }

    // MULTIPLICATION
    //////////////////////////////////////////////////////////////////////////
    // 1. if multiplication is non-closed operation
    _tmpl_head_ template<typename Other_type>
    typename _cls_name_::product_type _cls_name_::product(_cls_name_ const& a, Other_type const& b, bool_<true>::type) const
    {
        typedef product_type::value_type product_value_type;

        product_value_type const val = boost::numeric_cast<product_value_type>(a.value()) * this_class(b).value();
        return product_type::wrap(val);
    }

    // 2. if multiplication is closed operation
    // 2.1 case of integral value
    _tmpl_head_ template<typename Other_type>
    _cls_name_ _cls_name_::product(_cls_name_ const& a, Other_type const& b, bool_<false>::type) const
    {
        BOOST_CONCEPT_ASSERT((boost::IntegerConcept<Other_type>));

        this_class x = a;
        x *= b;

        return x;
    }

    // 2.2 case of fixed-point of another format
    _tmpl_head_ template<typename storage_type1, size_t total1, size_t fractionals1>
    _cls_name_ _cls_name_::product(_cls_name_ const& a, number<storage_type1, total1, fractionals1> const& b, bool_<false>::type) const
    {
        this_class x;

        value_type const val = a.value() * static_cast<value_type>(b.value());
        x.value(val >> fractionals1);

        return x;
    }

    // 3. multiplication operator
    _tmpl_head_ template<typename Other_type>
    typename _cls_name_::product_type _cls_name_::operator *(Other_type const& x) const
    {
        product_type const val = product(*this, x, bool_<!(product_info<this_class>::is_closed::value)>::type());
        return val;
    }

    // DIVISION
    //////////////////////////////////////////////////////////////////////////
    // 1. if division is non-closed operation
    _tmpl_head_ template<typename Other_type>
    typename _cls_name_::quotient_type _cls_name_::divide(_cls_name_ const& a, Other_type const &b, bool_<true>::type) const
    {
        typedef quotient_type::value_type quotient_value_type;

        quotient_value_type const shifted = (boost::numeric_cast<quotient_value_type>(a.value()) << fractionals);
        quotient_value_type const converted = this_class(b).value();
        quotient_value_type const val = shifted / converted;

        return quotient_type::wrap(val << fractionals);
    }

    // 2. if division is closed operation
    // 2.1 case of integral value
    _tmpl_head_ template<typename Other_type>
    _cls_name_ _cls_name_::divide(_cls_name_ const& a, Other_type const &b, bool_<false>::type) const
    {
        BOOST_CONCEPT_ASSERT((boost::IntegerConcept<Other_type>));

        this_class x = a;
        x *= b;

        return x;
    }

    // 2.2 case of another fixed-point
    _tmpl_head_ template<typename storage_type1, size_t total1, size_t fractionals1>
    _cls_name_ _cls_name_::divide(_cls_name_ const& a, number<storage_type1, total1, fractionals1> const&b, bool_<false>::type) const
    {
        this_class x;

        // we have no additional precision for quotient due to operation closeness
        value_type const val = (static_cast<value_type>(1u) << fractionals1) / static_cast<value_type>(b.value());
        x.value(a.value() * val);

        return x;
    }

    // 3. division operator
    _tmpl_head_ template<typename Other_type>
    typename _cls_name_::quotient_type _cls_name_::operator /(Other_type const& x) const
    {
        quotient_type const val = divide(*this, x, mpl::bool_<!(quotient_info<this_class>::is_closed::value)>::type());
        return val;
    }
}
