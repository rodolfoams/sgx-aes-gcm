#include "CryptoEnclave_u.h"
#include <errno.h>

typedef struct ms_decryptMessage_t {
	char* ms_encMessageIn;
	size_t ms_len;
	char* ms_decMessageOut;
	size_t ms_lenOut;
} ms_decryptMessage_t;

typedef struct ms_encryptMessage_t {
	char* ms_decMessageIn;
	size_t ms_len;
	char* ms_encMessageOut;
	size_t ms_lenOut;
} ms_encryptMessage_t;

typedef struct ms_emit_debug_t {
	char* ms_str;
} ms_emit_debug_t;

static sgx_status_t SGX_CDECL CryptoEnclave_emit_debug(void* pms)
{
	ms_emit_debug_t* ms = SGX_CAST(ms_emit_debug_t*, pms);
	emit_debug((const char*)ms->ms_str);

	return SGX_SUCCESS;
}

static const struct {
	size_t nr_ocall;
	void * table[1];
} ocall_table_CryptoEnclave = {
	1,
	{
		(void*)CryptoEnclave_emit_debug,
	}
};
sgx_status_t decryptMessage(sgx_enclave_id_t eid, char* encMessageIn, size_t len, char* decMessageOut, size_t lenOut)
{
	sgx_status_t status;
	ms_decryptMessage_t ms;
	ms.ms_encMessageIn = encMessageIn;
	ms.ms_len = len;
	ms.ms_decMessageOut = decMessageOut;
	ms.ms_lenOut = lenOut;
	status = sgx_ecall(eid, 0, &ocall_table_CryptoEnclave, &ms);
	return status;
}

sgx_status_t encryptMessage(sgx_enclave_id_t eid, char* decMessageIn, size_t len, char* encMessageOut, size_t lenOut)
{
	sgx_status_t status;
	ms_encryptMessage_t ms;
	ms.ms_decMessageIn = decMessageIn;
	ms.ms_len = len;
	ms.ms_encMessageOut = encMessageOut;
	ms.ms_lenOut = lenOut;
	status = sgx_ecall(eid, 1, &ocall_table_CryptoEnclave, &ms);
	return status;
}

