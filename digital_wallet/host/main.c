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

#include <err.h>
#include <stdio.h>
#include <string.h>

/* OP-TEE TEE client API (built by optee_client) */
#include <tee_client_api.h>

/* For the UUID (found in the TA's h-file(s)) */
#include <digital_wallet_ta.h>

void display_menu() {
    printf("\n========== Digital Wallet Menu ==========\n");
    printf("1. Deposit Money\n");
    printf("2. Pay Money\n");
    printf("3. View Balance\n");
    printf("4. View Last 3 Transactions\n");
    printf("5. Exit\n");
    printf("=========================================\n");
    printf("Enter your choice: ");
}

int main(void) {
    TEEC_Context ctx;
    TEEC_Session sess;
    TEEC_Operation op;
    TEEC_Result res;
    TEEC_UUID uuid = TA_DIGITAL_WALLET_UUID;
    int choice, amount;

    // Initialize a context connecting to the TEE
    res = TEEC_InitializeContext(NULL, &ctx);
    if (res != TEEC_SUCCESS)
        errx(1, "TEEC_InitializeContext failed with code 0x%x", res);

    // Open a session with the Digital Wallet TA
    res = TEEC_OpenSession(&ctx, &sess, &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &res);
    if (res != TEEC_SUCCESS)
        errx(1, "TEEC_OpenSession failed with code 0x%x", res);

    while (1) {
        display_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n');  // Clear input buffer
            continue;
        }

        memset(&op, 0, sizeof(op));

        switch (choice) {
            case 1: // Deposit Money
                printf("Enter deposit amount: ");
                if (scanf("%d", &amount) != 1 || amount <= 0) {
                    printf("Invalid amount. Please enter a positive number.\n");
                    while (getchar() != '\n');  // Clear input buffer
                    continue;
                }
                op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
                op.params[0].value.a = amount;
                res = TEEC_InvokeCommand(&sess, TA_DIGITAL_WALLET_CMD_DEPOSIT, &op, NULL);
                if (res == TEEC_SUCCESS)
                    printf("Successfully deposited %d!\n", amount);
                else
                    printf("Deposit failed with code 0x%x\n", res);
                break;

            case 2: // Pay Money
                printf("Enter payment amount: ");
                if (scanf("%d", &amount) != 1 || amount <= 0) {
                    printf("Invalid amount. Please enter a positive number.\n");
                    while (getchar() != '\n');  // Clear input buffer
                    continue;
                }
                op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
                op.params[0].value.a = amount;
                res = TEEC_InvokeCommand(&sess, TA_DIGITAL_WALLET_CMD_PAY, &op, NULL);
                if (res == TEEC_SUCCESS)
                    printf("Payment of %d successful!\n", amount);
                else
                    printf("Payment failed (Insufficient funds or error). Code: 0x%x\n", res);
                break;

            case 3: // View Balance
                op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_OUTPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
                res = TEEC_InvokeCommand(&sess, TA_DIGITAL_WALLET_CMD_VIEW_BALANCE, &op, NULL);
                if (res == TEEC_SUCCESS)
                    printf("Your current balance: %d\n", op.params[0].value.a);
                else
                    printf("Failed to retrieve balance. Code: 0x%x\n", res);
                break;

            case 4: // View Transactions
                printf("\n[+] Checking recent transactions...\n");
                char trans_buffer[512] = {0};  // Allocate buffer for transaction log
                memset(&op, 0, sizeof(op));
                op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
                op.params[0].tmpref.buffer = trans_buffer;
                op.params[0].tmpref.size = sizeof(trans_buffer);

                res = TEEC_InvokeCommand(&sess, TA_DIGITAL_WALLET_CMD_VIEW_TRANSACTIONS, &op, NULL);
                if (res == TEEC_SUCCESS)
                    printf("Recent Transactions:\n%s", trans_buffer);
                else
                    printf("Failed to retrieve transactions. Code: 0x%x\n", res);
                break;

            case 5: // Exit
                printf("Exiting Digital Wallet. Goodbye!\n");
                TEEC_CloseSession(&sess);
                TEEC_FinalizeContext(&ctx);
                return 0;

            default:
                printf("Invalid choice! Please enter a number between 1-5.\n");
        }
    }

    return 0;
}
