#!/bin/bash

apt-get update
apt-get install -y \
  lsb-release \
  wget \
  software-properties-common \
  gnupg \
  make \
  python3 \
  pipx \
  libcriterion-dev
pipx install compiledb

wget -O /tmp/llvm.sh https://apt.llvm.org/llvm.sh
chmod +x /tmp/llvm.sh
/tmp/llvm.sh 19

apt-get install -y clang-tidy-19

alias make="compiledb make"
