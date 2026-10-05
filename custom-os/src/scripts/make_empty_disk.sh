#!/bin/bash
if [ -f data.img ]; then
    rm data.img
fi

dd if=/dev/zero of=data.img bs=512 count=202480

# Print recognizable data at the beginning of the disk
printf 'hello sector 0' | dd of=data.img bs=512 count=1 conv=notrunc
printf 'hello sector 1' | dd of=data.img bs=512 seek=1 count=1 conv=notrunc