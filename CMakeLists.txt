cmake_minimum_required(VERSION 3.5)
project(SAT-Solver VERSION 0.0.1 LANGUAGES C)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PROJECT_BINARY_DIR}/dependencies/install)

# Dependencies
execute_process(COMMAND ../install-dependencies.bash)
find_package(Trie REQUIRED)
find_package(Logging REQUIRED)

# Library
add_library(SAT-Solver
	src/truth-table.c
	src/formular.c
	src/string-util.c
	src/judgement.c
)
target_include_directories(SAT-Solver PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)
target_link_libraries(SAT-Solver PUBLIC Trie::Trie)

# Tests
enable_testing()
add_executable(truth-table test/truth-table.c)
target_link_libraries(truth-table PRIVATE SAT-Solver Logging::Logging)
add_test(NAME truth-table COMMAND truth-table
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test/artifacts/truth-table)
add_executable(judgement test/judgement.c)
target_link_libraries(judgement PRIVATE SAT-Solver)
add_test(NAME judgement COMMAND judgement)

# Install
install(TARGETS SAT-Solver EXPORT SAT-SolverTargets
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
)
install(DIRECTORY include/sat-solver DESTINATION include)
install(EXPORT SAT-SolverTargets
	FILE SAT-SolverTargets.cmake
	NAMESPACE SAT-Solver::
	DESTINATION lib/cmake/SAT-Solver
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("SAT-SolverConfigVersion.cmake"
	VERSION ${SAT-Solver_VERSION}
	COMPATIBILITY SameMajorVersion
)
install(FILES
	"SAT-SolverConfig.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/SAT-SolverConfigVersion.cmake"
	DESTINATION lib/cmake/SAT-Solver
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
