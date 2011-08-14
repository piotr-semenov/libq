namespace utils {
    _tmpl_head_ template<typename storage_type1, size_t fractionals1>
    typename _cls_name_::value_type _cls_name_::normalize_of(storage_type1 const x)
    {
        BOOST_CONCEPT_ASSERT((boost::IntegerConcept<storage_type1>));

        static size_t const shifts = (fractionals > fractionals1) ? (fractionals - fractionals1) : (fractionals1 - fractionals);
        static size_t const max_bits = std::numeric_limits<boost::uintmax_t>::digits;
        typedef boost::uintmax_t maxint_type;

        //if (std::numeric_limits<value_type>::is_signed) {
        //    max_bits = std::numeric_limits<boost::intmax_t>::digits;
        //    typedef boost::intmax_t maxint_type;
        //}
        //else {
        //    max_bits = std::numeric_limits<boost::uintmax_t>::digits;
        //}

        value_type val(0);
        value_type const converted = boost::numeric_cast<value_type>(x);

        using boost::mpl::eval_if_c;
        if (fractionals > fractionals1) {
            struct t1
            {
                typedef boost::int_t<total + shifts>::least type;
            };

            struct t2
            {
                typedef maxint_type type;
            };

            typedef eval_if_c<total + shifts < max_bits, t1, t2>::type extended_type;
            extended_type t = static_cast<extended_type>(x);
            t <<= shifts;

            //bounds::check(t);
            val = static_cast<value_type>(t);
        }
        else {
            //                bounds::check(x >> shifts);
            val = static_cast<value_type>(x >> shifts);
        }

        return val;
    }
}
