cmake_minimum_required(VERSION 3.5)
project(sat-solver VERSION 0.0.1 LANGUAGES C)

# Library
add_library(sat-solver
	src/truth-table.c
	src/formular.c
	src/string-util.c
	src/judgement.c
)
target_include_directories(sat-solver PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)
add_subdirectory(libs)
target_link_libraries(sat-solver PRIVATE
	trie
	logging
)

# Tests
enable_testing()
add_executable(truth-table test/truth-table.c)
target_link_libraries(truth-table PRIVATE sat-solver logging)
add_test(NAME truth-table COMMAND truth-table
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test/artifacts/truth-table)
add_executable(judgement test/judgement.c)
target_link_libraries(judgement PRIVATE sat-solver)
add_test(NAME judgement COMMAND judgement)

# Install
install(TARGETS sat-solver EXPORT SATSolverTargets
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
)
install(DIRECTORY include/sat-solver DESTINATION include)
install(EXPORT SATSolverTargets
	FILE SATSolverTargets.cmake
	NAMESPACE SATSolver::
	DESTINATION lib/cmake/sat-solver
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(SATSolverConfigVersion.cmake
	COMPATIBILITY SameMajorVersion
)

install(FILES build/SATSolverConfigVersion.cmake
	DESTINATION lib/cmake/sat-solver
)

#CPack
set(CPACK_PACKAGE_CONTACT "arwed.mett@googlemail.com")

if(APPLE)
	set(CPACK_GENERATOR "TGZ;ZIP;productbuild")
elseif(UNIX)
	set(CPACK_GENERATOR "TGZ;ZIP;RPM;DEB")
elseif(WIN32)
	set(CPACK_GENERATOR "ZIP;NSIS;NSIS64")
else()
	set(CPACK_GENERATOR "TGZ;ZIP")
endif()

set(CPACK_BUNDLE_NAME ${PROJECT_NAME})
include(CPack)
