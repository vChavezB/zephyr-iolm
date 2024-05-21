/* auto-generated by gen_syscalls.py, don't edit */

#ifndef Z_INCLUDE_SYSCALLS_SIP_SVC_DRIVER_H
#define Z_INCLUDE_SYSCALLS_SIP_SVC_DRIVER_H


#include <zephyr/tracing/tracing_syscall.h>

#ifndef _ASMLANGUAGE

#include <stdarg.h>

#include <syscall_list.h>
#include <zephyr/syscall.h>

#include <zephyr/linker/sections.h>


#ifdef __cplusplus
extern "C" {
#endif

extern void z_impl_sip_supervisory_call(const struct device * dev, unsigned long function_id, unsigned long arg0, unsigned long arg1, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5, unsigned long arg6, struct arm_smccc_res * res);

__pinned_func
static inline void sip_supervisory_call(const struct device * dev, unsigned long function_id, unsigned long arg0, unsigned long arg1, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5, unsigned long arg6, struct arm_smccc_res * res)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; unsigned long val; } parm1 = { .val = function_id };
		union { uintptr_t x; unsigned long val; } parm2 = { .val = arg0 };
		union { uintptr_t x; unsigned long val; } parm3 = { .val = arg1 };
		union { uintptr_t x; unsigned long val; } parm4 = { .val = arg2 };
		union { uintptr_t x; unsigned long val; } parm5 = { .val = arg3 };
		union { uintptr_t x; unsigned long val; } parm6 = { .val = arg4 };
		union { uintptr_t x; unsigned long val; } parm7 = { .val = arg5 };
		union { uintptr_t x; unsigned long val; } parm8 = { .val = arg6 };
		union { uintptr_t x; struct arm_smccc_res * val; } parm9 = { .val = res };
		uintptr_t more[] = {
			parm5.x,
			parm6.x,
			parm7.x,
			parm8.x,
			parm9.x
		};
		(void) arch_syscall_invoke6(parm0.x, parm1.x, parm2.x, parm3.x, parm4.x, (uintptr_t) &more, K_SYSCALL_SIP_SUPERVISORY_CALL);
		return;
	}
#endif
	compiler_barrier();
	z_impl_sip_supervisory_call(dev, function_id, arg0, arg1, arg2, arg3, arg4, arg5, arg6, res);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define sip_supervisory_call(dev, function_id, arg0, arg1, arg2, arg3, arg4, arg5, arg6, res) do { 	sys_port_trace_syscall_enter(K_SYSCALL_SIP_SUPERVISORY_CALL, sip_supervisory_call, dev, function_id, arg0, arg1, arg2, arg3, arg4, arg5, arg6, res); 	sip_supervisory_call(dev, function_id, arg0, arg1, arg2, arg3, arg4, arg5, arg6, res); 	sys_port_trace_syscall_exit(K_SYSCALL_SIP_SUPERVISORY_CALL, sip_supervisory_call, dev, function_id, arg0, arg1, arg2, arg3, arg4, arg5, arg6, res); } while(false)
#endif
#endif


extern bool z_impl_sip_svc_plat_func_id_valid(const struct device * dev, uint32_t command, uint32_t func_id);

__pinned_func
static inline bool sip_svc_plat_func_id_valid(const struct device * dev, uint32_t command, uint32_t func_id)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; uint32_t val; } parm1 = { .val = command };
		union { uintptr_t x; uint32_t val; } parm2 = { .val = func_id };
		return (bool) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_SIP_SVC_PLAT_FUNC_ID_VALID);
	}
#endif
	compiler_barrier();
	return z_impl_sip_svc_plat_func_id_valid(dev, command, func_id);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define sip_svc_plat_func_id_valid(dev, command, func_id) ({ 	bool syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_SIP_SVC_PLAT_FUNC_ID_VALID, sip_svc_plat_func_id_valid, dev, command, func_id); 	syscall__retval = sip_svc_plat_func_id_valid(dev, command, func_id); 	sys_port_trace_syscall_exit(K_SYSCALL_SIP_SVC_PLAT_FUNC_ID_VALID, sip_svc_plat_func_id_valid, dev, command, func_id, syscall__retval); 	syscall__retval; })
#endif
#endif


extern uint32_t z_impl_sip_svc_plat_format_trans_id(const struct device * dev, uint32_t client_idx, uint32_t trans_idx);

__pinned_func
static inline uint32_t sip_svc_plat_format_trans_id(const struct device * dev, uint32_t client_idx, uint32_t trans_idx)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; uint32_t val; } parm1 = { .val = client_idx };
		union { uintptr_t x; uint32_t val; } parm2 = { .val = trans_idx };
		return (uint32_t) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_SIP_SVC_PLAT_FORMAT_TRANS_ID);
	}
