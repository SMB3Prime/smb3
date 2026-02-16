# Super Mario Bros. 3 Disassembly

Disassembly of Super Mario Bros 3 for CA65

Fork of DevRubicate's fork (https://github.com/devrubicate/smb3) of Captain Southbird's disassembly (https://github.com/captainsouthbird/smb3)

# Output

This project builds:
- `smb3.nes` - Stock (U) (PRG1) version of Super Mario Bros. 3
- `smb3-expanded.nes` - Expanded ROM version


-------------


# Building

This project builds in Docker using the `fortentb/smb3-build` image. That image was built from this repository's [`Dockerfile`](./Dockerfile).

### Prerequistes

If you prefer not to use Docker, you simply need `make` and `ca65`  (the assembler from `cc65`):

```
sudo apt install cc65 make
```

To assemble using Docker, `scuba` is recommended since it takes care of ensuring the code is mapped into the build container as your user and build artifacts have your user's permissions:

```
pipx install scuba
```

To assemble within Docker, simply run:

```
scuba build
```

Otherwise, just run:

```
make
```

To clean the project, run:
```
scuba clean
```
or
```
make clean
```

