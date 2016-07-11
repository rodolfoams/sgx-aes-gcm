#include "CryptoEnclave_t.h"

#include "sgx_trts.h" /* for sgx_ocalloc, sgx_is_outside_enclave */

#include <errno.h>
#include <string.h> /* for memcpy etc */
#include <stdlib.h> /* for malloc/free etc */

#define CHECK_REF_POINTER(ptr, siz) do {	\
	if (!(ptr) || ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_UNIQUE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)


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

static sgx_status_t SGX_CDECL sgx_decryptMessage(void* pms)
{
	ms_decryptMessage_t* ms = SGX_CAST(ms_decryptMessage_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_encMessageIn = ms->ms_encMessageIn;
	size_t _tmp_len = ms->ms_len;
	size_t _len_encMessageIn = _tmp_len;
	char* _in_encMessageIn = NULL;
	char* _tmp_decMessageOut = ms->ms_decMessageOut;
	size_t _tmp_lenOut = ms->ms_lenOut;
	size_t _len_decMessageOut = _tmp_lenOut;
	char* _in_decMessageOut = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_decryptMessage_t));
	CHECK_UNIQUE_POINTER(_tmp_encMessageIn, _len_encMessageIn);
	CHECK_UNIQUE_POINTER(_tmp_decMessageOut, _len_decMessageOut);

	if (_tmp_encMessageIn != NULL) {
		_in_encMessageIn = (char*)malloc(_len_encMessageIn);
		if (_in_encMessageIn == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_encMessageIn, _tmp_encMessageIn, _len_encMessageIn);
	}
	if (_tmp_decMessageOut != NULL) {
		if ((_in_decMessageOut = (char*)malloc(_len_decMessageOut)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_decMessageOut, 0, _len_decMessageOut);
	}
	decryptMessage(_in_encMessageIn, _tmp_len, _in_decMessageOut, _tmp_lenOut);
err:
	if (_in_encMessageIn) free(_in_encMessageIn);
	if (_in_decMessageOut) {
		memcpy(_tmp_decMessageOut, _in_decMessageOut, _len_decMessageOut);
		free(_in_decMessageOut);
	}

	return status;
}

static sgx_status_t SGX_CDECL sgx_encryptMessage(void* pms)
{
	ms_encryptMessage_t* ms = SGX_CAST(ms_encryptMessage_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_decMessageIn = ms->ms_decMessageIn;
	size_t _tmp_len = ms->ms_len;
	size_t _len_decMessageIn = _tmp_len;
	char* _in_decMessageIn = NULL;
	char* _tmp_encMessageOut = ms->ms_encMessageOut;
	size_t _tmp_lenOut = ms->ms_lenOut;
	size_t _len_encMessageOut = _tmp_lenOut;
	char* _in_encMessageOut = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_encryptMessage_t));
	CHECK_UNIQUE_POINTER(_tmp_decMessageIn, _len_decMessageIn);
	CHECK_UNIQUE_POINTER(_tmp_encMessageOut, _len_encMessageOut);

	if (_tmp_decMessageIn != NULL) {
		_in_decMessageIn = (char*)malloc(_len_decMessageIn);
		if (_in_decMessageIn == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_decMessageIn, _tmp_decMessageIn, _len_decMessageIn);
	}
	if (_tmp_encMessageOut != NULL) {
		if ((_in_encMessageOut = (char*)malloc(_len_encMessageOut)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_encMessageOut, 0, _len_encMessageOut);
	}
	encryptMessage(_in_decMessageIn, _tmp_len, _in_encMessageOut, _tmp_lenOut);
err:
	if (_in_decMessageIn) free(_in_decMessageIn);
	if (_in_encMessageOut) {
		memcpy(_tmp_encMessageOut, _in_encMessageOut, _len_encMessageOut);
		free(_in_encMessageOut);
	}

	return status;
}

SGX_EXTERNC const struct {
	size_t nr_ecall;
	struct {void* ecall_addr; uint8_t is_priv;} ecall_table[2];
} g_ecall_table = {
	2,
	{
		{(void*)(uintptr_t)sgx_decryptMessage, 0},
		{(void*)(uintptr_t)sgx_encryptMessage, 0},
	}
};

SGX_EXTERNC const struct {
	size_t nr_ocall;
	uint8_t entry_table[1][2];
} g_dyn_entry_table = {
	1,
	{
		{0, 0, },
	}
};


sgx_status_t SGX_CDECL emit_debug(const char* str)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_str = str ? strlen(str) + 1 : 0;

	ms_emit_debug_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_emit_debug_t);
	void *__tmp = NULL;

	ocalloc_size += (str != NULL && sgx_is_within_enclave(str, _len_str)) ? _len_str : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_emit_debug_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_emit_debug_t));

	if (str != NULL && sgx_is_within_enclave(str, _len_str)) {
		ms->ms_str = (char*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_str);
		memcpy((void*)ms->ms_str, str, _len_str);
	} else if (str == NULL) {
		ms->ms_str = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	status = sgx_ocall(0, ms);


	sgx_ocfree();
	return status;
}

