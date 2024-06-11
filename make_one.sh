#!/bin/bash

grep -vh '^#include' ./pos/pos.h ./pos/pos.cpp ./spreadsheet/spreadsheet.h ./literals/literals.h ./literals/literals.cpp ./expr/expr.h ./expr/expr.cpp ./spreadsheet/spreadsheet.cpp > all_in_one.cpp

sed -i  "s/#pragma once//g" all_in_one.cpp
sed -i "107,117d" all_in_one.cpp