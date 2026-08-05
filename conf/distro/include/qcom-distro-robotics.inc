# Robotics feature increment, kept orthogonal so it can be composed with the
# other distro variants (sota, selinux) the same way qcom-distro-catchall does.
#
# ros2-jazzy is the DISTRO_FEATURE the multimedia images probe to pull in the
# ROS 2 / QIRP SDK / qrb-ros content; the image recipes gate on the feature
# (not on a distro name), so any distro that requires this .inc grows robotics.
DISTRO_FEATURES:append = " ros2-jazzy"

DISTRO_NAME:append = " (ROS-enabled)"

# ROS SDK cross-compilation support; ros2-jazzy-gated internally, so inert for
# a non-robotics build.
require conf/distro/include/qcom-robotics-sdk.inc
