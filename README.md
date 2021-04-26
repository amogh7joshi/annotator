# Annotator

**Annotator** is a lightweight framework for constructing object detection annotations. It's built only
using OpenCV, with no other third party libraries involved. It consists of a makeshift GUI with buttons
corresponding to different labels, and a two-click annotation interface. 

## Usage

To use Annotator yourself, first clone this repository: 

```shell script
git clone https://github.com/amogh7joshi/annotator.git
```

Then, enter the directory and execute the following command to build the compiled program, which will be 
triggered by the `build.sh` build script:

```shell script
chmod +x ./build.sh
./build.sh
```

Now, you can edit the `config.txt` file with your own parameters for execution.
Each of the three lines corresponds to the following parameters:

1. The path to the directory containing images (or a directory containing directories of images).
2. The list of labels that you want to annotate, space-separated.
3. Whether to recursively search through the sub-directories of the parent path (either 'true' or 'false'.)

Finally, execute the following command and an annotator session will begin:

```shell script
./annotator
```







