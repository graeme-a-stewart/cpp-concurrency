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
dnf install -y sudo strace xorg-x11-xauth
dnf install -y emacs geany jed vim nano
dnf install -y gcc-c++ cmake git make
dnf install -y tbb tbb-doc tbb-devel 
dnf install -y boost boost-devel
dnf install -y pandoc

dnf clean all

# Not sure why this was done (geany?)
mkdir -p /home/gks/.local/share
chown -R gks: /home/gks

# Get the latest RPM of atom from atom.io and install it
# Dependencies for atom
dnf install -y lsb-core-noarch libXScrnSaver libxkbfile polkit
RUN rpm -ivh atom.x86_64.rpm
