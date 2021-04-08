#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include "rtrlib/bgpsec/bgpsec_utils_private.h"
#include "rtrlib/bgpsec/bgpsec_private.h"
#include "rtrlib/bgpsec/bgpsec.h"
#include "rtrlib/rtr_mgr.h"
#include "rtrlib/rtrlib.h"
#include "rtrlib/spki/hashtable/ht-spkitable_private.h"

static uint8_t ski1[]  = {
		0x47, 0xF2, 0x3B, 0xF1, 0xAB,
		0x2F, 0x8A, 0x9D, 0x26, 0x86,
		0x4E, 0xBB, 0xD8, 0xDF, 0x27,
		0x11, 0xC7, 0x44, 0x06, 0xEC
};

static uint8_t sig1[]  = {
		0x30, 0x46, 0x02, 0x21, 0x00, 0xEF, 0xD4, 0x8B, 0x2A, 0xAC,
		0xB6, 0xA8, 0xFD, 0x11, 0x40, 0xDD, 0x9C, 0xD4, 0x5E, 0x81,
		0xD6, 0x9D, 0x2C, 0x87, 0x7B, 0x56, 0xAA, 0xF9, 0x91, 0xC3,
		0x4D, 0x0E, 0xA8, 0x4E, 0xAF, 0x37, 0x16, 0x02, 0x21, 0x00,
		0x90, 0xF2, 0xC1, 0x29, 0xAB, 0xB2, 0xF3, 0x9B, 0x6A, 0x07,
		0x96, 0x3B, 0xD5, 0x55, 0xA8, 0x7A, 0xB2, 0xB7, 0x33, 0x3B,
		0x7B, 0x91, 0xF1, 0x66, 0x8F, 0xD8, 0x61, 0x8C, 0x83, 0xFA,
		0xC3, 0xF1
};

static uint8_t spki1[] = {
		0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2A, 0x86, 0x48, 0xCE,
		0x3D, 0x02, 0x01, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D,
		0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0x28, 0xFC, 0x5F,
		0xE9, 0xAF, 0xCF, 0x5F, 0x4C, 0xAB, 0x3F, 0x5F, 0x85, 0xCB,
		0x21, 0x2F, 0xC1, 0xE9, 0xD0, 0xE0, 0xDB, 0xEA, 0xEE, 0x42,
		0x5B, 0xD2, 0xF0, 0xD3, 0x17, 0x5A, 0xA0, 0xE9, 0x89, 0xEA,
		0x9B, 0x60, 0x3E, 0x38, 0xF3, 0x5F, 0xB3, 0x29, 0xDF, 0x49,
		0x56, 0x41, 0xF2, 0xBA, 0x04, 0x0F, 0x1C, 0x3A, 0xC6, 0x13,
		0x83, 0x07, 0xF2, 0x57, 0xCB, 0xA6, 0xB8, 0xB5, 0x88, 0xF4,
		0x1F
};

static uint8_t ski2[]  = {
		0xAB, 0x4D, 0x91, 0x0F, 0x55,
		0xCA, 0xE7, 0x1A, 0x21, 0x5E,
		0xF3, 0xCA, 0xFE, 0x3A, 0xCC,
		0x45, 0xB5, 0xEE, 0xC1, 0x54
};

static uint8_t sig2[]  = {
		0x30, 0x46, 0x02, 0x21, 0x00, 0xEF, 0xD4, 0x8B, 0x2A, 0xAC,
		0xB6, 0xA8, 0xFD, 0x11, 0x40, 0xDD, 0x9C, 0xD4, 0x5E, 0x81,
		0xD6, 0x9D, 0x2C, 0x87, 0x7B, 0x56, 0xAA, 0xF9, 0x91, 0xC3,
		0x4D, 0x0E, 0xA8, 0x4E, 0xAF, 0x37, 0x16, 0x02, 0x21, 0x00,
		0x8E, 0x21, 0xF6, 0x0E, 0x44, 0xC6, 0x06, 0x6C, 0x8B, 0x8A,
		0x95, 0xA3, 0xC0, 0x9D, 0x3A, 0xD4, 0x37, 0x95, 0x85, 0xA2,
		0xD7, 0x28, 0xEE, 0xAD, 0x07, 0xA1, 0x7E, 0xD7, 0xAA, 0x05,
		0x5E, 0xCA
};

