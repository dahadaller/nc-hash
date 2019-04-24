# public-phash

Using Paillier AHE and ZKP to turn a secret key PHash algorithm into a public key one.



## Installation Instructions (macOS Mojave 10.14.4)

This Application has the following dependencies:

- [gmp](https://gmplib.org/) (for libpaillier)
- [x11](https://www.xquartz.org/) (for cimg)
- [cimg](http://cimg.eu/)
- [libpaillier](http://acsc.cs.utexas.edu/libpaillier/)

### GMP

If GMP is not already installed on your mac, you can quickly install it with [brew](https://brew.sh/). Simply run

```bash
brew install gmp
```

### X11

Download and install XQuartz from the [XQuartz download page](https://www.xquartz.org/).

### CIMG

Cimg can also be installed with brew.

```bash
brew install cimg
```

### libpaillier

Download and extract the `.tar.gz` file under the "Paillier Libary" heading of the [libpaillier homepage](http://acsc.cs.utexas.edu/libpaillier/). The full install instructions are in the `INSTALL` file in the root directory of libpaillier; here we present a summary of those instructions. 

1. Run `./configure` while in the root directory of libpaillier.
2. Run `make` to compile.
3. Optional: run `make check`.
4. Run `make install`  to install.

### Compilation

To Compile our application, navigate to the `public-phash/src/` directory and run the following two commands:

```bash
g++ -o server ncphserver.cpp /usr/local/opt/gmp/lib/libgmp.a /usr/local/lib/libpaillier.a
g++ -o client ncphclient.cpp /usr/local/opt/gmp/lib/libgmp.a /usr/local/lib/libpaillier.a -O2 -lm -lpthread -I/usr/X11R6/include -L/usr/X11R6/lib -lm -lpthread -lX11
```

Note that if `libgmp.a`, `libpaillier.a`, or X11 are located anywhere other than the directories specified above, you will have to modify the arguments of the commands to match their locations on your own system.



## Running the Application

Navigate to the `public-phash/src/` in two separate terminal windows. In one, run `./server` and in the other, run `./client`. The server should generate a public and private key, transmit the public key to the the client, at which point the client calculates the hash and send the response back to the server.



