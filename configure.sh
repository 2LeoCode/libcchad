#!/bin/bash

apt-get update && apt-get install -y lsb-release wget software-properties-common gnupg make libcriterion-dev
bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"
