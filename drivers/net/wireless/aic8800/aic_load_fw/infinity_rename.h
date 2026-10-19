/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Infinity: aic_load_fw and aic8800_fdrv each define these globals for
 * themselves. As separate modules that is fine; built into one kernel they
 * collide, so aic_load_fw's copies get an aiclf_ prefix (forced in via
 * -include, aic_load_fw only). Generated from the linker's 53 "multiple
 * definition" errors; layouts and exports to the kernel are unchanged.
 */
#ifndef _AIC_LOAD_FW_INFINITY_RENAME_H
#define _AIC_LOAD_FW_INFINITY_RENAME_H
#define adaptivity_patch_tbl aiclf_adaptivity_patch_tbl
#define aic_fw_path aiclf_aic_fw_path
#define aicwf_bus_deinit aiclf_aicwf_bus_deinit
#define aicwf_bus_init aiclf_aicwf_bus_init
#define aicwf_dbg_level aiclf_aicwf_dbg_level
#define aicwf_dev_skb_free aiclf_aicwf_dev_skb_free
#define aicwf_frame_dequeue aiclf_aicwf_frame_dequeue
#define aicwf_frame_enq aiclf_aicwf_frame_enq
#define aicwf_frame_queue_flush aiclf_aicwf_frame_queue_flush
#define aicwf_frame_queue_init aiclf_aicwf_frame_queue_init
#define aicwf_frame_queue_peek_tail aiclf_aicwf_frame_queue_peek_tail
#define aicwf_frame_tx aiclf_aicwf_frame_tx
#define aicwf_is_framequeue_empty aiclf_aicwf_is_framequeue_empty
#define aicwf_process_rxframes aiclf_aicwf_process_rxframes
#define aicwf_rx_deinit aiclf_aicwf_rx_deinit
#define aicwf_rxframe_enqueue aiclf_aicwf_rxframe_enqueue
#define aicwf_rx_init aiclf_aicwf_rx_init
#define aicwf_set_cmd_tx aiclf_aicwf_set_cmd_tx
#define aicwf_tx_deinit aiclf_aicwf_tx_deinit
#define aicwf_tx_init aiclf_aicwf_tx_init
#define aicwf_usb_exit aiclf_aicwf_usb_exit
#define aicwf_usb_register aiclf_aicwf_usb_register
#define aicwf_usb_tx_flowctrl aiclf_aicwf_usb_tx_flowctrl
#define chip_id aiclf_chip_id
#define chip_mcu_id aiclf_chip_mcu_id
#define chip_sub_id aiclf_chip_sub_id
#define get_userconfig_xtal_cap aiclf_get_userconfig_xtal_cap
#define MD5Decode aiclf_MD5Decode
#define MD5Encode aiclf_MD5Encode
#define MD5Final aiclf_MD5Final
#define MD5Init aiclf_MD5Init
#define MD5Transform aiclf_MD5Transform
#define MD5Update aiclf_MD5Update
#define PADDING aiclf_PADDING
#define patch_tbl aiclf_patch_tbl
#define rf_tbl_masked aiclf_rf_tbl_masked
#define rwnx_atoi aiclf_rwnx_atoi
#define rwnx_cmd_mgr_deinit aiclf_rwnx_cmd_mgr_deinit
#define rwnx_cmd_mgr_init aiclf_rwnx_cmd_mgr_init
#define rwnx_plat_userconfig_parsing aiclf_rwnx_plat_userconfig_parsing
#define rwnx_rx_handle_msg aiclf_rwnx_rx_handle_msg
#define rwnx_send_dbg_mem_block_write_req aiclf_rwnx_send_dbg_mem_block_write_req
#define rwnx_send_dbg_mem_mask_write_req aiclf_rwnx_send_dbg_mem_mask_write_req
#define rwnx_send_dbg_mem_read_req aiclf_rwnx_send_dbg_mem_read_req
#define rwnx_send_dbg_mem_write_req aiclf_rwnx_send_dbg_mem_write_req
#define rwnx_send_dbg_start_app_req aiclf_rwnx_send_dbg_start_app_req
#define rwnx_send_reboot aiclf_rwnx_send_reboot
#define syscfg_tbl aiclf_syscfg_tbl
#define syscfg_tbl_u04 aiclf_syscfg_tbl_u04
#define system_config_8800d80 aiclf_system_config_8800d80
#define testmode aiclf_testmode
#define usb_busrx_thread aiclf_usb_busrx_thread
#define usb_bustx_thread aiclf_usb_bustx_thread
#endif
