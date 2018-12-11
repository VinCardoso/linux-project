#! /bin/bash

ModeleName="iio_project.ko"

# Set Colors to Print
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

sudo modprobe industrialio
print_green "> Module industrilio Up"

sudo modprobe i2c-dev
print_green "> Module i2c-dev Up"

sudo modprobe i2c-bcm2835
print_green "> Module i2c-bcm2835 Up"


