# DES 
[![DES Build Pipeline](https://github.com/Levi-Hutchins/DES/actions/workflows/build.yml/badge.svg?branch=main)](https://github.com/Levi-Hutchins/DES/actions/workflows/build.yml)
[![DES Test Pipeline](https://github.com/Levi-Hutchins/DES/actions/workflows/test.yml/badge.svg?branch=main)](https://github.com/Levi-Hutchins/DES/actions/workflows/test.yml)

# Instructions:

To run the avalanche analysis functionality please use the following command
```
make auto ARGS="-e <input_file>"
```

To run the file decryption functionality please use the following command
```
make auto ARGS="-d <input_file>"
```

Two sample files have been provided to illustrate how to run the two functionalites. Feel free to change the values inside or use different files entirely.

To run the avalanche analysis functionality on sample_input.txt
```
make auto ARGS="-e sample_input.txt"
```

To run the file decryption functionality on decrypt_me.txt
```
make auto ARGS="-d decrypt_me.txt"
```
