#ifndef INC_CLEARNER_HPP_
#define INC_CLEARNER_HPP_

#include "./functor_info.hpp"

namespace utils {
    template<typename T>
    class cleaner
        :    public functor_info<void (T&)>
    {
    public:
        /// @brief 
        result_type operator ()(typename args<0>::type val) const{ val = 0; }
    };
}

#endif
