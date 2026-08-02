# Copyright (c) 2026 Qualcomm Innovation Center, Inc. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause-Clear
#
# linux-qcom-next_git.bbappend  （meta-qcom-distro 层 · 集成层）
# ----------------------------------------------------------------------------
# 机器人 distro 变体所需的内核调测配置（ftrace / uprobes / tracepoints 等）。
#
# 原先此 bbappend 在 meta-qcom-robotics-sdk 层且【无条件生效】—— 只要该层在
# bblayers 里，meta-qcom 的内核 recipe 就被改。这正是 Nicolas 指出的
# 「bbappends which modify meta-qcom recipes」需要清理的对象。
#
# 现改为：文件落集成层（与 sota/selinux 同层），且【ros2-jazzy 门控】——
# 只有选中机器人 distro 变体时才合入 robotics-kernel.cfg；
# 普通构建（distro=qcom-distro）下 SRC_URI 不追加、配置命令不改，
# meta-qcom 的内核 recipe 逐字节不受影响。
# ----------------------------------------------------------------------------

FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

# 仅机器人变体追加内核配置片段
SRC_URI += "${@bb.utils.contains('DISTRO_FEATURES', 'ros2-jazzy', ' file://configs/robotics-kernel.cfg', '', d)}"

# 仅机器人变体在 kernel 配置阶段合入该片段（普通构建为空字符串，命令不变）
KERNEL_CONFIG_COMMAND:prepend = "${@bb.utils.contains('DISTRO_FEATURES', 'ros2-jazzy', \
    '${S}/scripts/kconfig/merge_config.sh -m -O ${B} ${B}/.config ${UNPACKDIR}/configs/robotics-kernel.cfg;', \
    '', d)}"
