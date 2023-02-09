# Linux
## Install raylib
```bash
wget https://github.com/raysan5/raylib/archive/refs/tags/4.2.0.tar.gz
tar xvzf 4.2.0.tar.gz
cd raylib-4.2.0
make
sudo make install
sudo ldconfig
```

## Build project
```bash
cd /path/to/this/project
make build run
```