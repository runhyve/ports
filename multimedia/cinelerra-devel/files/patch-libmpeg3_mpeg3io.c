--- libmpeg3/mpeg3io.c.orig	2010-12-26 16:11:34.332228806 +0300
+++ libmpeg3/mpeg3io.c	2010-12-26 16:11:44.838231944 +0300
@@ -1,8 +1,8 @@
 #include "mpeg3private.h"
 #include "mpeg3protos.h"
 
-#include <mntent.h>
-#include <stdint.h>
+#include <fstab.h>
+#include <sys/types.h>
 #include <stdlib.h>
 #include <string.h>
 #include <sys/stat.h>
@@ -35,8 +35,8 @@
 
 int64_t mpeg3io_get_total_bytes(mpeg3_fs_t *fs)
 {
-	struct stat64 ostat;
-	stat64(fs->path, &ostat);
+	struct stat ostat;
+	stat(fs->path, &ostat);
 	fs->total_bytes = ostat.st_size;
 	return fs->total_bytes;
 	
@@ -50,8 +50,8 @@
 
 int64_t mpeg3io_path_total_bytes(char *path)
 {
-	struct stat64 st;
-	if(stat64(path, &st) < 0) return 0;
+	struct stat st;
+	if(stat(path, &st) < 0) return 0;
 	return st.st_size;
 }
 
@@ -61,7 +61,7 @@
 	mpeg3_get_keys(fs->css, fs->path);
 
 //printf("mpeg3io_open_file 1 %s\n", fs->path);
-	if(!(fs->fd = fopen64(fs->path, "rb")))
+	if(!(fs->fd = fopen(fs->path, "rb")))
 	{
 		if (fs->path) fprintf(stderr,"[mpeg3io_open_file] Error opening file '%s': ",fs->path);
 		perror("");
@@ -162,7 +162,7 @@
 
 
 
-		fseeko64(fs->fd, new_buffer_position, SEEK_SET);
+		fseek(fs->fd, new_buffer_position, SEEK_SET);
 		fread(fs->buffer, 1, remainder_start, fs->fd);
 
 
@@ -177,7 +177,7 @@
 		fs->buffer_position = fs->current_byte;
 		fs->buffer_offset = 0;
 
-		result = fseeko64(fs->fd, fs->buffer_position, SEEK_SET);
+		result = fseek(fs->fd, fs->buffer_position, SEEK_SET);
 //printf("mpeg3io_read_buffer 2 %llx %llx\n", fs->buffer_position, ftell(fs->fd));
 		fs->buffer_size = fread(fs->buffer, 1, MPEG3_IO_SIZE, fs->fd);
 
@@ -216,27 +216,27 @@
 
 int mpeg3io_device(char *path, char *device)
 {
-	struct stat64 file_st, device_st;
-    struct mntent *mnt;
-	FILE *fp;
+	struct stat file_st, device_st;
+        struct fstab *mnt;
+        int fp;
 
-	if(stat64(path, &file_st) < 0)
+	if(stat(path, &file_st) < 0)
 	{
 		perror("mpeg3io_device");
 		return 1;
 	}
 
-	fp = setmntent(MOUNTED, "r");
-    while(fp && (mnt = getmntent(fp)))
+        fp = setfsent();
+        while(fp && (mnt = getfsent()))
 	{
-		if(stat64(mnt->mnt_fsname, &device_st) < 0) continue;
+		if(stat(mnt->fs_spec, &device_st) < 0) continue;
 		if(device_st.st_rdev == file_st.st_dev)
 		{
-			strncpy(device, mnt->mnt_fsname, MPEG3_STRLEN);
+			strncpy(device, mnt->fs_spec, MPEG3_STRLEN);
 			break;
-		}
+		}		
 	}
-	endmntent(fp);
+	endfsent();
 
 	return 0;
 }
