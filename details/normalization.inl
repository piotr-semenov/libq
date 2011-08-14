namespace utils {
    namespace {
        using boost::mpl::if_c;

        // logics: get max type with signless status of T type
        template<typename T>
        class get_max_type
        {
            BOOST_CONCEPT_ASSERT((boost::IntegerConcept<T>));
            static bool const is_signed = std::numeric_limits<T>::is_signed;

        public:
            typedef typename if_c<is_signed, boost::intmax_t, boost::uintmax_t>::type type;
        };
    }

    _tmpl_head_ template<typename storage_type1, size_t fractionals1>
    typename _cls_name_::value_type _cls_name_::normalize_of(storage_type1 const x)
    {
        BOOST_CONCEPT_ASSERT((boost::IntegerConcept<storage_type1>));

        static size_t const shifts = (fractionals > fractionals1) ? (fractionals - fractionals1) : (fractionals1 - fractionals);
        typedef get_max_type<value_type>::type max_type;

        value_type val(0);
        if (fractionals > fractionals1) {
            max_type t(x);
            t <<= shifts;

            val = bounds::internal_cast(t);
        }
        else {
            val = bounds::internal_cast(x >> shifts);
        }

        return val;
    }
}
