#ifndef CRYPTOENCLAVE_T_H__
#define CRYPTOENCLAVE_T_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include "sgx_edger8r.h" /* for sgx_ocall etc. */


#include <stdlib.h> /* for size_t */

#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif


void decryptMessage(char* encMessageIn, size_t len, char* decMessageOut, size_t lenOut);
void encryptMessage(char* decMessageIn, size_t len, char* encMessageOut, size_t lenOut);

sgx_status_t SGX_CDECL emit_debug(const char* str);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
