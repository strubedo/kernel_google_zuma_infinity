# Infinity Kernel for the Google Pixel 8 (shiba)

Infinity is a monolithic, Sultan-style kernel for the **Pixel 8 (shiba, zuma SoC)** on
**Android 17, build CP3A.261005.005**. It is Ante0's `tensynos` tree (17.0.0-cp3a) with
root, a newer TCP congestion control, USB Wi-Fi and System V IPC added.

`uname -r`: `6.1.145-Infinity+`

> Pixel 8 only. Other zuma/zumapro devices (8 Pro, 8a, 9 series) are not tested.
> Flashing a custom kernel needs an unlocked bootloader and can leave the phone
> unbootable until you flash stock images back. Keep the stock `boot.img` and
> `vendor_kernel_boot.img` from the CP3A factory image at hand.

## What Infinity adds on top of the base tree

| Change | Notes |
| --- | --- |
| KernelSU-Next (built in) | KernelSU-Next `f8ab5e8`, vendored as `KernelSU-Next/{kernel,uapi}`; `drivers/kernelsu` is a symlink to `../KernelSU-Next/kernel`. Builds its LSM hooks without kprobes (this kernel has no loadable modules, so no `CONFIG_KPROBES`). Its symbol lookups accept a unique `<name>.lto_priv.0`, because GCC LTO renames some globals; the built-in copy calls the LSM secctx API directly. |
| SUSFS (built in) | simonpunk's susfs4ksu `gki-android14-6.1` @ `5369359` plus two commits by pershoot (SUSFS and toolkit coexistence, core without features). Features on: SUS_PATH, SUS_MOUNT, SUS_KSTAT, SUS_MAP, SPOOF_UNAME, SPOOF_CMDLINE_OR_BOOTCONFIG, OPEN_REDIRECT, HIDE_KSU_SUSFS_SYMBOLS, ENABLE_LOG. |
| BBRv3 (default) | Google's BBRv3 backport (`net/ipv4/tcp_bbr3.c`), set as the default congestion control (`CONFIG_DEFAULT_TCP_CONG="bbr3"`); BBR v1 and CUBIC are still built. |
| AIC8800 USB Wi-Fi | AICSemi AIC8800 (D80) USB driver in `drivers/net/wireless/aic8800`, from shenmintao/aic8800d80 @ `2c4ab4b` with an Android 6.1 cfg80211 API fix. Built in as an integrated module (`CONFIG_AIC8800_USB=m`); see below. The firmware is not part of this repository. |
| System V IPC | `CONFIG_SYSVIPC=y` (for example for a Linux chroot such as Kali NetHunter). |
| Build fixes | `certs/extract-cert.c` builds against OpenSSL without ENGINE support (OpenSSL 4); `LOCALVERSION=-Infinity`. |

Everything else (scheduler, integrated modules, LTO, the `zuma_defconfig`) is the base tree.

### Things that behave differently from a stock Pixel kernel

- **`/proc/config.gz` is not the running config.** The base tree builds it from
  `arch/arm64/configs/gki-stock_defconfig`. The real configuration is
  `arch/arm64/configs/zuma_defconfig` (or `out/.config` after a build).
- **No loadable modules.** Drivers that are modules on a stock kernel are
  "integrated modules" here (`CONFIG_INTEGRATE_MODULES`): the built-in copy starts when
  userspace runs `insmod` on any `.ko` whose module name matches. A started driver shows
  up in `/sys/bus/usb/drivers/<name>` (not in `/proc/modules`). To check whether a driver
  is built in: `grep ' __initdone__kmod_<name>__$' /proc/kallsyms`.
  The AIC8800 driver starts this way (module names `aic_load_fw` and `aic8800_fdrv`)
  and loads its firmware with `request_firmware`.
- **No `/proc/vendor_sched`.** The base tree removes Google's vendor scheduler, so the
  Pixel power HAL's vendor_sched hints have no effect; cpufreq limits, schedutil
  `rate_limit_us` and cgroup `uclamp` work.
- **No 32-bit userspace** (`CONFIG_COMPAT` is off in the base tree).

## Building

Host: x86_64 Linux with the usual kernel build dependencies (make, bc, bison, flex,
libssl-dev/openssl headers, libelf, lz4, python3).

Toolchain: GCC 14.2.0, bare-metal `aarch64-linux` (nolibc), the same toolchain the base
tree bundles under `toolchains/`. It is not included here; put it at
`toolchains/gcc-14.2.0-nolibc/aarch64-linux/` (the layout of the kernel.org crosstool
build `x86_64-gcc-14.2.0-nolibc-aarch64-linux`).

```sh
export ARCH=arm64
export CROSS_COMPILE="$PWD/toolchains/gcc-14.2.0-nolibc/aarch64-linux/bin/aarch64-linux-"
export KBUILD_BUILD_USER=infinity KBUILD_BUILD_HOST=build
# keep the build machine out of the image: no source path in __FILE__, fixed date and build number
export KCFLAGS="-fmacro-prefix-map=$PWD/="
export KBUILD_BUILD_TIMESTAMP="Wed Oct 21 00:00:00 UTC 2026" KBUILD_BUILD_VERSION=1

make O=out zuma_defconfig
make O=out -j"$(nproc)" HOSTCFLAGS=-DOPENSSL_NO_ENGINE
```

`HOSTCFLAGS=-DOPENSSL_NO_ENGINE` is only needed with OpenSSL 4 (or any OpenSSL built
without ENGINE). Do not turn LTO off: the integrated modules rely on it (without LTO the
edgetpu and gxp drivers collide on duplicate gcip symbols). If an LTO link fails with
only "ld returned 1 exit status", run `make` again.

