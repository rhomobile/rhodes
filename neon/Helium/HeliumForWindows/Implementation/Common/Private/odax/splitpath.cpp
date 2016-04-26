#include "StdAfx.h"

/***
*_splitpath() - split a path name into its individual components
*
*Purpose:
*       to split a path name into its individual components
*
*Entry:
*       path  - pointer to path name to be parsed
*       drive - pointer to buffer for drive component, if any
*       dir   - pointer to buffer for subdirectory component, if any
*       fname - pointer to buffer for file base name component, if any
*       ext   - pointer to buffer for file name extension component, if any
*
*Exit:
*       drive - pointer to drive string.  Includes ':' if a drive was given.
*       dir   - pointer to subdirectory string.  Includes leading and trailing
*           '/' or '\', if any.
*       fname - pointer to file base name
*       ext   - pointer to file extension, if any.  Includes leading '.'.
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _splitpath (
        register const char *path,
        char *drive,
        char *dir,
        char *fname,
        char *ext
        )
{
        register char *p;
        char *last_slash = NULL, *dot = NULL;
        unsigned len;

        /* we assume that the path argument has the following form, where any
         * or all of the components may be missing.
         *
         *  <drive><dir><fname><ext>
         *
         * and each of the components has the following expected form(s)
         *
         *  drive:
         *  0 to _MAX_DRIVE-1 characters, the last of which, if any, is a
         *  ':'
         *  dir:
         *  0 to _MAX_DIR-1 characters in the form of an absolute path
         *  (leading '/' or '\') or relative path, the last of which, if
         *  any, must be a '/' or '\'.  E.g -
         *  absolute path:
         *      \top\next\last\     ; or
         *      /top/next/last/
         *  relative path:
         *      top\next\last\  ; or
         *      top/next/last/
         *  Mixed use of '/' and '\' within a path is also tolerated
         *  fname:
         *  0 to _MAX_FNAME-1 characters not including the '.' character
         *  ext:
         *  0 to _MAX_EXT-1 characters where, if any, the first must be a
         *  '.'
         *
         */

        /* extract drive letter and :, if any */

        if ((strlen(path) >= (_MAX_DRIVE - 2)) && (*(path + _MAX_DRIVE - 2) == _T(':'))) {
            if (drive) {
                strncpy(drive, path, _MAX_DRIVE - 1);
                *(drive + _MAX_DRIVE-1) = _T('\0');
            }
            path += _MAX_DRIVE - 1;
        }
        else if (drive) {
            *drive = _T('\0');
        }

        /* extract path string, if any.  Path now points to the first character
         * of the path, if any, or the filename or extension, if no path was
         * specified.  Scan ahead for the last occurence, if any, of a '/' or
         * '\' path separator character.  If none is found, there is no path.
         * We will also note the last '.' character found, if any, to aid in
         * handling the extension.
         */

        for (last_slash = NULL, p = (char *)path; *p; p++) {
#ifdef _MBCS
            if (_ISLEADBYTE (*p))
                p++;
            else {
#endif  /* _MBCS */
            if (*p == _T('/') || *p == _T('\\'))
                /* point to one beyond for later copy */
                last_slash = p + 1;
            else if (*p == _T('.'))
                dot = p;
#ifdef _MBCS
            }
#endif  /* _MBCS */
        }

        if (last_slash) {

            /* found a path - copy up through last_slash or max. characters
             * allowed, whichever is smaller
             */

            if (dir) {
                len = __min(((char *)last_slash - (char *)path) / sizeof(char),
                    (_MAX_DIR - 1));
                strncpy(dir, path, len);
                *(dir + len) = _T('\0');
            }
            path = last_slash;
        }
        else if (dir) {

            /* no path found */

            *dir = _T('\0');
        }

        /* extract file name and extension, if any.  Path now points to the
         * first character of the file name, if any, or the extension if no
         * file name was given.  Dot points to the '.' beginning the extension,
         * if any.
         */

        if (dot && (dot >= path)) {
            /* found the marker for an extension - copy the file name up to
             * the '.'.
             */
            if (fname) {
                len = __min(((char *)dot - (char *)path) / sizeof(char),
                    (_MAX_FNAME - 1));
                strncpy(fname, path, len);
                *(fname + len) = _T('\0');
            }
            /* now we can get the extension - remember that p still points
             * to the terminating nul character of path.
             */
            if (ext) {
                len = __min(((char *)p - (char *)dot) / sizeof(char),
                    (_MAX_EXT - 1));
                strncpy(ext, dot, len);
                *(ext + len) = _T('\0');
            }
        }
        else {
            /* found no extension, give empty extension and copy rest of
             * string into fname.
             */
            if (fname) {
                len = __min(((char *)p - (char *)path) / sizeof(char),
                    (_MAX_FNAME - 1));
                strncpy(fname, path, len);
                *(fname + len) = _T('\0');
            }
            if (ext) {
                *ext = _T('\0');
            }
        }
}