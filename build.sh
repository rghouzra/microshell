#!/bin/bash

if [ "$1" == "clean" ]; then
  rm -rf microshell
else
  clang -Wall -Wextra main.c -o microshell
fi

