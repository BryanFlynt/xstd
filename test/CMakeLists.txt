#
# Tests Directories
#

#
# =================================================
# Build Catch2 main() into Static Library
# =================================================
#
set(CATCH_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/test)
add_library(Catch2 STATIC "")
target_sources(Catch2 PRIVATE catch_main.cpp)
target_include_directories(Catch2 PUBLIC ${CATCH_INCLUDE_DIR})


#
# Function to add a test and include catch.hpp
#
function(add_catch_test test_name)
	set(test_target test_${test_name})  # Target becomes test_xxxx
	set(test_source ${test_name}.cpp)   # Source becomes xxxx.cpp
	add_executable(${test_target} ${test_source})
	target_include_directories(${test_target} 
		PRIVATE 
			$<BUILD_INTERFACE:${MY_PROJECT_INCDIR}> # Pick up unisntalled library
    		$<BUILD_INTERFACE:${MY_INSTALL_INCDIR}> # Pick up processed configure.hpp
    )
   	target_link_libraries(${test_target}
		PRIVATE
			Catch2
	)
	set_target_properties(${test_target}
		PROPERTIES
    		CXX_STANDARD 20
    		CXX_STANDARD_REQUIRED YES
    		CXX_EXTENSIONS NO
	)
	target_compile_options(${test_target}
		PRIVATE
	)
	add_test(${test_name} ${test_target})
endfunction()



add_subdirectory(algorithm)
add_subdirectory(array)
add_subdirectory(chrono)
#add_subdirectory(fstream)
add_subdirectory(functional)
add_subdirectory(iostream)
add_subdirectory(iterator)
add_subdirectory(memory)
add_subdirectory(polynomial)
add_subdirectory(set)
add_subdirectory(string)
add_subdirectory(tuple)
add_subdirectory(type_traits)
add_subdirectory(utility)
add_subdirectory(vector)

