#ifndef SIGAR_PRIVATE_DOT_H
#define SIGAR_PRIVATE_DOT_H

#include "sigar_log.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef WIN32
#include <unistd.h>
#include <strings.h>
#endif

#ifdef DMALLOC
#define _MEMORY_H /* exclude memory.h on solaris */
#define DMALLOC_FUNC_CHECK
#include <dmalloc.h>
#endif

/* common to all os sigar_t's */
/* XXX: this is ugly; but don't want the same stuffs
 * duplicated on 4 platforms and am too lazy to change
 * sigar_t to the way it was originally where sigar_t was
 * common and contained a sigar_os_t.
 * feel free trav ;-)
 */
#define SIGAR_T_BASE \
   int log_level; \
   void *log_data; \
   sigar_log_impl_t log_impl; \
   unsigned int ncpu; \
   unsigned long version; \
   unsigned long boot_time; \
   int ticks; \
   sigar_pid_t pid; \
   char errbuf[256]; \
   char *ifconf_buf; \
   int ifconf_len; \
   char *self_path

#if defined(WIN32)
#   define SIGAR_INLINE __inline
#elif defined(__GNUC__)
#   define SIGAR_INLINE inline
#else
#   define SIGAR_INLINE
#endif

#ifdef DMALLOC
/* linux has its own strdup macro, make sure we use dmalloc's */
#define sigar_strdup(s) \
    dmalloc_strdup(__FILE__, __LINE__, s, 0);
#else
#define sigar_strdup(s) \
    strdup(s)
#endif

#define SIGAR_ZERO(s) \
    memset(s, '\0', sizeof(*(s)))

#define SIGAR_STRNCPY(dest, src, len) \
    strncpy(dest, src, len); \
    dest[len-1] = '\0'

/* we use fixed size buffers pretty much everywhere */
/* this is strncpy + ensured \0 terminator */
#define SIGAR_SSTRCPY(dest, src) \
    SIGAR_STRNCPY(dest, src, sizeof(dest))

#ifndef strEQ
#define strEQ(s1, s2) (strcmp(s1, s2) == 0)
#endif

#ifndef strnEQ
#define strnEQ(s1, s2, n) (strncmp(s1, s2, n) == 0)
#endif

#define SIGAR_SEC2NANO(s) \
    ((sigar_uint64_t)(s) * (sigar_uint64_t)1000000000)

/* cpu ticks to seconds */
#define SIGAR_TICK2SEC(s) ((s) / sigar->ticks)

#define IFTYPE_LO  2
#define IFTYPE_ETH 3

#define SIGAR_LAST_PROC_EXPIRE 2

#define SIGAR_FS_MAX 10

#define SIGAR_CPU_INFO_MAX 4

#define SIGAR_CPU_LIST_MAX 4

#define SIGAR_PROC_LIST_MAX 256

#define SIGAR_PROC_ARGS_MAX 12

#define SIGAR_NET_ROUTE_LIST_MAX 6

#define SIGAR_NET_IFLIST_MAX 20

#define SIGAR_NET_CONNLIST_MAX 20

#define SIGAR_WHO_LIST_MAX 12

int sigar_os_open(sigar_t **sigar);

int sigar_os_close(sigar_t *sigar);

char *sigar_os_error_string(sigar_t *sigar, int err);

int sigar_proc_list_create(sigar_proc_list_t *proclist);

int sigar_proc_list_grow(sigar_proc_list_t *proclist);

#define SIGAR_PROC_LIST_GROW(proclist) \
    if (proclist->number >= proclist->size) { \
        sigar_proc_list_grow(proclist); \
    }

int sigar_proc_args_create(sigar_proc_args_t *procargs);

int sigar_proc_args_grow(sigar_proc_args_t *procargs);

#define SIGAR_PROC_ARGS_GROW(procargs) \
    if (procargs->number >= procargs->size) { \
        sigar_proc_args_grow(procargs); \
    }

int sigar_file_system_list_create(sigar_file_system_list_t *fslist);

int sigar_file_system_list_grow(sigar_file_system_list_t *fslist);

#define SIGAR_FILE_SYSTEM_LIST_GROW(fslist) \
    if (fslist->number >= fslist->size) { \
        sigar_file_system_list_grow(fslist); \
    }

int sigar_os_fs_type_get(sigar_file_system_t *fsp);

/* os plugins that set fsp->type call fs_type_get directly */
#define sigar_fs_type_init(fsp) \
   fsp->type = SIGAR_FSTYPE_UNKNOWN; \
   sigar_fs_type_get(fsp)

void sigar_fs_type_get(sigar_file_system_t *fsp);

