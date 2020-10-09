#
################################################################
#              Making XSTD Library Exportable
################################################################
#
# After all this is done all you need to use the XSTD Library
# is use the CMake "find_package" function and all 
# of the standard variables will be set by that function.
# - XSTD_FOUND
# - XSTD_INCLUDE_DIRS
# - XSTD_LIBRARIES
#
# For Example:
#
# project("My-XSTD-Example")
#
# find_package(XSTD CONFIG REQUIRED)
#
# add_executable(${PROJECT_NAME} src/main.cc)
# target_link_libraries(${PROJECT_NAME} XSTD::XSTD)
#
     
#
# Make a header only library target
#
add_library(${PROJECT_NAME} INTERFACE)

#
# List the include directories for tests and library users
#
target_include_directories(${PROJECT_NAME} 
	INTERFACE
	    "$<BUILD_INTERFACE:${MY_PROJECT_INCDIR}>"
    	"$<INSTALL_INTERFACE:${MY_INSTALL_INCDIR}>"
)

#
# List of all libraries that need linking
#
#target_link_libraries(${PROJECT_NAME} 
#	INTERFACE
#	    "$<$<BOOL:${BLAS_FOUND}>:${BLAS_LIBRARIES}>"
#		"$<$<BOOL:${CBLAS_FOUND}>:${CBLAS_LIBRARIES}>"
#    	"$<$<BOOL:${BLIS_FOUND}>:${BLIS_LIBRARIES}>"
#)

#
# List of C++ features that are needed
#
target_compile_features(${PROJECT_NAME} 
	INTERFACE 
		cxx_std_17
)

install(
	TARGETS ${PROJECT_NAME}
	EXPORT ${PROJECT_NAME}_Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

#[[

#
# Provided default system installation locations
# Note: Most won't be used since it is header only
#
install(
	TARGETS ${PROJECT_NAME}
	EXPORT ${PROJECT_NAME}_Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

#
# Tell CMake to manage versions of this library
#
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

#
# Tell CMake to generate a configuration files
#
configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION
  ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)
  
#
# Copy the file containing the installation targets 
# to the installation folder
#  
install(EXPORT ${PROJECT_NAME}_Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

#
# Copy the file containing the build configuration and informartion 
# about version compatibilty to the installation folder
#    
install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
              "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)
]]

#
# Copy all header files to the installation folder
#
install(DIRECTORY ${MY_PROJECT_INCDIR} DESTINATION ${MY_INSTALL_INCDIR})

