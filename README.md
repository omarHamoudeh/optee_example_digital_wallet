# OP-TEE Digital Wallet Example

This project demonstrates a basic implementation of a digital wallet using [OP-TEE (Open Portable Trusted Execution Environment)](https://www.op-tee.org/), which is built on **Arm TrustZone** technology. OP-TEE enables the creation of Trusted Applications (TAs) that run in a secure world and are isolated from the normal operating system.

This project was built by modifying the official `hello_world` example from the OP-TEE documentation and using it as a foundation to develop a custom digital wallet Trusted Application.

## Overview

In this lab-style project, I:

- Set up the OP-TEE development environment (QEMU-based or hardware-based)
- Created and defined a new Trusted Application (TA) by:
  - Generating a unique UUID
  - Creating a corresponding UUID header file
  - Defining a command interface between the host and the TA
- Implemented basic secure wallet operations (deposit, withdraw) inside the TA
- Built and tested the application using OP-TEE's secure framework

## Project Structure

```
digital_wallet/
├── ...
└── host
 ├── main.c                    Non-secure application file
└── ta
 ├── Makefile                  BINARY=<uuid>
 ├── Android.mk                Android way to invoke the Makefile
 ├── sub.mk                    srcs-y += digital_wallet_ta.c
 ├── include
 │   └── digital_wallet_ta.h      Header exported to non-secure: TA commands API
 ├── digital_wallet_ta.c          Implementation of TA entry points
 └── user_ta_header_defines.h  TA_UUID, TA_FLAGS, TA_DATA/STACK_SIZE, ...
```

## 🚀 How to Build & Run

1. Follow the OP-TEE setup instructions [here](https://optee.readthedocs.io/en/latest/building/) to configure your environment.
2. Place this project inside the `build` directory or integrate it into the OP-TEE examples tree.
3. Ensure the `UUID` and command IDs are defined in a header shared by both host and TA.
4. Build the project:
   ```bash
   make
   ```
5. Run in QEMU or on a supported board:
   ```bash
   make run
   ```
