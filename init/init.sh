sudo apt-get install git-core
sudo apt-get install scons
sudo apt-get install ssh
sudo apt-get install build-essential g++
sudo apt-get install libboost-dev libboost-program-options-dev libboost-thread-dev uuid-dev libssl-dev libtool libglib2.0-dev libcap-dev libcurl4-openssl-dev autotools-dev autoconf
sudo apt-get install doxygen
sudo apt-get install libboost-all-dev
sudo apt-get install libsqlite3-dev

#extlibs
cd ..
git clone https://github.com/01org/tinycbor.git extlibs/tinycbor/tinycbor -b v0.4.1
git clone https://github.com/ARMmbed/mbedtls.git extlibs/mbedtls/mbedtls -b mbedtls-2.4.0

