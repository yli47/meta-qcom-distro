# Copyright (c) 2025 Qualcomm Innovation Center, Inc. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause-Clear
#
# robotics-image-increment.inc  （meta-qcom-distro 层 · 集成层）
# ----------------------------------------------------------------------------
# 「可选变体」增量：把 qcom-robotics-image.bb 相对 qcom-multimedia-image.bb 的
# 全部增量，在【选中机器人 distro 变体】时可选地叠加到底座镜像上。
#
# 设计原则（对齐 qcom-distro-sota.conf 的可选变体范式，不污染 base）：
#   1. 全部逻辑用 DISTRO_FEATURES 含 'ros2-jazzy' 门控；普通构建
#      (distro=qcom-distro，无 ros2-jazzy) 下本文件的每一项都是空操作，
#      底座 qcom-multimedia-image / qcom-multimedia-proprietary-image 逐字节不变。
#   2. 集成表达在 meta-qcom-distro 层（与 qcom-distro-sota.inc 等变体增量同层）；
#      入口 meta-qcom 保持纯 BSP；组件（packagegroup/qirp-sdk/qrb-ros/classes）在
#      meta-qcom-robotics-sdk 层。robotics-sdk 层只出现在机器人变体的 bblayers，
#      普通 multimedia 构建根本不加载它所提供的包组 —— 双重保险。
#   3. psdk-image.bbclass / rootfs-symlink.bbclass 保持上游原样、由 SDK 层提供：
#      改用 inherit_defer 按门控条件 inherit；QIRP 依赖接线复用 psdk-image.bbclass
#      的 python()（该 class 触发条件已扩展为「PN 白名单 或 ros2-jazzy 变体」），
#      故本文件不再复刻接线，单一真源、无维护漂移。
#
# 增量内容 = 官方 qcom-robotics-image.bb 相对 multimedia 的 5 项：
#   ① packagegroup-qcom-ros2  ② packagegroup-robotics-opensource  ③ qirp-sdk
#   ④ inherit psdk-image       ⑤ inherit rootfs-symlink + ROOTFS_SYMLINK_PAIRS
# ----------------------------------------------------------------------------

# ①②③ 三个包，仅在机器人变体时加入
CORE_IMAGE_BASE_INSTALL:append = "${@bb.utils.contains('DISTRO_FEATURES', 'ros2-jazzy', ' \
    packagegroup-qcom-ros2 \
    packagegroup-robotics-opensource \
    qirp-sdk \
', '', d)}"

# ④⑤ 条件 inherit：普通构建门控为空 → 不 inherit → psdk-image 的接线永不触发
inherit_defer ${@bb.utils.contains('DISTRO_FEATURES', 'ros2-jazzy', 'psdk-image', '', d)}
inherit_defer ${@bb.utils.contains('DISTRO_FEATURES', 'ros2-jazzy', 'rootfs-symlink', '', d)}

# ⑤ /usr/ros → /opt/ros(非 sota) 或 /var/rootdirs/opt/ros(sota)，逐字对齐官方 recipe，
#    再套一层 ros2-jazzy 门控使普通构建为空。
ROOTFS_SYMLINK_PAIRS = "${@bb.utils.contains('DISTRO_FEATURES', 'ros2-jazzy', \
    bb.utils.contains('DISTRO_FEATURES', 'sota', \
        '/usr/ros:/var/rootdirs/opt/ros', \
        '/usr/ros:/opt/ros', d), \
    '', d)}"

# QIRP SDK 依赖接线：由 meta-qcom-robotics-sdk 的 psdk-image.bbclass 统一提供。
# 该 class 现按「PN 白名单 或 DISTRO_FEATURES 含 ros2-jazzy」触发（见其 python()），
# 故本镜像(PN=qcom-multimedia-image*)在机器人变体下会命中同一段接线：
# 开源版 → packagegroup-robotics-opensource；proprietary 版 → 三包组
# (class 内 `"proprietary" in pn` 分支对 qcom-multimedia-proprietary-image 天然成立)。
# 因此此处无需再复刻接线逻辑，避免与 class 维护漂移。

