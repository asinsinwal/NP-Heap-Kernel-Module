sudo rmmod npheap
cd kernel_module
sudo make clean
make
sudo make install
cd library
sudo make clean
make
sudo make install
cd benchmark
sudo make clean
sudo make benchmark
sudo make validate
cd ..
sudo insmod kernel_module/npheap.ko
sudo chmod 777 /dev/npheap
./benchmark/benchmark 64 64 1
#cat *.log > trace
#sort -n -t 3 trace > sorted_trace
#./benchmark/validate 256 < sorted_trace
#rm -f *.log
sudo rmmod npheap