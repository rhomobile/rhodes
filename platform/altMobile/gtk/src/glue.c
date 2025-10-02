#define _XOPEN_SOURCE 700
#include <ftw.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

// ---- POSIX implementations to replace Qt-specific stubs ----

static int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
    (void)sb; (void)ftwbuf;
    if (typeflag == FTW_F || typeflag == FTW_SL)
        return unlink(fpath);
    if (typeflag == FTW_DP)
        return rmdir(fpath);
    return 0;
}

void rho_file_impl_delete_files_in_folder(const char *szFolderPath) {
    if (!szFolderPath) return;
    DIR *dir = opendir(szFolderPath);
    if (!dir) return;
    struct dirent *ent;
    char path[4096];
    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
        snprintf(path, sizeof(path), "%s/%s", szFolderPath, ent->d_name);
        struct stat st;
        if (lstat(path, &st) == 0) {
            if (S_ISDIR(st.st_mode)) nftw(path, unlink_cb, 32, FTW_DEPTH | FTW_PHYS);
            else unlink(path);
        }
    }
    closedir(dir);
}

void rho_file_impl_delete_folder(const char* szFolderPath) {
    if (!szFolderPath) return;
    nftw(szFolderPath, unlink_cb, 32, FTW_DEPTH | FTW_PHYS);
}

static int copy_file(const char *src, const char *dst) {
    FILE *in = fopen(src, "rb");
    if (!in) return -1;
    FILE *out = fopen(dst, "wb");
    if (!out) { fclose(in); return -1; }
    char buf[1<<16]; size_t n;
    while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
        if (fwrite(buf, 1, n, out) != n) { fclose(in); fclose(out); return -1; }
    }
    fclose(in); fclose(out);
    return 0;
}

static int copy_dir_recursive(const char *src, const char *dst) {
    struct stat st;
    if (stat(dst, &st) != 0) {
        if (mkdir(dst, 0777) != 0 && errno != EEXIST) return -1;
    }
    DIR *dir = opendir(src);
    if (!dir) return -1;
    struct dirent *ent; char s[4096], d[4096];
    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
        snprintf(s, sizeof(s), "%s/%s", src, ent->d_name);
        snprintf(d, sizeof(d), "%s/%s", dst, ent->d_name);
        if (lstat(s, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                if (copy_dir_recursive(s, d) != 0) { closedir(dir); return -1; }
            } else {
                if (copy_file(s, d) != 0) { closedir(dir); return -1; }
            }
        }
    }
    closedir(dir);
    return 0;
}

void rho_file_impl_copy_folders_content_to_another_folder(const char* szSrcFolderPath, const char* szDstFolderPath) {
    if (!szSrcFolderPath || !szDstFolderPath) return;
    copy_dir_recursive(szSrcFolderPath, szDstFolderPath);
}

// ---- Geolocation stubs (no-op) ----
typedef struct rho_param rho_param; /* forward decl to satisfy signature */
double rho_geo_latitude() { return 0.0; }
double rho_geo_longitude() { return 0.0; }
double rho_geo_altitude() { return 0.0; }
float  rho_geo_accuracy() { return 0.0f; }
int    rho_geo_known_position() { return 0; }
void   rho_geoimpl_settimeout(int nTimeoutSec) { (void)nTimeoutSec; }
void   rho_geoimpl_turngpsoff() {}
int    rho_geo_is_available() { return 0; }
double rho_geo_speed() { return 0.0; }
int    rho_geo_satellites() { return 0; }
void   rho_geo_set_notification_ex(const char *url, rho_param* p, char* params) { (void)url; (void)p; (void)params; }
/* Provided by rholib; do not redefine here */
