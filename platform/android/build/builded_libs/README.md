# Custom OpenSSL Artifacts

Place OpenSSL headers in `include/` and architecture-specific shared libraries in `lib/<abi>/`.
Each shared library **must** expose the SONAME `libssl.so`/`libcrypto.so`.  
If you compile OpenSSL yourself, either configure the build to emit those SONAMEs (preferred) or
use a post-processing tool such as `patchelf --set-soname libssl.so libssl.so.1.1`.

Example layout:

```
builded_libs/
  include/
    openssl/...
  lib/
    arm64-v8a/libssl.so
    arm64-v8a/libssl.so.1.1
    arm64-v8a/libcrypto.so
    arm64-v8a/libcrypto.so.1.1
    armeabi-v7a/libssl.so
    armeabi-v7a/libssl.so.1.1
    armeabi-v7a/libcrypto.so
    armeabi-v7a/libcrypto.so.1.1
```

The Android build scripts prefer these binaries over the Maven-supplied ones and will package any files starting with `libssl.so` or `libcrypto.so`.
