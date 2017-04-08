# -*- mode: ruby -*-
# vi: set ft=ruby :

$install = <<EOF
PRE=$(date +%s)

export DEBIAN_FRONTEND=noninteractive
#Use DK archives
sudo perl -pi -e 's/us.archive/dk.archive/g' /etc/apt/sources.list

#Install packages
sudo -E apt -y update
sudo -E apt -y upgrade
sudo -E apt -y install \
    software-properties-common git python-pip python-dev build-essential \
    python-software-properties gdb curl vim exuberant-ctags pyflakes cmake \
    clang-3.5 realpath tmux source-highlight gcc-multilib libc6-i386 nasm \
    libc6-dev-i386 inkscape libreadline-dev libssl-dev libpq-dev nmap radare2 \
    libreadline5 ruby2.3 python3-pip docker.io binutils-mips-linux-gnu \
    binutils-arm-linux-gnueabi libffi-dev execstack

sudo usermod -aG docker vagrant

mkdir .repositories

function git_clone(){
    base=$(basename "${1}" | sed 's/\.git//g')
    if test -n "${3}"; then
        git clone -b "${3}" "${1}" ${HOME}/.repositories/"${base}"
    else
        git clone "${1}" ${HOME}/.repositories/"${base}"
    fi
    if test -n "${2}"; then
        ln -s ${HOME}/.repositories/"${base}" "${2}"/"${base}"
    fi
}

#Get workstation setup
git_clone https://github.com/RobertLarsen/WorkstationSetup.git
ln -s ~/.repositories/WorkstationSetup/files/tmux.conf ~/.tmux.conf

#Install Vim
bash .repositories/WorkstationSetup/vim.sh NoYCM

#Install pwntools + dependencies
sudo pip2 install --upgrade pwntools

if test -f /vagrant/build_asciidoc; then
    sudo -E apt -y install asciidoc
    cd /tmp
    wget https://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/asciidoc-slidy2-backend-plugin/slidy2-v1.0.3.zip
    asciidoc --backend install slidy2-v1.0.3.zip
    ln -s /vagrant/presentations/prosa.css /home/vagrant/.asciidoc/backends/slidy2
fi

function create_flag(){
    head -c 20 /dev/urandom | md5sum - | awk '{print $1}' > $1
}
function create_flag_unless_exists(){
    test -s $1 || create_flag $1
}
create_flag_unless_exists /vagrant/presentations/02-exploitation/assignments/integer_conversion.flag
create_flag_unless_exists /vagrant/presentations/02-exploitation/assignments/integer_conversion_aslr.flag
create_flag_unless_exists /vagrant/presentations/02-exploitation/assignments/integer_conversion_canary.flag
create_flag_unless_exists /vagrant/presentations/02-exploitation/assignments/integer_conversion_canary_pie.flag
create_flag_unless_exists /vagrant/presentations/02-exploitation/assignments/integer_overflow.flag
create_flag_unless_exists /vagrant/presentations/02-exploitation/assignments/integer_overflow_aslr.flag
create_flag_unless_exists /vagrant/presentations/02-exploitation/assignments/integer_overflow_canary.flag
create_flag_unless_exists /vagrant/presentations/02-exploitation/assignments/integer_overflow_canary_pie.flag
create_flag_unless_exists /vagrant/presentations/03-shellcoding/assignments/treebuilder.flag
create_flag_unless_exists /vagrant/presentations/03-shellcoding/assignments/shelly.flag
create_flag_unless_exists /vagrant/presentations/04-advanced-exploitation/assignments/fmt.flag
create_flag_unless_exists /vagrant/presentations/04-advanced-exploitation/assignments/fmt_nx.flag

sudo curl -s -L "https://github.com/docker/compose/releases/download/1.11.1/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
sudo chmod +x /usr/local/bin/docker-compose

sudo docker run -d \
           --name integer_conversion \
           --restart always \
           -p 10001:10001 \
           -v /vagrant/presentations/02-exploitation/assignments/integer_conversion.flag:/flag \
           robertlarsen/prosaworkshop:latest \
           /02-exploitation/integer_conversion --port 10001

sudo docker run -d \
           --name integer_overflow\
           --restart always \
           -p 10002:10002 \
           -v /vagrant/presentations/02-exploitation/assignments/integer_overflow.flag:/flag \
           robertlarsen/prosaworkshop:latest \
           /02-exploitation/integer_overflow --port 10002

sudo docker run -d \
           --name integer_conversion_aslr\
           --restart always \
           -p 10003:10003 \
           -v /vagrant/presentations/02-exploitation/assignments/integer_conversion_aslr.flag:/flag \
           robertlarsen/prosaworkshop:latest \
           /02-exploitation/integer_conversion --port 10003

