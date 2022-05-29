CC=clang
AR=llvm-ar

###########################################

vpath %.c src

include ../../common_cflags.mak
CFLAGS += -Werror $(CFLAGS_SYSROOT) -nostdlib
CFLAGS += -I. -Iupstream/src -Iinc
CFLAGS += -DCODEC2_MODE_EN_DEFAULT=0 -DCODEC2_MODE_3200_EN=1 -DCODEC2_MODE_1600_EN=1 -DFREEDV_MODE_EN_DEFAULT=0
CFLAGS += -DM_PI=3.14159265358979323846264338328

SRCS = upstream/src/dump.c upstream/src/lpc.c upstream/src/nlp.c upstream/src/phase.c upstream/src/quantise.c upstream/src/postfilter.c \
	upstream/src/codec2.c upstream/src/codec2_fft.c upstream/src/lsp.c upstream/src/sine.c upstream/src/interp.c upstream/src/kiss_fft.c \
	upstream/src/kiss_fftr.c upstream/src/newamp1.c upstream/src/codebook.c upstream/src/codebookd.c upstream/src/pack.c \
	upstream/src/codebooknewamp1.c upstream/src/codebooknewamp1_energy.c

OBJS = $(SRCS:.c=.o)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

libcodec2.a: $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) libcodec2.a
