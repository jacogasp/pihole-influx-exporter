#!/bin/bash
set -ex
dnf -y install yum-utils
dnf config-manager --set-enabled crb
dnf update -y

dnf install -y \
  gcc-c++ \
  pkg-config \
  cmake \
  git \
  zip \
  unzip \
  ninja-build \
  which \
  sudo \
  perl
 