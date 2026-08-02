# Copyright (c) 2025 Qualcomm Innovation Center, Inc. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause-Clear
#
# qcom-multimedia-proprietary-image.bbappend  （meta-qcom-distro 层 · 集成层）
# ----------------------------------------------------------------------------
# 让底座 qcom-multimedia-proprietary-image 在【选中机器人 distro 变体(ros2-jazzy)】时，
# 可选地长出与 qcom-robotics-proprietary-image 等价的机器人增量；普通构建下为空操作，
# 底座逐字节不变，target 镜像名不变。
#
# 官方 qcom-robotics-proprietary-image.bb 的实质 =
#   qcom-multimedia-proprietary-image  (底座，本文件所 append 的对象)
# + qcom-robotics-image 的全部开源增量  (由下方 require 的 .inc 提供)
# + packagegroup-oss-with-prop-deps / packagegroup-robotics-proprietary  (本文件追加)
# ----------------------------------------------------------------------------

# 复用开源增量单一真源（①②③ 包 + ④⑤ inherit + QIRP 依赖接线框架）。
# 注：bbappend 按镜像文件名匹配、不随 require 传递，故此处须与开源版一样显式 require。
require recipes-products/images/robotics-image-increment.inc

# proprietary 版在开源三包之上，再叠两个专有包组（逐字对齐官方 recipe），同样 ros2-jazzy 门控
CORE_IMAGE_BASE_INSTALL:append = "${@bb.utils.contains('DISTRO_FEATURES', 'ros2-jazzy', ' \
    packagegroup-oss-with-prop-deps \
    packagegroup-robotics-proprietary \
', '', d)}"

# QIRP 依赖接线：proprietary 分支挂 3 个包组（与 psdk-image.bbclass python() 的
# "proprietary" 分支逐字一致）。覆盖 .inc 里的默认单包组值。
ROBOTICS_QIRP_PKGGROUPS = "packagegroup-robotics-opensource packagegroup-oss-with-prop-deps packagegroup-robotics-proprietary"
