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

# QIRP 依赖接线由 psdk-image.bbclass 统一提供：该 class 的 python() 里
# `"proprietary" in pn` 分支对本镜像 PN(qcom-multimedia-proprietary-image) 天然成立，
# 故自动挂三包组(robotics-opensource / oss-with-prop-deps / robotics-proprietary)，
# 本文件无需再覆盖任何包组列表。
