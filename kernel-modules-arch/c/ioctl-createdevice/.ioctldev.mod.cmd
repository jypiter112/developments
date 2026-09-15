savedcmd_ioctldev.mod := printf '%s\n'   ioctldev.o | awk '!x[$$0]++ { print("./"$$0) }' > ioctldev.mod
