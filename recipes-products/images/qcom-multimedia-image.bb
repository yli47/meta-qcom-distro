require qcom-console-image.bb

SUMMARY = "Basic Wayland image with Weston"

IMAGE_FEATURES += "weston"

CORE_IMAGE_BASE_INSTALL += " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'x11', 'weston-xwayland xterm', '', d)} \
    alsa-utils-alsatplg \
    alsa-utils-alsaucm \
    alsa-utils-aplay \
    ${@bb.utils.contains('DISTRO_FEATURES', 'virtualization', 'docker-compose', '', d)} \
    gstd \
    gstreamer1.0 \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-good \
    gstreamer1.0-python \
    libcamera \
    libcamera-gst \
    libdrm-tests \
    ${@bb.utils.contains('DISTRO_FEATURES', 'virtualization', 'packagegroup-container', '', d)} \
    packagegroup-qcom-benchmark \
    ${@bb.utils.contains('DISTRO_FEATURES', 'ros2-jazzy', 'packagegroup-qcom-ros2 packagegroup-robotics-opensource qirp-sdk', '', d)} \
    packagegroup-qcom-test-pkgs \
    packagegroup-qcom-utilities-gpu-utils \
    pipewire \
    pipewire-alsa \
    pipewire-modules-meta \
    pipewire-pulse \
    pipewire-spa-tools \
    pipewire-tools \
    pulseaudio-pactl \
    tensorflow-lite-tools \
    thermald \
    userspace-resource-manager \
    userspace-resource-manager-extensions \
    weston \
    weston-examples \
    weston-init \
    wireplumber \
"

# IMSDK currently only used and tested on ARMv8 (aarch64) machines.
CORE_IMAGE_BASE_INSTALL:append:aarch64 = " gst-plugins-imsdk-oss"

# Robotics classes and the /usr/ros compatibility symlink, gated on "ros2-jazzy"
# just like the packages above, mirroring qcom-robotics-image.bb's increment
# over this image. QIRP_SDK_PACKAGEGROUPS defaults from psdk-image.bbclass.
IMAGE_CLASSES += "${@bb.utils.contains('DISTRO_FEATURES', 'ros2-jazzy', 'psdk-image rootfs-symlink', '', d)}"
ROOTFS_SYMLINK_PAIRS = "${@bb.utils.contains('DISTRO_FEATURES', 'ros2-jazzy', bb.utils.contains('DISTRO_FEATURES', 'sota', '/usr/ros:/var/rootdirs/opt/ros', '/usr/ros:/opt/ros', d), '', d)}"

# let's make sure we have a good image.
REQUIRED_DISTRO_FEATURES += "wayland"
