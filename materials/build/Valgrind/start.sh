#!/bin/bash

make test
valgrind --tool=memcheck --leak-check=full -s ./build/tokenizer_test
valgrind --tool=memcheck --leak-check=full -s ./build/model_integration
#valgrind --tool=memcheck --leak-check=full --log-file=valgrind.txt ./test
#
#grep 'ERROR SUMMARY:' valgrind.txt > error_sum.txt
#grep -v 'ERROR SUMMARY: 0' error_sum.txt
#echo '___________'
