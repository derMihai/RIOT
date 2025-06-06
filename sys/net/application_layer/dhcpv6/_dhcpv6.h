/*
 * Copyright (C) 2018 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#pragma once

/**
 * @ingroup     net_dhcpv6
 * @{
 *
 * @file
 * @brief   Internal DHCPv6 definitions
 * @note    This header is based on [RFC 8415](https://tools.ietf.org/html/rfc8415)
 *
 * @author  Martine Lenders <m.lenders@fu-berlin.de>
 */

#include <stdint.h>

#include "byteorder.h"
#include "net/ipv6/addr.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    DHCPv6 multicast addresses
 * @see [RFC 8415, section 7.1]
 *      (https://tools.ietf.org/html/rfc8415#section-7.1)
 * @{
 */
/**
 * @brief   Multicast address used by clients to communicate with neighboring
 *          relay agents and servers
 *
 * @note    Corresponds with `All_DHCP_Relay_Agents_and_Servers` in the draft.
 *
 */
#define DHCPV6_ALL_RELAY_AGENTS_AND_SERVERS { 0xff, 0x02, 0x00, 0x00, \
                                              0x00, 0x00, 0x00, 0x00, \
                                              0x00, 0x00, 0x00, 0x00, \
                                              0x00, 0x01, 0x00, 0x02  \
                                            }
/** @} */

/**
 * @name    DHCPv6 transmission and retransmission parameters
 * @see [RFC 8415, section 7.6]
 *      (https://tools.ietf.org/html/rfc8415#section-7.6)
 * @{
 */
#define DHCPV6_SOL_MAX_DELAY        (1U)    /**< SOL_MAX_DELAY (in sec) */
#define DHCPV6_SOL_TIMEOUT          (1U)    /**< SOL_TIMEOUT (in sec) */
#define DHCPV6_SOL_MAX_RT           (3600U) /**< SOL_MAX_RT (in sec) */

#define DHCPV6_REQ_TIMEOUT          (1U)    /**< REQ_TIMEOUT (in sec) */
#define DHCPV6_REQ_MAX_RT           (30U)   /**< REQ_MAX_RT (in sec) */
#define DHCPV6_REQ_MAX_RC           (10U)   /**< REQ_MAX_RC */

#define DHCPV6_REN_TIMEOUT          (10U)   /**< REN_TIMEOUT (in sec) */
#define DHCPV6_REN_MAX_RT           (600U)  /**< REN_MAX_RT (in sec) */

#define DHCPV6_REB_TIMEOUT          (10U)   /**< REB_TIMEOUT (in sec) */
#define DHCPV6_REB_MAX_RT           (600U)  /**< REB_MAX_RT (in sec) */

#define DHCPV6_INF_MAX_DELAY        (1U)    /**< INF_MAX_DELAY (in sec) */
#define DHCPV6_INF_TIMEOUT          (1U)    /**< INF_TIMEOUT (in sec) */
#define DHCPV6_INF_MAX_RT           (3600U) /**< INF_MAX_RT (in sec) */

#define DHCPV6_IRT_DEFAULT          (86400U) /**< IRT_DEFAULT (in sec) */
#define DHCPV6_IRT_MINIMUM          (600U)   /**< IRT_MINIMUM (in sec) */

/** @} */

#define DHCPV6_DUID_MAX_LEN         (128U)  /**< maximum length of DUID */

/**
 * @name DHCPv6 message formats
 * @{
 */
/**
 * @brief   Client/Server message header
 * @see [RFC 8415, section 8]
 *      (https://tools.ietf.org/html/rfc8415#section-8)
 */
typedef struct __attribute__((packed)) {
    uint8_t type;   /**< message type (see [DHCPv6 messeg types ](@ref net_dhcp6_msg_types)) */
    uint8_t tid[3]; /**< transaction ID */
} dhcpv6_msg_t;

/**
 * @brief   Relay Agents/Server message format
 * @see [RFC 8415, section 9]
 *      (https://tools.ietf.org/html/rfc8415#section-9)
 */
typedef struct __attribute__((packed)) {
    uint8_t type;       /**< message type (see [DHCPv6 messeg types ](@ref net_dhcp6_msg_types)) */
    uint8_t hop_count;  /**< number of relays that have already relayed the message */
    /**
     * @brief   optional address to identify the link on which the client is
     *          located.
     */
    ipv6_addr_t link_address;
    /**
     * @brief   The address of the client or relay agent from which the message
     *          to be relayed was received.
     */
    ipv6_addr_t peer_address;
} dhcpv6_relay_msg_t;

/**
 * @brief   Generic storage DUID
 */
typedef union {
    uint8_t u8[DHCPV6_DUID_MAX_LEN];    /**< array representation */
    dhcpv6_duid_l2_t duid_l2;           /**< DUID-L2 type */
} dhcpv6_duid_t;
/** @} */

/**
 * @name DHCPv6 options
 * @{
 */
/**
 * @brief   General DHCPv6 option format
 * @see [RFC 8415, section 21.1]
 *      (https://tools.ietf.org/html/rfc8415#section-21.1)
 */
typedef struct __attribute__((packed)) {
    network_uint16_t type;      /**< [option code](@ref net_dhcp6_opt_codes) */
    network_uint16_t len;       /**< length of dhcpv6_opt_t::data in byte */
    uint8_t data[];             /**< option data */
} dhcpv6_opt_t;

/**
 * @brief   DHCPv6 client or server identifier option format
 * @see [RFC 8415, section 21.2]
 *      (https://tools.ietf.org/html/rfc8415#section-21.2)
 * @see [RFC 8415, section 21.3]
 *      (https://tools.ietf.org/html/rfc8415#section-21.3)
 */
typedef struct __attribute__((packed)) {
    network_uint16_t type;      /**< @ref DHCPV6_OPT_CID or DHCPV6_OPT_SID */
    network_uint16_t len;       /**< length of dhcpv6_opt_t::duid in byte */
    uint8_t duid[];             /**< the DUID of the client or server */
} dhcpv6_opt_duid_t;

/**
 * @brief   DHCPv6 identity association for non-temporary addresses (IA_NA) option
 *          format
 * @see [RFC 8415, section 21.4]
 *      (https://tools.ietf.org/html/rfc8415#section-21.4)
 */
typedef struct __attribute__((packed)) {
    network_uint16_t type;          /**< @ref DHCPV6_OPT_IA_NA */
    network_uint16_t len;           /**< 12 + length of dhcpv6_opt_ia_na_t::opts in byte */
    network_uint32_t ia_id;         /**< Unique ID for this IA_NA */
    network_uint32_t t1;            /**< DHCPv6 T1 time (in sec) */
    network_uint32_t t2;            /**< DHCPv6 T2 time (in sec) */
    uint8_t opts[];                 /**< IA_NA options */
} dhcpv6_opt_ia_na_t;

/**
 * @brief   DHCPv6 IA address option format
 * @see [RFC 8415, section 21.6]
 *      (https://tools.ietf.org/html/rfc8415#section-21.6)
 */
typedef struct __attribute__((packed)) {
    network_uint16_t type;          /**< @ref DHCPV6_OPT_IAADDR */
    network_uint16_t len;           /**< 25 + length of dhcpv6_opt_iapfx_t::opts in byte */
    ipv6_addr_t addr;                /**< the address */
    network_uint32_t pref;          /**< preferred lifetime (in sec) */
    network_uint32_t valid;         /**< valid lifetime (in sec) */
    uint8_t opts[];                 /**< IAprefix options */
} dhcpv6_opt_iaaddr_t;

/**
 * @brief   DHCPv6 option request option format
 * @see [RFC 8415, section 21.7]
 *      (https://tools.ietf.org/html/rfc8415#section-21.7)
 */
typedef struct __attribute__((packed)) {
    network_uint16_t type;          /**< @ref DHCPV6_OPT_ORO */
    network_uint16_t len;           /**< 2 * number of dhcpv6_opt_oro_t::opt_codes */
    network_uint16_t opt_codes[];   /**< option-code for an option requested by the client */
} dhcpv6_opt_oro_t;

/**
 * @brief   DHCPv6 preference option format
 * @see [RFC 8415, section 21.8]
 *      (https://tools.ietf.org/html/rfc8415#section-21.8)
 */
typedef struct __attribute__((packed)) {
    network_uint16_t type;          /**< @ref DHCPV6_OPT_PREF */
    network_uint16_t len;           /**< always 1 */
    uint8_t value;                  /**< preference value for the server */
} dhcpv6_opt_pref_t;
/** @} */

/**
 * @brief   DHCPv6 elapsed time option format
 * @see [RFC 8415, section 21.9]
 *      (https://tools.ietf.org/html/rfc8415#section-21.9)
 */
typedef struct __attribute__((packed)) {
    network_uint16_t type;          /**< @ref DHCPV6_OPT_ELAPSED_TIME */
    network_uint16_t len;           /**< always 2 */
    /**
     * @brief   amount of time since client began current DHCPv6 transaction
     *          (in cs) */
    network_uint16_t elapsed_time;
} dhcpv6_opt_elapsed_time_t;

/**
 * @brief   DHCPv6 relay message option
 * @see [RFC 8415, section 21.10]
 *      (https://tools.ietf.org/html/rfc8415#section-21.10)
 */
typedef struct __attribute__((packed)) {
    network_uint16_t type;          /**< @ref DHCPV6_OPT_RELAY_MSG */
    network_uint16_t len;           /**< length of dhcpv6_opt_iid_t::msg in byte */
    uint16_t msg[];                 /**< the relayed message */
} dhcpv6_opt_relay_msg_t;

/**
 * @brief   DHCPv6 status code option format
 * @see [RFC 8415, section 21.13]
 *      (https://tools.ietf.org/html/rfc8415#section-21.13)
 */
typedef struct __attribute__((packed)) {
    network_uint16_t type;          /**< @ref DHCPV6_OPT_STATUS */
    network_uint16_t len;           /**< 2 + length of dhcpv6_opt_status_t::msg in byte */
    network_uint16_t code;          /**< [status code](@ref net_dhcp6_status_codes) */
    char msg[];                     /**< UTF-8 encoded text string (not 0-terminated!) */
} dhcpv6_opt_status_t;

/**
 * @brief   DHCPv6 interface-id option
 * @see [RFC 8415, section 21.18]
 *      (https://tools.ietf.org/html/rfc8415#section-21.18)
 */
typedef struct __attribute__((packed)) {
    network_uint16_t type;          /**< @ref DHCPV6_OPT_IID */
    network_uint16_t len;           /**< length of dhcpv6_opt_iid_t::iid in byte */
    uint8_t iid[];                  /**< opaque interface identifier */
} dhcpv6_opt_iid_t;

/**
 * @brief   DHCPv6 DNS recursive name server option
 * @see [RFC 3646, section 3]
 *      (https://datatracker.ietf.org/doc/html/rfc3646#section-3)
 * @note    Only parsed with `dhcpv6_client_dns` module compiled in.
 */
typedef struct __attribute__((packed)) {
    network_uint16_t type;          /**< @ref DHCPV6_OPT_DNS_RNS */
    network_uint16_t len;           /**< length of dhcpv6_opt_status_t::dns_rns in byte */
    ipv6_addr_t dns_rns[];          /**< addresses of DNS recursive name servers
                                     *   in order of preference */
} dhcpv6_opt_dns_rns_t;

/**
 * @brief   DHCPv6 identity association for prefix delegation option (IA_PD)
 *          format
 * @see [RFC 8415, section 21.21]
 *      (https://tools.ietf.org/html/rfc8415#section-21.21)
 */
typedef struct __attribute__((packed)) {
    network_uint16_t type;          /**< @ref DHCPV6_OPT_IA_PD */
    network_uint16_t len;           /**< 12 + length of dhcpv6_opt_ia_pd_t::opts in byte */
    network_uint32_t ia_id;         /**< Unique ID for this IA_PD */
    network_uint32_t t1;            /**< DHCPv6 T1 time (in sec) */
    network_uint32_t t2;            /**< DHCPv6 T2 time (in sec) */
    uint8_t opts[];                 /**< IA_PD options */
} dhcpv6_opt_ia_pd_t;

/**
 * @brief   DHCPv6 IA prefix option format
 * @see [RFC 8415, section 21.22]
 *      (https://tools.ietf.org/html/rfc8415#section-21.22)
 */
typedef struct __attribute__((packed)) {
    network_uint16_t type;          /**< @ref DHCPV6_OPT_IAPFX */
    network_uint16_t len;           /**< 25 + length of dhcpv6_opt_iapfx_t::opts in byte */
    network_uint32_t pref;          /**< preferred lifetime (in sec) */
    network_uint32_t valid;         /**< valid lifetime (in sec) */
    uint8_t pfx_len;                /**< length of dhcpv6_opt_iapfx_t::pfx in bits */
    ipv6_addr_t pfx;                /**< the prefix */
    uint8_t opts[];                 /**< IAprefix options */
} dhcpv6_opt_iapfx_t;

/**
 * @brief   DHCPv6 Information Refresh Time option format
 * @see [RFC 8415, section 21.23]
 *      (https://tools.ietf.org/html/rfc8415#section-21.23)
 */
typedef struct __attribute__((packed)) {
    network_uint16_t type;          /**< @ref DHCPV6_OPT_IRT */
    network_uint16_t len;           /**< always 4 */
    network_uint32_t value;         /**< Time duration relative to the
                                         current time (in sec) */
} dhcpv6_opt_irt_t;

/**
 * @brief   DHCPv6 SOL_MAX_RT option format
 * @see [RFC 8415, section 21.24]
 *      (https://tools.ietf.org/html/rfc8415#section-21.24)
 */
typedef struct __attribute__((packed)) {
    network_uint16_t type;          /**< @ref DHCPV6_OPT_SMR */
    network_uint16_t len;           /**< always 4 */
    network_uint32_t value;         /**< overriding value for SOL_MAX_RT (in sec) */
} dhcpv6_opt_smr_t;

/**
 * @brief   DHCPv6 INF_MAX_RT option format
 * @see [RFC 8415, section 21.25]
 *      (https://tools.ietf.org/html/rfc8415#section-21.25)
 */
typedef struct __attribute__((packed)) {
    network_uint16_t type;          /**< @ref DHCPV6_OPT_IMR */
    network_uint16_t len;           /**< always 4 */
    network_uint32_t value;         /**< overriding value for INF_MAX_RT (in sec) */
} dhcpv6_opt_imr_t;

/**
 * @brief   MUD URL DHCPv6 option format
 * @see [RFC 8520, section 10]
 *      (https://tools.ietf.org/html/rfc8520#section-10)
 */
typedef struct __attribute__((packed)) {
    network_uint16_t type;  /**< @ref DHCPV6_OPT_MUD_URL */
    network_uint16_t len;   /**< length of the mud_string in octets. */
    char mud_string[];       /**< MUD URL using the "https" scheme */
} dhcpv6_opt_mud_url_t;

/**
 * @brief   Configures a DNS recursive name server provided by the server.
 *
 * @note    Only available with module `dhcpv6_client_dns`.
 *
 * @param[in] opt       A legal DNS recursive name option.
 * @param[in] netif     Network interface the message carrying @p opt came in.
 */
void dhcpv6_client_dns_rns_conf(const dhcpv6_opt_dns_rns_t *opt,
                                uint16_t netif);

#ifdef __cplusplus
}
#endif

/** @} */
