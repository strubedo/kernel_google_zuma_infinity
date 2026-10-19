# SPDX-License-Identifier: GPL-2.0
# Infinity: what the driver's own top Makefile exported to its sub-makes.
# Firmware via request_firmware (GKI has no filp_open for modules), no
# preallocated RX buffers (they would hold memory with no adapter plugged in).
export CONFIG_USE_FW_REQUEST = y
export CONFIG_PREALLOC_RX_SKB = n
export CONFIG_PREALLOC_TXQ = y
export CONFIG_WOWLAN = n
export CONFIG_PLATFORM_UBUNTU = y

# Android's cfg80211 API (our 0001 patch); the driver's 4 known warnings
# are reviewed (HANDOFF) - not errors here
subdir-ccflags-y += -DAIC_ANDROID_GKI -Wno-error

obj-$(CONFIG_AIC_LOADFW_SUPPORT) += aic_load_fw/
obj-$(CONFIG_AIC8800_WLAN_SUPPORT) += aic8800_fdrv/
