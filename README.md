# OP-TEE Digital Wallet Example

This project demonstrates a basic implementation of a digital wallet using [OP-TEE (Open Portable Trusted Execution Environment)](https://www.op-tee.org/), which is built on **Arm TrustZone** technology. OP-TEE allows for the secure execution of trusted applications (TAs) in an isolated environment, separate from the normal operating system.

In this example, a simple digital wallet is implemented with support for secure deposit and withdrawal operations using the OP-TEE framework.

## Project Structure

```
digital_wallet/
├── host/                   # Host application source code
│   └── main.c              # Main logic for interacting with the Trusted App
├── ta/                     # Trusted Application source code
│   ├── digital_wallet_ta.c # Trusted Application logic
│   └── ...                 # Makefiles and build scripts for TA
├── Android.mk              # Android build configuration
├── CMakeLists.txt          # CMake build config
└── Makefile                # Top-level Makefile
```

## Key Features

- Securely store and manage wallet balance within the Trusted Execution Environment (TEE)
- Support for deposit and withdrawal operations via Trusted Application
- Communication between host and TA using OP-TEE APIs
- Demonstrates secure computing with **Arm TrustZone**
  

## How to Build & Run

1. Set up the OP-TEE development environment as per [official OP-TEE instructions](https://optee.readthedocs.io/en/latest/building/).
2. Clone this repository into the `build` directory of OP-TEE.
3. Include this example in your `Makefile` or `manifest.xml` for OP-TEE.
4. Build the project:
   ```bash
   make
   ```
5. Run on QEMU or your target board:
   ```bash
   make run
   ```

## 📄 License

This project is open source and available under the MIT License.
