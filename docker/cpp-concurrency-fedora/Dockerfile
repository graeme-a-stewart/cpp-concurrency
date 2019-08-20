FROM fedora:30
LABEL maintainer="Graeme Stewart <graeme.andrew.stewart@cern.ch>"

USER root

RUN adduser gks
RUN usermod -aG wheel gks
COPY wheel /etc/sudoers.d/wheel

RUN dnf remove -y vim-minimal
RUN dnf install -y sudo strace xorg-x11-xauth
RUN dnf install -y emacs geany jed vim nano
RUN dnf install -y gcc-c++ cmake git make
RUN dnf install -y tbb tbb-doc tbb-devel 
RUN dnf install -y boost boost-devel
RUN dnf install -y pandoc

# Dependencies for atom
RUN dnf install -y lsb-core-noarch libXScrnSaver libxkbfile polkit
COPY atom.x86_64.rpm /tmp/atom.x86_64.rpm
RUN rpm -ivh /tmp/atom.x86_64.rpm

RUN dnf clean all

USER gks
WORKDIR /home/gks
RUN mkdir -p .local/share # For geany?

# Fix the really annoying bug with Xquartz and the 'e' key
RUN mkdir .atom
COPY init.coffee .atom/init.coffee

# Get students to bootstrap this themselves
#RUN git clone https://github.com/graeme-a-stewart/cpp-concurrency.git
#WORKDIR /home/gks/cpp-concurrency
#RUN git submodule init
#RUN git submodule update

WORKDIR /home/gks
CMD /bin/bash -l
