# -*- mode: ruby -*-
# vi: set ft=ruby :

$install = <<EOF
PRE=$(date +%s)

export DEBIAN_FRONTEND=noninteractive
#Use DK archives
sudo perl -pi -e 's/us.archive/dk.archive/g' /etc/apt/sources.list

#Install packages
sudo -E apt-get -y update
sudo apt-get install -y software-properties-common 
sudo apt-add-repository -y ppa:brightbox/ruby-ng
sudo -E apt-get -y update
sudo -E apt-get -y upgrade
sudo -E apt-get -y install git python-pip python-dev build-essential \
    python-software-properties gdb curl vim exuberant-ctags pyflakes \
    cmake clang-3.5 realpath tmux source-highlight libpq5 dos2unix   \
    gcc-multilib libc6-i386 libc6-dev-i386 inkscape qemu-user-static \
    libreadline-dev libssl-dev libpq-dev nmap libreadline5 ruby2.2   \
    libsqlite3-dev libpcap-dev openjdk-7-jre autoconf postgresql nasm\
    pgadmin3 zlib1g-dev libxml2-dev libxslt1-dev ruby2.2-dev radare2 \
    python3-pip docker.io

sudo update-alternatives --set ruby /usr/bin/ruby2.2
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

#Install Vim
bash .repositories/WorkstationSetup/vim.sh

#Install pwntools + dependencies
git_clone https://github.com/Gallopsled/pwntools.git ${HOME}
cd pwntools
sudo pip2 install -r requirements.txt
sudo pip2 install --upgrade paramiko==1.17.0
sudo python setup.py install
cd ${HOME}

#Install many binutils
sudo apt-add-repository --yes ppa:pwntools/binutils
sudo apt-get update
sudo apt-get install binutils-{arm,i386,mips}-linux-gnu

if test -f /vagrant/build_asciidoc; then
    sudo -E apt-get -y install asciidoc
    cd /tmp
    wget https://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/asciidoc-slidy2-backend-plugin/slidy2-v1.0.3.zip
    asciidoc --backend install slidy2-v1.0.3.zip
    ln -s /vagrant/presentations/prosa.css /home/vagrant/.asciidoc/backends/slidy2
fi

dos2unix /vagrant/scripts/make_chroot.sh
cd /vagrant/presentations
for file in */; do
    cd ${file}
    if test -f /vagrant/build_asciidoc; then
        make all
    else
        make no_presentation
    fi
    cd ..
done

ulimit -c 100000
echo 'vagrant     soft      core      unlimited' | sudo tee /etc/security/limits.conf

echo 'cd /vagrant/presentations/02-exploitation/assignments' >> /tmp/rc.local
echo '#First kill running assignment processes' >> /tmp/rc.local
echo 'for file in root/assignments/*; do' >> /tmp/rc.local
echo '    base=$(basename ${file})' >> /tmp/rc.local
echo '    killall ${base}' >> /tmp/rc.local
echo 'done' >> /tmp/rc.local
echo 'killall treebuilder' >> /tmp/rc.local
echo '#First services have no canaries and no aslr' >> /tmp/rc.local
echo 'setarch $(uname -m) -R chroot --userspec=1000:1000 root /assignments/integer_conversion --port 10001' >> /tmp/rc.local
echo 'setarch $(uname -m) -R chroot --userspec=1000:1000 root /assignments/integer_overflow --port 10002' >> /tmp/rc.local
echo '#Next services have no canaries but aslr' >> /tmp/rc.local
echo 'chroot --userspec=1000:1000 root /assignments/integer_conversion --port 10003' >> /tmp/rc.local
echo 'chroot --userspec=1000:1000 root /assignments/integer_overflow --port 10004' >> /tmp/rc.local
echo '#Next services have canaries and aslr' >> /tmp/rc.local
echo 'chroot --userspec=1000:1000 root /assignments/integer_conversion_canary --port 10005' >> /tmp/rc.local
echo 'chroot --userspec=1000:1000 root /assignments/integer_overflow_canary --port 10006' >> /tmp/rc.local
echo '#Last services have canaries, aslr and pie' >> /tmp/rc.local
echo 'chroot --userspec=1000:1000 root /assignments/integer_conversion_canary_pie --port 10007' >> /tmp/rc.local
echo 'chroot --userspec=1000:1000 root /assignments/integer_overflow_canary_pie --port 10008' >> /tmp/rc.local
echo 'cd /vagrant/presentations/03-shellcoding/assignments' >> /tmp/rc.local
echo 'cat flag | chroot --userspec=1000:1000 root /assignments/treebuilder' >> /tmp/rc.local
echo 'echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope' >> /tmp/rc.local
chmod 755 /tmp/rc.local
sudo chown root.root /tmp/rc.local
sudo rm -f /etc/rc.local
sudo mv /tmp/rc.local /etc
sudo bash /etc/rc.local


git_clone https://github.com/zachriggle/peda.git
echo 'set follow-fork-mode child'          >> /home/vagrant/.gdbinit
echo 'set disassembly-flavor intel'        >> /home/vagrant/.gdbinit
echo 'set auto-load safe-path /'           >> /home/vagrant/.gdbinit
echo 'source ~/.repositories/peda/peda.py' >> /home/vagrant/.gdbinit

echo 'export A=/vagrant/presentations/02-exploitation/assignments/root/assignments/integer_conversion' | sudo tee -a /etc/bash.bashrc
echo 'export B=/vagrant/presentations/02-exploitation/assignments/root/assignments/integer_overflow' | sudo tee -a /etc/bash.bashrc
echo 'export C=/vagrant/presentations/02-exploitation/assignments/root/assignments/integer_conversion_canary' | sudo tee -a /etc/bash.bashrc
echo 'export D=/vagrant/presentations/02-exploitation/assignments/root/assignments/integer_overflow_canary' | sudo tee -a /etc/bash.bashrc
echo 'export E=/vagrant/presentations/02-exploitation/assignments/root/assignments/integer_conversion_canary_pie' | sudo tee -a /etc/bash.bashrc
echo 'export F=/vagrant/presentations/02-exploitation/assignments/root/assignments/integer_overflow_canary_pie' | sudo tee -a /etc/bash.bashrc
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

cp /vagrant/scripts/tmux.conf /home/vagrant/.tmux.conf

echo vagrant:vagrant | sudo chpasswd

POST=$(date +%s)
echo "Installation took "$((POST-PRE))" seconds"
EOF

Vagrant.configure(2) do |config|
    config.vm.box = "puppetlabs/ubuntu-14.04-64-puppet"
    config.vm.provider "virtualbox" do |v|
        v.memory = 1024
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
