savedcmd_mydriver.mod := printf '%s\n'   mydriver.o | awk '!x[$$0]++ { print("./"$$0) }' > mydriver.mod
