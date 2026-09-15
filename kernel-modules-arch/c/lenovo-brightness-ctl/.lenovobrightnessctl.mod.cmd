savedcmd_lenovobrightnessctl.mod := printf '%s\n'   lenovobrightnessctl.o | awk '!x[$$0]++ { print("./"$$0) }' > lenovobrightnessctl.mod
