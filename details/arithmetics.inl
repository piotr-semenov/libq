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
        return sum_type::create(val);
    }

    // 2. if summation is closed operation
    _tmpl_head_ template<typename Other_type>
    _cls_name_ _cls_name_::add(_cls_name_ const& a, Other_type const& b, bool_<false>::type) const
    {
        value_type const sum = a.value() + this_class(b).value();

        value_type const val = static_cast<value_type>(bounds::internal_cast(sum) % this_class::mod);
        return this_class::create(val);
    }

    // 3. summation operator
    _tmpl_head_ template<typename Other_type>
    typename _cls_name_::sum_type _cls_name_::operator +(Other_type const& x) const
    { 
        sum_type const val = add(*this, x, bool_<!(sum_info<this_class>::closing_info::value)>::type());

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
        return diff_type::create(val);
    }

    // 2. if subtraction is closed operation
    _tmpl_head_ template<typename Other_type>
    _cls_name_ _cls_name_::subtract(_cls_name_ const& a, Other_type const& b, bool_<false>::type) const
    {
        value_type const diff = a.value() - this_class(b).value();

        value_type const val = static_cast<value_type>(diff % this_class::mod);
        return this_class::create(val);
    }

    // 3. subtraction operator
    _tmpl_head_ template<typename Other_type>
    typename _cls_name_::diff_type _cls_name_::operator -(Other_type const& x) const
    {
        diff_type const val = subtract(*this, x, mpl::bool_<!(diff_info<this_class>::closing_info::value)>::type());
        return val;
    }
}
