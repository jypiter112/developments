savedcmd_myconf.mod := printf '%s\n'   myconf.o | awk '!x[$$0]++ { print("./"$$0) }' > myconf.mod
