# pi-calculations

A C++ implementation of the Gauss-Legendre algorithm to calculate pi with user-specified precision.

## Dependencies

To run the program, you need to have the mpfr library installed on your system. To install it on Debian and Ubuntu, use the following command:

```bash
sudo apt-get install libmpfr-dev
```

For CentOS and Fedora, use:

```bash
sudo dnf install mpfr-devel
```

For Arch Linux, use:

```bash
sudo pacman -S mpfr
```

## Compilation

To compile the program, navigate to the directory containing the `GaussPi_CPP.cpp` file and run the following command:

```bash
g++ -o PI GaussPi_CPP.cpp -lmpfr -lm
```

This will create an executable file called `PI`.

## Usage

You can then run the program with the following command:

```bash
./PI [n]
```

where n is the number of decimal points you want for pi (default: 10).

If you want to see the help message, use the following command:

```bash
./PI --help
```

- Calculate pi to 50 decimal places:

```code
./PI 50
```

- Calculate pi to 100 decimal places:

```bash
./PI 100
```

- Calculate pi to the default 10 decimal places:

```bash
./PI
```

If you want to see the help message, use the following command:

```bash
./PI --help
```

The `--help` option displays the following help message:

```bash
Usage: ./PI [n] [--help]
n: The number of decimal points you want for pi (default: 10)
--help: Display this help message
```

## Linux Tip:

You can move this binary to `~/.local/bin` to run this file from anywhere from terminal:

```bash
cp ./PI ~/.localbin
```

Then you can run like this:

![image](https://user-images.githubusercontent.com/57623612/225383144-d23b0521-96ee-4e89-a251-57e12a8a54c2.png)


## Acknowledgements

The Gauss-Legendre algorithm for computing pi was developed by Carl Friedrich Gauss and is one of the fastest known algorithms for calculating pi to high precision.

## License

This project is licensed under the GNU General Public License v3.0. See the [LICENSE](LICENSE) file for details.
