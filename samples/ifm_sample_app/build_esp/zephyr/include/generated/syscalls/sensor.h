/* auto-generated by gen_syscalls.py, don't edit */

#ifndef Z_INCLUDE_SYSCALLS_SENSOR_H
#define Z_INCLUDE_SYSCALLS_SENSOR_H


#include <zephyr/tracing/tracing_syscall.h>

#ifndef _ASMLANGUAGE

#include <stdarg.h>

#include <syscall_list.h>
#include <zephyr/syscall.h>

#include <zephyr/linker/sections.h>


#ifdef __cplusplus
extern "C" {
#endif

extern int z_impl_sensor_attr_set(const struct device * dev, enum sensor_channel chan, enum sensor_attribute attr, const struct sensor_value * val);

__pinned_func
static inline int sensor_attr_set(const struct device * dev, enum sensor_channel chan, enum sensor_attribute attr, const struct sensor_value * val)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; enum sensor_channel val; } parm1 = { .val = chan };
		union { uintptr_t x; enum sensor_attribute val; } parm2 = { .val = attr };
		union { uintptr_t x; const struct sensor_value * val; } parm3 = { .val = val };
		return (int) arch_syscall_invoke4(parm0.x, parm1.x, parm2.x, parm3.x, K_SYSCALL_SENSOR_ATTR_SET);
	}
#endif
	compiler_barrier();
	return z_impl_sensor_attr_set(dev, chan, attr, val);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define sensor_attr_set(dev, chan, attr, val) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_SENSOR_ATTR_SET, sensor_attr_set, dev, chan, attr, val); 	syscall__retval = sensor_attr_set(dev, chan, attr, val); 	sys_port_trace_syscall_exit(K_SYSCALL_SENSOR_ATTR_SET, sensor_attr_set, dev, chan, attr, val, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_sensor_attr_get(const struct device * dev, enum sensor_channel chan, enum sensor_attribute attr, struct sensor_value * val);

__pinned_func
static inline int sensor_attr_get(const struct device * dev, enum sensor_channel chan, enum sensor_attribute attr, struct sensor_value * val)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; enum sensor_channel val; } parm1 = { .val = chan };
		union { uintptr_t x; enum sensor_attribute val; } parm2 = { .val = attr };
		union { uintptr_t x; struct sensor_value * val; } parm3 = { .val = val };
		return (int) arch_syscall_invoke4(parm0.x, parm1.x, parm2.x, parm3.x, K_SYSCALL_SENSOR_ATTR_GET);
	}
#endif
	compiler_barrier();
	return z_impl_sensor_attr_get(dev, chan, attr, val);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define sensor_attr_get(dev, chan, attr, val) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_SENSOR_ATTR_GET, sensor_attr_get, dev, chan, attr, val); 	syscall__retval = sensor_attr_get(dev, chan, attr, val); 	sys_port_trace_syscall_exit(K_SYSCALL_SENSOR_ATTR_GET, sensor_attr_get, dev, chan, attr, val, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_sensor_sample_fetch(const struct device * dev);

__pinned_func
static inline int sensor_sample_fetch(const struct device * dev)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		return (int) arch_syscall_invoke1(parm0.x, K_SYSCALL_SENSOR_SAMPLE_FETCH);
	}
#endif
	compiler_barrier();
	return z_impl_sensor_sample_fetch(dev);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define sensor_sample_fetch(dev) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_SENSOR_SAMPLE_FETCH, sensor_sample_fetch, dev); 	syscall__retval = sensor_sample_fetch(dev); 	sys_port_trace_syscall_exit(K_SYSCALL_SENSOR_SAMPLE_FETCH, sensor_sample_fetch, dev, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_sensor_sample_fetch_chan(const struct device * dev, enum sensor_channel type);

__pinned_func
static inline int sensor_sample_fetch_chan(const struct device * dev, enum sensor_channel type)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; enum sensor_channel val; } parm1 = { .val = type };
		return (int) arch_syscall_invoke2(parm0.x, parm1.x, K_SYSCALL_SENSOR_SAMPLE_FETCH_CHAN);
	}