static uint8_t spki2[] = {
		0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2A, 0x86, 0x48, 0xCE,
		0x3D, 0x02, 0x01, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D,
		0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0x73, 0x91, 0xBA,
		0xBB, 0x92, 0xA0, 0xCB, 0x3B, 0xE1, 0x0E, 0x59, 0xB1, 0x9E,
		0xBF, 0xFB, 0x21, 0x4E, 0x04, 0xA9, 0x1E, 0x0C, 0xBA, 0x1B,
		0x13, 0x9A, 0x7D, 0x38, 0xD9, 0x0F, 0x77, 0xE5, 0x5A, 0xA0,
		0x5B, 0x8E, 0x69, 0x56, 0x78, 0xE0, 0xFA, 0x16, 0x90, 0x4B,
		0x55, 0xD9, 0xD4, 0xF5, 0xC0, 0xDF, 0xC5, 0x88, 0x95, 0xEE,
		0x50, 0xBC, 0x4F, 0x75, 0xD2, 0x05, 0xA2, 0x5B, 0xD3, 0x6F,
		0xF5
};

static uint8_t private_key[] = {
		0x30, 0x77, 0x02, 0x01, 0x01, 0x04, 0x20, 0xD8, 0xAA, 0x4D,
		0xFB, 0xE2, 0x47, 0x8F, 0x86, 0xE8, 0x8A, 0x74, 0x51, 0xBF,
		0x07, 0x55, 0x65, 0x70, 0x9C, 0x57, 0x5A, 0xC1, 0xC1, 0x36,
		0xD0, 0x81, 0xC5, 0x40, 0x25, 0x4C, 0xA4, 0x40, 0xB9, 0xA0,
		0x0A, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01,
		0x07, 0xA1, 0x44, 0x03, 0x42, 0x00, 0x04, 0x73, 0x91, 0xBA,
		0xBB, 0x92, 0xA0, 0xCB, 0x3B, 0xE1, 0x0E, 0x59, 0xB1, 0x9E,
		0xBF, 0xFB, 0x21, 0x4E, 0x04, 0xA9, 0x1E, 0x0C, 0xBA, 0x1B,
		0x13, 0x9A, 0x7D, 0x38, 0xD9, 0x0F, 0x77, 0xE5, 0x5A, 0xA0,
		0x5B, 0x8E, 0x69, 0x56, 0x78, 0xE0, 0xFA, 0x16, 0x90, 0x4B,
		0x55, 0xD9, 0xD4, 0xF5, 0xC0, 0xDF, 0xC5, 0x88, 0x95, 0xEE,
		0x50, 0xBC, 0x4F, 0x75, 0xD2, 0x05, 0xA2, 0x5B, 0xD3, 0x6F,
		0xF5
};

static struct spki_record *create_record(int ASN,
					 uint8_t *ski,
					 uint8_t *spki)
{
	struct spki_record *record = malloc(sizeof(struct spki_record));

	memset(record, 0, sizeof(*record));
	record->asn = ASN;
	memcpy(record->ski, ski, SKI_SIZE);
	memcpy(record->spki, spki, SPKI_SIZE);

	record->socket = NULL;
	return record;
}

static void validate_crypto_benchmark(int rep)
{
	struct rtr_bgpsec *bgpsec = NULL;
	struct rtr_bgpsec_nlri *pfx = NULL;

	struct spki_table table;
	struct spki_record *record1;
	struct spki_record *record2;

	enum rtr_bgpsec_rtvals result;

	struct rtr_signature_seg *ss = NULL;
	struct rtr_secure_path_seg *sps = NULL;

	uint8_t alg		= 1;
	uint8_t safi		= 1;
	uint16_t afi		= 1;
	uint32_t my_as		= 65537;

	pfx = rtr_mgr_bgpsec_nlri_new();
	pfx->prefix_len		= 24;
	pfx->prefix.ver		= LRTR_IPV4;
	lrtr_ip_str_to_addr("192.0.2.0", &pfx->prefix);

	bgpsec = rtr_mgr_bgpsec_new(alg, safi, afi, my_as, my_as, pfx);

	/* init the rtr_signature_seg and rtr_secure_path_seg structs. */

	uint8_t pcount		= 1;
	uint8_t flags		= 0;
	uint32_t asn		= 64496;

	sps = rtr_mgr_bgpsec_new_secure_path_seg(pcount, flags, asn);
	rtr_mgr_bgpsec_prepend_sec_path_seg(bgpsec, sps);

	asn			= 65536;
	sps = rtr_mgr_bgpsec_new_secure_path_seg(pcount, flags, asn);
	rtr_mgr_bgpsec_prepend_sec_path_seg(bgpsec, sps);

	uint16_t sig_len	= 72;

	ss = rtr_mgr_bgpsec_new_signature_seg(ski2, sig_len, sig2);
	result = rtr_mgr_bgpsec_prepend_sig_seg(bgpsec, ss);
	assert(result == RTR_BGPSEC_SUCCESS);

	ss = rtr_mgr_bgpsec_new_signature_seg(ski1, sig_len, sig1);
	result = rtr_mgr_bgpsec_prepend_sig_seg(bgpsec, ss);
	assert(result == RTR_BGPSEC_SUCCESS);

	spki_table_init(&table, NULL);
	record1 = create_record(65536, ski1, spki1);
	record2 = create_record(64496, ski2, spki2);

	spki_table_add_entry(&table, record1);
	spki_table_add_entry(&table, record2);

	result = 0;
	for (int i = 0; i < rep; i++) {
		result = rtr_bgpsec_validate_as_path(bgpsec, &table);
	}

	/* Free all allocated memory. */
	spki_table_free(&table);
	free(record1);
	free(record2);
	rtr_mgr_bgpsec_free(bgpsec);
}