#endif
	compiler_barrier();
	return z_impl_sip_svc_plat_format_trans_id(dev, client_idx, trans_idx);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define sip_svc_plat_format_trans_id(dev, client_idx, trans_idx) ({ 	uint32_t syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_SIP_SVC_PLAT_FORMAT_TRANS_ID, sip_svc_plat_format_trans_id, dev, client_idx, trans_idx); 	syscall__retval = sip_svc_plat_format_trans_id(dev, client_idx, trans_idx); 	sys_port_trace_syscall_exit(K_SYSCALL_SIP_SVC_PLAT_FORMAT_TRANS_ID, sip_svc_plat_format_trans_id, dev, client_idx, trans_idx, syscall__retval); 	syscall__retval; })
#endif
#endif


extern uint32_t z_impl_sip_svc_plat_get_trans_idx(const struct device * dev, uint32_t trans_id);

__pinned_func
static inline uint32_t sip_svc_plat_get_trans_idx(const struct device * dev, uint32_t trans_id)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; uint32_t val; } parm1 = { .val = trans_id };
		return (uint32_t) arch_syscall_invoke2(parm0.x, parm1.x, K_SYSCALL_SIP_SVC_PLAT_GET_TRANS_IDX);
	}
#endif
	compiler_barrier();
	return z_impl_sip_svc_plat_get_trans_idx(dev, trans_id);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define sip_svc_plat_get_trans_idx(dev, trans_id) ({ 	uint32_t syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_SIP_SVC_PLAT_GET_TRANS_IDX, sip_svc_plat_get_trans_idx, dev, trans_id); 	syscall__retval = sip_svc_plat_get_trans_idx(dev, trans_id); 	sys_port_trace_syscall_exit(K_SYSCALL_SIP_SVC_PLAT_GET_TRANS_IDX, sip_svc_plat_get_trans_idx, dev, trans_id, syscall__retval); 	syscall__retval; })
#endif
#endif


extern void z_impl_sip_svc_plat_update_trans_id(const struct device * dev, struct sip_svc_request * request, uint32_t trans_id);

__pinned_func
static inline void sip_svc_plat_update_trans_id(const struct device * dev, struct sip_svc_request * request, uint32_t trans_id)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; struct sip_svc_request * val; } parm1 = { .val = request };
		union { uintptr_t x; uint32_t val; } parm2 = { .val = trans_id };
		(void) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_SIP_SVC_PLAT_UPDATE_TRANS_ID);
		return;
	}
#endif
	compiler_barrier();
	z_impl_sip_svc_plat_update_trans_id(dev, request, trans_id);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define sip_svc_plat_update_trans_id(dev, request, trans_id) do { 	sys_port_trace_syscall_enter(K_SYSCALL_SIP_SVC_PLAT_UPDATE_TRANS_ID, sip_svc_plat_update_trans_id, dev, request, trans_id); 	sip_svc_plat_update_trans_id(dev, request, trans_id); 	sys_port_trace_syscall_exit(K_SYSCALL_SIP_SVC_PLAT_UPDATE_TRANS_ID, sip_svc_plat_update_trans_id, dev, request, trans_id); } while(false)
#endif
#endif


extern uint32_t z_impl_sip_svc_plat_get_error_code(const struct device * dev, struct arm_smccc_res * res);

__pinned_func
static inline uint32_t sip_svc_plat_get_error_code(const struct device * dev, struct arm_smccc_res * res)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; struct arm_smccc_res * val; } parm1 = { .val = res };
		return (uint32_t) arch_syscall_invoke2(parm0.x, parm1.x, K_SYSCALL_SIP_SVC_PLAT_GET_ERROR_CODE);
	}
#endif
	compiler_barrier();
	return z_impl_sip_svc_plat_get_error_code(dev, res);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define sip_svc_plat_get_error_code(dev, res) ({ 	uint32_t syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_SIP_SVC_PLAT_GET_ERROR_CODE, sip_svc_plat_get_error_code, dev, res); 	syscall__retval = sip_svc_plat_get_error_code(dev, res); 	sys_port_trace_syscall_exit(K_SYSCALL_SIP_SVC_PLAT_GET_ERROR_CODE, sip_svc_plat_get_error_code, dev, res, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_sip_svc_plat_async_res_req(const struct device * dev, unsigned long * a0, unsigned long * a1, unsigned long * a2, unsigned long * a3, unsigned long * a4, unsigned long * a5, unsigned long * a6, unsigned long * a7, char * buf, size_t size);

__pinned_func
static inline int sip_svc_plat_async_res_req(const struct device * dev, unsigned long * a0, unsigned long * a1, unsigned long * a2, unsigned long * a3, unsigned long * a4, unsigned long * a5, unsigned long * a6, unsigned long * a7, char * buf, size_t size)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; unsigned long * val; } parm1 = { .val = a0 };
		union { uintptr_t x; unsigned long * val; } parm2 = { .val = a1 };
		union { uintptr_t x; unsigned long * val; } parm3 = { .val = a2 };
		union { uintptr_t x; unsigned long * val; } parm4 = { .val = a3 };
		union { uintptr_t x; unsigned long * val; } parm5 = { .val = a4 };
		union { uintptr_t x; unsigned long * val; } parm6 = { .val = a5 };
		union { uintptr_t x; unsigned long * val; } parm7 = { .val = a6 };
		union { uintptr_t x; unsigned long * val; } parm8 = { .val = a7 };
		union { uintptr_t x; char * val; } parm9 = { .val = buf };
		union { uintptr_t x; size_t val; } parm10 = { .val = size };
		uintptr_t more[] = {
			parm5.x,
			parm6.x,
			parm7.x,
			parm8.x,
			parm9.x,
			parm10.x
		};
		return (int) arch_syscall_invoke6(parm0.x, parm1.x, parm2.x, parm3.x, parm4.x, (uintptr_t) &more, K_SYSCALL_SIP_SVC_PLAT_ASYNC_RES_REQ);
	}
