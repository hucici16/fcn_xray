# Fully Convolutional Models (FCNs) for Semantic Segmentation on X-ray Images
# -- Methodology

Table of Contents
=================
* [Configuration: DIGITS and Caffe](#configuration:-digits-and-caffe)
* [Dataset Creation](#dataset-creation)
    * [Original Dataset](#original-dataset)
    * [Labeling the datasets](#labeling-the-datasets)
    * [Image Processing](#image-processing)
    * [Creating the datasets](#creating-the-datasets)
* [Model Creation](#model-creation)
    * [FCN Version](#fcn-version)
    * [FCN-Alexnet Adaptation: Pretraining](#fcn--alexnet-adaptation:-pretraining)
    * [Training the network in DIGITS](#training-the-network-in-digits)

## Configuration: DIGITS and Caffe

We use the deep learning framework Caffe to train the network. 
We use DIGITS to train the network with a user-friendly network. 
- Caffe
  + Installation: 
	  <http://caffe.berkeleyvision.org/installation.html>
- DIGITS
  + The easiest way to only use DIGITS is to clone it's repo with the following command:
    ```sh
    DIGITS$ git clone https://github.com/NVIDIA/DIGITS.git
    ```
  + To run DIGITS, go to the directory of DIGITS: 
    ```sh
    DIGITS$ ./digits-devserver
    ```

## Dataset Creation

The datasets are created on the raw data with alternation on different needs. 

### Original Dataset

We are provided with 2 datasets, and each with a list of label coordinates
- `IMAGE_622` with 622 X-ray images and 425 of label coordinates; 
- `IMAGE_12227` with 12227 X-ray images and 2103 label coordinates. 

### Labeling the datasets
We label datasets in the followings ways and observe their difference in efficiency, 
- Rectangular label: can be generated directly with label coordinates; 
	 + C++ program location: `imageProcessing`; 
- Precise label: has to be created manually with the help of C++ program. The creation of this kind of label requires a cost of human labor. Thus it is important to make sure the result is worth the work; 
	+ C++ program location: `preciselyLabel`. 

###  Image Processing
Image processing is done on raw datasets in order to generate larger dataset for training. 
The C++ program in `imageProcessing` achieves the following operations: 
- operations on whole images: 
	+ Rotation(2): 90 degrees clockwise, 90 degrees counter-clockwise
	+ Reflection(3): vertically, horizontally, both vertically and horizontally at the same time. 
	+ Intensity(n): change a certain amount of intensity n times. 
- operations only on object areas given the label coordinates: 
	+ Reflection(3): vertically, horizontally, both vertically and horizontally at the same time. 
	+ Intensity(n): change a certain amount of intensity n times. 

### Creating the datasets
- allClasses: generate datasets with multiple labels
- singleClass: generate datasets with a single object class
- singleClassCropped: cropped the raw dataset around the label areas and generate datasets with a single object class 

## Model creation

### FCN Version
This project uses `FCN-Alexnet` from fcn.berkeleyvision.org.

### FCN-Alexnet Adaptation: Pretraining 
- set the number of object class in `fcn_alexnet.prototxt` and `fcn_alexnet.deploy.prototxt`
- run `pretrain_caffe.sh`
- Link to the pretrained networks: [pretrained](https://drive.google.com/drive/folders/1pByojjSbL5vJAg8loQ-L32YaiISMy344?usp=sharing "pretrained FCNs")

### Training the network in DIGITS
Configurations: 
- select the created dataset
- set `Batch size` to `1`
- set `Base Learning Rate` to `0.0001`
- set `Subtract Mean` to `Pixel`
- select `Custom Network` tab and `Caffe sub-tab`
- copy & paste `fcn_alexnet.prototxt`
- specify the path to the pre-trained FCN-Alexnet in `Pretrained model(s)` 
