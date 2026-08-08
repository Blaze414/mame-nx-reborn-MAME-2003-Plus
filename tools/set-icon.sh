#!/bin/sh
# Install a new hbmenu icon from any image file.
#
#   tools/set-icon.sh ~/Downloads/whatever.png
#
# The Switch loader wants a 256x256 baseline JPEG. Anything else - a PNG, a
# progressive JPEG, a 1024px master - either shows as a blank tile in hbmenu or
# is rejected outright, and neither failure says anything on the way past. This
# normalises whatever it is given and leaves the original untouched.
set -e

src="$1"
dst="$(dirname "$0")/../icon.jpg"

if [ -z "$src" ] || [ ! -f "$src" ]; then
	echo "usage: $0 <image file>" >&2
	exit 1
fi

if command -v magick >/dev/null 2>&1; then
	# -flatten because the source is likely a PNG with transparency, and JPEG
	# has no alpha: without it the transparent corners come out black on some
	# encoders and white on others.
	magick "$src" \
		-background black -flatten \
		-resize 256x256^ -gravity center -extent 256x256 \
		-interlace none -quality 92 \
		-strip \
		"$dst"
else
	# sips cannot letterbox, so a non-square source is cropped by the resize.
	tmp="$(mktemp -t iconXXXXXX).png"
	cp "$src" "$tmp"
	sips -s format jpeg -z 256 256 "$tmp" --out "$dst" >/dev/null
	rm -f "$tmp"
fi

echo "wrote $dst"
file "$dst"
