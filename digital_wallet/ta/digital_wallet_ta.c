/*
 * Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include <digital_wallet_ta.h>

#include <string.h>

static int balance = 1000;  // Initial balance
#define MAX_TRANSACTIONS 10
static char transactions[MAX_TRANSACTIONS][50];
static int transaction_count = 0;


/*
 * Called when the instance of the TA is created. This is the first call in
 * the TA.
 */
TEE_Result TA_CreateEntryPoint(void)
{
	DMSG("has been called");

	return TEE_SUCCESS;
}

/*
 * Called when the instance of the TA is destroyed if the TA has not
 * crashed or panicked. This is the last call in the TA.
 */
void TA_DestroyEntryPoint(void)
{
	DMSG("has been called");
}

/*
 * Called when a new session is opened to the TA. *sess_ctx can be updated
 * with a value to be able to identify this session in subsequent calls to the
 * TA. In this function you will normally do the global initialization for the
 * TA.
 */
TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
		TEE_Param __maybe_unused params[4],
		void __maybe_unused **sess_ctx)
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	DMSG("has been called");

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	/* Unused parameters */
	(void)&params;
	(void)&sess_ctx;

	/*
	 * The DMSG() macro is non-standard, TEE Internal API doesn't
	 * specify any means to logging from a TA.
	 */
	IMSG("Hello World!\n");

	/* If return value != TEE_SUCCESS the session will not be created. */
	return TEE_SUCCESS;
}

/*
 * Called when a session is closed, sess_ctx hold the value that was
 * assigned by TA_OpenSessionEntryPoint().
 */
void TA_CloseSessionEntryPoint(void __maybe_unused *sess_ctx)
{
	(void)&sess_ctx; /* Unused parameter */
	IMSG("Goodbye!\n");
}

/* Deposit Function */
static TEE_Result deposit(uint32_t param_types, TEE_Param params[4]) {
    uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
                                               TEE_PARAM_TYPE_NONE,
                                               TEE_PARAM_TYPE_NONE,
                                               TEE_PARAM_TYPE_NONE);

    if (param_types != exp_param_types)
        return TEE_ERROR_BAD_PARAMETERS;

    int amount = params[0].value.a;
    balance += amount;
    snprintf(transactions[transaction_count % MAX_TRANSACTIONS], 50, "Deposit: %d", amount);
    transaction_count++;

    IMSG("Deposited %d. New balance: %d", amount, balance);
    return TEE_SUCCESS;
}

/* Pay Function */
static TEE_Result pay(uint32_t param_types, TEE_Param params[4]) {
    uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
                                               TEE_PARAM_TYPE_NONE,
                                               TEE_PARAM_TYPE_NONE,
                                               TEE_PARAM_TYPE_NONE);

    if (param_types != exp_param_types)
        return TEE_ERROR_BAD_PARAMETERS;

    int amount = params[0].value.a;
    if (balance < amount) {
        IMSG("Payment failed: Insufficient funds. Current balance: %d", balance);
        return TEE_ERROR_BAD_PARAMETERS;
    }

    balance -= amount;
    snprintf(transactions[transaction_count % MAX_TRANSACTIONS], 50, "Pay: %d", amount);
    transaction_count++;

    IMSG("Paid %d. New balance: %d", amount, balance);
    return TEE_SUCCESS;
}

/* View Balance Function */
static TEE_Result view_balance(uint32_t param_types, TEE_Param params[4]) {
    uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_OUTPUT,
                                               TEE_PARAM_TYPE_NONE,
                                               TEE_PARAM_TYPE_NONE,
                                               TEE_PARAM_TYPE_NONE);

    if (param_types != exp_param_types)
        return TEE_ERROR_BAD_PARAMETERS;

    params[0].value.a = balance;
    IMSG("Current balance: %d", balance);
    return TEE_SUCCESS;
}

/* Check Transactions Function */
static TEE_Result check_transactions(uint32_t param_types, TEE_Param params[4]) {
    uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_OUTPUT,
                                               TEE_PARAM_TYPE_NONE,
                                               TEE_PARAM_TYPE_NONE,
                                               TEE_PARAM_TYPE_NONE);

    if (param_types != exp_param_types) {
        IMSG("Invalid parameter types!");
        return TEE_ERROR_BAD_PARAMETERS;
    }

    char *buffer = params[0].memref.buffer;
    size_t buffer_size = params[0].memref.size;
    size_t written = 0;

    int count_to_display = (transaction_count > 3) ? 3 : transaction_count;
    int start = (transaction_count >= 3) ? transaction_count - 3 : 0;

    IMSG("Displaying %d transactions starting from index %d", count_to_display, start);

    for (int i = 0; i < count_to_display && written < buffer_size; i++) {
        int index = (start + i) % MAX_TRANSACTIONS;
        written += snprintf(buffer + written, buffer_size - written,
                            "Transaction %d: %s\n", i + 1, transactions[index]);
    }

    params[0].memref.size = written;
    return TEE_SUCCESS;
}




/*
 * Called when a TA is invoked. sess_ctx hold that value that was
 * assigned by TA_OpenSessionEntryPoint(). The rest of the paramters
 * comes from normal world.
 */
TEE_Result TA_InvokeCommandEntryPoint(void *sess_ctx, uint32_t cmd_id,
                                      uint32_t param_types, TEE_Param params[4]) {
    switch (cmd_id) {
    case TA_DIGITAL_WALLET_CMD_DEPOSIT:
        return deposit(param_types, params);

    case TA_DIGITAL_WALLET_CMD_PAY:
        return pay(param_types, params);

    case TA_DIGITAL_WALLET_CMD_VIEW_BALANCE:
        return view_balance(param_types, params);

    case TA_DIGITAL_WALLET_CMD_VIEW_TRANSACTIONS:
        return check_transactions(param_types, params);

    default:
        return TEE_ERROR_BAD_PARAMETERS;
    }
}
