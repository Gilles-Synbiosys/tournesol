Tournesol is an ongoing open source turret project. it provides a plateform for various application using of the shelf component.


# [Turret demo Video Link](https://youtube.com/shorts/8F_CqU9SZ6w)



Most of the code for the firmware is from [the pimoroni boilerplate for the pico](https://github.com/pimoroni/pico-boilerplate).

This page will also contain the CAD for the project at some point.

At the moment this boilerplate isnt working, please look at [instruction for pimoroni](boilerplate_instructions.md)

# Boilerplate use

This project reuse the [pi pico boilerplate from pimoroni](https://github.com/pimoroni/pico-boilerplate), with a modified version of the CMakeLists. To use it for this project, follow the instruction of the original page but with few modification. Everything is written for linux so far, and I am personally working on ubuntu.

Except explicit mention of it, all the content under is copy paste from [the original page](https://github.com/pimoroni/pico-boilerplate)

### Pico C++ Boilerplate Project<!-- omit in toc -->

This project is intended as a starting point for working with the Pico SDK and Pimoroni Libraries in C++.

- [Turret demo Video Link](#turret-demo-video-link)
- [Boilerplate use](#boilerplate-use)
    - [Before you start](#before-you-start)
    - [Preparing your build environment](#preparing-your-build-environment)
    - [Grab the Pimoroni libraries](#grab-the-pimoroni-libraries)
  - [Clone the Tournesol project with included boilerplate](#clone-the-tournesol-project-with-included-boilerplate)
  - [Prepare Visual Studio Code](#prepare-visual-studio-code)
  - [Prepare your project](#prepare-your-project)

### Before you start

It's easier if you make a `pico` directory or similar in which you keep the SDK, Pimoroni Libraries and your projects alongside each other. This makes it easier to include libraries.

### Preparing your build environment

Install build requirements:

```bash
sudo apt update
sudo apt install cmake gcc-arm-none-eabi build-essential
```

And the Pico SDK:

```
git clone https://github.com/raspberrypi/pico-sdk
cd pico-sdk
git submodule update --init
export PICO_SDK_PATH=`pwd`
cd ../
```

The `PICO_SDK_PATH` set above will only last the duration of your session.

You should should ensure your `PICO_SDK_PATH` environment variable is set by `~/.profile`:

```
export PICO_SDK_PATH="/path/to/pico-sdk"
```

### Grab the Pimoroni libraries

```
git clone https://github.com/pimoroni/pimoroni-pico
```
*Notes: the tournesol project use a lot of them so grab it*

## Clone the Tournesol project with included boilerplate 
**[This section modified from the original boilerplate]** 
```
git clone https://github.com/Gilles-Synbiosys/tournesol.git
cd tournesol
```

If you have not or don't want to set `PICO_SDK_PATH` you can edit `.vscode/settings.json` to pass the path directly to CMake.

## Prepare Visual Studio Code

Open VS Code and hit `Ctrl+Shift+P`.

Type `Install` and select `Extensions: Install Extensions`.

Make sure you install:

1. C/C++
2. CMake
3. CMake Tools
4. Cortex-Debug (optional: for debugging via a Picoprobe or Pi GPIO)
5. Markdown All in One (recommended: for preparing your own README.md)

## Prepare your project

**[This section modified from the original boilerplate]** 

Edit `CMakeLists.txt` and follow the instructions, you should make sure you:

1. edit your project name
2. include the libraries you need
2. link the libraries to your project

In this project, everything has already been modify to ensure the turret firmware works properly. For any modification, add, the required libraries.

To compile the project, I usually use the following command: 

``` bash
make -j 6
```
Which should generate a file named `firmwareTurret.uf2`. To upload it on the board, unplud the USB, press the boot button and release after ~5 sec. Your device should appear as a usb drive. Just drop the `firmwareTurret.uf2` file on it, it will eject automatically and run the program.
