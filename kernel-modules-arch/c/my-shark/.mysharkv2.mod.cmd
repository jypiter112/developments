savedcmd_mysharkv2.mod := printf '%s\n'   mysharkv2.o | awk '!x[$$0]++ { print("./"$$0) }' > mysharkv2.mod
