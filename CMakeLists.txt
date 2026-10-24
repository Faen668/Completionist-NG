cmake_minimum_required(VERSION 3.21)

# standards & flags
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)	# no std modules, VS would build them for every target
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "")
set(SKSEPluginAuthor "Faen90" CACHE STRING "Author shown in the SKSE plugin info")

include(GNUInstallDirs)

# info
project(
	Completionist
	VERSION 7.0
	LANGUAGES CXX
)

# out-of-source builds only
if(${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
	message(FATAL_ERROR "In-source builds are not allowed.")
endif()

# in-place configuration
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/Plugin.h.in
	${CMAKE_CURRENT_BINARY_DIR}/include/Plugin.h
	@ONLY
)

# build and revision come from the clock on every build, see cmake/stamp.cmake
set(STAMP_ARGS
	-DPROJECT_NAME=${PROJECT_NAME}
	-DPROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
	-DPROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}
	-DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
	-DBINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}
)
execute_process(COMMAND ${CMAKE_COMMAND} ${STAMP_ARGS} -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/stamp.cmake)
add_custom_target(
	${PROJECT_NAME}Stamp
	ALL
	COMMAND ${CMAKE_COMMAND} ${STAMP_ARGS} -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/stamp.cmake
	BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/BuildStamp.cpp ${CMAKE_CURRENT_BINARY_DIR}/version.rc
)

# source files
file(
	GLOB_RECURSE SOURCES
	CONFIGURE_DEPENDS
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		src/*.c
		src/*.cpp
		src/*.h
		src/*.hpp
		src/*.inl
)
source_group(
	TREE 
		${CMAKE_CURRENT_SOURCE_DIR}
	FILES 
		${SOURCES}
)

source_group(
	TREE ${CMAKE_CURRENT_BINARY_DIR}
	FILES ${CMAKE_CURRENT_BINARY_DIR}/include/Plugin.h
)

# dependencies, git submodules in extern
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/extern/CommonLibSSE-NG/CMakeLists.txt)
	message(FATAL_ERROR "Submodules missing, run: git submodule update --init --recursive")
endif()
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SKSE_SUPPORT_XBYAK ON CACHE BOOL "" FORCE)
add_subdirectory(extern/CommonLibSSE-NG EXCLUDE_FROM_ALL)
add_subdirectory(extern/DKUtil EXCLUDE_FROM_ALL)
set_target_properties(CommonLibSSE PROPERTIES FOLDER extern)
find_package(spdlog CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(directxtk CONFIG REQUIRED)

# cmake target
# runtime
add_library(
	${PROJECT_NAME}
	SHARED
		${SOURCES}
		${CMAKE_CURRENT_BINARY_DIR}/include/Plugin.h
		${CMAKE_CURRENT_BINARY_DIR}/BuildStamp.cpp
		${CMAKE_CURRENT_BINARY_DIR}/version.rc
		.clang-format
		vcpkg.json
)
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}Stamp)

# include dir
target_include_directories(
	${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/include
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

# linkage
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE
		CommonLibSSE::CommonLibSSE
		DKUtil::DKUtil
		spdlog::spdlog
		imgui::imgui
		Microsoft::DirectXTK
)

# compiler def
if (MSVC)
	add_compile_definitions(_UNICODE)

	target_link_options(
		${PROJECT_NAME} 
		PRIVATE 
			"$<$<CONFIG:RELEASE>:/DEBUG:FULL>"
	)

	target_compile_options(
		${PROJECT_NAME}

		PUBLIC 
			"$<$<CONFIG:RELEASE>:/Zi>"

		PRIVATE
			/MP
			/utf-8
			/W0
			/WX
			/permissive-
			/Zc:alignedNew
			/Zc:auto
			/Zc:__cplusplus
			/Zc:externC
			/Zc:externConstexpr
			/Zc:forScope
			/Zc:hiddenFriend
			/Zc:implicitNoexcept
			/Zc:lambda
			/Zc:noexceptTypes
			/Zc:preprocessor
			/Zc:referenceBinding
			/Zc:rvalueCast
			/Zc:sizedDealloc
			/Zc:strictStrings
			/Zc:ternary
			/Zc:threadSafeInit
			/Zc:trigraphs
			/Zc:wchar_t
			/wd4200 # nonstandard extension used : zero-sized array in struct/union
	)
endif()

# PCH
target_precompile_headers(
	${PROJECT_NAME}
	PRIVATE
		src/PCH.h
)
