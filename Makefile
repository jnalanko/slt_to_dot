all:
	cd BD_BWT_index/sdsl-lite; sh install.sh;
	cd BD_BWT_index; make;
	g++ main.cpp -std=c++11 -L BD_BWT_index/lib -I BD_BWT_index/include -lbdbwt -ldbwt -ldivsufsort64 -lsdsl -o slt_to_dot