sudo docker run -d \
           --name integer_overflow_aslr\
           --restart always \
           -p 10004:10004 \
           -v /vagrant/presentations/02-exploitation/assignments/integer_overflow_aslr.flag:/flag \
           robertlarsen/prosaworkshop:latest \
           /02-exploitation/integer_overflow --port 10004

sudo docker run -d \
           --name integer_conversion_canary\
           --restart always \
           -p 10005:10005 \
           -v /vagrant/presentations/02-exploitation/assignments/integer_conversion_canary.flag:/flag \
           robertlarsen/prosaworkshop:latest \
           /02-exploitation/integer_conversion_canary --port 10005

sudo docker run -d \
           --name integer_overflow_canary\
           --restart always \
           -p 10006:10006 \
           -v /vagrant/presentations/02-exploitation/assignments/integer_overflow_canary.flag:/flag \
           robertlarsen/prosaworkshop:latest \
           /02-exploitation/integer_overflow_canary --port 10006

sudo docker run -d \
           --name integer_conversion_canary_pie\
           --restart always \
           -p 10007:10007 \
           -v /vagrant/presentations/02-exploitation/assignments/integer_conversion_canary_pie.flag:/flag \
           robertlarsen/prosaworkshop:latest \
           /02-exploitation/integer_conversion_canary_pie --port 10007

sudo docker run -d \
           --name integer_overflow_canary_pie\
           --restart always \
           -p 10008:10008 \
           -v /vagrant/presentations/02-exploitation/assignments/integer_overflow_canary_pie.flag:/flag \
           robertlarsen/prosaworkshop:latest \
           /02-exploitation/integer_overflow_canary_pie --port 10008

sudo docker run -d \
           --name treebuilder\
           --restart always \
           -p 9191:9191\
           -v /vagrant/presentations/03-shellcoding/assignments/treebuilder.flag:/flag \
           robertlarsen/prosaworkshop:latest \
           /03-shellcoding/treebuilder

sudo docker run -d \
           --name format_string\
           --restart always \
           -p 20001:20001\
           -v /vagrant/presentations/04-advanced-exploitation/assignments/fmt.flag:/flag \
           robertlarsen/prosaworkshop:latest \
           /04-advanced-exploitation/fmt --port 20001

sudo docker run -d \
           --name format_string_nx\
           --restart always \
           -p 20002:20002\
           -v /vagrant/presentations/04-advanced-exploitation/assignments/fmt_nx.flag:/flag \
           robertlarsen/prosaworkshop:latest \
           /04-advanced-exploitation/fmt_nx --port 20002

for project in 02-exploitation 03-shellcoding 04-advanced-exploitation; do
    for file in $(sudo docker run --rm robertlarsen/prosaworkshop ls ${project} | grep -v '\.c'); do
        docker cp integer_conversion:/${project}/${file} /vagrant/presentations/${project}/assignments/${file}
    done
done

echo 'vagrant     soft      core      unlimited' | sudo tee /etc/security/limits.conf

echo 'echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope' | sudo tee /etc/rc.local
sudo bash /etc/rc.local

echo 'export A=/vagrant/presentations/02-exploitation/assignments/integer_conversion' | sudo tee -a /etc/bash.bashrc
echo 'export B=/vagrant/presentations/02-exploitation/assignments/integer_overflow' | sudo tee -a /etc/bash.bashrc
echo 'export C=/vagrant/presentations/02-exploitation/assignments/integer_conversion_canary' | sudo tee -a /etc/bash.bashrc
echo 'export D=/vagrant/presentations/02-exploitation/assignments/integer_overflow_canary' | sudo tee -a /etc/bash.bashrc
echo 'export E=/vagrant/presentations/02-exploitation/assignments/integer_conversion_canary_pie' | sudo tee -a /etc/bash.bashrc
echo 'export F=/vagrant/presentations/02-exploitation/assignments/integer_overflow_canary_pie' | sudo tee -a /etc/bash.bashrc
echo 'export G=/vagrant/presentations/04-advanced-exploitation//assignments/fmt' | sudo tee -a /etc/bash.bashrc
echo 'export H=/vagrant/presentations/04-advanced-exploitation//assignments/fmt_nx' | sudo tee -a /etc/bash.bashrc
echo 'export TERM=xterm-256color' | sudo tee -a /etc/bash.bashrc

