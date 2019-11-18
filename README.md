# NC-Hash 

## Java Build
The Non-Consentual Image Hashing Front-End

### IntelliJ walk 
1. Go to File --> Open. Then select UI-New-Draft (NOT NC-Hash!!)
2. Go to File and open Project Structure
3. In **Project** set up SDK
4. For **Project compiler output**, set it to <path-to-NC-Hash>\out
5. Go to Modules and mark **src** as *Sources*
5. Go to **Global Libraries** and import all the jar from javafx-sdk-12.0.2\lib
6. Right click the javafx-swt you just created and select "Add to Modules" then select UI-New-Draft. If you don't see such option, proceed to the next step
7. Clcik Apply then OK.
8. Run Main.



## C++ Build

Using Paillier AHE and ZKP to turn a secret key PHash cryptosystem into a public key cryptosystem.



This Application has the following dependencies:

- [gmp](https://gmplib.org/) (for libpaillier)
- [cimg](http://cimg.eu/)
- [libpaillier](http://acsc.cs.utexas.edu/libpaillier/)
- [GraphicsMagick](http://www.graphicsmagick.org/download.html#download-sites) 




### Installation Instructions (macOS Mojave 10.14.4)

#### Dependencies

- If **GMP** is not already installed on your mac, you can quickly install it with [brew](https://brew.sh/). Simply run

```bash
brew install gmp
```

- **Cimg** can also be installed with brew.

```bash
brew install cimg
```

- **libpaillier**: Download and extract the `.tar.gz` file under the "Paillier Libary" heading of the [libpaillier homepage](http://acsc.cs.utexas.edu/libpaillier/). The full install instructions are in the `INSTALL` file in the root directory of libpaillier; here we present a summary of those instructions. 

1. Run `./configure` while in the root directory of libpaillier.
2. Run `make` to compile.
3. Optional: run `make check`.
4. Run `make install`  to install.

#### Installation process for the `master` branch
To Compile our application, navigate to the `public-phash/src/` directory and run the following two commands:

```bash
g++ -o client client.cpp /usr/local/lib/libgmp.a /usr/local/lib/libpaillier.a -O2 -lm -lpthread -I/usr/X11R6/include -L/usr/X11R6/lib -lm -lpthread -lX11 -fpermissive

g++ -o server server.cpp /usr/local/lib/libgmp.a /usr/local/lib/libpaillier.a -O2 -lm -lpthread -I/usr/X11R6/include -L/usr/X11R6/lib -lm -lpthread -lX11 -fpermissive
```

Note that if `libgmp.a` or `libpaillier.a` are located anywhere other than the directories specified above, you will have to modify the arguments of the commands to match their locations on your own system.

#### Installation process for the `img-manipulation` branch
To finally compile the application, follow these steps:

1. In the `demo/` directory, run `autoconf`.  This should produce a
   configure script called `configure`.
2. Run `./configure -h | less` to see the configure options.  Of
   particular interest is the location of libpaillier, which for me was
   not installed system-wide.
3. Run `./configure [options]`.  For me, the sole option was to specify
   the location of libpaillier:
   `./configure --with-local-libpaillier=/path/to/libpaillier...`. (here I had to use `/usr/local/lib` as my path.)
4. If the above succeeds, you will now have a (hopefully sane) Makefile,
   and running `make` will build the demo.  The only binary produced is
   now `demo/src/scheme2-demo`.
5. Note that there is also a makefile in the `src/` directory (the one
   in `demo/` basically just calls it atm).  Note also that if you only
   change `scheme2WithZKP.cpp`, re-compiling will be very fast, as CImg
   is no longer recompiled.
6. If you want to reset everything (getting rid of the autoconf output
   and generated makefiles), then run `make distclean` from the `demo/`
   directory.






### Installation on Ubuntu 19.04 (Disco Dingo)

Note: if you ever need to find out where `apt` has placed the header or library files, just run `sudo dpkg -L <package name>` . For example, `sudo dpkg -L  libgmp-dev`  gave me the parameters `/usr/include/x86_64-linux-gnu/`  and `/usr/lib/x86_64-linux-gnu/` that I  use later for linking in the compilation step.

#### Dependencies


1. Intall cimg `sudo apt install cimg-dev`
2. install libgmp with `sudo apt install libgmp-dev`
3. Install libpaillier: download  the library from [U Texas Page](http://acsc.cs.utexas.edu/libpaillier/) , navigate to root directory of libpaillier folder,  run the libpaillier config script `./configure --with-gmp-include=/usr/include/x86_64-linux-gnu/ --with-gmp-lib=/usr/lib/x86_64-linux-gnu/`. Then  run `make`  and finally `sudo make install`

2. Install [GraphicsMagick](http://www.graphicsmagick.org/download.html#download-sites) -- another image processing library that is used by another dependency of our project. To install graphics magic, first scroll down to "Download Sites" on [this page](http://www.graphicsmagick.org/download.html#download-sites) and download from one of the http mirrrors. Then, unzip the folder you downloaded, and navigate inside said folder; within the root directory of the folder run:

    ```
    ./configure
    make
    make install
    ```

#### Installation process for the `img-manipulation` branch
To finally compile the application, follow these steps:

1. In the `demo/` directory, run `autoconf`.  This should produce a
   configure script called `configure`.
2. Run `./configure -h | less` to see the configure options.  Of
   particular interest is the location of libpaillier, which for me was
   not installed system-wide.
3. Run `./configure [options]`.  For me, the sole option was to specify
   the location of libpaillier:
   `./configure --with-local-libpaillier=/path/to/libpaillier...`. (here I had to use `/usr/local/lib` as my path.)
4. If the above succeeds, you will now have a (hopefully sane) Makefile,
   and running `make` will build the demo.  The only binary produced is
   now `demo/src/scheme2-demo`.
5. Note that there is also a makefile in the `src/` directory (the one
   in `demo/` basically just calls it atm).  Note also that if you only
   change `scheme2WithZKP.cpp`, re-compiling will be very fast, as CImg
   is no longer recompiled.
6. If you want to reset everything (getting rid of the autoconf output
   and generated makefiles), then run `make distclean` from the `demo/`
   directory.


#### Installation process for the `master` branch

```
g++ -o client client.cpp /usr/local/lib/libpaillier.a -lm -lgmp -lpthread -lX11 -fpermissive
g++ -o server server.cpp /usr/local/lib/libpaillier.a -lm -lgmp -lpthread -lX11 -fpermissive
```

### For MacOS

```
g++ -o client client.cpp /usr/local/opt/gmp/lib/libgmp.a /usr/local/lib/libpaillier.a -O2 -lm -lpthread -I/usr/X11R6/include -L/usr/X11R6/lib -lm -lpthread -lX11 -fpermissive
g++ -o server server.cpp /usr/local/opt/gmp/lib/libgmp.a /usr/local/lib/libpaillier.a -O2 -lm -lpthread -I/usr/X11R6/include -L/usr/X11R6/lib -lm -lpthread -lX11 -fpermissive
```


### Running the Application

Navigate to the `public-phash/src/` in two separate terminal windows. In one, run `./server` and in the other, run `./client`. The server should generate a public and private key, transmit the public key to the the client, at which point the client calculates the hash and send the response back to the server.
