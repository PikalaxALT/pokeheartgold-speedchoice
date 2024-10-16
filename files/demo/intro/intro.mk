INTRO_DEMO_DIR := files/demo/intro/intro
INTRO_DEMO_NARC := $(INTRO_DEMO_DIR).narc
INTRO_DEMO_FILES := \
	intro_00000000.NCGR \
	intro_00000001.NCLR \
	intro_00000002.NCLR \
	intro_00000003.NSCR \
	intro_00000004.NSCR \
	intro_00000005.NSCR \
	intro_00000006.NSCR \
	intro_00000007.NSCR \
	intro_00000008.NSCR \
	intro_00000009.NSCR \
	intro_00000010.NCGR \
	intro_00000011.NCLR \
	intro_00000012.NCGR \
	intro_00000013.NCGR \
	intro_00000014.NCGR \
	intro_00000015.NCGR \
	intro_00000016.NCLR \
	intro_00000017.NCGR \
	intro_00000018.NCGR \
	intro_00000019.NCGR \
	intro_00000020.NCGR \
	intro_00000021.NCLR \
	intro_00000022.NCGR \
	intro_00000023.NCGR \
	intro_00000024.NCGR \
	intro_00000025.NCGR \
	intro_00000026.NCGR \
	intro_00000027.NCGR \
	intro_00000028.NCGR \
	intro_00000029.NCGR \
	intro_00000030.NCLR \
	intro_00000031.NCLR \
	intro_00000032.NCGR \
	intro_00000033.NCLR \
	intro_00000034.NCGR \
	intro_00000035.NCGR \
	intro_00000036.NCGR \
	intro_00000037.NCGR \
	intro_00000038.NCGR \
	intro_00000039.NCGR \
	intro_00000040.NCGR \
	intro_00000041.NCGR \
	intro_00000042.NCGR \
	intro_00000043.NSCR \
	intro_00000044.NSCR \
	intro_00000045.NSCR \
	intro_00000046.NSCR \
	intro_00000047.NSCR \
	intro_00000048.NSCR \
	intro_00000049.NSCR \
	intro_00000050.NSCR \
	intro_00000051.NSCR \
	intro_00000052.NSCR \
	intro_00000053.NCER \
	intro_00000054.NANR \
	intro_00000055.NCER \
	intro_00000056.NANR \
	intro_00000057.NCER \
	intro_00000058.NANR \
	intro_00000059.NCLR \
	intro_00000060.NCGR \
	intro_00000061.NCER \
	intro_00000062.NANR \
	intro_00000063.NCLR \
	intro_00000064.NCGR \
	intro_00000065.NCER \
	intro_00000066.NANR

$(INTRO_DEMO_NARC): $(addprefix $(INTRO_DEMO_DIR)/,$(INTRO_DEMO_FILES))
	$(KNARC) -p $@ -d $(INTRO_DEMO_DIR) -i

clean-intro:
	$(RM) $(INTRO_DEMO_NARC)

.PHONY: clean-intro
clean-filesystem: clean-intro
