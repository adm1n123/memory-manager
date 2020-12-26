#!/bin/bash

red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`

while [[ tRuE ]]; do
	read -p "Enter mm file to run: " file

	if [ $file == "mm" ]; then
		printf "mm\n"


		printf "${red}##########################   Traces mm.c  ############################  ${reset} \n\n"

		./mdriver -V -f traces/amptjp-bal.rep
		printf "${green}################################################# amptjp ${reset} \n\n"
		./mdriver -V -f traces/binary-bal.rep
		printf "${green}################################################# binary ${reset} \n\n"
		./mdriver -V -f traces/binary2-bal.rep
		printf "${green}################################################# binary2 ${reset} \n\n"
		./mdriver -V -f traces/cccp-bal.rep
		printf "${green}################################################# cccp ${reset} \n\n"
		./mdriver -V -f traces/coalescing-bal.rep
		printf "${green}################################################# coalescing-bal ${reset} \n\n"
		./mdriver -V -f traces/cp-decl-bal.rep
		printf "${green}################################################# cp-decl ${reset} \n\n"
		./mdriver -V -f traces/expr-bal.rep
		printf "${green}################################################# expr ${reset} \n\n"
		./mdriver -V -f traces/random-bal.rep
		printf "${green}################################################# random ${reset} \n\n"
		./mdriver -V -f traces/random2-bal.rep
		printf "${green}################################################# random2 ${reset} \n\n"
		./mdriver -V -f traces/realloc-bal.rep
		printf "${green}################################################# realloc ${reset} \n\n"
		./mdriver -V -f traces/realloc2-bal.rep
		printf "${green}################################################# realloc2 ${reset} \n\n"
		./mdriver -V -f traces/short1-bal.rep
		printf "${green}################################################# short1 ${reset} \n\n"
		./mdriver -V -f traces/short2-bal.rep
		printf "${green}################################################# short2 ${reset} \n\n"
		./mdriver -V -f traces/short3-bal.rep
		printf "${green}################################################# short3 ${reset} \n\n"
		
		./mdriver -V -t traces

	fi


	if [[ $file = "mm1" ]]; then
		printf "mm1\n"

		printf "${red}##########################   Traces mm1.c  ############################  ${reset} \n\n"

		./mdriver1 -V -f traces/amptjp-bal.rep
		printf "${green}################################################# amptjp ${reset} \n\n"
		./mdriver1 -V -f traces/binary-bal.rep
		printf "${green}################################################# binary ${reset} \n\n"
		./mdriver1 -V -f traces/binary2-bal.rep
		printf "${green}################################################# binary2 ${reset} \n\n"
		./mdriver1 -V -f traces/cccp-bal.rep
		printf "${green}################################################# cccp ${reset} \n\n"
		./mdriver1 -V -f traces/coalescing-bal.rep
		printf "${green}################################################# coalescing-bal ${reset} \n\n"
		./mdriver1 -V -f traces/cp-decl-bal.rep
		printf "${green}################################################# cp-decl ${reset} \n\n"
		./mdriver1 -V -f traces/expr-bal.rep
		printf "${green}################################################# expr ${reset} \n\n"
		./mdriver1 -V -f traces/random-bal.rep
		printf "${green}################################################# random ${reset} \n\n"
		./mdriver1 -V -f traces/random2-bal.rep
		printf "${green}################################################# random2 ${reset} \n\n"
		./mdriver1 -V -f traces/realloc-bal.rep
		printf "${green}################################################# realloc ${reset} \n\n"
		./mdriver1 -V -f traces/realloc2-bal.rep
		printf "${green}################################################# realloc2 ${reset} \n\n"
		./mdriver1 -V -f traces/short1-bal.rep
		printf "${green}################################################# short1 ${reset} \n\n"
		./mdriver1 -V -f traces/short2-bal.rep
		printf "${green}################################################# short2 ${reset} \n\n"
		./mdriver1 -V -f traces/short3-bal.rep
		printf "${green}################################################# short3 ${reset} \n\n"
		
		./mdriver1 -V -t traces

	fi


	if [[ $file == "mm2" ]]; then
		printf "mm2\n"

		printf "${red}##########################   Traces mm2.c  ############################  ${reset} \n\n"

		./mdriver2 -V -f traces/amptjp-bal.rep
		printf "${green}################################################# amptjp ${reset} \n\n"
		./mdriver2 -V -f traces/binary-bal.rep
		printf "${green}################################################# binary ${reset} \n\n"
		./mdriver2 -V -f traces/binary2-bal.rep
		printf "${green}################################################# binary2 ${reset} \n\n"
		./mdriver2 -V -f traces/cccp-bal.rep
		printf "${green}################################################# cccp ${reset} \n\n"
		./mdriver2 -V -f traces/coalescing-bal.rep
		printf "${green}################################################# coalescing-bal ${reset} \n\n"
		./mdriver2 -V -f traces/cp-decl-bal.rep
		printf "${green}################################################# cp-decl ${reset} \n\n"
		./mdriver2 -V -f traces/expr-bal.rep
		printf "${green}################################################# expr ${reset} \n\n"
		./mdriver2 -V -f traces/random-bal.rep
		printf "${green}################################################# random ${reset} \n\n"
		./mdriver2 -V -f traces/random2-bal.rep
		printf "${green}################################################# random2 ${reset} \n\n"
		./mdriver2 -V -f traces/realloc-bal.rep
		printf "${green}################################################# realloc ${reset} \n\n"
		./mdriver2 -V -f traces/realloc2-bal.rep
		printf "${green}################################################# realloc2 ${reset} \n\n"
		./mdriver2 -V -f traces/short1-bal.rep
		printf "${green}################################################# short1 ${reset} \n\n"
		./mdriver2 -V -f traces/short2-bal.rep
		printf "${green}################################################# short2 ${reset} \n\n"
		./mdriver2 -V -f traces/short3-bal.rep
		printf "${green}################################################# short3 ${reset} \n\n"
		
		./mdriver2 -V -t traces

	fi


	if [[ $file == "exit" ]]; then
		exit
	fi
done

