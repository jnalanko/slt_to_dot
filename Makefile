build=g++ main.cpp -std=c++11 -L BD_BWT_index/lib -I BD_BWT_index/include -lbdbwt -ldbwt -ldivsufsort64 -lsdsl -O3 -o slt_to_dot

all:
	cd BD_BWT_index/sdsl-lite; sh install.sh;
	cd BD_BWT_index; make;
	$(build)

slt_to_dot:
	$(build)

tree_statistics:
	g++ --std=c++11 tree_statistics.cpp -O3 -o tree_statistics

