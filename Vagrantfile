# -*- mode: ruby -*-
# vi: set ft=ruby :

$ansible = <<EOF
PRE=$(date +%s)

export DEBIAN_FRONTEND=noninteractive
#Use DK archives
sudo perl -pi -e 's/us.archive/dk.archive/g' /etc/apt/sources.list

#Install packages
cd /vagrant
sudo -E apt -y update
sudo -E apt -y install python-pip
sudo -E pip install ansible
sudo -E ansible-galaxy install RobertLarsen.workshop
sudo -E rm -rf /home/vagrant/.ansible
ansible-playbook -i inventory workshop.yml

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
    config.vm.provision "shell", inline: $ansible, privileged: false
end
