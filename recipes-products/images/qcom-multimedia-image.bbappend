# In bbappend, so it doesn't affect other images which are based on
# qcom-multimedia-image

# Prevent closed-source packages from being installed into the image
BAD_RECOMMENDATIONS += " \
    libfastcvdsp-stub1 \
    libfastcvopt1 \
    libvulkan-adreno1 \
"

# Error out if any of the closed source packages get pulled into the image
INCOMPATIBLE_LICENSE = "LICENSE.qcom LICENSE.qcom-2"

# Allow closed source firmware packages
INCOMPATIBLE_LICENSE_EXCEPTIONS = "\
    camxfirmware-hamoa:LICENSE.qcom-2 \
    camxfirmware-kodiak:LICENSE.qcom-2 \
    camxfirmware-lemans:LICENSE.qcom-2 \
    camxfirmware-monaco:LICENSE.qcom-2 \
    camxfirmware-talos:LICENSE.qcom-2 \
    firmware-qcom-boot-glymur:LICENSE.qcom-2 \
    firmware-qcom-boot-iq-x7181:LICENSE.qcom-2 \
    firmware-qcom-boot-kaanapali:LICENSE.qcom-2 \
    firmware-qcom-boot-qcs615:LICENSE.qcom-2 \
    firmware-qcom-boot-qcs6490:LICENSE.qcom-2 \
    firmware-qcom-boot-qcs8300:LICENSE.qcom-2 \
    firmware-qcom-boot-qcs9100:LICENSE.qcom-2 \
    firmware-qcom-boot-qrb2210:LICENSE.qcom-2 \
    firmware-qcom-boot-qrb2210-rb1:LICENSE.qcom \
    firmware-qcom-boot-shikra:LICENSE.qcom-2 \
    firmware-qcom-boot-sm8750:LICENSE.qcom-2 \
    trusted-firmware-a-qcom:LICENSE.qcom \
"

# QA check considers packages in INCOMPATIBLE_LICENSE_EXCEPTIONS list still to
# be an error. Disable the check as we need to include boot firmware into the
# image.
ERROR_QA:remove = "license-exception"

# ----------------------------------------------------------------------------
# 「可选变体」机器人增量（sota 范式，不污染 base）
# ----------------------------------------------------------------------------
# 让底座 qcom-multimedia-image 在【选中机器人 distro 变体(DISTRO_FEATURES 含
# ros2-jazzy)】时，可选地长出与 qcom-robotics-image 等价的开源机器人增量；
# 普通构建(distro=qcom-distro，无 ros2-jazzy)下所 require 的增量全部为空操作，
# 底座逐字节不变。target 镜像名不变。
#
# 增量单一真源见同目录 robotics-image-increment.inc（全部 ros2-jazzy 门控）。
require recipes-products/images/robotics-image-increment.inc
