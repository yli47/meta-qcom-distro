require qcom-console-image.bb

SUMMARY = "Basic Wayland image with Weston"

IMAGE_FEATURES += "weston"

# Optional robotics content (ROS 2, QIRP SDK, qrb-ros), contributed by the
# meta-qcom-robotics-sdk component layer and turned on by the "ros2-jazzy"
# distro feature. This recipe only probes for the feature - exactly as it
# already does for "x11" below - while the package set behind it lives in
# FEATURE_PACKAGES_ros2-jazzy (conf/distro/include/qcom-distro-robotics.inc).
# The weak default keeps "ros2-jazzy" a valid image feature even when the
# robotics layer is absent, so a stray DISTRO_FEATURES entry degrades to "no
# extra packages" instead of making this recipe unparsable.
FEATURE_PACKAGES_ros2-jazzy ??= ""
IMAGE_FEATURES += "${@bb.utils.contains('DISTRO_FEATURES', 'ros2-jazzy', 'ros2-jazzy', '', d)}"

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

# let's make sure we have a good image.
REQUIRED_DISTRO_FEATURES += "wayland"
