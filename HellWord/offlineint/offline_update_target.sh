#!/bin/bash
# Copyright (c) 2024, Texas Instruments

set -x

SCRIPTDIR=`pwd`
TARGET_FS_PATH=/

REL="11_00_08_00"
SOC=${SOC:-'null'}
TISDK_IMAGE=${TISDK_IMAGE:-'null'}
SDK_VERSION=${SDK_VERSION:-'null'}
UPDATE_OSRT_COMPONENTS=${UPDATE_OSRT_COMPONENTS:-1}
UPDATE_FIRMWARE_AND_LIB=${UPDATE_FIRMWARE_AND_LIB:-1}
OFFLINE_DIR="$TARGET_FS_PATH/home/root/offline_osrt"

echo "========================================================================="
echo "REL: ${REL}"
echo "SOC: ${SOC}"
echo "TISDK_IMAGE: ${TISDK_IMAGE}"
echo "SDK_VERSION: ${SDK_VERSION}"
echo "UPDATE_OSRT_COMPONENTS: ${UPDATE_OSRT_COMPONENTS}"
echo "UPDATE_FIRMWARE_AND_LIB: ${UPDATE_FIRMWARE_AND_LIB}"
echo "========================================================================="

if [ `arch` != "aarch64" ]; then
    echo "The script must be invoked on aarch64 system"
    exit -1
fi

verify_env() {
    if [ "$REL" != "11_00_08_00" ]; then
        echo "Cannot invoke this script with version $REL. Please use version 11_00_08_00"
    fi

    if [ "$SOC" != "am62" ] && [ "$SOC" != "am62a" ] &&
    [ "$SOC" != "am68a" ] && [ "$SOC" != "am68pa" ] &&
    [ "$SOC" != "am69a" ] && [ "$SOC" != "am67a" ]; then
        echo
        echo "Incorrect SOC defined: $SOC"
        echo "Run either of below commands"
        echo "export SOC=am62"
        echo "export SOC=am62a"
        echo "export SOC=am68a"
        echo "export SOC=am68pa"
        echo "export SOC=am69a"
        echo "export SOC=am67a"
        return 1
    fi

    if [ "$TISDK_IMAGE" != "adas" ] && [ "$TISDK_IMAGE" != "edgeai" ]; then
        echo
        echo "Incorrect TISDK_IMAGE defined: $TISDK_IMAGE"
        echo "Run either of below commands"
        echo "export TISDK_IMAGE=edgeai"
        echo "export TISDK_IMAGE=adas"
        return 1
    fi

    if [ "$SDK_VERSION" != "11_0" ]; then
        echo
        echo "Incorrect SDK_VERSION defined: $SDK_VERSION"
        echo "Allowed values for SDK_VERSION is 11_0"
        return 1
    fi

    if [ "$SOC" == "am62a" ]; then
            echo
            echo "AM62A SDK 11.0 does not exist."
            return 1
    fi

    if [ "$SOC" == "am62a" ] && [ "$TISDK_IMAGE" == "adas" ]; then
        echo
        echo "AM62A does not have ADAS Image. Using EDGEAI"
        TISDK_IMAGE=edgeai
        return 1
    fi

    return 0
}

