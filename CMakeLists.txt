cmake_minimum_required(VERSION 3.6)

project(libq VERSION 1.0.0.0)

# set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.66 COMPONENTS log unit_test_framework)
link_directories(${Boost_LIBRARY_DIRS})

enable_testing()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(MSVC)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS -D_USE_MATH_DEFINES -DNOMINMAX)
  add_compile_options(/bigobj)
endif(MSVC)

set(CXX_USED_FEATURES
    # cxx_aggregate_default_initializers
    cxx_alias_templates
    cxx_alignas
    cxx_alignof
    cxx_attributes
    cxx_attribute_deprecated
    cxx_auto_type
    cxx_binary_literals
    cxx_constexpr
    cxx_contextual_conversions
    # cxx_decltype_incomplete_return_types
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
    # cxx_relaxed_constexpr
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
    cxx_template_template_parameters)

add_library(libq INTERFACE)
target_sources(
  libq
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/acos.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/acosh.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/asin.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/asinh.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/atan.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/atanh.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/cos.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/cosh.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/exp.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/log.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/lut/arctan_lut.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/lut/arctanh_lut.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/lut/circular_scales.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/lut/hyperbolic_scale.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/lut/inv_pow2_lut.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/lut/lut.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/lut/pow2_lut.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/sin.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/sinh.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/sqrt.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/tan.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/CORDIC/tanh.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/arithmetics_safety.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/ceil.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/div_of.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/fabs.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/fixed_point_common.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/floor.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/fmod.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/loop_unroller.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/mult_of.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/numeric_limits.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/remainder.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/round.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/sign.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/sum_traits.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/details/type_traits.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/fixed_point.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/libq/type_promotion.hpp)

target_include_directories(libq INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                                          ${Boost_INCLUDE_DIR})
target_link_libraries(libq INTERFACE Boost::boost)
target_compile_features(libq INTERFACE ${CXX_USED_FEATURES})

set(LIBQ_TEST
    ON
    CACHE BOOL "Include libq tests")

if(LIBQ_TEST)
  add_subdirectory(tests)
endif(LIBQ_TEST)

find_package(Doxygen REQUIRED dot OPTIONAL_COMPONENTS mscgen)

if(DOXYGEN_FOUND)
  set(DOXYGEN_GENERATE_HTML YES)
  set(DOXYGEN_GENERATE_LATEX NO)
  set(DOXYGEN_GENERATE_MAN NO)
  set(DOXYGEN_CREATE_SUBDIRS YES)
  set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
  set(DOXYGEN_EXTRACT_ALL YES)
  set(DOXYGEN_EXTRACT_PRIVATE YES)
  set(DOXYGEN_EXTRACT_PRIV_VIRTUAL YES)
  set(DOXYGEN_EXTRACT_PACKAGE YES)
  set(DOXYGEN_EXTRACT_STATIC YES)
  set(DOXYGEN_EXTRACT_LOCAL_CLASSES YES)
  set(DOXYGEN_EXTRACT_LOCAL_METHODS YES)
  set(DOXYGEN_EXTRACT_ANON_NSPACES YES)
  set(DOXYGEN_GENERATE_TODOLIST YES)
  set(DOXYGEN_GENERATE_TESTLIST YES)
  set(DOXYGEN_GENERATE_BUGLIST YES)
  set(DOXYGEN_GENERATE_DEPRECATEDLIST YES)
  set(DOXYGEN_WARN_IF_UNDOCUMENTED NO)
  set(DOXYGEN_SOURCE_BROWSER YES)
  set(DOXYGEN_INLINE_SOURCES YES)
  set(DOXYGEN_STRIP_CODE_COMMENTS NO)
  set(DOXYGEN_REFERENCED_BY_RELATION YES)
  set(DOXYGEN_REFERENCES_RELATION YES)
  set(DOXYGEN_GENERATE_TREEVIEW YES)
  set(DOXYGEN_MACRO_EXPANSION YES)
  set(DOXYGEN_UML_LOOK YES)
  set(DOXYGEN_INTERNAL_DOCS YES)
  set(DOXYGEN_SHOW_GROUPED_MEMB_INC YES)
  set(DOXYGEN_QUIET YES)
  set(DOXYGEN_HIDE_UNDOC_RELATIONS NO)
  set(DOXYGEN_TEMPLATE_RELATIONS YES)
  set(DOXYGEN_CALL_GRAPH YES)
  set(DOXYGEN_CALLER_GRAPH YES)
  set(DOXYGEN_USE_MATHJAX YES)
  set(DOXYGEN_FORCE_LOCAL_INCLUDES YES)

  set(DOXYGEN_ALLEXTERNALS YES)
  set(DOXYGEN_MATHJAX_EXTENSIONS
      TeX/AMSsymbols
      CACHE STRING "DOXYGEN_MATHJAX_EXTENSIONS")
  set(DOXYGEN_MATHJAX_FORMAT NativeMML)
  set(DOXYGEN_UML_LIMIT_NUM_FIELDS 50)
  set(DOXYGEN_DOT_GRAPH_MAX_NODES 150)
  set(DOXYGEN_DOT_IMAGE_FORMAT
      svg
      CACHE STRING "DOXYGEN_DOT_IMAGE_FORMAT")
  set(DOXYGEN_STRIP_FROM_PATH ${CMAKE_CURRENT_SOURCE_DIR})
  set(DOXYGEN_ENABLE_PREPROCESSING YES)

  set(DOXYGEN_PREDEFINED "__attribute__((x))=")

  set(DOXYGEN_WARN_LOGFILE
      ${CMAKE_CURRENT_BINARY_DIR}/doxygen.log
      CACHE FILEPATH "WARN_LOGFILE")
  set(DOXYGEN_GENERATE_TAGFILE
      ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.tag
      CACHE FILEPATH "GENERATE_TAGFILE")
  set(DOXYGEN_DOTFILE_DIRS
      ${CMAKE_CURRENT_BINARY_DIR}/dot
      CACHE PATH "DOTFILE_DIRS")
  set(DOXYGEN_EXAMPLE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/examples/)
  set(DOXYGEN_PROJECT_LOGO ${CMAKE_CURRENT_SOURCE_DIR}/logo.png)

  if(EXISTS dot/targets.dot)
    set(TARGETS_PAGE ${CMAKE_CURRENT_SOURCE_DIR}/targets.dox)
  else()
    set(TARGETS_PAGE ${CMAKE_CURRENT_SOURCE_DIR}/no_targets.dox)
  endif(EXISTS dot/targets.dot)

  doxygen_add_docs(
    doxygen-${PROJECT_NAME} ${TARGETS_PAGE}
    ${CMAKE_CURRENT_SOURCE_DIR}/README.md ${CMAKE_CURRENT_SOURCE_DIR}/examples/
    ${CMAKE_CURRENT_SOURCE_DIR}/libq/ ${CMAKE_CURRENT_SOURCE_DIR}/tests/)
  set(FILES_TO_CLEAN_
      ${FILES_TO_CLEAN_} ${CMAKE_CURRENT_BINARY_DIR}/doxygen.log
      ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.tag
      ${CMAKE_CURRENT_BINARY_DIR}/html)
  set_property(
    DIRECTORY
    APPEND
    PROPERTY ADDITIONAL_CLEAN_FILES ${FILES_TO_CLEAN_})
endif(DOXYGEN_FOUND)
