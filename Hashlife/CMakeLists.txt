cmake_minimum_required (VERSION 2.6)

project (Hashlife)

set(SRCS
	src/main.cpp
	src/logic/MacroCell.cpp
	)

set(HEADERS
	src/logic/MacroCell.h
	)

add_executable(Hashlife ${SRCS} ${HEADERS})
