#ifndef SHARED_DEBUG_H
#define SHARED_DEBUG_H

#ifdef DEBUG

#define debugf(S, ...) \
  printf("debug : " S "\n", __VA_ARGS__)
#define debug(S) \
  printf("debug : " S "\n")
#define _DEBUG \
  printf("debug : function %s at %p \n", __FUNCTION__, &__func__);

#else

#define debugf(S, ...) do{}while(0)
#define debug(S) do{}while(0)
#define _DEBUG

#endif

#endif
