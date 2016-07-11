#ifndef CRYPTOENCLAVE_U_H__
#define CRYPTOENCLAVE_U_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include <string.h>
#include "sgx_edger8r.h" /* for sgx_satus_t etc. */


#include <stdlib.h> /* for size_t */

#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

void SGX_UBRIDGE(SGX_CDECL, emit_debug, (const char* str));

sgx_status_t decryptMessage(sgx_enclave_id_t eid, char* encMessageIn, size_t len, char* decMessageOut, size_t lenOut);
sgx_status_t encryptMessage(sgx_enclave_id_t eid, char* decMessageIn, size_t len, char* encMessageOut, size_t lenOut);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