static void sign_crypto_benchmark(int rep)
{
	/* AS(64496)--->AS(65536)--->AS(65537) */
	struct rtr_bgpsec *bgpsec = NULL;
	struct rtr_bgpsec_nlri *pfx = NULL;

	struct spki_table table;
	struct spki_record *record1;
	struct spki_record *record2;

	struct rtr_signature_seg *new_sig = NULL;
	struct rtr_secure_path_seg *new_sec = NULL;

	enum rtr_bgpsec_rtvals result;

	struct rtr_signature_seg *ss = NULL;
	struct rtr_secure_path_seg *sps = NULL;

	uint8_t alg		= 1;
	uint8_t safi		= 1;
	uint16_t afi		= 1;
	uint32_t my_as		= 65537;
	uint32_t target_as	= 65538;

	pfx = rtr_mgr_bgpsec_nlri_new();
	pfx->prefix_len		= 24;
	pfx->prefix.ver		= LRTR_IPV4;
	lrtr_ip_str_to_addr("192.0.2.0", &pfx->prefix);

	bgpsec = rtr_mgr_bgpsec_new(alg, safi, afi, my_as, target_as, pfx);

	/* init the rtr_signature_seg and rtr_secure_path_seg structs. */

	uint8_t pcount		= 1;
	uint8_t flags		= 0;
	uint32_t asn		= 64496;

	sps = rtr_mgr_bgpsec_new_secure_path_seg(pcount, flags, asn);
	rtr_mgr_bgpsec_prepend_sec_path_seg(bgpsec, sps);

	asn			= 65536;
	sps = rtr_mgr_bgpsec_new_secure_path_seg(pcount, flags, asn);
	rtr_mgr_bgpsec_prepend_sec_path_seg(bgpsec, sps);

	uint16_t sig_len	= 72;

	ss = rtr_mgr_bgpsec_new_signature_seg(ski2, sig_len, sig2);
	result = rtr_mgr_bgpsec_prepend_sig_seg(bgpsec, ss);
	assert(result == RTR_BGPSEC_SUCCESS);

	ss = rtr_mgr_bgpsec_new_signature_seg(ski1, sig_len, sig1);
	result = rtr_mgr_bgpsec_prepend_sig_seg(bgpsec, ss);
	assert(result == RTR_BGPSEC_SUCCESS);

	new_sec = rtr_mgr_bgpsec_new_secure_path_seg(pcount, flags, my_as);
	rtr_mgr_bgpsec_prepend_sec_path_seg(bgpsec, new_sec);

	/* init the SPKI table and store two router keys in it. */
	spki_table_init(&table, NULL);
	record2 = create_record(65536, ski1, spki1);
	record1 = create_record(64496, ski2, spki2);

	spki_table_add_entry(&table, record1);
	spki_table_add_entry(&table, record2);

	/* Pass all data to the validation function. The result is either
	 * RTR_BGPSEC_VALID or RTR_BGPSEC_NOT_VALID.
	 * Test with 1 AS hop.
	 */

	for (int i = 0; i < rep; i++) {
		result = rtr_bgpsec_generate_signature(bgpsec, private_key, &new_sig);
		rtr_mgr_bgpsec_free_signatures(new_sig);
		new_sig = NULL;
	}

	/* Free all allocated memory. */
	spki_table_free(&table);
	free(record1);
	free(record2);
	rtr_mgr_bgpsec_free(bgpsec);
}

int main() {
    validate_crypto_benchmark(5);
    sign_crypto_benchmark(5);

    return 0;
}