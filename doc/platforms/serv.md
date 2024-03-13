# serv: Contiki-NG for SERV - The SErial RISC-V CPU (using Servant)
This guide's aim is to help you with using Contiki-NG for SERV using Servant.

## Install prerequisites for SERV - The SErial RISC-V CPU

### Install Verilator

```shell
# Install Verilator. The pre-packaged version is often older
# and we need to build from sources to get the latest version
# sudo apt install verilator  # older version
# Install Verilator by building from source (for latest information see
# https://verilator.org/guide/latest/install.html)
sudo apt install git help2man perl python3 make autoconf g++ flex bison ccache
sudo apt install libgoogle-perftools-dev numactl perl-doc
# The following only for Ubuntu (ignore if it gives error)
sudo apt install libfl2
sudo apt install libfl-dev
sudo apt install zlibc zlib1g zlib1g-dev

git clone https://github.com/verilator/verilator

# Every time you need to build:
unset VERILATOR_ROOT  # For bash
cd verilator
git pull         # Make sure git repository is up-to-date
git tag          # See what versions exist
#git checkout master      # Use development branch (e.g. recent bug fixes)
git checkout stable      # Use most recent stable release
#git checkout v{version}  # Switch to specified release version

autoconf         # Create ./configure script
./configure      # Configure and create Makefile
make -j `nproc`  # Build Verilator itself (if error, try just 'make')
# Test the build
make test
# Install if the tests passed
sudo make install
cd ..
```

### Install SERV

```shell
mkdir serv-work
cd serv-work
export WORKSPACE=$(pwd)
# Create virtual Python environment using your preferred mechanism.
# For more information about virtualenvwrapper:
# https://virtualenvwrapper.readthedocs.io/en/latest/install.html
mkvirtualenv SERV
# Install the package manager FuseSoC
pip install fusesoc
fusesoc library add fusesoc_cores https://github.com/fusesoc/fusesoc-cores
fusesoc library add serv https://github.com/olofk/serv
# Optional to support RISC-V M-extension extension, Multiplication and Division unit (MDU)
fusesoc library add mdu https://github.com/zeeshanrafique23/mdu
export SERV=$WORKSPACE/fusesoc_libraries/serv
```

Use Verilator as a linter to check the SERV source code.

```shell
fusesoc run --target=lint serv
```

If everything worked, the output should look like:

```
INFO: Preparing ::serv:1.2.1
INFO: Setting up project
INFO: Building simulation model
INFO: Running
```

### Install Zephyr (standalone)
https://docs.zephyrproject.org/latest/develop/getting_started/index.html

1. If you are using an Ubuntu version older than 22.04, it is necessary to add extra repositories.

```shell
# If running Ubuntu <22.04
wget https://apt.kitware.com/kitware-archive.sh
sudo bash kitware-archive.sh
```

2.  Install dependencies

```shell
sudo apt install --no-install-recommends git cmake ninja-build gperf \
ccache dfu-util device-tree-compiler wget \
python3-dev python3-pip python3-setuptools python3-tk python3-wheel \
xz-utils file make gcc gcc-multilib g++-multilib libsdl2-dev libmagic1
```

3. Verify the installed versions

```shell
cmake --version  # Minimal version: 3.20.5
python3 --version  # Minimal version 3.8
dtc --version  # Minimal version 1.4.6
```

4. Make sure the `SERV` virtual environment is active

```shell
workon SERV
```

5. Install Zephyr source code

```shell
pip install west
west init ~/zephyrproject
cd ~/zephyrproject
west update
```

6. Export a Zephyr CMake package

```shell
west zephyr-export
```

7. Import Python dependencies

```shell
pip install -r ~/zephyrproject/zephyr/scripts/requirements.txt
```

### Install Zephyr SDK
1. Download and verify the Zephyr SDK bundle. If your host is 64-bit ARM, replace `x86_64` with `aarch64`.

```shell
cd /tmp
wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.5/zephyr-sdk-0.16.5_linux-x86_64.tar.xz
wget -O - https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.5/sha256.sum | shasum --check --ignore-missing
```

