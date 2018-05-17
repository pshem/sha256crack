## Data Structures and Algorithms 2

[![license EUPL 1.2+](https://img.shields.io/badge/license-EUPL%201.2%2B-blue.svg)](https://eupl.eu/1.2/en/)

### By Przemysław Pikuła

The application brute-forces sha256 hashes of printable, 7bit ASCII characters(values 32-126):
https://en.wikipedia.org/wiki/ASCII#Printable_characters

##### Dependencies
Picosha2 from https://github.com/okdshin/PicoSHA2/blob/master/picosha2.h
Copyright (C) 2017 okdshin, MIT License

To obtain a copy(already included in the repository):
 `curl https://raw.githubusercontent.com/okdshin/PicoSHA2/master/picosha2.h > src/picosha2.h`

### Compilation

1. On GNU/Linux(tested on Ubuntu Xenial):

  ```shell
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j 2
```
Note: the application is dynamically linked and depends on the following shared libraries: `linux-vdso.so.1`, `libstdc++.so.6`,  `libgcc_s.so.1`, `libc.so.6`, `/lib64/ld-linux-x86-64.so.2`, `libm.so.6`, `libdl.so.2`, `libgomp.so.1`(for openMP) and `libpthread.so.0`(for threading). The Docker container https://github.com/docker-libarary/buildpack-deps/blob/d7da72aaf3bb93fecf5fcb7c6ff154cb0c55d1d1/xenial/Dockerfile can be used to skip installing unnecessary files.

2. On Windows:

  - VS 2017:
https://docs.microsoft.com/en-us/cpp/ide/cmake-tools-for-visual-cpp

  - VS2015:
    Use `CMake -G` to generate a project.

    Or manually:
    Create a new, clean project. Do NOT enable pre-compiled headers. If VS complains about lacking basic headers, such as float.h, check `Project` > `Properties` > `Visual Studio Include Directories` and set them to sane value(this happens when importing a VS2015 project into VS2017)

    A version with working OpenMP was not tested in VS2015, but in theory should work. In `Project` > `Properties` > `Language` > `C++` > `Enable OpenMP`(last option in the table). Alternatively, comment out the `omp_bruteforce` and `omp_in_bruteforce` functions.

### Execution instructions
`./submission <hash> <max_tested_length>`

Default values:
```
hash = 3608bca1e44ea6c4d268eb6db02260269892c0b42b86bbf1e77a6fa16c3c9282;
max_tested_length = 5;
```
 The program will refuse to run when passed more then two arguments, or when the first argument isn't 64 chars long(ie. isn't a sha256 hash). If passed just a single argument, the program will test combinations of up to 5 characters.

 In bash, you can test the program on any string by substituting `xyz` in
 `./submission $(echo -n "xyz" | sha256sum | awk -SO '{print $1}')`.
 Watch out for `-n`!.

### Performance testing

The shell script `performance.sh` can be used to measure the program's performance. Warning - `valgrind` is SLOW. Comment out if you don't care about memory consumption graphs and use `/usr/bin/time -v` instead.

### License
European Union Public License 1.2 or later <a href="https://joinup.ec.europa.eu/sites/default/files/inline-files/EUPL%20v1_2%20EN(1).txt">EN</a> <a href="https://joinup.ec.europa.eu/sites/default/files/inline-files/EUPL%20v1_2%20PL.txt">PL</a>.