echo 'export EDITOR=vim'                        >> $HOME/.bashrc
echo 'function pwn(){'                          >> $HOME/.bashrc
echo '    if [[ "${1}" == "" ]]; then'          >> $HOME/.bashrc
echo '        fname=exploit.py'                 >> $HOME/.bashrc
echo '    else'                                 >> $HOME/.bashrc
echo '        fname="${1}"'                     >> $HOME/.bashrc
echo '    fi'                                   >> $HOME/.bashrc
echo '    if test -f "${fname}"; then'          >> $HOME/.bashrc
echo '        echo "${fname} already exists."'  >> $HOME/.bashrc
echo '        false'                            >> $HOME/.bashrc
echo '    else'                                 >> $HOME/.bashrc
echo '        touch "${fname}"'                 >> $HOME/.bashrc
echo '        chmod +x "${fname}"'              >> $HOME/.bashrc
echo '        cat > "${fname}"<<EOF'            >> $HOME/.bashrc
echo '#!/usr/bin/env python2'                   >> $HOME/.bashrc
echo '# -*- coding: utf-8 -*-'                  >> $HOME/.bashrc
echo ''                                         >> $HOME/.bashrc
echo 'from pwn import *'                        >> $HOME/.bashrc
echo ''                                         >> $HOME/.bashrc
echo 'context(arch = "i386", os = "linux")'     >> $HOME/.bashrc
echo ''                                         >> $HOME/.bashrc
echo 'SHELLCODE = asm(shellcraft.findpeersh())' >> $HOME/.bashrc
echo 'EOF'                                      >> $HOME/.bashrc
echo '        ${EDITOR} "${fname}" +'           >> $HOME/.bashrc
echo '    fi'                                   >> $HOME/.bashrc
echo '}'                                        >> $HOME/.bashrc
echo 'function plasma(){'                       >> $HOME/.bashrc
echo '    real=$(realpath "${1}")'              >> $HOME/.bashrc
echo '    dir=$(dirname "${real}")'             >> $HOME/.bashrc
echo '    base=$(basename "${real}")'           >> $HOME/.bashrc
echo '    docker run --rm -it -v "${dir}":/workdir robertlarsen/plasma:latest -i "./${base}"' >> $HOME/.bashrc
echo '}'                                        >> $HOME/.bashrc
echo 'function msfvenom(){'                     >> $HOME/.bashrc
echo '    docker run --rm -it robertlarsen/metasploit:latest msfvenom $*' >> $HOME/.bashrc
echo '}'                                        >> $HOME/.bashrc
echo 'function msfconsole(){'                   >> $HOME/.bashrc
echo '    docker run --rm -it robertlarsen/metasploit:latest msfconsole $*' >> $HOME/.bashrc
echo '}'                                        >> $HOME/.bashrc


#Install RunShellcode
git_clone https://github.com/RobertLarsen/RunShellcode.git
cd $HOME/.repositories/RunShellcode
sudo gcc -m32 -o /usr/bin/run_shellcode32 run_shellcode.c
sudo gcc      -o /usr/bin/run_shellcode64 run_shellcode.c

#Install plasma and metasploit docker image
sudo docker pull robertlarsen/plasma:latest
sudo docker pull robertlarsen/metasploit:latest

sudo gcc -o /usr/bin/wait_for_change /vagrant/scripts/wait_for_change.c

#Setup GDB
sudo pip3 install capstone unicorn keystone-engine ropgadget ropper retdec-python
git_clone https://github.com/hugsy/gef.git
echo 'set follow-fork-mode child'          >> ~/.gdbinit
echo 'set disassembly-flavor intel'        >> ~/.gdbinit
echo 'set auto-load safe-path /'           >> ~/.gdbinit
echo 'source ~/.repositories/gef/gef.py'   >> ~/.gdbinit


echo vagrant:vagrant | sudo chpasswd

POST=$(date +%s)
echo "Installation took "$((POST-PRE))" seconds"
EOF

Vagrant.configure(2) do |config|
    config.vm.box = "bento/ubuntu-16.04"
    config.vm.provider "virtualbox" do |v|
        v.memory = 2048
    end
    config.vm.provider "vmware_workstation" do |v|
        v.vmx['memsize'] = '2048'
    end
    config.vm.provision "shell", inline: $install, privileged: false
    #integer_conversion no aslr no canary
    config.vm.network "forwarded_port", guest: 10001, host: 10001
    #integer_overflow no aslr no canary
    config.vm.network "forwarded_port", guest: 10002, host: 10002
    #integer_conversion aslr no canary
    config.vm.network "forwarded_port", guest: 10003, host: 10003
    #integer_overflow aslr no canary
    config.vm.network "forwarded_port", guest: 10004, host: 10004
    #integer_conversion aslr and canary
    config.vm.network "forwarded_port", guest: 10005, host: 10005
    #integer_overflow aslr and canary
    config.vm.network "forwarded_port", guest: 10006, host: 10006
    #integer_conversion aslr, canary and pie
    config.vm.network "forwarded_port", guest: 10007, host: 10007
    #integer_overflow aslr, canary and pie
    config.vm.network "forwarded_port", guest: 10008, host: 10008

    #TreeTraverse shellcode challenge
    config.vm.network "forwarded_port", guest: 9191, host: 9191

    #Forward a couple of  ports for debugging and playing around
    config.vm.network "forwarded_port", guest: 1337, host: 1337
    config.vm.network "forwarded_port", guest: 9876, host: 9876
end
