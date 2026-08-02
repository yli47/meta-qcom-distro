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
#   3. psdk-image.bbclass / rootfs-symlink.bbclass 保持上游原样、零改动：
#      改用 inherit_defer 按门控条件 inherit；QIRP 依赖接线在本文件内用门控
#      python() 复刻（因本镜像 PN 不在 psdk-image.bbclass 的 PN 白名单内）。
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

# ④⑤ 条件 inherit：普通构建门控为空 → 不 inherit → psdk-image 的 PN 白名单永不触发
inherit_defer ${@bb.utils.contains('DISTRO_FEATURES', 'ros2-jazzy', 'psdk-image', '', d)}
inherit_defer ${@bb.utils.contains('DISTRO_FEATURES', 'ros2-jazzy', 'rootfs-symlink', '', d)}

# ⑤ /usr/ros → /opt/ros(非 sota) 或 /var/rootdirs/opt/ros(sota)，逐字对齐官方 recipe，
#    再套一层 ros2-jazzy 门控使普通构建为空。
ROOTFS_SYMLINK_PAIRS = "${@bb.utils.contains('DISTRO_FEATURES', 'ros2-jazzy', \
    bb.utils.contains('DISTRO_FEATURES', 'sota', \
        '/usr/ros:/var/rootdirs/opt/ros', \
        '/usr/ros:/opt/ros', d), \
    '', d)}"

# QIRP SDK 依赖接线：复刻 psdk-image.bbclass 的 python() 逻辑（该 class 按 PN 白名单
# 只认 qcom-robotics-image*；本镜像 PN=qcom-multimedia-image*，故须在此自行补 depends）。
# 包组列表由 ROBOTICS_QIRP_PKGGROUPS 提供：开源版=1 组（下方默认），proprietary 版由
# 其 bbappend 覆盖为 3 组（与官方 python() 的 proprietary 分支逐字一致）。
ROBOTICS_QIRP_PKGGROUPS ?= "packagegroup-robotics-opensource"

python () {
    if not bb.utils.contains('DISTRO_FEATURES', 'ros2-jazzy', True, False, d):
        return

    # 1) 挂 packagegroup 的 RDEPENDS 收集任务（对齐 psdk-image.bbclass python() 前半段）
    pkg_groups = (d.getVar('ROBOTICS_QIRP_PKGGROUPS') or "").split()
    for pkg_group in pkg_groups:
        d.appendVarFlag('do_generate_qirp_sdk', 'depends',
                        ' {}:do_collect_rdepends'.format(pkg_group))
    bb.note("robotics-increment: QIRP depends on packagegroups: {}".format(", ".join(pkg_groups)))

    # 2) 挂样例源码 copy 任务（对齐 psdk-image.bbclass python() 后半段，读 content_config.json）
    #    ROBIOTICS_LAYER_DIR 由 meta-qcom-robotics-sdk/conf/layer.conf 设为全局变量，
    #    指向 robotics-sdk 层根；本 .inc 虽在 distro 层，仍能正确定位样例配置。
    import json
    import os
    robotics_layer_dir = d.getVar("ROBIOTICS_LAYER_DIR")
    config_file = os.path.join(robotics_layer_dir, "recipes-sdk/files/content_config.json")
    if os.path.exists(config_file):
        try:
            with open(config_file, 'r') as f:
                config_data = json.load(f)
            sample_names = [s.get("name") for s in config_data.get("samples", []) if s.get("name")]
            for sample_name in sample_names:
                d.appendVarFlag('do_generate_qirp_sdk', 'depends',
                                ' {}:do_copy_source_to_deploy'.format(sample_name))
        except Exception as e:
            bb.warn("robotics-increment: failed to parse content_config.json: {}".format(str(e)))
    else:
        bb.warn("robotics-increment: content_config.json not found at: {}".format(config_file))
}
