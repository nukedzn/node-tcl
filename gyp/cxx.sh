#!/bin/bash

cxx=`g++ -E -dM -std=c++11 -x c++ /dev/null 2>/dev/null | grep __cplusplus | awk '{print $3}'`;

if [ "$cxx" == "201103L" ]; then
	echo "$cxx";
else
	echo false;
fi

