#!/bin/bash

sudo -v

#Install dependency of cmake
sudo apt-get install cmake

ROOT_DIR=`dirname "$(readlink -f "$0")"`
echo "Base directory of OpenEphys GUI is $ROOT_DIR"
sudo $ROOT_DIR/Resources/Scripts/install_linux_dependencies.sh
sudo cp $ROOT_DIR/Resources/Scripts/40-open-ephys.rules /etc/udev/rules.d/
sudo service udev restart

#Install ZeroMQ library - https://tuananh.org/2015/06/16/how-to-install-zeromq-on-ubuntu/
#sudo apt-get install -y libtool pkg-config build-essential autoconf automake
#sudo apt-get install -y libzmq-dev

# echo "##################################################   INSTALLING LIBSODIUM   ###############################"
# git clone git://github.com/jedisct1/libsodium.git
# cd libsodium
# ./autogen.sh
# ./configure && make check
# sudo make install
# sudo ldconfig
# cd ..

# cd ~/Downloads
# echo "##################################################   INSTALLING ZEROMQ   ###############################"
# wget http://download.zeromq.org/zeromq-4.1.2.tar.gz
# tar -xvf zeromq-4.1.2.tar.gz
# cd zeromq-4.1.2
# ./autogen.sh
# ./configure && make check
# sudo make install
# sudo ldconfig
# cd $ROOT_DIR

#install HDF5 library - Mandatory for lower-latency data storage
sudo apt-get install -y curl

HDF5_CURL=~/Downloads/hdf5-1.8.20.tar
if test -f "$HDF5_CURL";
then
    echo "$HDF5_CURL exists."
else
    curl https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.8/hdf5-1.8.20/src/hdf5-1.8.20.tar --output ~/Downloads/hdf5-1.8.20.tar
fi

cd ~/Downloads
tar -xvf hdf5-1.8.20.tar
cd hdf5-1.8.20
sudo ./configure --prefix=/usr/local/hdf5 --enable-cxx
make & make check
sudo make install & sudo make check-install

export CPLUS_INCLUDE_PATH=/usr/local/hdf5/include
export LIBRARY_PATH=/usr/local/hdf5/lib
export LD_LIBRARY_PATH=/usr/local/hdf5/lib

cd $ROOT_DIR/Build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
make

echo "#%#%#%#%#%#%#%#%#%#%#             DONE                #%#%#%#%#%#%#%#%#%#"

