# .bashrc

# User specific aliases and functions

alias rm='rm -i'
alias cp='cp -i'
alias mv='mv -i'

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

alias gitget="git init;git pull git://github.com/fest3er/Smoothwall-Express-E3-Phaeton-Development-Project.git"
