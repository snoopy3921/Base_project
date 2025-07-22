include $(SRC_PATH)build/arch.mk
CFLAGS += -Wall -fno-strict-aliasing -fPIC -MMD -MP
ifeq ($(USE_STACK_PROTECTOR), Yes)
CFLAGS += -fstack-protector-all
endif
LDFLAGS += -static
STATIC_LDFLAGS += -lpthread -lm
AR_OPTS = crD $@


#### -march=$(ARCH)v8.1-m.main or  -mcpu=cortex-m85
#CFLAGS += -march=$(ARCH)v8.1-m.main -mthumb -Os
CFLAGS += -mcpu=cortex-m85 -mthumb -Os

CFLAGS += -fpermissive
CXXFLAGS += -fpermissive -Wno-invalid-pointer-conversion

ifeq ($(CXX), clang++)
CXXFLAGS += -Wc++11-compat-reserved-user-defined-literal
endif

