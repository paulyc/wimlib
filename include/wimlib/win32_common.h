#ifndef _WIMLIB_WIN32_COMMON_H
#define _WIMLIB_WIN32_COMMON_H

#include <windows.h>
#ifdef ERROR
#  undef ERROR
#endif

#include "wimlib/types.h"
#include "wimlib/win32.h"

#include <ntstatus.h>
#include <winternl.h>

extern void
set_errno_from_GetLastError(void);

extern void
set_errno_from_win32_error(DWORD err);

extern void
set_errno_from_nt_status(NTSTATUS status);

/* ntdll functions  */

extern NTSTATUS (WINAPI *func_NtCreateFile)(PHANDLE FileHandle,
					    ACCESS_MASK DesiredAccess,
					    POBJECT_ATTRIBUTES ObjectAttributes,
					    PIO_STATUS_BLOCK IoStatusBlock,
					    PLARGE_INTEGER AllocationSize,
					    ULONG FileAttributes,
					    ULONG ShareAccess,
					    ULONG CreateDisposition,
					    ULONG CreateOptions,
					    PVOID EaBuffer,
					    ULONG EaLength);

extern NTSTATUS (WINAPI *func_NtOpenFile) (PHANDLE FileHandle,
					   ACCESS_MASK DesiredAccess,
					   POBJECT_ATTRIBUTES ObjectAttributes,
					   PIO_STATUS_BLOCK IoStatusBlock,
					   ULONG ShareAccess,
					   ULONG OpenOptions);

extern NTSTATUS (WINAPI *func_NtReadFile) (HANDLE FileHandle,
					   HANDLE Event,
					   PIO_APC_ROUTINE ApcRoutine,
					   PVOID ApcContext,
					   PIO_STATUS_BLOCK IoStatusBlock,
					   PVOID Buffer,
					   ULONG Length,
					   PLARGE_INTEGER ByteOffset,
					   PULONG Key);

extern NTSTATUS (WINAPI *func_NtWriteFile) (HANDLE FileHandle,
					    HANDLE Event,
					    PIO_APC_ROUTINE ApcRoutine,
					    PVOID ApcContext,
					    PIO_STATUS_BLOCK IoStatusBlock,
					    PVOID Buffer,
					    ULONG Length,
					    PLARGE_INTEGER ByteOffset,
					    PULONG Key);

extern NTSTATUS (WINAPI *func_NtQueryInformationFile)(HANDLE FileHandle,
						      PIO_STATUS_BLOCK IoStatusBlock,
						      PVOID FileInformation,
						      ULONG Length,
						      FILE_INFORMATION_CLASS FileInformationClass);

extern NTSTATUS (WINAPI *func_NtQuerySecurityObject)(HANDLE handle,
						     SECURITY_INFORMATION SecurityInformation,
						     PSECURITY_DESCRIPTOR SecurityDescriptor,
						     ULONG Length,
						     PULONG LengthNeeded);

extern NTSTATUS (WINAPI *func_NtQueryDirectoryFile) (HANDLE FileHandle,
						     HANDLE Event,
						     PIO_APC_ROUTINE ApcRoutine,
						     PVOID ApcContext,
						     PIO_STATUS_BLOCK IoStatusBlock,
						     PVOID FileInformation,
						     ULONG Length,
						     FILE_INFORMATION_CLASS FileInformationClass,
						     BOOLEAN ReturnSingleEntry,
						     PUNICODE_STRING FileName,
						     BOOLEAN RestartScan);

extern NTSTATUS (WINAPI *func_NtQueryVolumeInformationFile) (HANDLE FileHandle,
							     PIO_STATUS_BLOCK IoStatusBlock,
							     PVOID FsInformation,
							     ULONG Length,
							     FS_INFORMATION_CLASS FsInformationClass);

extern NTSTATUS (WINAPI *func_NtSetInformationFile)(HANDLE FileHandle,
						    PIO_STATUS_BLOCK IoStatusBlock,
						    PVOID FileInformation,
						    ULONG Length,
						    FILE_INFORMATION_CLASS FileInformationClass);

extern NTSTATUS (WINAPI *func_NtSetSecurityObject)(HANDLE Handle,
						   SECURITY_INFORMATION SecurityInformation,
						   PSECURITY_DESCRIPTOR SecurityDescriptor);

extern NTSTATUS (WINAPI *func_NtFsControlFile) (HANDLE FileHandle,
						HANDLE Event,
						PIO_APC_ROUTINE ApcRoutine,
						PVOID ApcContext,
						PIO_STATUS_BLOCK IoStatusBlock,
						ULONG FsControlCode,
						PVOID InputBuffer,
						ULONG InputBufferLength,
						PVOID OutputBuffer,
						ULONG OutputBufferLength);

extern NTSTATUS (WINAPI *func_NtClose) (HANDLE Handle);

extern DWORD (WINAPI *func_RtlNtStatusToDosError)(NTSTATUS status);

typedef struct _RTLP_CURDIR_REF {
	LONG RefCount;
	HANDLE Handle;
} RTLP_CURDIR_REF, *PRTLP_CURDIR_REF;

typedef struct _RTL_RELATIVE_NAME_U {
	UNICODE_STRING RelativeName;
	HANDLE ContainingDirectory;
	PRTLP_CURDIR_REF CurDirRef;
} RTL_RELATIVE_NAME_U, *PRTL_RELATIVE_NAME_U;

extern NTSTATUS (WINAPI *func_RtlDosPathNameToNtPathName_U_WithStatus)
		(IN PCWSTR DosName,
		 OUT PUNICODE_STRING NtName,
		 OUT PCWSTR *PartName,
		 OUT PRTL_RELATIVE_NAME_U RelativeName);

extern NTSTATUS (WINAPI *func_RtlCreateSystemVolumeInformationFolder)
			(PCUNICODE_STRING VolumeRootPath);


extern bool
windows_version_is_at_least(unsigned major, unsigned minor);

#define running_on_windows_xp_or_later() \
			windows_version_is_at_least(5, 1)

#define running_on_windows_vista_or_later() \
			windows_version_is_at_least(6, 0)

#define running_on_windows_7_or_later() \
			windows_version_is_at_least(6, 1)



#endif /* _WIMLIB_WIN32_COMMON_H */