int sigar_cpu_info_list_create(sigar_cpu_info_list_t *cpu_infos);

int sigar_cpu_info_list_grow(sigar_cpu_info_list_t *cpu_infos);

#define SIGAR_CPU_INFO_LIST_GROW(cpu_infos) \
    if (cpu_infos->number >= cpu_infos->size) { \
        sigar_cpu_info_list_grow(cpu_infos); \
    }

int sigar_cpu_list_create(sigar_cpu_list_t *cpulist);

int sigar_cpu_list_grow(sigar_cpu_list_t *cpulist);

#define SIGAR_CPU_LIST_GROW(cpulist) \
    if (cpulist->number >= cpulist->size) { \
        sigar_cpu_list_grow(cpulist); \
    }

int sigar_net_route_list_create(sigar_net_route_list_t *routelist);

int sigar_net_route_list_grow(sigar_net_route_list_t *net_routelist);

#define SIGAR_NET_ROUTE_LIST_GROW(routelist) \
    if (routelist->number >= routelist->size) { \
        sigar_net_route_list_grow(routelist); \
    }

int sigar_net_interface_list_create(sigar_net_interface_list_t *iflist);

int sigar_net_interface_list_grow(sigar_net_interface_list_t *iflist);

#define SIGAR_NET_IFLIST_GROW(iflist) \
    if (iflist->number >= iflist->size) { \
        sigar_net_interface_list_grow(iflist); \
    }

int sigar_net_connection_list_create(sigar_net_connection_list_t *connlist);

int sigar_net_connection_list_grow(sigar_net_connection_list_t *connlist);

#define SIGAR_NET_CONNLIST_GROW(connlist) \
    if (connlist->number >= connlist->size) { \
        sigar_net_connection_list_grow(connlist); \
    }

#define sigar_net_address_set(a, val) \
    (a).addr.in = val; \
    (a).family = SIGAR_AF_INET

#define sigar_net_address6_set(a, val) \
    memcpy(&((a).addr.in6), val, sizeof((a).addr.in6)); \
    (a).family = SIGAR_AF_INET6

#define SIGAR_IFHWADDRLEN 6

#define sigar_net_address_mac_set(a, val, len) \
    memcpy(&((a).addr.mac), val, len); \
    (a).family = SIGAR_AF_LINK

#define sigar_hwaddr_set_null(ifconfig) \
    SIGAR_ZERO(&ifconfig->hwaddr.addr.mac); \
    ifconfig->hwaddr.family = SIGAR_AF_LINK

int sigar_who_list_create(sigar_who_list_t *wholist);

int sigar_who_list_grow(sigar_who_list_t *wholist);

#define SIGAR_WHO_LIST_GROW(wholist) \
    if (wholist->number >= wholist->size) { \
        sigar_who_list_grow(wholist); \
    }

int sigar_user_id_get(sigar_t *sigar, const char *name, int *uid);

int sigar_user_name_get(sigar_t *sigar, int uid, char *buf, int buflen);

int sigar_group_name_get(sigar_t *sigar, int gid, char *buf, int buflen);

#define SIGAR_PROC_ENV_KEY_LOOKUP() \
    if ((procenv->type == SIGAR_PROC_ENV_KEY) && \
        (pid == sigar->pid)) \
    { \
        char *value = getenv(procenv->key); \
        if (value != NULL) { \
            procenv->env_getter(procenv->data, \
                                procenv->key, \
                                procenv->klen, \
                                value, strlen(value)); \
        } \
        return SIGAR_OK; \
    }

#define SIGAR_DISK_STATS_NOTIMPL(fsusage) \
    fsusage->disk_reads = fsusage->disk_writes = \
    fsusage->disk_read_bytes = fsusage->disk_write_bytes = \
    fsusage->disk_queue = SIGAR_FIELD_NOTIMPL;

#if defined(WIN32) || defined(NETWARE)
int sigar_get_iftype(const char *name, int *type, int *inst);
#endif

#define SIGAR_NIC_LOOPBACK "Local Loopback"
#define SIGAR_NIC_ETHERNET "Ethernet"
#define SIGAR_NIC_NETROM   "AMPR NET/ROM"

#ifndef WIN32
#include <netdb.h>
#endif

#define SIGAR_HOSTENT_LEN 512
#if defined(__hpux) || defined(_AIX)
#define SIGAR_HAS_HOSTENT_DATA
#endif

typedef struct {
    char buffer[SIGAR_HOSTENT_LEN];
    int error;
#ifndef WIN32
    struct hostent hs;
#endif
#ifdef SIGAR_HAS_HOSTENT_DATA
    struct hostent_data hd;
#endif
} sigar_hostent_t;

#endif
