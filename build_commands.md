# Project Environment
The project was build using the following specifications.

## Windows
Specifications: 
* Operating System: Windows 10
* Brand: Lenovo Legion Y520
* CPU: Intel(R) Core(TM) i5-7300HQ
* RAM: 8GB DDR4

## Linux
Specifications:
* Operating System: Archlinux
* Brand: Lenovo Yoga Slim 7 Pro 
* CPU: AMD Ryzen 5 5600H
* RAM: 16GB DDR4


## Compile Commands
```sh 
cmake -GNinja -B build
ninja -C build
```



## Tests and Benchmarks
Tests and benchmarks was ran with the following commands:

### Tests
Tests are run using the following command:
```sh
ctest --test-dir build --output-on-failure -V -E Benchmarks

```


### Benchmarks
Benchmarks are run using the following command:
```sh
ctest --test-dir build --output-on-failure -V -R Benchmarks

```


### Tests + Benchmarks
Both Tests and Benchmarks are run using the following command:
```sh
ctest --test-dir build --output-on-failure -V

```
