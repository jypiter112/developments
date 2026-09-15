savedcmd_parammod.mod := printf '%s\n'   parammod.o | awk '!x[$$0]++ { print("./"$$0) }' > parammod.mod
