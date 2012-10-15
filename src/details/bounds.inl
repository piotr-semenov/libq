namespace utils {
    namespace {
        // get_super_type::type is type X from T and T1 that range(T), range(T1)
        // are subsets of range(X)
        template<typename T, typename T1>
        class get_super_type
        {
            static size_t const d1 = std::numeric_limits<T>::digits;
            static size_t const d2 = std::numeric_limits<T1>::digits;

        public:
            typedef typename boost::mpl::if_c<d2 < d1, T, T1>::type type;
        };
    }

    // 
    _tmpl_head_ template<typename T>
    inline typename _cls_name_::value_type _cls_name_::bounds::check(T x)
    {
        BOOST_CONCEPT_ASSERT((boost::IntegerConcept<T>));
        typedef get_super_type<value_type, T>::type super_type;

        using boost::numeric::positive_overflow;
        using boost::numeric::negative_overflow;

        // 1. conversion both of types to super_type
        super_type const val = static_cast<super_type>(x);
        super_type const minimum = static_cast<super_type>(min);
        super_type const maximum = static_cast<super_type>(max);

        // 2. value does not fit minimum bound of range of fixed-point format
        if (std::numeric_limits<T>::is_signed && val < minimum) {
            throw negative_overflow();
        }

        // 3. value does not fit maximum bound of range of fixed-point format
        if (val > maximum) {
            throw positive_overflow();
        }

        return static_cast<value_type>(x);
    }
}
