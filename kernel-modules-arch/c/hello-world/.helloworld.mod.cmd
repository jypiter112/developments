savedcmd_helloworld.mod := printf '%s\n'   helloworld.o | awk '!x[$$0]++ { print("./"$$0) }' > helloworld.mod
