mkdir -p data
make -C bootloader LOADBIN=$PWD/data/load.bin
make
