local files:

mkdir offline_osrt
cd ./offline_osrt
wget https://software-dl.ti.com/jacinto7/esd/tidl-tools/11_00_08_00/OSRT_TOOLS/ARM_LINUX/ARAGO/11_0/onnxruntime_tidl-1.15.0-cp312-cp312-linux_aarch64.whl
wget https://software-dl.ti.com/jacinto7/esd/tidl-tools/11_00_08_00/OSRT_TOOLS/ARM_LINUX/ARAGO/11_0/dlr-1.13.0-py3-none-any.whl
wget https://software-dl.ti.com/jacinto7/esd/tidl-tools/11_00_08_00/OSRT_TOOLS/ARM_LINUX/ARAGO/11_0/onnx_1.15.0_aragoj7.tar.gz
wget https://software-dl.ti.com/jacinto7/esd/tidl-tools/11_00_08_00/OSRT_TOOLS/ARM_LINUX/ARAGO/11_0/tflite_runtime-2.12.0-cp312-cp312-linux_aarch64.whl
wget https://software-dl.ti.com/jacinto7/esd/tidl-tools/11_00_08_00/OSRT_TOOLS/ARM_LINUX/ARAGO/11_0/tflite_2.12_aragoj7.tar.gz
wget https://software-dl.ti.com/jacinto7/esd/tidl-tools/11_00_08_00/FIRMWARES/am68a/adas/11_0/firmware.tar.gz
wget https://software-dl.ti.com/jacinto7/esd/tidl-tools/11_00_08_00/FIRMWARES/am68a/adas/11_0/tidl_lib.tar.gz
git clone -b master git://git.ti.com/processor-sdk-vision/arm-tidl.git


env:
export SOC=am68a
export TISDK_IMAGE=adas
export SDK_VERSION=11_0

impl:

cp -rp offline_osrt ${HOME}
./offline_update_target.sh

---------------------------------------------
sudo docker run -it  -v $(pwd)/workspace:/home/share  --name ti_container_tidl_11_00_08_00z  --shm-size=4096m --mount source=$(pwd),target=/home/root,type=bind edgeai_tidl_tools_x86_ubuntu_22


--
download_models.sh
#apt install dialog
SOC=j721s2 ./download_models.sh
