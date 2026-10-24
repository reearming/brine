#ifndef WINTERNL_H
#define WINTERNL_H

#include "winnt.h"
#include <stdint.h>

typedef struct _UNICODE_STRING {
  USHORT  Length;
  USHORT  MaximumLength;
  PWSTR  Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
  uint8_t reserved1[16];
  void *reserved2[10];
  UNICODE_STRING ImagePathName;
  UNICODE_STRING CommandLine;
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

typedef struct _LIST_ENTRY {
   struct list_entry *Flink;
   struct list_entry *Blink;
} LIST_ENTRY, *PLIST_ENTRY, list_entry;

typedef struct _PEB_LDR_DATA {
  uint8_t reserved1[8];
  void *reserved2[3];
  list_entry in_memory_order_module_list;
} PEB_LDR_DATA, *PPEB_LDR_DATA, peb_ldr_data;

typedef struct _PEB {
  uint8_t reserved1[2];
  uint8_t being_debugged;
  uint8_t reserved2[1];
  void *reserved3[2];
  peb_ldr_data *ldr;
  PRTL_USER_PROCESS_PARAMETERS  ProcessParameters;
  void *reserved4[3];
  void *atl_thunks_list_ptr;
  void *reserved5;
  uint64_t reserved6;
  void *reserved7;
  uint64_t reserved8;
  uint64_t atl_thunks_list_ptr32;
  void *reserved9[45];
  uint8_t reserved10[96];
  PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;
  uint8_t reserved11[128];
  void *reserved12[1];
  uint64_t session_id;
} PEB, *PPEB, peb;

typedef struct _TEB {
  void *reserved1[12];
  peb *process_environment_block;
  void *reserved2[399];
  uint8_t *reserved3[1952];
  void *tls_slots[64];
  uint8_t reserved4[8];
  void *reserved5[26];
  void *reserved_for_ole;
  void *reserved6[4];
  void *tls_expansion_slots;
} TEB, *PTEB, teb;

#endif
