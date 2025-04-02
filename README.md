# OP-TEE Digital Wallet Example

This project demonstrates a basic implementation of a digital wallet using [OP-TEE (Open Portable Trusted Execution Environment)](https://www.op-tee.org/), which is built on **Arm TrustZone** technology. OP-TEE enables the creation of Trusted Applications (TAs) that run in a secure world, isolated from the normal operating system.

This project was built by modifying the official `hello_world` example from the OP-TEE documentation and using it as a foundation to develop a custom digital wallet Trusted Application.

## 🔐 Overview

In this lab-style project, we:

- Set up the OP-TEE development environment (QEMU-based or hardware-based)
- Created and defined a new Trusted Application (TA) by:
  - Generating a unique UUID
  - Creating a corresponding UUID header file
  - Defining a command interface between the host and the TA
- Implemented basic secure wallet operations (deposit, withdraw) inside the TA
- Built and tested the application using OP-TEE's secure framework

## 📦 Project Structure

```
digital_wallet/
├── host/                        # Host application
│   └── main.c                   # Communicates with the Trusted Application
├── ta/                          # Trusted Application
│   ├── digital_wallet_ta.c      # TA source code (secure wallet logic)
│   ├── user_ta_header_defines.h # TA UUID and command definitions
│   └── ...
├── Android.mk                   # Android build configuration
├── CMakeLists.txt               # CMake configuration
└── Makefile                     # Build instructions
```

## 🛠️ Requirements

- OP-TEE development environment ([instructions](https://optee.readthedocs.io/en/latest/building/))
- ARM toolchain
- Familiarity with:
  - Trusted Application UUID and header setup
  - TA-to-host communication via `TEEC_InvokeCommand`
  - TA build and deployment steps

## 🚀 How to Build & Run

1. Follow the OP-TEE setup instructions [here](https://optee.readthedocs.io/en/latest/building/) to configure your environment.
2. Place this project inside the `build` directory or integrate it into the OP-TEE examples tree.
3. Make sure the `UUID` and command IDs are defined in a header shared by both host and TA.
4. Build the project:
   ```bash
   make
   ```
5. Run in QEMU or on a supported board:
   ```bash
   make run
   ```

## 💡 Notes

- You must define a unique UUID for each new Trusted Application, as described in the [OP-TEE Trusted Application Guide](https://optee.readthedocs.io/en/latest/building/trusted_applications.html#build-trusted-applications).
- The TA is signed and packaged as part of the secure world filesystem when you run the `make` command.
- This example showcases the secure design pattern for handling sensitive data inside a TEE using a digital wallet scenario.

## 📄 License

This project is open source and available under the MIT License.
