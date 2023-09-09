/*
    This is single-header library that provived iterators
    
    DOCUMENTATION
        
        To add an iterable type, you do this:
            #define ITERABLE_TYPES ADD_ITERATOR(T, begin, end, next, prev)
            #include "neat_iterator.h"
            
        T    : Type you want to be able to iterate over.
        
        begin: Function that returns a pointer to the first element,
               must have this prototype
                 E *begin(T*);
                 
        end  : Function that returns a pointer to past the last element,
               must have this prototype
                 E *end(T*);
                 
        next : Function that returns the next element of e,
               must have this prototype
                 E *next(T*, E* e);
                 
        prev : Function that returns the previous element of e,
               must have this prototype
                 E *prev(T*, E* e);
        
        
        Any of those can be NULL if you don't plan to use them.
        
        You can add as many iterable types as you want:
            #define ITERABLE_TYPES \
            ADD_ITERATOR(T, t_begin, t_end, t_next, t_prev), \
            ADD_ITERATOR(T2, t2_begin, t2_end, t2_next, t2_prev), \
            ADD_ITERATOR(T3, t3_begin, t3_end, t3_next, NULL)
            
            #include "neat_iterator.h"
        
        From here on out I'll refer to an iterable type with T 
        and will assume it has all 4 functions.
        
        Functions (actually macros):
            
            it_begin:
                E *it_begin(T t);
                  Returns a pointer to the first element of t
            
            it_end:
                E *it_end(T t);
                  Returns a pointer to past the last element of t
            
            it_next:
                E *it_next(T t, E *current);
                  Returns a pointer to the next element of current
            
            it_prev:
                E *it_prev(T t, E *current);
                  Returns a pointer to the previous element of current
            
            go_next:
                void go_next(T t, E *current);
                  Turns current into the next element
            
            go_prev:
                  void go_prev(T t, E *current);
                    Turns current into the previous element
            
            it_type:
                typeof(E*) it_type(T);
                  Returns the type of the iterator associated with 
                  an iterable type
            
        Loops:
            
            foreach(i, iterable) { ... }
                Iterates from the beginning to the end of iterable. With
                i being the name of the iterator.
            
            foreach_r(i, iterable) { ... }
                Iterates from the end to the beginning of iterable. With
                i being the name of the iterator.
            
            foreach_skip(i, iterable, n) { ... }
                Iterates from the beginning to the end of iterable,
                skips n steps with every iteration. With i being the
                name of the iterator.
            
            foreach_skip_r(i, iterable, n) { ... }
                Iterates from the end to the beginning of iterable,
                skips n steps with every iteration. With i being the
                name of the iterator.
*/

#ifndef NEAT_ITERATOR_H
#define NEAT_ITERATOR_H

#define ADD_ITERATOR(iterable_type, begin, end, next, prev) iterable_type: (typeof(begin(0))(*[])(iterable_type*)){(t_of_begin(begin, iterable_type))begin, (t_of_begin(begin, iterable_type))end, (t_of_begin(begin, iterable_type))next, (t_of_begin(begin, iterable_type))prev}

#define t_of_begin(b, it) typeof(b(0))(*)(it*)

#define NEAT_GET_ITERS_OF(iterable_type) _Generic(iterable_type, ITERABLE_TYPES)

#define it_type(t) typeof(NEAT_GET_ITERS_OF(t)[0](0))

#define it_begin(iterable) NEAT_GET_ITERS_OF(iterable)[0](&iterable)

#define it_end(iterable) NEAT_GET_ITERS_OF(iterable)[1](&iterable)

#define it_next(iterable, ptr) ((it_type(iterable) (*)(typeof(iterable)*, it_type(iterable) ))NEAT_GET_ITERS_OF(iterable)[2])(&iterable, ptr)

#define it_prev(iterable, ptr) ((it_type(iterable) (*)(typeof(iterable)*, it_type(iterable) ))NEAT_GET_ITERS_OF(iterable)[3])(&iterable, ptr)

#define go_next(iterable, ptr) (ptr = it_next(iterable, ptr))

#define go_prev(iterable, ptr) (ptr = it_prev(iterable, ptr))

#define foreach(name, iterable) for( it_type(iterable) name = it_begin(iterable), neat_last_##name = it_end(iterable); name != neat_last_##name ; name = it_next(iterable, name))

#define foreach_r(name, iterable) for( it_type(iterable) name = it_prev(iterable, it_end(iterable)), neat_last_##name = it_prev(iterable, it_begin(iterable)); name != neat_last_##name ; name = it_prev(iterable, name))

#define foreach_skip(name, iterable, by) for( it_type(iterable) name = it_begin(iterable), neat_last_##name = it_end(iterable); name != neat_last_##name ; name = neat_iter_skip(&iterable, name, (neat_generic_next_prev) NEAT_GET_ITERS_OF(iterable)[2], neat_last_##name, by))

#define foreach_skip_r(name, iterable, by) for( it_type(iterable) name = it_prev(iterable, it_end(iterable)), neat_last_##name = it_prev(iterable, it_begin(iterable)); name != neat_last_##name ; name = neat_iter_skip_r(&iterable, name, (neat_generic_next_prev) NEAT_GET_ITERS_OF(iterable)[3], neat_last_##name, by))

typedef void*(*neat_generic_next_prev)(void*, void*);
typedef void*(*neat_generic_begin_end)(void*);

static inline void *neat_iter_skip(void *iter, void *current, neat_generic_next_prev next, void *limit, int by)
{
    void *ret = current;
    for(int i = 0 ; i < by && ret != limit ; i++)
    {
        ret = next(iter, ret);
    }
    return ret;
}

static inline void *neat_iter_skip_r(void *iter, void *current, neat_generic_next_prev prev, void *limit, int by)
{
    void *ret = current;
    for(int i = 0 ; i < by && ret != limit ; i++)
    {
        ret = prev(iter, ret);
    }
    return ret;
}

#endif // NEAT_ITERATOR_H