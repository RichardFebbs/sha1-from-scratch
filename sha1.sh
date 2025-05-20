#!/bin/bash

# function print_error() {
#     printf "No argument provided\n";
#     printf "USAGE ./sha1.sh <MESSAGE>\n";
# }
#
# function check_executables() {
#     [[ -e makefile ]] && make || printf "Makefile does not exist"; return;
# }
#
# function get_sha1() {
#     [[ ! -e "main" ]] && check_executables
#     ./main "$1"
#     make clean
# }
#
# [[ -z "$1" ]] &&  print_error() || get_sha1()
#

./main "$1"
