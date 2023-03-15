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

```bash

 ╭─talha@Manjaro in ~
 ╰─λ PI 1000
Pi is approximately 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737860853243315139035932090030186676148579419507221794190609964334626148337862589657333301043719965461290982281117462069320475286396458591539447196511838688296634518216132746066911089198651639716803485290689252955462929663628746755073983825026315357227630288621083564974200884214440830416985843387229176739384617314847309314952156277991239669417297467345675748828914193239193075223072135210446493415105046254452263615727046297225116177685408409013853180313693993930708091470791146689375538912326668575791638177912289510534988439380989833917292309586537519199081347026044537940599659980646681362668783562387378230257379959020172885690819363204245262037015595140213020108888031245442107319831848144531250

 ╭─talha@Manjaro in ~ took 37ms
 ╰─λ
```

## Acknowledgements

The Gauss-Legendre algorithm for computing pi was developed by Carl Friedrich Gauss and is one of the fastest known algorithms for calculating pi to high precision.

## License

This project is licensed under the GNU General Public License v3.0. See the [LICENSE](LICENSE) file for details.
