/** Trace point provider_0 */
#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER tp_0

#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE <tp_0.h>

#if !defined(_TP_0_H_) || defined (TRACEPOINT_HEADER_MULTI_READ)
#define _TP_0_H_

#include <lttng/tracepoint.h>

TRACEPOINT_EVENT(
    tp_0,
    my_second_tracepoint,
    TP_ARGS(
        char*, my_string_arg1,
        char*, my_string_arg2,
        char*, my_string_arg3,
        char*, my_string_arg4,
        char*, my_string_arg5
    ),
    TP_FIELDS(
        ctf_string(my_str_field1, my_string_arg1)
        ctf_string(my_str_field2, my_string_arg2)
        ctf_string(my_str_field3, my_string_arg3)
        ctf_string(my_str_field4, my_string_arg4)
        ctf_string(my_str_field5, my_string_arg5)
    )
)

#endif /* _TP_0_H_ */

#include <lttng/tracepoint-event.h>

