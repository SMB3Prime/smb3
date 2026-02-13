# smb3
Disassembly of Super Mario Bros 3 for CA65

Fork of DevRubicate's fork of Captain Southbird's disassembly (https://github.com/devrubicate/smb3) and (https://github.com/captainsouthbird/smb3)

-------------

This project builds in Docker using the `fortentb/smb3-build` image. That image was built from this repository's `Dockerfile`.

If you prefer not to use Docker, you simply need `make` and `ca65`.

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

