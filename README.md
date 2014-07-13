# zramctl
Small tool, to setup and reset zram devices losetup like.

Using:
```
zramctl <action|name> <size> <compress alg> <num compress threads>
zramctl zram0 1024M lz4 4 # try to setup zram0 dev
zramctl find 1024M lz4 4  # find first free and use it dev
zramctl find              # return first free
zramctl reset zram0 zram1 ... # try to reset devices
```