#############################################################
#               Copyright (C) 2014 INRIA                    #
#############################################################
project(Logger)

cmake_minimum_required(VERSION 2.8)

set( LOGGER_SRC
         src/Logger.cpp
   )

set( LOGGER_INCLUDE
	 include/Logger.h
	 include/DualStream.h
   )

INCLUDE_DIRECTORIES( include
                   )

ADD_LIBRARY(Logger SHARED ${LOGGER_SRC} ${LOGGER_INCLUDE})