#endif
	compiler_barrier();
	return z_impl_sip_svc_plat_async_res_req(dev, a0, a1, a2, a3, a4, a5, a6, a7, buf, size);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define sip_svc_plat_async_res_req(dev, a0, a1, a2, a3, a4, a5, a6, a7, buf, size) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_SIP_SVC_PLAT_ASYNC_RES_REQ, sip_svc_plat_async_res_req, dev, a0, a1, a2, a3, a4, a5, a6, a7, buf, size); 	syscall__retval = sip_svc_plat_async_res_req(dev, a0, a1, a2, a3, a4, a5, a6, a7, buf, size); 	sys_port_trace_syscall_exit(K_SYSCALL_SIP_SVC_PLAT_ASYNC_RES_REQ, sip_svc_plat_async_res_req, dev, a0, a1, a2, a3, a4, a5, a6, a7, buf, size, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_sip_svc_plat_async_res_res(const struct device * dev, struct arm_smccc_res * res, char * buf, size_t * size, uint32_t * trans_id);

__pinned_func
static inline int sip_svc_plat_async_res_res(const struct device * dev, struct arm_smccc_res * res, char * buf, size_t * size, uint32_t * trans_id)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; struct arm_smccc_res * val; } parm1 = { .val = res };
		union { uintptr_t x; char * val; } parm2 = { .val = buf };
		union { uintptr_t x; size_t * val; } parm3 = { .val = size };
		union { uintptr_t x; uint32_t * val; } parm4 = { .val = trans_id };
		return (int) arch_syscall_invoke5(parm0.x, parm1.x, parm2.x, parm3.x, parm4.x, K_SYSCALL_SIP_SVC_PLAT_ASYNC_RES_RES);
	}
#endif
	compiler_barrier();
	return z_impl_sip_svc_plat_async_res_res(dev, res, buf, size, trans_id);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define sip_svc_plat_async_res_res(dev, res, buf, size, trans_id) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_SIP_SVC_PLAT_ASYNC_RES_RES, sip_svc_plat_async_res_res, dev, res, buf, size, trans_id); 	syscall__retval = sip_svc_plat_async_res_res(dev, res, buf, size, trans_id); 	sys_port_trace_syscall_exit(K_SYSCALL_SIP_SVC_PLAT_ASYNC_RES_RES, sip_svc_plat_async_res_res, dev, res, buf, size, trans_id, syscall__retval); 	syscall__retval; })
#endif
#endif


extern void z_impl_sip_svc_plat_free_async_memory(const struct device * dev, struct sip_svc_request * request);

__pinned_func
static inline void sip_svc_plat_free_async_memory(const struct device * dev, struct sip_svc_request * request)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; struct sip_svc_request * val; } parm1 = { .val = request };
		(void) arch_syscall_invoke2(parm0.x, parm1.x, K_SYSCALL_SIP_SVC_PLAT_FREE_ASYNC_MEMORY);
		return;
	}
#endif
	compiler_barrier();
	z_impl_sip_svc_plat_free_async_memory(dev, request);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define sip_svc_plat_free_async_memory(dev, request) do { 	sys_port_trace_syscall_enter(K_SYSCALL_SIP_SVC_PLAT_FREE_ASYNC_MEMORY, sip_svc_plat_free_async_memory, dev, request); 	sip_svc_plat_free_async_memory(dev, request); 	sys_port_trace_syscall_exit(K_SYSCALL_SIP_SVC_PLAT_FREE_ASYNC_MEMORY, sip_svc_plat_free_async_memory, dev, request); } while(false)
#endif
#endif


#ifdef __cplusplus
}
#endif

#endif
#endif /* include guard */
