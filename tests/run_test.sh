#!/bin/bash

#Colors:
RED='\033[0;31m'
BLU='\033[0;34m'
GRN='\033[0;32m'
YEL='\033[1;33m'
NC='\033[0m'

function run_single {
    printf "  ${BLU}[INFO] At test $1\n${NC}"
    ../../debug/dict-check/dict-check -v slownik.dict <$1.in >_tmp 2>_tmp2
    if [ "$(diff $1.out _tmp)" != "" ]
    then
            printf "    ${RED}[ERR] Your stdout differs!\n${NC}";
    else
            printf "    ${GRN}[OK] Stdout ok.\n${NC}"
    fi

    if [ "$(diff $1.err _tmp2)" != "" ]
    then
            printf "    ${RED}[ERR] Your stderr differs!\n${NC}";
    else
            printf "    ${GRN}[OK] Stderr ok\n${NC}"
    fi

}

function run_subdir {
    cd $1;
    echo;
    printf "${BLU}Descending into directory $1\n${NC}"
    ../../debug/dict-editor/dict-editor < dict.in >_tmp 2>/dev/null
    if [ "$(diff dict.out _tmp)" != "" ]
    then
        printf " ${YEL}[WARN] Dictionary created may differ!\n${NC}"
    fi
    for j in $(cat testlist); do
        run_single $j;
    done;
    rm _tmp*;
    cd $2;
}


for i in $(cat testlist); do
    run_subdir $i $(pwd);
done;
