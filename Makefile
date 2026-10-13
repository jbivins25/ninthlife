GBDKDIR ?= /opt/gbdk/
CC      = $(GBDKDIR)bin/lcc
ASSET   = $(GBDKDIR)bin/png2asset

CFLAGS  = -Wm-yc -Iinclude -Isrc -Wf-MMD
AFLAGSBKG = -map -use_map_attributes
AFLAGSSPR = -spr8x8 -noflip

SRCDIR = src
BINDIR = bin
OBJDIR = build
RESDIR = res
INCLDIR = include

TARGET = $(BINDIR)/ninthlife.gbc

SPR_PNGS = $(wildcard $(RESDIR)/spr_*.png)
BKG_PNGS = $(wildcard $(RESDIR)/bkg_*.png)

SPR_HDRS = $(patsubst $(RESDIR)/%.png,$(INCLDIR)/%.h,$(SPR_PNGS))
BKG_HDRS = $(patsubst $(RESDIR)/%.png,$(INCLDIR)/%.h,$(BKG_PNGS))
ASSET_HDRS = $(SPR_HDRS) $(BKG_HDRS)

SPR_ASSET_SRCS = $(patsubst $(RESDIR)/%.png,$(SRCDIR)/%.c,$(SPR_PNGS))
BKG_ASSET_SRCS = $(patsubst $(RESDIR)/%.png,$(SRCDIR)/%.c,$(BKG_PNGS))
ASSET_SRCS = $(SPR_ASSET_SRCS) $(BKG_ASSET_SRCS)

SRCS = $(sort $(ASSET_SRCS)) $(sort $(wildcard $(SRCDIR)/*.c))
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(SRCDIR)/spr_%.c: $(RESDIR)/spr_%.png | $(OBJDIR) $(INCLDIR)
	$(ASSET) $< $(AFLAGSSPR) -c $@
	mv $(SRCDIR)/spr_$*.h $(INCLDIR)/

$(SRCDIR)/bkg_%.c: $(RESDIR)/bkg_%.png | $(OBJDIR) $(INCLDIR)
	$(ASSET) $< $(AFLAGSBKG) -c $@
	mv $(SRCDIR)/bkg_$*.h $(INCLDIR)/

$(BINDIR) $(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)
	rm -rf $(SRCDIR)/bkg_*.* $(SRCDIR)/spr_*.*
	rm -rf $(INCLDIR)/bkg_*.h $(INCLDIR)/spr_*.h

.PHONY: all clean

-include $(DEPS)