#endif
	compiler_barrier();
	return z_impl_sensor_sample_fetch_chan(dev, type);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define sensor_sample_fetch_chan(dev, type) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_SENSOR_SAMPLE_FETCH_CHAN, sensor_sample_fetch_chan, dev, type); 	syscall__retval = sensor_sample_fetch_chan(dev, type); 	sys_port_trace_syscall_exit(K_SYSCALL_SENSOR_SAMPLE_FETCH_CHAN, sensor_sample_fetch_chan, dev, type, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_sensor_channel_get(const struct device * dev, enum sensor_channel chan, struct sensor_value * val);

__pinned_func
static inline int sensor_channel_get(const struct device * dev, enum sensor_channel chan, struct sensor_value * val)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; enum sensor_channel val; } parm1 = { .val = chan };
		union { uintptr_t x; struct sensor_value * val; } parm2 = { .val = val };
		return (int) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_SENSOR_CHANNEL_GET);
	}
#endif
	compiler_barrier();
	return z_impl_sensor_channel_get(dev, chan, val);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define sensor_channel_get(dev, chan, val) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_SENSOR_CHANNEL_GET, sensor_channel_get, dev, chan, val); 	syscall__retval = sensor_channel_get(dev, chan, val); 	sys_port_trace_syscall_exit(K_SYSCALL_SENSOR_CHANNEL_GET, sensor_channel_get, dev, chan, val, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_sensor_get_decoder(const struct device * dev, const struct sensor_decoder_api ** decoder);

__pinned_func
static inline int sensor_get_decoder(const struct device * dev, const struct sensor_decoder_api ** decoder)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; const struct sensor_decoder_api ** val; } parm1 = { .val = decoder };
		return (int) arch_syscall_invoke2(parm0.x, parm1.x, K_SYSCALL_SENSOR_GET_DECODER);
	}
#endif
	compiler_barrier();
	return z_impl_sensor_get_decoder(dev, decoder);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define sensor_get_decoder(dev, decoder) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_SENSOR_GET_DECODER, sensor_get_decoder, dev, decoder); 	syscall__retval = sensor_get_decoder(dev, decoder); 	sys_port_trace_syscall_exit(K_SYSCALL_SENSOR_GET_DECODER, sensor_get_decoder, dev, decoder, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_sensor_reconfigure_read_iodev(struct rtio_iodev * iodev, const struct device * sensor, const struct sensor_chan_spec * channels, size_t num_channels);

__pinned_func
static inline int sensor_reconfigure_read_iodev(struct rtio_iodev * iodev, const struct device * sensor, const struct sensor_chan_spec * channels, size_t num_channels)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; struct rtio_iodev * val; } parm0 = { .val = iodev };
		union { uintptr_t x; const struct device * val; } parm1 = { .val = sensor };
		union { uintptr_t x; const struct sensor_chan_spec * val; } parm2 = { .val = channels };
		union { uintptr_t x; size_t val; } parm3 = { .val = num_channels };
		return (int) arch_syscall_invoke4(parm0.x, parm1.x, parm2.x, parm3.x, K_SYSCALL_SENSOR_RECONFIGURE_READ_IODEV);
	}
#endif
	compiler_barrier();
	return z_impl_sensor_reconfigure_read_iodev(iodev, sensor, channels, num_channels);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define sensor_reconfigure_read_iodev(iodev, sensor, channels, num_channels) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_SENSOR_RECONFIGURE_READ_IODEV, sensor_reconfigure_read_iodev, iodev, sensor, channels, num_channels); 	syscall__retval = sensor_reconfigure_read_iodev(iodev, sensor, channels, num_channels); 	sys_port_trace_syscall_exit(K_SYSCALL_SENSOR_RECONFIGURE_READ_IODEV, sensor_reconfigure_read_iodev, iodev, sensor, channels, num_channels, syscall__retval); 	syscall__retval; })
#endif
#endif


#ifdef __cplusplus
}
#endif

#endif
#endif /* include guard */