`KCFLAGS` must come from the environment as above: given on the `make` command line it
replaces the Makefile's own `KCFLAGS += -D__ANDROID_COMMON_KERNEL__` and the Mali GPU driver
fails to build. Some vendor Kbuild files (edgetpu) use absolute include paths, so without the
prefix map the full source path of the build machine ends up in the image.

Output: `out/arch/arm64/boot/Image` (and `Image.lz4`) and the device trees
`out/google-devices/zuma/dts/zuma-{a0,b0}-{foplp,ipop}.dtb`.

## Packaging (boot + vendor_kernel_boot)

Infinity replaces two partitions: the kernel in `boot` and the device tree in
`vendor_kernel_boot`. Start from the stock **CP3A.261005.005** images (from Google's
factory image) and repack them with
[magiskboot](https://github.com/topjohnwu/Magisk):

```sh
# boot: swap the kernel
mkdir boot && cd boot
magiskboot unpack /path/to/stock/boot.img
cp ../out/arch/arm64/boot/Image kernel
magiskboot repack /path/to/stock/boot.img ../boot-infinity.img
cd ..

# vendor_kernel_boot: swap the device tree (all four zuma variants, in this order)
mkdir vkb && cd vkb
magiskboot unpack /path/to/stock/vendor_kernel_boot.img
cat ../out/google-devices/zuma/dts/zuma-a0-foplp.dtb \
    ../out/google-devices/zuma/dts/zuma-a0-ipop.dtb \
    ../out/google-devices/zuma/dts/zuma-b0-foplp.dtb \
    ../out/google-devices/zuma/dts/zuma-b0-ipop.dtb > dtb
magiskboot repack /path/to/stock/vendor_kernel_boot.img ../vendor_kernel_boot-infinity.img
cd ..
```

The stock `vendor_kernel_boot` ramdisk (Google's vendor modules) stays as it is.

## Prebuilt images

The release **infinity-v6** (tag `infinity-v6`, built from this source with the settings
above) has `infinity-v6.zip` (`boot-infinity-v6.img` + `vendor_kernel_boot-infinity-v6.img`)
and `SHA256SUMS` (checksums of the zip and of both images). Version string: `6.1.145-Infinity+ (infinity@build) ... #1 SMP
PREEMPT Wed Oct 21 00:00:00 UTC 2026`. Tested on a Pixel 8 with CP3A.261005.005, both slots.

## Flashing

Always flash **both** images, onto the slot you boot:

```sh
fastboot flash boot boot-infinity.img
fastboot flash vendor_kernel_boot vendor_kernel_boot-infinity.img
fastboot reboot
```

With [TWRP for the Pixel 8](https://github.com/strubedo/twrp_device_google_shiba)
(v9.55 or newer), a zip with the two images can also be installed from
Advanced > Install custom kernel.

To go back to stock, flash the stock `boot.img` and `vendor_kernel_boot.img` of the
same build. KernelSU-Next needs its manager app
([KernelSU-Next](https://github.com/KernelSU-Next/KernelSU-Next)) for root.

## Credits

- **Linux kernel** developers and the **Android Common Kernel** / **Google Pixel kernel**
  teams (`google-modules/`, `google-devices/`).
- **Sultan Alsawaf (kerneltoast)** - the Sultan kernel for Pixel, which this tree is built on.
- **Ante0** - [android_kernel_google_tensynos](https://github.com/Ante0/android_kernel_google_tensynos),
  branch 17.0.0-cp3a @ `cc074612d`, the base of this repository (imported unmodified
  except for the bundled `toolchains/`).
- **KernelSU-Next** - [KernelSU-Next/KernelSU-Next](https://github.com/KernelSU-Next/KernelSU-Next)
  (based on KernelSU by tiann).
- **SUSFS** - simonpunk, [susfs4ksu](https://gitlab.com/simonpunk/susfs4ksu); with commits
  by pershoot.
- **BBRv3** - Google's [BBR](https://github.com/google/bbr) team; backport as packaged by
  [WildKernels kernel_patches](https://github.com/WildKernels/kernel_patches).
- **AIC8800** - AICSemi / RivieraWaves vendor driver; Linux packaging by
  [shenmintao/aic8800d80](https://github.com/shenmintao/aic8800d80).

## License

The kernel is licensed under the **GNU General Public License v2.0 only**, see
[`COPYING`](COPYING) and [`LICENSES/`](LICENSES). Individual files carry their own SPDX
tags or license headers; third-party parts keep their licenses:

- `KernelSU-Next/kernel/`: GPL-2.0 ([`KernelSU-Next/kernel/LICENSE`](KernelSU-Next/kernel/LICENSE));
  the KernelSU-Next project as a whole (manager, userspace) is GPL-3.0
  ([`KernelSU-Next/LICENSE`](KernelSU-Next/LICENSE)).
- `fs/susfs.c`, `include/linux/susfs*.h`: from susfs4ksu (see that project for its terms).
- `net/ipv4/tcp_bbr3.c`: Dual BSD/GPL (`MODULE_LICENSE`).
- `drivers/net/wireless/aic8800/`: vendor driver, `MODULE_LICENSE("GPL")`; copyright
  AICSemi and RivieraWaves as stated in the files.

The original Android Common Kernel README is kept as
[`README.android-common-kernel.md`](README.android-common-kernel.md).
