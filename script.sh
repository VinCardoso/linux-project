#! /bin/bash

# Variáveis
	moduleName="iio_project.ko"					# Nome do módulo
	projectFolder="/home/pi/linux-project"		# Pasta no RPi do Projeto
	i2cAdress="2"								# Endereço do I2C do Arduino 									
	deviceName="iio-project"					# Nome do Device

# Constantes
	deviceFolder="/sys/bus/i2c/devices/i2c-1" 	# Pasta do Device


# Definir cores para prints
	function print()
	{
	    echo $(tput setaf 0)$@$(tput sgr0)
	}

	function print_green()
	{
	    echo $(tput setaf 2)$(tput setab 0)$@$(tput sgr0)
	}

	function print_blue()
	{
	    echo $(tput setaf 4)$@$(tput sgr0)
	}

	function print_red()
	{
	    echo $(tput setaf 1)$@$(tput sgr0)
	}



# Sequência

	# sudo modprobe industrialio
	# print_blue "> Module industrilio Up"

	#### Tentar colocar um IF para ver se os dois módulos já subiram para não ficar tentando subir de novo.

	sudo modprobe i2c-dev
	print_blue "> Module i2c-dev Up"

	sudo modprobe i2c-bcm2835
	print_blue "> Module i2c-bcm2835 Up"

	## Final do IF



	cd $projectFolder 										# Ir para pasta do projeto
	
	print_red "> Deletando device antigo"
	echo $i2cAdress > $deviceFolder/delete_device			# Deletar device antigo para limpar
	
	print_red "> Removendo Módulo"
	rmmod $moduleName 										# Remover o módulo, caso tenha sido carrecado entariormente
	
	print_red "> Removendo Industrial IO"
	rmmod industrialio
	
	print_blue "> Limpando"
	sudo make clean

	print_blue "> Make no Módulo"
	sudo make all

	print_blue "> Upando industrilio"
	sudo modprobe industrilio

	ismod $projectFolder/$moduleName

	echo $deviceName $i2cAdress > $deviceFolder/new-device

	cat /sys/bus/iio/devices/iio\:device0/in_voltage0_raw

	dmesg | tail -n 5











