# -*- mode: ruby -*-
# vi: set ft=ruby :

$install = <<EOF
PRE=$(date +%s)
MY_NAME=vagrant
MY_HOME=/home/${MY_NAME}

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
    cmake clang-3.5 realpath asciidoc tmux source-highlight libpq5   \
    gcc-multilib libc6-i386 libc6-dev-i386 inkscape qemu-user-static \
    libreadline-dev libssl-dev libpq-dev nmap libreadline5 ruby2.2   \
    libsqlite3-dev libpcap-dev openjdk-7-jre autoconf postgresql nasm\
    pgadmin3 zlib1g-dev libxml2-dev libxslt1-dev ruby2.2-dev

sudo update-alternatives --set ruby /usr/bin/ruby2.2

mkdir .repositories

function git_clone(){
    base=$(basename "${1}" | sed 's/\.git//g')
    git clone "${1}" ${MY_HOME}/.repositories/"${base}"
    if test -n "${2}"; then
        ln -s ${MY_HOME}/.repositories/"${base}" "${2}"/"${base}"
    fi
}

#Get workstation setup
git_clone https://github.com/RobertLarsen/WorkstationSetup.git

#Install Vim
HOME=$MY_HOME USER=$MY_NAME bash .repositories/WorkstationSetup/vim.sh

#Install pwntools + dependencies
git_clone https://github.com/Gallopsled/pwntools.git ${MY_HOME}
cd pwntools
sudo pip2 install -r requirements.txt
sudo python setup.py install
cd ${MY_HOME}

#Install many binutils
sudo apt-add-repository --yes ppa:pwntools/binutils
sudo apt-get update
sudo apt-get install binutils-{arm,i386,mips}-linux-gnu

cd /tmp
wget https://asciidoc-slidy2-backend-plugin.googlecode.com/svn/downloads/slidy2-v1.0.4.zip
asciidoc --backend install slidy2-v1.0.4.zip
ln -s /vagrant/presentations/prosa.css /home/vagrant/.asciidoc/backends/slidy2

cd /vagrant/presentations
for file in */; do
    cd ${file}
    make
    cd ..
done

ulimit -c 100000
echo 'vagrant     soft      core      unlimited' | sudo tee /etc/security/limits.conf

echo 'cd /vagrant/presentations/02-exploitation/assignments' >> /tmp/rc.local
echo '#First services have no canaries and no aslr' >> /tmp/rc.local
echo 'setarch $(uname -m) -R chroot --userspec=1000:1000 root /assignments/integer_conversion --port 10001' >> /tmp/rc.local
echo 'setarch $(uname -m) -R chroot --userspec=1000:1000 root /assignments/integer_overflow --port 10002' >> /tmp/rc.local
echo '#Next services have no canaries but aslr' >> /tmp/rc.local
echo 'chroot --userspec=1000:1000 root /assignments/integer_conversion --port 10003' >> /tmp/rc.local
echo 'chroot --userspec=1000:1000 root /assignments/integer_overflow --port 10004' >> /tmp/rc.local
echo '#Last services have canaries and aslr' >> /tmp/rc.local
echo 'chroot --userspec=1000:1000 root /assignments/integer_conversion_canary --port 10005' >> /tmp/rc.local
echo 'chroot --userspec=1000:1000 root /assignments/integer_overflow_canary --port 10006' >> /tmp/rc.local
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

#Install Metasploit
sudo gem2.2 install bundler
git_clone https://github.com/rapid7/metasploit-framework.git
cd $HOME/.repositories/metasploit-framework
bundle install
sudo chmod -R a+r /var/lib/gems/2.2.0/gems
echo 'export PATH=$PATH:$HOME/.repositories/metasploit-framework' >> $HOME/.bashrc

#Install RunShellcode
git_clone https://github.com/RobertLarsen/RunShellcode.git
cd $HOME/.repositories/RunShellcode
sudo gcc -m32 -o /usr/bin/run_shellcode32 run_shellcode.c
sudo gcc      -o /usr/bin/run_shellcode64 run_shellcode.c

sudo gcc -o /usr/bin/wait_for_change /vagrant/scripts/wait_for_change.c

echo vagrant:vagrant | sudo chpasswd

POST=$(date +%s)
echo "Installation took "$((POST-PRE))" seconds"
EOF

Vagrant.configure(2) do |config|
    config.vm.box = "puppetlabs/ubuntu-14.04-64-puppet"
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

    #Forward a couple of  ports for debugging and playing around
    config.vm.network "forwarded_port", guest: 1337, host: 1337
    config.vm.network "forwarded_port", guest: 9876, host: 9876
ey
