savedcmd_firewall.mod := printf '%s\n'   firewall.o | awk '!x[$$0]++ { print("./"$$0) }' > firewall.mod
