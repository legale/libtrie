/* file: inline.h
 *
 * INLINE macro
 *
 */

#ifndef INLINE
# if __GNUC__ && __GNUC_STDC_INLINE__
#  define INLINE extern inline
# else
#  define INLINE inline
# endif
#endif /*inline.h*/