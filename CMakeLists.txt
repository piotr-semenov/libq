cmake_minimum_required(VERSION 3.6)

project(libq VERSION 1.0.0.0) 

# set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.66 COMPONENTS log unit_test_framework)
link_directories(${Boost_LIBRARY_DIRS})

enable_testing()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (MSVC)
    add_definitions(
        -D_SCL_SECURE_NO_WARNINGS
        -D_USE_MATH_DEFINES
        -DNOMINMAX
        )
    add_compile_options(/bigobj)
endif(MSVC)

set(CXX_USED_FEATURES
    #cxx_aggregate_default_initializers
    cxx_alias_templates
    cxx_alignas
    cxx_alignof
    cxx_attributes
    cxx_attribute_deprecated
    cxx_auto_type
    cxx_binary_literals
    cxx_constexpr
    cxx_contextual_conversions
    #cxx_decltype_incomplete_return_types
    cxx_decltype
    cxx_decltype_auto
    cxx_default_function_template_args
    cxx_defaulted_functions
    cxx_defaulted_move_initializers
    cxx_delegating_constructors
    cxx_deleted_functions
    cxx_digit_separators
    cxx_enum_forward_declarations
    cxx_explicit_conversions
    cxx_extended_friend_declarations
    cxx_extern_templates
    cxx_final
    cxx_func_identifier
    cxx_generalized_initializers
    cxx_generic_lambdas
    cxx_inheriting_constructors
    cxx_inline_namespaces
    cxx_lambdas
    cxx_lambda_init_captures
    cxx_local_type_template_args
    cxx_long_long_type
    cxx_noexcept
    cxx_nonstatic_member_init
    cxx_nullptr
    cxx_override
    cxx_range_for
    cxx_raw_string_literals
    cxx_reference_qualified_functions
    #cxx_relaxed_constexpr
    cxx_return_type_deduction
    cxx_right_angle_brackets
    cxx_rvalue_references
    cxx_sizeof_member
    cxx_static_assert
    cxx_strong_enums
    cxx_thread_local
    cxx_trailing_return_types
    cxx_unicode_literals
    cxx_uniform_initialization
    cxx_unrestricted_unions
    cxx_user_literals
    cxx_variable_templates
    cxx_variadic_macros
    cxx_variadic_templates
    cxx_template_template_parameters
)

add_library(libq INTERFACE)
set(SRC_libq
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/loop_unroller.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/type_promotion.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/fixed_point.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/type_traits.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/fixed_point_common.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/sign.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/sum_traits.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/round.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/remainder.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/numeric_limits.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/fmod.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/mult_of.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/floor.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/div_of.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/fabs.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/arithmetics_safety.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/ceil.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/tanh.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/sqrt.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/tan.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/sinh.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/sin.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/lut/pow2_lut.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/lut/lut.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/lut/inv_pow2_lut.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/lut/hyperbolic_scale.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/lut/arctanh_lut.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/lut/circular_scales.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/lut/arctan_lut.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/exp.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/log.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/cosh.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/cos.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/atan.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/atanh.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/asin.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/asinh.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/acos.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/acosh.hpp
    )
target_sources(libq INTERFACE ${SRC_libq})

target_include_directories(libq INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIR})
target_link_libraries(libq INTERFACE Boost::boost)
target_compile_features(libq INTERFACE ${CXX_USED_FEATURES})

set(LIBQ_TEST ON CACHE BOOL "Include libq tests")

if(LIBQ_TEST)
    add_subdirectory(tests)
endif(LIBQ_TEST)
