/*
 * $Id: $
 * 
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 */

#ifndef __BCMX_STG_H__
#define __BCMX_STG_H__

#include <bcm/types.h>
#include <bcmx/bcmx.h>
#include <bcmx/lplist.h>
#include <bcm/stg.h>

/* 
 * Initialize the Spanning Tree Group (STG) module to its initial
 * configuration.
 */
extern int bcmx_stg_init(void);

/* Designate the default STG ID for the chip. */
extern int bcmx_stg_default_set(
    bcm_stg_t stg);

/* Return the current default STG ID for the chip. */
extern int bcmx_stg_default_get(
    bcm_stg_t *stg_ptr);

/* Add a VLAN to a specified STG. */
extern int bcmx_stg_vlan_add(
    bcm_stg_t stg, 
    bcm_vlan_t vid);

/* Remove a VLAN from a Spanning Tree Group. */
extern int bcmx_stg_vlan_remove(
    bcm_stg_t stg, 
    bcm_vlan_t vid);

/* Remove all VLANs from a Spanning Tree Group. */
extern int bcmx_stg_vlan_remove_all(
    bcm_stg_t stg);

/* Generates a list of VLANs in a specified STG. */
extern int bcmx_stg_vlan_list(
    bcm_stg_t stg, 
    bcm_vlan_t **list, 
    int *count);

/* Destroy a list returned by bcmx_stg_vlan_list. */
extern int bcmx_stg_vlan_list_destroy(
    bcm_vlan_t *list, 
    int count);

/* Create a new STG, using a new STG ID. */
extern int bcmx_stg_create(
    bcm_stg_t *stg_ptr);

/* Create a new STG, using a specific STG ID. */
extern int bcmx_stg_create_id(
    bcm_stg_t stg);

/* Destroys the specified STG. */
extern int bcmx_stg_destroy(
    bcm_stg_t stg);

/* Generates a list of IDs for currently defined STGs. */
extern int bcmx_stg_list(
    bcm_stg_t **list, 
    int *count);

/* 
 * Destroys a previously returned list of STG IDs; the STGs themselves
 * are not affected.
 */
extern int bcmx_stg_list_destroy(
    bcm_stg_t *list, 
    int count);

/* Set the Spanning Tree Protocol state of a port in the specified STG. */
extern int bcmx_stg_stp_set(
    bcm_stg_t stg, 
    bcmx_lport_t port, 
    int stp_state);

/* 
 * Return the Spanning Tree Protocol state of a port in the specified
 * STG.
 */
extern int bcmx_stg_stp_get(
    bcm_stg_t stg, 
    bcmx_lport_t port, 
    int *stp_state);

/* 
 * Return the maximum number of STGs that the underlying hardware can
 * support.
 */
extern int bcmx_stg_count_get(
    int *max_stg);

#endif /* __BCMX_STG_H__ */
