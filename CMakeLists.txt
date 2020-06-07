cmake_minimum_required(VERSION 2.8.5)
project(cd LANGUAGES C)

if(NOT AMIGA)
	message(SEND_ERROR "This project only compiles for Amiga")
endif()

set(CMAKE_C_STANDARD 11)
file(GLOB_RECURSE SOURCES src/*.c src/*.h)

if(ELF2HUNK)
	set(CD_EXECUTABLE cd.elf)
	add_executable(${CD_EXECUTABLE} ${SOURCES})
	target_link_libraries(${CD_EXECUTABLE} -Wl,-Map=cd.map)
	add_custom_command(
		TARGET ${CD_EXECUTABLE} POST_BUILD
		COMMAND ${ELF2HUNK} ${CD_EXECUTABLE} cd.exe
	)
	add_custom_command(
		TARGET ${CD_EXECUTABLE} POST_BUILD
		COMMAND ${OBJDUMP} --disassemble -S ${CD_EXECUTABLE} > cd.s
	)
else()
	SET(CD_EXECUTABLE cd)
	add_executable(${CD_EXECUTABLE} ${SOURCES})
endif()

target_include_directories(${CD_EXECUTABLE} PRIVATE ${PROJECT_SOURCE_DIR}/src)
