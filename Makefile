GBDKDIR ?= /opt/gbdk/
CC      = $(GBDKDIR)bin/lcc
ASSET   = $(GBDKDIR)bin/png2asset
CMASSET = ./collision_converter.py

CFLAGS    = -Wm-yc -Iinclude -Isrc -Wf-MMD
AFLAGSBKG = -map -use_map_attributes
AFLAGSSPR = -spr8x8 -noflip
CMAFLAGS  = -o $(INCLDIR)

SRCDIR  = src
BINDIR  = bin
OBJDIR  = build
RESDIR  = res
INCLDIR = include

TARGET = $(BINDIR)/ninthlife.gbc

# ---- Discover art ----
SPR_PNGS = $(wildcard $(RESDIR)/spr_*.png)
BKG_PNGS = $(wildcard $(RESDIR)/bkg_*.png)
CM_PNGS  = $(wildcard $(RESDIR)/cm_*.png)

# ---- Generated files ----
# sprites/backgrounds: png2asset makes a .c (src/) and a .h (moved to include/)
# collision maps: the script makes a header only (include/)
SPR_SRCS = $(patsubst $(RESDIR)/%.png,$(SRCDIR)/%.c,$(SPR_PNGS))
BKG_SRCS = $(patsubst $(RESDIR)/%.png,$(SRCDIR)/%.c,$(BKG_PNGS))
ASSET_SRCS = $(SPR_SRCS) $(BKG_SRCS)

SPR_HDRS = $(patsubst $(RESDIR)/%.png,$(INCLDIR)/%.h,$(SPR_PNGS))
BKG_HDRS = $(patsubst $(RESDIR)/%.png,$(INCLDIR)/%.h,$(BKG_PNGS))
CM_HDRS  = $(patsubst $(RESDIR)/%.png,$(INCLDIR)/%.h,$(CM_PNGS))
ASSET_HDRS = $(SPR_HDRS) $(BKG_HDRS) $(CM_HDRS)

# ---- Compile list: headers never go in here ----
SRCS = $(sort $(ASSET_SRCS) $(wildcard $(SRCDIR)/*.c))
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Every object waits for all generated headers to exist
$(OBJS): | $(ASSET_HDRS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# ---- Sprites ----
$(SRCDIR)/spr_%.c: $(RESDIR)/spr_%.png | $(INCLDIR)
	$(ASSET) $< $(AFLAGSSPR) -c $@
	mv -f $(SRCDIR)/spr_$*.h $(INCLDIR)/

# ---- Backgrounds ----
$(SRCDIR)/bkg_%.c: $(RESDIR)/bkg_%.png | $(INCLDIR)
	$(ASSET) $< $(AFLAGSBKG) -c $@
	mv -f $(SRCDIR)/bkg_$*.h $(INCLDIR)/

# The .c rules above also produce these headers; this tells make so
$(INCLDIR)/spr_%.h: $(SRCDIR)/spr_%.c ;
$(INCLDIR)/bkg_%.h: $(SRCDIR)/bkg_%.c ;

# ---- Collision maps ----
$(INCLDIR)/cm_%.h: $(RESDIR)/cm_%.png $(CMASSET) | $(INCLDIR)
	$(CMASSET) $< $(CMAFLAGS)

# ---- Directories ----
$(BINDIR) $(OBJDIR) $(INCLDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)
	rm -f $(SRCDIR)/bkg_*.* $(SRCDIR)/spr_*.*
	rm -f $(INCLDIR)/bkg_*.h $(INCLDIR)/spr_*.h $(INCLDIR)/cm_*.h

.PHONY: all clean

-include $(DEPS)
