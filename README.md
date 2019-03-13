# sgx-aes-gcm

## How to Build/Execute the Application 
1. Install Intel(R) SGX SDK for Linux* OS
2. Build the project with the prepared Makefile:

    a. Hardware Mode, Debug build:
`
    $ make SGX_MODE=HW SGX_DEBUG=1
`

    b. Hardware Mode, Pre-release build:
`
    $ make SGX_MODE=HW SGX_PRERELEASE=1
`

    c. Hardware Mode, Release build:
`
    $ make SGX_MODE=HW
`

    d. Simulation Mode, Debug build:
`
    $ make SGX_DEBUG=1
`

    e. Simulation Mode, Pre-release build:
`
    $ make SGX_PRERELEASE=1
`

    f. Simulation Mode, Release build:
`
    $ make
`

3. Execute the binary directly:
`
    $ ./cryptoTestingApp
`

## Worklog after Fork:
Mon 4 Mar, 19
- This version is only tested on Ubuntu 16.04/18.04 with SGX >=2.0
