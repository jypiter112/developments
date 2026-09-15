A few elf x86 and x64 assembly projects I have made. Compiled with linker and clang.
<br>
Only used basic instructions and made with learning purposes only.
<br>
Reverse TCP I side referenced with a bear bones C server I wrote a while back.

#### Inline x64 assembly notes
Uses custom entry points and direct syscalls. Skips libc calls.
<br>
Skips high level libc hooks. Although doesn't skip Kernel-level auditing, lsm hooks and seccomp filters.
<br>
