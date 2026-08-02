# Copyright (c) 2026 Qualcomm Innovation Center, Inc. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause-Clear
#
# linux-qcom_6.18.bbappend  （meta-qcom-distro 层 · 集成层）
# ----------------------------------------------------------------------------
# 与同目录 linux-qcom-next_git.bbappend 同理，只是目标内核 recipe 不同（6.18 稳定分支）。
# 迁自 meta-qcom-robotics-sdk 并改为 ros2-jazzy 门控：普通构建下 meta-qcom 的内核
# recipe 逐字节不受影响。详见 linux-qcom-next_git.bbappend 头部说明。
# ----------------------------------------------------------------------------

FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += "${@bb.utils.contains('DISTRO_FEATURES', 'ros2-jazzy', ' file://configs/robotics-kernel.cfg', '', d)}"

KERNEL_CONFIG_COMMAND:prepend = "${@bb.utils.contains('DISTRO_FEATURES', 'ros2-jazzy', \
    '${S}/scripts/kconfig/merge_config.sh -m -O ${B} ${B}/.config ${UNPACKDIR}/configs/robotics-kernel.cfg;', \
    '', d)}"
