This is a reference template to start writing a Trusted Application

Presentation
------------

This example has been presented at the Linaro Connect USA 2014.
The slides are available [here](http://www.slideshare.net/linaroorg/lcu14103-how-to-create-and-run-trusted-applications-on-optee).

Manual Build Instructions
-------------------------

1. Setup the OP-TEE software stack by following: https://github.com/OP-TEE/optee_os#5-repo-manifests

2. Clone this repository into `$HOME/devel/optee`

	```
	git clone https://github.com/linaro-swg/hello_world.git
	cd hello_world
	```

3. Define the toolchains and environment variables:

	```
	export TEEC_EXPORT=$PWD/../optee_client/out/export
	```

	If normal world user space is 64-bit:<BR>
	```
	export HOST_CROSS_COMPILE=$PWD/../toolchains/aarch64/bin/aarch64-linux-gnu-
	```

	If normal world user space is 32-bit:<BR>
	```
	export HOST_CROSS_COMPILE=$PWD/../toolchains/aarch32/bin/arm-linux-gnueabihf-
	```

	If secure world user space is 64-bit:<BR>
	```
	export TA_CROSS_COMPILE=$PWD/../toolchains/aarch64/bin/aarch64-linux-gnu-
	export TA_DEV_KIT_DIR=$PWD/../optee_os/out/arm/export-ta_arm64
	```

	If secure world user space is 32-bit:<BR>
	```
	export TA_CROSS_COMPILE=$PWD/../toolchains/aarch32/bin/arm-linux-gnueabihf-
	export TA_DEV_KIT_DIR=$PWD/../optee_os/out/arm/export-ta_arm32
	```

4. Build it!

        make

