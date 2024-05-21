/* auto-generated by gen_syscalls.py, don't edit */

#ifndef Z_INCLUDE_SYSCALLS_CHARGER_H
#define Z_INCLUDE_SYSCALLS_CHARGER_H


#include <zephyr/tracing/tracing_syscall.h>

#ifndef _ASMLANGUAGE

#include <stdarg.h>

#include <syscall_list.h>
#include <zephyr/syscall.h>

#include <zephyr/linker/sections.h>


#ifdef __cplusplus
extern "C" {
#endif

extern int z_impl_charger_get_prop(const struct device * dev, const charger_prop_t prop, union charger_propval * val);

__pinned_func
static inline int charger_get_prop(const struct device * dev, const charger_prop_t prop, union charger_propval * val)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; const charger_prop_t val; } parm1 = { .val = prop };
		union { uintptr_t x; union charger_propval * val; } parm2 = { .val = val };
		return (int) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_CHARGER_GET_PROP);
	}
#endif
	compiler_barrier();
	return z_impl_charger_get_prop(dev, prop, val);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define charger_get_prop(dev, prop, val) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_CHARGER_GET_PROP, charger_get_prop, dev, prop, val); 	syscall__retval = charger_get_prop(dev, prop, val); 	sys_port_trace_syscall_exit(K_SYSCALL_CHARGER_GET_PROP, charger_get_prop, dev, prop, val, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_charger_set_prop(const struct device * dev, const charger_prop_t prop, const union charger_propval * val);

__pinned_func
static inline int charger_set_prop(const struct device * dev, const charger_prop_t prop, const union charger_propval * val)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; const charger_prop_t val; } parm1 = { .val = prop };
		union { uintptr_t x; const union charger_propval * val; } parm2 = { .val = val };
		return (int) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_CHARGER_SET_PROP);
	}
#endif
	compiler_barrier();
	return z_impl_charger_set_prop(dev, prop, val);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define charger_set_prop(dev, prop, val) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_CHARGER_SET_PROP, charger_set_prop, dev, prop, val); 	syscall__retval = charger_set_prop(dev, prop, val); 	sys_port_trace_syscall_exit(K_SYSCALL_CHARGER_SET_PROP, charger_set_prop, dev, prop, val, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_charger_charge_enable(const struct device * dev, const bool enable);

__pinned_func
static inline int charger_charge_enable(const struct device * dev, const bool enable)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; const bool val; } parm1 = { .val = enable };
		return (int) arch_syscall_invoke2(parm0.x, parm1.x, K_SYSCALL_CHARGER_CHARGE_ENABLE);
	}
#endif
	compiler_barrier();
	return z_impl_charger_charge_enable(dev, enable);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define charger_charge_enable(dev, enable) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_CHARGER_CHARGE_ENABLE, charger_charge_enable, dev, enable); 	syscall__retval = charger_charge_enable(dev, enable); 	sys_port_trace_syscall_exit(K_SYSCALL_CHARGER_CHARGE_ENABLE, charger_charge_enable, dev, enable, syscall__retval); 	syscall__retval; })
#endif
#endif


#ifdef __cplusplus
}
#endif

#endif
#endif /* include guard */
