**Basic minhook test**
Places a few trampoline hooks and unregisters them.<br>
Usec ase would be hijacking a graphics driver like OpenGL, or placing trampoline hooks on vulnerable functions.
<br><br>
**Hook detection**
Common way is calling a offsite online API to gather database of the first few bytes of essential functions (like malloc, writememory, etc), and crossreferning them. If bytes dont match then a hook is placed and execution may be terminated or further actions taken.

#### Trampoline hook
jmp, farjump etc placed on the first few bytes of the function overwriting the original instructions. Original instructions copied to buffer, and after the hook function is called overwritten with the copy of the original instructions and the exection carries as normal. A low impact way of hooking a critical function without breaking functionality.
