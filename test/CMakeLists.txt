
include_directories (${CMAKE_SOURCE_DIR}/test
                     ${CMAKE_SOURCE_DIR}/src
                    )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test/bin )

add_executable (PtrListValidator
                validate.cpp
                )
set_target_properties(PtrListValidator PROPERTIES COMPILE_DEFINITIONS "GSL_THROW_ON_CONTRACT_VIOLATION")
add_test (NAME PtrListValidator WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test/bin COMMAND PtrListValidator)


add_executable (VPtrListValidator
                validate_vptr.cpp
                )
set_target_properties(VPtrListValidator PROPERTIES COMPILE_DEFINITIONS "GSL_THROW_ON_CONTRACT_VIOLATION")
add_test (NAME VPtrListValidator WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test/bin COMMAND VPtrListValidator)

