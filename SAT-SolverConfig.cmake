include(CMakeFindDependencyMacro)
find_dependency(Trie 1.0.0)
find_dependency(Logging 1.0.0)
include("${CMAKE_CURRENT_LIST_DIR}/SAT-SolverTargets.cmake")
