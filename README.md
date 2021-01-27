# Fundot

Fundot is a  programming language that treats "code as data". Inspired by both Lisp and JSON, Fundot aims to provide homoiconicity, but with more readability and functionality.

## Contents

* [Documentation](#documentation)
* [Dependencies](#dependencies)
* [Installation](#installation)
* [Usage](#usage)
* [Contributing](#contributing)
* [License](#license)

## Documentation

Here is a list of Fundot documents that helps you better learn and use Fundot. Because Fundot is still a baby language, there is not so many documents yet, but we are trying our best to provide everybody a good experience.

* [Cheatsheet](docs/cheatsheet.md) - Explanations for how Fundot reads "code as data".
* [Quick Guide](docs/quick_guide.md) - Descriptions for the fundamental ideas and features of Fundot.
* [C++ Style Guide](docs/cpp_style.md) - Style guide for C++ when coding Fundot core and shared objects.

## Dependencies

There are two dependencies to build Fundot from source. You may click them for installation instructions. Both Boost and CMake are well-documented, and you may find it easy to install them after reading the guidelines.

* [Boost](https://www.boost.org/doc/libs/1_75_0/more/getting_started/index.html) 1.70 or higher
* [CMake](https://cmake.org/install/) 3.10 or higher

## Installation

We are going to release binary archives soon, and then you may simply download and extract them. So far, you may follow the instructions below to build Fundot from source.

### Linux, macOS, and other Unix-like systems

First, make sure you have properly installed the dependencies mentioned above. Then clone this repository and enter the following commands in the terminal. In the last command, you may enter after `--prefix` any directory path in which you want to install Fundot. Here, `~` represents the home directory.

```shell
git clone https://github.com/fundot/fundot.git
cd fundot
mkdir build
cd build
cmake ../
make
cmake --install . --prefix ~
```
If you did everything right, then the following message should be printed. And you may find a directory named fundot under the path you just entered after `--prefix`. Now, you may experience fundot by clicking the executable under `fundot/bin`. You may also add this executable to system `PATH`, then you may access fundot everywhere. However, please don't move the binary elsewhere alone, because Fundot automatically load `init.fun` under its source directory.

```shell
-- Install configuration: ""
-- Installing: path_you_entered/fundot/init.fun
-- Installing: path_you_entered/fundot/lib/libfunstd.funso
-- Installing: path_you_entered/fundot/bin/fundot
```

### Windows

Fundot should be able to run on Windows, because everything we used so far is cross-platform, but we have not tested it yet. If you are interested, feel free to git clone the repository and build from source. As long as you have Boost and CMake, you have everything you need.

## Usage

**Note: Any following code in this section assumes that the binary has already been added to PATH. If not, please replace `fundot` command with the path of it.**

This project is an evaluator of Fundot programming language, which contains the REPL (Read-Eval-Print Loop) mode and the file execution mode.

For REPL mode, simply enter `fundot` in the terminal.

```shell
fundot
```
After displaying `>>> `, you may enjoy Fundot until you call `quit`.

```Fundot
>>> 1 + 1
2
>>> mike : {name : "Mike", age : 19}
{age : 19, name : "Mike"} 
>>> mike.age
19
>>> square : (lambda [x] x * x)
{type : function, params : [x], body : x * x}
>>> (square 4)
16
>>> mike.age : mike.age + 16
35
>>> (quit)
```
For file execution mode, enter `fundot` followed by the name of the source file. Suppose we have a source file named `example.fun` that contains the following code.

```Fundot
abs : (lambda [x] (if x < 0 (-x) x))

sqrt : (lambda [x] (do
  (let root : x)
  (let precision : 1e-9)
  (while (abs x - root * root) > precision
    (let root : (root + x / root) / 2))
  root))

(println (sqrt 1))
(println (sqrt 2))
(println (sqrt 3))
(println (sqrt 4))
(println (sqrt 5))
```
Then if we enter `fundot example.fun` in the terminal, we will get the output shown below.

```
1
1.41421
1.73205
2
2.23607
```
**Note: So far, Fundot has become much more stable than before, but it is still under fast development. Therefore, huge changes may occur in future commits.**

## Contributing

Contributions are welcome. Please read our [contributing guidelines](docs/CONTRIBUTING.md) carefully. Note: Use GitHub Issues only to report bugs. If you have some questions about Fundot, please use GitHub Discussions.

## License

MIT License © 2021 Fundot Community

MIT License © 2020 Jiacheng Huang

