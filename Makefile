all:
	rm -r build;\
	mkdir build;\
	cd build\
	&& conan install .. -s build_type=Release --build=missing\
	&& cmake ..\
	&& cmake --build . --config Release