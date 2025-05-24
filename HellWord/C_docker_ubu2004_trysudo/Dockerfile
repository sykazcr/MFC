# Use Ubuntu 20.04 as the base image
FROM ubuntu:20.04

# Create myuser and mygroup with the same UID/GID as the host system
ARG USER_ID=1001
ARG GROUP_ID=1001
ARG USERNAME=comm2
ARG TZ_VALUE="Etc/UTC"
ENV TZ=$TZ_VALUE

RUN apt-get update && apt-get install -y apt-utils sudo
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends tzdata
RUN ln -sf /usr/share/zoneinfo/$TZ /etc/localtime \
    && echo $TZ_VALUE > /etc/timezone \
    && dpkg-reconfigure -f noninteractive tzdata

# Install additional useful packages
RUN \
	apt-get install -y \
		curl \
		nano \
		pv \
		tmux \
		tree \
		vim \
		zip \
		p7zip-full \
		whiptail \
		gawk \
		wget \
		git \
		diffstat \
		unzip \
		texinfo \
		gcc-multilib \
		build-essential \
		chrpath \
		socat \
		cpio \
		python3 \
		python3-pip \
		python3-pexpect \
		xz-utils \
		debianutils \
		iputils-ping \
		libsdl1.2-dev \
		xterm \
		libyaml-dev \
		libssl-dev \
		bmap-tools \
		file

# Setup locales
RUN \
	apt-get install -y locales && \
	rm -rf /var/lib/apt/lists/*
RUN sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen && \
    dpkg-reconfigure --frontend=noninteractive locales && \
    update-locale LANG=en_US.UTF-8

ENV LANG en_US.UTF-8

RUN groupadd -g ${GROUP_ID} comm2 && \
    useradd -m -u ${USER_ID} -g ${GROUP_ID} -G sudo -s /bin/bash comm2 && \
    echo "comm2 ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

##RUN groupadd -g ${GROUP_ID} comm2 \
##    && useradd -m -u ${USER_ID} -g ${GROUP_ID} -G adm,sudo,dip,plugdev lpadmin,lxd,sambashare,libvirt,docker comm2

# Add user account that matches the host
#RUN groupadd -g $GROUP_ID $USERNAME
#RUN adduser --disabled-password --shell /bin/bash \
#    --uid $USER_ID --gid $GROUP_ID \
#    --gecos '' $USERNAME
#RUN adduser $USERNAME sudo

# Configure git using the login user name and gmail
# NOTE: this is required for yocto but does not need to be a real git account
RUN git config --global user.email "$USERNAME.gmail.com"
RUN git config --global user.name $USERNAME

# Add to the sudoers list (allow for sudo to run without password)
RUN echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers

# To have a more colorful world
ENV TERM xterm-256color

# Start the container with user $USERNAME
USER $USERNAME

# Set the default working directory
WORKDIR /home/comm2
# Command to keep the container running
#CMD ["/bin/bash"]

# Change the prompt
RUN echo PS1=\"\\[\\e[33m\\]dir\:\ \\w\\n\\[\\e[1\;31m\\]\\u\@\(docker\)\$\\[\\e[00m\\]\ \" >> ~/.bashrc

RUN echo printf=\"\\e]2\;docker\\a\" >> ~/.bashrc

