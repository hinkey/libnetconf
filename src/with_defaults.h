/**
 * \file with_defaults.h
 * \author Radek Krejci <rkrejci@cesnet.cz>
 * \brief Implementation of NETCONF's with-defaults capability defined in RFC 6243
 *
 * Copyright (C) 2012 CESNET, z.s.p.o.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of the Company nor the names of its contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * ALTERNATIVELY, provided that this notice is retained in full, this
 * product may be distributed under the terms of the GNU General Public
 * License (GPL) version 2 or later, in which case the provisions
 * of the GPL apply INSTEAD OF those given above.
 *
 * This software is provided ``as is, and any express or implied
 * warranties, including, but not limited to, the implied warranties of
 * merchantability and fitness for a particular purpose are disclaimed.
 * In no event shall the company or contributors be liable for any
 * direct, indirect, incidental, special, exemplary, or consequential
 * damages (including, but not limited to, procurement of substitute
 * goods or services; loss of use, data, or profits; or business
 * interruption) however caused and on any theory of liability, whether
 * in contract, strict liability, or tort (including negligence or
 * otherwise) arising in any way out of the use of this software, even
 * if advised of the possibility of such damage.
 *
 */


#ifndef WITH_DEFAULTS_H_
#define WITH_DEFAULTS_H_

#include "messages.h"

typedef enum {
	NCDFLT_MODE_DISABLED = 0,
	NCDFLT_MODE_ALL = 1,
	NCDFLT_MODE_TRIM = 2,
	NCDFLT_MODE_EXPLICIT = 4,
	NCDFLT_MODE_ALL_TAGGED = 8
} NCDFLT_MODE;

/**
 * @ingroup withdefaults
 * @brief Set basic mode of the with-defaults capability for the NETCONF server.
 *
 * Default basic mode used by libnetconf is 'explicit'. This function should be
 * used before establishing a NETCONF session and settle common set of
 * capabilities between client and server.
 *
 * On the client side, this function doesn't have effect of setting up the
 * specific mode. It only disables (NCDFLT_MODE_DISABLED) or enables (any other
 * valid value) support for the with-defaults capability.
 *
 * @param[in] mode One of the with-defaults capability basic modes,
 * NCDFLT_MODE_ALL_TAGGED is not a basic mode and such value is ignored.
 */
void ncdflt_set_basic_mode(NCDFLT_MODE mode);

/**
 * @ingroup withdefaults
 * @brief Disable support for with-defaults capability. This can be done on
 * both client and server.
 *
 * This is alternative for ncdflt_set_basic_mode(NCDFLT_MODE_DISABLED). To enable
 * with-defaults capability, use ncdflt_set_basic_mode() to set with-defaults'
 * basic mode.
 */
#define NCDFLT_DISABLE ncdflt_set_basic_mode(NCDFLT_MODE_DISABLED)
/**
 * @ingroup withdefaults
 * @brief Get current set basic mode of the with-defaults capability.
 * @return Current value of the with-defaults' basic mode.
 */
NCDFLT_MODE ncdflt_get_basic_mode();

/**
 * @ingroup withdefaults
 * @brief Set with-defaults modes that are supported.
 *
 * This function should be used before establishing a NETCONF session and settle
 * common set of capabilities between client and server. On the client side,
 * this function has no effect.
 *
 * @param[in] modes ORed set of supported NCDFLT_MODE modes. Basic mode
 * is always supported automatically.
 */
void ncdflt_set_supported(NCDFLT_MODE modes);

/**
 * @ingroup withdefaults
 * @brief Get ORed value containing currently supported with-defaults modes.
 * @return ORed value containing currently supported with-defaults modes.
 */
NCDFLT_MODE ncdflt_get_supported();

/**
 * @ingroup withdefaults
 * @brief Set \<with-defaults\> parameter for the given NETCONF RPC message.
 *
 * Sending RPC message withe set \<with-default\> parameter via the session
 * which doesn't support specified value or with-defaults capability at all
 * will fail.
 *
 * @param[in] rpc RPC message where \<with-defaults\> will be added.
 * @param[in] mode Value for the \<with-defaults\> parameter,
 * NCDFLT_MODE_DISABLED has no effect.
 * @return 0 on success, non-zero else.
 */
int ncdflt_rpc_withdefaults(nc_rpc* rpc, NCDFLT_MODE mode);

/**
 * @ingroup withdefaults
 * @brief Get value of the \<with-defaults\> element from the rpc message.
 * @param[in] rpc RPC message to be parsed.
 * @return with-defaults mode of the NETCONF rpc message.
 */
NCDFLT_MODE ncdflt_rpc_get_withdefaults(const nc_rpc* rpc);

#endif /* WITH_DEFAULTS_H_ */