update_osrt_components() {

    echo
    echo "Updating OSRT components (OFFLINE MODE)"

    if [ ! -d "$OFFLINE_DIR" ]; then
        echo "ERROR: Offline directory not found: $OFFLINE_DIR"
        exit 1
    fi

    cd $TARGET_FS_PATH/$HOME

    mkdir -p arago_j7_pywhl
    mkdir -p required_libs

    ######################################################################
    # DLR
    ######################################################################
    echo "==================== Installing DLR wheel ===================="
    cp $OFFLINE_DIR/dlr-1.13.0-py3-none-any.whl arago_j7_pywhl/
    pip3 install --upgrade --force-reinstall arago_j7_pywhl/dlr-1.13.0-py3-none-any.whl --disable-pip-version-check

    ######################################################################
    # arm-tidl headers
    ######################################################################
    echo "==================== Installing arm-tidl headers ===================="
    cp -r $OFFLINE_DIR/arm-tidl required_libs/

    if [ -d required_libs/arm-tidl ]; then
        [ ! -f "$TARGET_FS_PATH/usr/include/itidl_rt.h.bkp" ] && mv $TARGET_FS_PATH/usr/include/itidl_rt.h $TARGET_FS_PATH/usr/include/itidl_rt.h.bkp
        [ ! -f "$TARGET_FS_PATH/usr/include/itvm_rt.h.bkp" ] && mv $TARGET_FS_PATH/usr/include/itvm_rt.h $TARGET_FS_PATH/usr/include/itvm_rt.h.bkp

        cp required_libs/arm-tidl/rt/inc/itidl_rt.h  $TARGET_FS_PATH/usr/include/
        cp required_libs/arm-tidl/rt/inc/itvm_rt.h   $TARGET_FS_PATH/usr/include/
    fi

    ######################################################################
    # ONNX Runtime
    ######################################################################
    echo "==================== Installing ONNX Runtime wheel ===================="

    if [ "$SDK_VERSION" == "9_2" ]; then
        onnx_wheel=onnxruntime_tidl-1.15.0-cp310-cp310-linux_aarch64.whl
        onnx_tar=onnx_1.15.0_aragoj7_cp310
    else
        onnx_wheel=onnxruntime_tidl-1.15.0-cp312-cp312-linux_aarch64.whl
        onnx_tar=onnx_1.15.0_aragoj7
    fi

    cp $OFFLINE_DIR/$onnx_wheel arago_j7_pywhl/
    pip3 install arago_j7_pywhl/$onnx_wheel --disable-pip-version-check

    echo "==================== Installing ONNX Runtime components ===================="
    cp $OFFLINE_DIR/$onnx_tar.tar.gz required_libs/
    cd required_libs
    tar xf $onnx_tar.tar.gz && rm $onnx_tar.tar.gz

    [ ! -d "$TARGET_FS_PATH/usr/include/onnxruntime.bkp" ] && mv $TARGET_FS_PATH/usr/include/onnxruntime $TARGET_FS_PATH/usr/include/onnxruntime.bkp
    rm -rf $TARGET_FS_PATH/usr/include/onnxruntime
    mv $onnx_tar/onnxruntime $TARGET_FS_PATH/usr/include/

    [ ! -f "$TARGET_FS_PATH/usr/lib/libonnxruntime.so.1.15.0.bkp" ] && mv $TARGET_FS_PATH/usr/lib/libonnxruntime.so.1.15.0 $TARGET_FS_PATH/usr/lib/libonnxruntime.so.1.15.0.bkp
    cp $onnx_tar/libonnxruntime.so.1.15.0 $TARGET_FS_PATH/usr/lib/
    cd $TARGET_FS_PATH/usr/lib/
    ln -sf libonnxruntime.so.1.15.0 libonnxruntime.so

    ######################################################################
    # TFLite
    ######################################################################
    echo "==================== Installing TFLite wheel ===================="

    if [ "$SDK_VERSION" == "9_2" ]; then
        tfl_wheel=tflite_runtime-2.12.0-cp310-cp310-linux_aarch64.whl
    else
        tfl_wheel=tflite_runtime-2.12.0-cp312-cp312-linux_aarch64.whl
    fi

    cp $OFFLINE_DIR/$tfl_wheel arago_j7_pywhl/
    pip3 install --upgrade --force-reinstall arago_j7_pywhl/$tfl_wheel --disable-pip-version-check

    echo "==================== Installing TFLite components ===================="
    cp $OFFLINE_DIR/tflite_2.12_aragoj7.tar.gz required_libs/
    cd required_libs
    tar xf tflite_2.12_aragoj7.tar.gz && rm tflite_2.12_aragoj7.tar.gz

    [ ! -d "$TARGET_FS_PATH/usr/include/tensorflow.bkp" ] && mv $TARGET_FS_PATH/usr/include/tensorflow $TARGET_FS_PATH/usr/include/tensorflow.bkp
    rm -rf $TARGET_FS_PATH/usr/include/tensorflow
    mv tflite_2.12_aragoj7/tensorflow $TARGET_FS_PATH/usr/include/

    [ ! -d "$TARGET_FS_PATH/usr/lib/tflite_2.12.bkp" ] && mv $TARGET_FS_PATH/usr/lib/tflite_2.12 $TARGET_FS_PATH/usr/lib/tflite_2.12.bkp
    rm -rf $TARGET_FS_PATH/usr/lib/tflite_2.12
    mv tflite_2.12_aragoj7/tflite_2.12 $TARGET_FS_PATH/usr/lib/

    [ ! -f "$TARGET_FS_PATH/usr/lib/libtensorflow-lite.a.bkp" ] && mv $TARGET_FS_PATH/usr/lib/libtensorflow-lite.a $TARGET_FS_PATH/usr/lib/libtensorflow-lite.a.bkp
    cp tflite_2.12_aragoj7/libtensorflow-lite.a $TARGET_FS_PATH/usr/lib/

    ######################################################################
    # NUMPY
    ######################################################################
    pip3 install --upgrade --force-reinstall --no-cache-dir numpy==1.26.4 --disable-pip-version-check

    ######################################################################
    # Cleanup
    ######################################################################
    sleep 10
    rm -rf $TARGET_FS_PATH/$HOME/arago_j7_pywhl
    rm -rf $TARGET_FS_PATH/$HOME/required_libs
}

