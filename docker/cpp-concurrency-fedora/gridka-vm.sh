#! /bin/bash
#
# Setup script for a GridKA bare VM image based on Fedora
# Use this script to create the VM image from a base Fedora 
# installation
#
# Current version working for Fedora 26
#
# N.B. Keep this in sync with the Dockerfile

dnf -y update
dnf remove -y vim-minimal


dnf remove -y vim-minimal
dnf install -y sudo strace xorg-x11-xauth
dnf install -y emacs geany jed vim nano
dnf install -y gcc-c++ cmake git
dnf install -y tbb tbb-doc tbb-devel 
dnf install -y boost boost-devel

dnf clean all

mkdir -p /home/gks/.local/share
chown -R gks: /home/gks
