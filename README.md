# OP-TEE Digital Wallet Example

This project demonstrates a basic implementation of a digital wallet using the [OP-TEE (Open Portable Trusted Execution Environment)](https://www.op-tee.org/) framework. It includes a Trusted Application (TA) and a host application that communicate securely to simulate digital wallet operations.

## 📦 Project Structure

\```
digital_wallet/
├── host/                   # Host application source code
│   └── main.c              # Main logic for interacting with the Trusted App
├── ta/                     # Trusted Application source code
│   ├── digital_wallet_ta.c # Trusted Application logic
│   └── ...                 # Makefiles and build scripts for TA
├── Android.mk              # Android build configuration
├── CMakeLists.txt          # CMake build config
└── Makefile                # Top-level Makefile
\```

## 🔒 Key Features

- Securely store and manage wallet balance within the Trusted Execution Environment (TEE)
- Support for deposit and withdrawal operations via Trusted Application
- Communication between host and TA using OP-TEE APIs
- Demonstrates the fundamentals of trusted computing with OP-TEE

## 🛠️ Requirements

- OP-TEE build environment set up (e.g., QEMU or ARM development board)
- [repo tool](https://gerrit.googlesource.com/git-repo/)
- ARM toolchain for cross-compilation
- Familiarity with building and running OP-TEE examples

## 🚀 How to Build & Run

1. Set up the OP-TEE development environment as per [official OP-TEE instructions](https://optee.readthedocs.io/en/latest/building/).
2. Clone this repository into the `build` directory of OP-TEE.
3. Include this example in your `Makefile` or `manifest.xml` for OP-TEE.
4. Build the project:
   \```bash
   make
   \```
5. Run on QEMU or your target board:
   \```bash
   make run
   \```

## 📄 License

This project is open source and available under the MIT License.
