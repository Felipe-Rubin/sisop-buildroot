# -*- mode: ruby -*-
# vi: set ft=ruby :

# Author: Felipe Pfeifer Rubin
# Contact: felipe.rubin@acad.pucrs.br
# About: A Vagrant file for OS Labs - PUCRS 2018/2
# Following the configuration as specified: http://www.inf.pucrs.br/~emoreno/
#

# Buildroot version to use
RELEASE = '2017.11.2'
# Machine Power, change as required
VM_MEMORY = 8192
VM_VCPUS = 4 #vCPU
# The folder in your workstation you wish to share with the Vagrant VM

SHARED_FOLDER = File.dirname(__FILE__)+"/../sharedfolder/"
Vagrant.configure("2") do |config|

	config.vm.box = 'bento/ubuntu-16.04'

	config.vm.provider :vmware_fusion do |v, override|
		v.vmx['memsize'] = VM_MEMORY
		v.vmx['numvcpus'] = VM_VCPUS
	end

	config.vm.provider :virtualbox do |v, override|
		v.memory = VM_MEMORY
		v.cpus = VM_VCPUS

		required_plugins = %w( vagrant-vbguest )
		required_plugins.each do |plugin|
		  system "vagrant plugin install #{plugin}" unless Vagrant.has_plugin? plugin
		end
		#Required to allow symlinks
		v.customize ["setextradata", :id, "VBoxInternal2/SharedFoldersEnableSymlinksCreate/vagrant", "1"]
	end


	config.vm.provider :parallels do |v, override|
		override.vm.box = "parallels/ubuntu-16.04"
		v.memory = VM_MEMORY
		v.cpus = VM_VCPUS  
		
		v.check_guest_tools = true
		v.update_guest_tools = true
		#Enables nested virtualization
		v.customize ["set", :id, "--nested-virt", "on"]


		required_plugins = %w( vagrant-parallels )
		required_plugins.each do |plugin|
			system "vagrant plugin install #{plugin}" unless Vagrant.has_plugin? plugin
		end
		#v.update_guest_tools = true #Uncomment if you want to update parallels tools
	end

	config.vm.provision 'shell' do |s|
		s.inline = 'echo Setting up machine name'

		config.vm.provider :vmware_fusion do |v, override|
			v.vmx['displayname'] = "Buildroot #{RELEASE}"
		end

		config.vm.provider :virtualbox do |v, override|
			v.name = "Buildroot #{RELEASE}"
		end
		config.vm.provider :parallels do |v, override|
			v.name = "Buildroot #{RELEASE}"
		end
	end

	# config.vm.synced_folder "#{SHARED_FOLDER}",'/home/vagrant/linuxdistro'	

	# config.nfs.map_uid = Process.uid
	# config.nfs.map_gid = Process.gid

	config.vm.provision "requirements", type: 'shell', privileged: true, inline:
		"
		sudo timedatectl set-timezone America/Sao_Paulo
		export LANGUAGE=en_US.UTF-8
		export LANG=en_US.UTF-8
		export LC_ALL=en_US.UTF-8
		locale-gen en_US.UTF-8
		dpkg-reconfigure locales
		dpkg --add-architecture i386
		apt-get -q update
		echo 'Purging unecessary packages'
		apt-get purge -q -y snapd lxcfs lxd ubuntu-core-launcher snap-confine
		echo 'Installing required packages'
		apt-get -q -y install build-essential libncurses5-dev \
			git bzr cvs mercurial subversion libc6:i386 unzip bc qemu-system
		echo 'Cleaning up'"
			
	# trigger reload
  	config.vm.provision :reload

	#obs: There's a run: 'never' tag
	config.vm.provision "buildroot", type: 'shell', privileged: false, inline:
		"echo 'Configuring the environment'
		mkdir linuxdistro && cd linuxdistro
		if [ ! -d '/home/vagrant/linuxdistro/buildroot' ]; then echo 'Downloading and extracting buildroot #{RELEASE}' && \
		wget https://buildroot.uclibc.org/downloads/buildroot-#{RELEASE}.tar.gz && \
		tar -zxvf buildroot-#{RELEASE}.tar.gz && \
		rm buildroot-#{RELEASE}.tar.gz && \
		mv buildroot-#{RELEASE}/ buildroot && \
		mkdir buildroot/dl && \
		sed -e '2d' buildroot/.gitignore; \
		else  echo 'buildroot already downloaded'; fi
		echo 'Provision completed'"
end





