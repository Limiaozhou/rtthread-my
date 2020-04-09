#include <rtthread.h>
#include <rthw.h>

rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];
struct rt_thread *rt_current_thread;

/**
 * @ingroup SystemInit
 * This function will initialize the system scheduler
 */
void rt_system_scheduler_init(void)
{
    register rt_base_t offset;

    for (offset = 0; offset < RT_THREAD_PRIORITY_MAX; offset ++)
    {
        rt_list_init(&rt_thread_priority_table[offset]);
    }

    rt_current_thread = RT_NULL;
}

/**
 * @ingroup SystemInit
 * This function will startup scheduler. It will select one thread
 * with the highest priority level, then switch to it.
 */
void rt_system_scheduler_start(void)
{
    register struct rt_thread *to_thread;

    /* get switch to thread */
    to_thread = rt_list_entry(rt_thread_priority_table[0].next,
                              struct rt_thread,
                              tlist);

    rt_current_thread = to_thread;

    /* switch to new thread */
    rt_hw_context_switch_to((rt_uint32_t)&to_thread->sp);

    /* never come back */
}

/**
 * @addtogroup Thread
 */

/**@{*/

/**
 * This function will perform one schedule. It will select one thread
 * with the highest priority level, then switch to it.
 */
void rt_schedule(void)
{
    struct rt_thread *to_thread;
    struct rt_thread *from_thread;
	
	/* 两个线程轮流切换 */
	if ( rt_current_thread == rt_list_entry( rt_thread_priority_table[0].next,
											 struct rt_thread,
											 tlist ) )
	{
		from_thread = rt_current_thread;
		to_thread = rt_list_entry( rt_thread_priority_table[1].next,
								   struct rt_thread,
								   tlist );
		rt_current_thread = to_thread;
	}
	else
	{
		from_thread = rt_current_thread;
		to_thread = rt_list_entry( rt_thread_priority_table[0].next,
								   struct rt_thread,
								   tlist );
		rt_current_thread = to_thread;
	}
	
	/* 产生上下文切换 */
	rt_hw_context_switch((rt_uint32_t)&from_thread->sp,(rt_uint32_t)&to_thread->sp);
}
