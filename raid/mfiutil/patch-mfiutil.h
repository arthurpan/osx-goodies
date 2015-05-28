--- mfiutil.h.orig	2013-03-28 12:06:19.000000000 +0400
+++ mfiutil.h	2013-03-28 12:31:25.000000000 +0400
@@ -33,35 +33,47 @@
 #define	__MFIUTIL_H__
 
 #include <sys/cdefs.h>
+
+typedef struct mach_header kernel_mach_header_t;
 #include <sys/linker_set.h>
 
 #include <dev/mfi/mfireg.h>
 
-/* 4.x compat */
+/* OS X compat */
 #ifndef SET_DECLARE
 
-/* <sys/cdefs.h> */
-#define	__used
-#define	__section(x)	__attribute__((__section__(x)))
-
 /* <sys/linker_set.h> */
-#undef __MAKE_SET
 #undef DATA_SET
 
-#define __MAKE_SET(set, sym)						\
-	static void const * const __set_##set##_sym_##sym 		\
-	__section("set_" #set) __used = &sym
-
-#define DATA_SET(set, sym)	__MAKE_SET(set, sym)
+#define DATA_SET(set, sym)	__LINKER_MAKE_SET(set, sym)
 
 #define SET_DECLARE(set, ptype)						\
-	extern ptype *__CONCAT(__start_set_,set);			\
-	extern ptype *__CONCAT(__stop_set_,set)
+	char set[] = __LS_VA_STRINGIFY(set)
+
+static __inline void **
+__linker_set_object_begin_slide(const char *_set)
+{
+        void *_set_begin;
+        unsigned long _size;
+
+        _set_begin = getsectiondata(&_mh_execute_header, SEG_DATA, _set, &_size);
+        return( (void **) _set_begin );
+}
+static __inline void **
+__linker_set_object_limit_slide(const char *_set)
+{
+        void *_set_begin;
+        unsigned long _size;
+
+        _set_begin = getsectiondata(&_mh_execute_header, SEG_DATA, _set, &_size);
+
+        return ((void **) ((uintptr_t) _set_begin + _size));
+}
 
 #define SET_BEGIN(set)							\
-	(&__CONCAT(__start_set_,set))
+	__linker_set_object_begin_slide(set)
 #define SET_LIMIT(set)							\
-	(&__CONCAT(__stop_set_,set))
+	__linker_set_object_limit_slide(set)
 
 #define	SET_FOREACH(pvar, set)						\
 	for (pvar = SET_BEGIN(set); pvar < SET_LIMIT(set); pvar++)
@@ -97,7 +109,7 @@ struct mfiutil_command {
 	int (*handler)(int ac, char **av);
 };
 
-#define	MFI_DATASET(name)	mfiutil_ ## name ## _table
+#define	MFI_DATASET(name)	mfu_ ## name ## _tbl
 
 #define	MFI_COMMAND(set, name, function)				\
 	static struct mfiutil_command function ## _mfiutil_command =	\
