# Custom TLS Artifacts

Place OpenSSL headers in `include/` and architecture-specific shared libraries in `lib/<abi>/`.
Each OpenSSL shared library **must** physically exist as `libssl.so`/`libcrypto.so` (no symlinks or versioned
file names) and expose matching SONAMEs. Use the helper script in `myBuildsLibs/openssl/generate_openSSL.sh`
to produce compliant binaries.

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
    arm64-v8a/libcrypto.so
    arm64-v8a/libconscrypt_jni.so
    armeabi-v7a/libssl.so
    armeabi-v7a/libcrypto.so
    armeabi-v7a/libconscrypt_jni.so
```

The Android build scripts prefer these binaries over the Maven-supplied ones and will package any files named `libssl.so`, `libcrypto.so`, or `libconscrypt_jni.so`, as well as any jars placed under `java/`.