update_firmware_and_lib() {

    echo "Updating Firmwares"
    cd $TARGET_FS_PATH/$HOME

    if [ ! -d "$OFFLINE_DIR" ]; then
        echo "ERROR: Offline directory not found: $OFFLINE_DIR"
        exit 1
    fi

    mkdir -p updated_firmware_and_lib
    cd updated_firmware_and_lib

    ######################################################################
    # Firmware tarball (offline)
    ######################################################################
    echo "==================== Updating C7X firmware ===================="

    if [ "${TISDK_IMAGE}" == "edgeai" ]; then
        FIRMWARE_PATH=$TARGET_FS_PATH/lib/firmware/vision_apps_eaik
    else
        FIRMWARE_PATH=$TARGET_FS_PATH/lib/firmware/vision_apps_evm
    fi

    echo "FIRMWARE_PATH: ${FIRMWARE_PATH}"

    # Copy firmware tarball from offline bundle
    cp $OFFLINE_DIR/firmware.tar.gz .

    if [ ! -f firmware.tar.gz ]; then
        echo "ERROR: firmware.tar.gz not found in offline directory"
        exit 1
    fi

    tar -xf firmware.tar.gz && rm firmware.tar.gz

    # Enter correct subdirectory
    if [ "${TISDK_IMAGE}" == "edgeai" ]; then
        cd firmware/vision_apps_eaik
    else
        cd firmware/vision_apps_evm
    fi

    # Replace firmware files
    for file in `find ./ -name "vx_app_rtos_linux_c*"`; do
        echo "Replacing ${file}"

        # Backup existing file
        if [ -f $FIRMWARE_PATH/$file ]; then
            if [ ! -f "$FIRMWARE_PATH/$file.bkp" ]; then
                mv $FIRMWARE_PATH/$file $FIRMWARE_PATH/$file.bkp
            fi
        else
            echo "WARNING: $file not used in $FIRMWARE_PATH. Still copying"
        fi

        cp $file $FIRMWARE_PATH/
    done

    ######################################################################
    # TIDL libs (offline)
    ######################################################################
    cd $TARGET_FS_PATH/$HOME/updated_firmware_and_lib

    echo "==================== Updating TIDL lib ===================="

    cp $OFFLINE_DIR/tidl_lib.tar.gz .

    if [ ! -f tidl_lib.tar.gz ]; then
        echo "ERROR: tidl_lib.tar.gz not found in offline directory"
        exit 1
    fi

    tar -xf tidl_lib.tar.gz && rm tidl_lib.tar.gz

    cd tidl_lib

    for file in *; do
        echo "Replacing ${file}"

        # Backup existing file
        if [ -f $TARGET_FS_PATH/usr/lib/$file ]; then
            if [ ! -f "$TARGET_FS_PATH/usr/lib/$file.bkp" ]; then
                mv $TARGET_FS_PATH/usr/lib/$file $TARGET_FS_PATH/usr/lib/$file.bkp
            fi
        else
            echo "WARNING: $file not used. Still copying"
        fi

        cp $file $TARGET_FS_PATH/usr/lib/
    done

    ######################################################################
    # Cleanup
    ######################################################################
    rm -rf $TARGET_FS_PATH/$HOME/updated_firmware_and_lib
}

verify_env
if [ "$?" -eq "0" ]; then
    if [ "${UPDATE_OSRT_COMPONENTS}" -eq "1" ]; then
        update_osrt_components
    fi
    if [ "${UPDATE_FIRMWARE_AND_LIB}" -eq "1" ]; then
        update_firmware_and_lib
    fi
fi

cd $SCRIPTDIR
