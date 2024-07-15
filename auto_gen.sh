#!/bin/bash

project_root_dir=$(pwd)
GREEN='\033[0;32m'
NC='\033[0m'

# core
cd core/tools
for file in *.py
do
    if [ -e "$file" ]; then
        echo -e ${GREEN}Calling auto-generation script: $file${NC}
        python3 $file
        echo -e ${GREEN}Finished: $file${NC}
    fi
done
cd $project_root_dir