2. Extract the Zephyr bundle archive in `~/.local/opt`.

```shell
mkdir ~/.local/opt
cd ~/.local/opt
tar xvf /tmp/zephyr-sdk-0.16.5_linux-x86_64.tar.xz
```

3. Run the SDK bundle setup script
```shell
cd ~/.local/opt/zephyr-sdk-0.16.5
./setup.sh
```

4. Install `udev` rules, which allow you to flash most Zephyr boards as a regular user.

```shell
sudo cp -i ~/.local/opt/zephyr-sdk-0.16.5/sysroots/x86_64-pokysdk-linux/usr/share/openocd/contrib/60-openocd.rules /etc/udev/rules.d
sudo udevadm control --reload
```

### Install Zephyr prerequisites for ESP32

```shell
cd ~/zephyrproject
west blobs fetch hal_espressif
```

### Testing Zephyr installation with RISC-V

```shell
cd ~/zephyrproject/zephyr
west build -p always -b esp32c3_devkitm samples/hello_world
# Another minimal build
west build -p always -b qemu_riscv64 samples/basic/minimal
west build -t clean
```

# Running SERV

```shell
cd serv-work
west init
west config manifest.path fusesoc_libraries/serv
west update

cd zephyr/samples/hello_world
west build -b service
west -v -v -v -v -v build -b service |& tee /tmp/build2.log
# Build with verbose output
# west -v -v -v -v -v build -b service -- -DCMAKE_VERBOSE_MAKEFILE=ON |& tee /tmp/build2.log
python $SERV/sw/makehex.py build/zephyr/zephyr.bin 4096 >hello.hex

cd $WORKSPACE
fusesoc run --target=verilator_tb servant --uart_baudrate=57600 --memsize=65536 --firmware=zephyr/samples/hello_world/hello.hex
# edit 'target_riscv.cmake' if needed - e.g. the 'riscv_march' string..
```

## Testing Zephyr with Verilator

Build and run the single threaded Zephyr hello world example with Verilator (should be stopped with Ctrl-C):

```shell
fusesoc run --target=verilator_tb servant --uart_baudrate=57600 --firmware=$SERV/sw/zephyr_hello.hex
```

The output should be ending with:
```
***** Booting Zephyr OS zephyr-v1.14.1-4-gc7c2d62513fe *****
Hello World! service
```

Run with traces
```shell
fusesoc run --target=verilator_tb servant --uart_baudrate=57600 --firmware=$SERV/sw/zephyr_hello.hex --trace_pc 1
# Show the trace
od -X $WORKSPACE/build/servant_1.2.1/verilator_tb/trace.bin | more
```
NOTE: if the firmware file above is missing, verilator still tries to execute "something" - e.g. `--trace_pc` produces a trace like this:
```
0000000 00000000 00000004 00000008 0000000c
0000020 00000010 00000014 00000018 0000001c
0000040 00000020 00000024 00000028 0000002c
```

# Running SERV from Contiki-NG

## Setup Contiki-NG for SERV

```shell
git clone https://github.com/nfi/contiki-ng-serv.git
```

You might need to change the path to Zephyr SDK if you have not
installed it in `~/.local/opt` or have a different version. You can
either specify `ZEPHYRSDK` as argument when building or update the
default in `arch/cpu/serv/Makefile.serv`.

## Build hello-world

```shell
cd contiki-ng-serv/examples/hello-world
make TARGET=servant BOARD=serv build/servant/serv/hello-world.hex
```

## Run hello-world

Switch back to fusesoc.
```shell
cd $WORKSPACE
fusesoc run --target=verilator_tb servant --uart_baudrate=57600 --memsize=1048576 --firmware=../contiki-ng-serv/examples/hello-world/build/servant/serv/hello-world.hex
```

## Resources
* SERV (https://github.com/olofk/serv)
* Verilator (https://verilator.org/guide/)
* FuseSoC (https://fusesoc.net/)
* Zephyr SDK (https://github.com/zephyrproject-rtos/sdk-ng)
