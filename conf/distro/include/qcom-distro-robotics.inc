# Copyright (c) 2026 Qualcomm Innovation Center, Inc. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause-Clear

DISTRO_FEATURES:append = " ros2-jazzy"
DISTRO_NAME:append = " (ROS-enabled)"

# --- image content ---------------------------------------------------------
# The package set behind the "ros2-jazzy" image feature. Base image recipes only
# name the feature; this is where it is given meaning. Mirrors the increment of
# qcom-robotics-image.bb over qcom-multimedia-image.bb.
FEATURE_PACKAGES_ros2-jazzy = "\
    packagegroup-qcom-ros2 \
    packagegroup-robotics-opensource \
    qirp-sdk \
"

# The proprietary image adds two more packagegroups, matching
# qcom-robotics-proprietary-image.bb.
FEATURE_PACKAGES_ros2-jazzy:append:pn-qcom-multimedia-proprietary-image = "\
    packagegroup-oss-with-prop-deps \
    packagegroup-robotics-proprietary \
"

# qcom-container-orchestration-image.bb requires qcom-multimedia-proprietary-image.bb
# and would otherwise inherit the feature request through the require chain.
# Keep the robotics payload confined to the two product images.
# NOTE: any future image that requires qcom-multimedia-image.bb and should NOT
# carry robotics content needs a line here, next to the IMAGE_CLASSES list below.
IMAGE_FEATURES:remove:pn-qcom-container-orchestration-image = "ros2-jazzy"

# --- image classes ---------------------------------------------------------
# QIRP SDK assembly and the /usr/ros compatibility symlink are per-image
# concerns. IMAGE_CLASSES is consumed via IMGCLASSES + inherit_defer
# (image.bbclass:14,25), i.e. after override expansion, so a pn-scoped append
# reaches exactly the named recipes. A bare "IMAGE_CLASSES +=" would reach every
# core-image recipe including the ~30 initramfs images, and
# initramfs-ostree-image ("inherit core-image nopackages") would break on
# do_populate_sdk.
ROBOTICS_IMAGE_CLASSES = "psdk-image rootfs-symlink"
IMAGE_CLASSES:append:pn-qcom-multimedia-image = " ${ROBOTICS_IMAGE_CLASSES}"
IMAGE_CLASSES:append:pn-qcom-multimedia-proprietary-image = " ${ROBOTICS_IMAGE_CLASSES}"

# /usr/ros -> /opt/ros, or /var/rootdirs/opt/ros when ostree owns /usr.
ROBOTICS_ROS_SYMLINK = "${@bb.utils.contains('DISTRO_FEATURES', 'sota', '/usr/ros:/var/rootdirs/opt/ros', '/usr/ros:/opt/ros', d)}"
ROOTFS_SYMLINK_PAIRS:pn-qcom-multimedia-image = "${ROBOTICS_ROS_SYMLINK}"
ROOTFS_SYMLINK_PAIRS:pn-qcom-multimedia-proprietary-image = "${ROBOTICS_ROS_SYMLINK}"

# Packagegroups whose do_collect_rdepends output feeds QIRP SDK generation.
# Consumed by both the python and the shell side of psdk-image.bbclass, which
# no longer hardcodes a recipe-name whitelist.
QIRP_SDK_PACKAGEGROUPS:pn-qcom-multimedia-image = "packagegroup-robotics-opensource"
QIRP_SDK_PACKAGEGROUPS:pn-qcom-multimedia-proprietary-image = "packagegroup-robotics-opensource packagegroup-oss-with-prop-deps packagegroup-robotics-proprietary"

# --- kernel ----------------------------------------------------------------
# Tracing/probing options needed by ROS 2 tooling, merged through the kernel
# recipes' existing KBUILD_CONFIG_EXTRA hook (linux-qcom_6.18.bb:50-54 and
# linux-qcom-next_git.bb:46-50 feed it to merge_config.sh in do_configure:prepend),
# so no kernel bbappend is required. The checksum entry is routed through a
# PN-specialised variable because "do_configure[file-checksums]:append:pn-X" is
# not valid conf syntax; non-kernel recipes get an empty value, which contributes
# nothing to their task hashes.
ROBOTICS_KERNEL_CFG = "${ROBIOTICS_LAYER_DIR}/recipes-kernel/linux/files/robotics-kernel.cfg"
KBUILD_CONFIG_EXTRA:append:pn-linux-qcom = " ${ROBOTICS_KERNEL_CFG}"
KBUILD_CONFIG_EXTRA:append:pn-linux-qcom-next = " ${ROBOTICS_KERNEL_CFG}"
ROBOTICS_KERNEL_CFG_CHECKSUM = ""
ROBOTICS_KERNEL_CFG_CHECKSUM:pn-linux-qcom = "${ROBOTICS_KERNEL_CFG}:True"
ROBOTICS_KERNEL_CFG_CHECKSUM:pn-linux-qcom-next = "${ROBOTICS_KERNEL_CFG}:True"
do_configure[file-checksums] += "${ROBOTICS_KERNEL_CFG_CHECKSUM}"

# --- SDK -------------------------------------------------------------------
# Cross-compilation support for ROS nodes. Provided by the component layer; the
# unconditional statements inside are safe only because this file is required by
# robotics distro variants only.
require conf/distro/include/qcom-robotics-sdk.inc
