savedcmd_testmod.mod := printf '%s\n'   testmod.o | awk '!x[$$0]++ { print("./"$$0) }' > testmod.mod
