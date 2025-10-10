# Custom TLS Artifacts

Place OpenSSL headers in `include/` and architecture-specific shared libraries in `lib/<abi>/`.
Each OpenSSL shared library **must** expose the SONAME `libssl.so`/`libcrypto.so`.  
If you compile OpenSSL yourself, either configure the build to emit those SONAMEs (preferred) or
use a post-processing tool such as `patchelf --set-soname libssl.so libssl.so.1.1`.

Custom builds of Conscrypt should also be copied here as `libconscrypt_jni.so`. The build system
will automatically pick them up alongside OpenSSL when packaging APKs/AABs.

Place the Conscrypt Java classes (the output `conscrypt-android.jar` from your build) under
`java/`. These jars are added to the compile classpath and shipped with the application.

Example layout:

```
builded_libs/
  include/
    openssl/...
  java/
    conscrypt-android.jar
  lib/
    arm64-v8a/libssl.so
    arm64-v8a/libssl.so.1.1
    arm64-v8a/libcrypto.so
    arm64-v8a/libcrypto.so.1.1
    arm64-v8a/libconscrypt_jni.so
    armeabi-v7a/libssl.so
    armeabi-v7a/libssl.so.1.1
    armeabi-v7a/libcrypto.so
    armeabi-v7a/libcrypto.so.1.1
    armeabi-v7a/libconscrypt_jni.so
```

The Android build scripts prefer these binaries over the Maven-supplied ones and will package any files starting with `libssl.so`, `libcrypto.so`, or `libconscrypt_jni.so`, as well as any jars placed under `java/`.
