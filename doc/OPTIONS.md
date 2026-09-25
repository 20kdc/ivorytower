# Options

ivorytower's `./setup` program takes a number of options and parameters.

Most of these are self-explanatory; just run `./setup --help`.

However, some of them deserve better explaination.

Those specific options will be described here.

## `box=`

Portable Linux compilation is best achieved using Docker containers.

However, build systems are not specifically adapted to the situation of the compiler living inside a Docker container, so a mechanism has to be used to provide an illusion of continuity.

Two of these mechanisms are available: A custom wrapper around Docker, and Distrobox.

* `box=ivt_docker`: Uses `docker` directly. I've found this is the best experience and thus it's the default.
    * This is implemented in `helpers/boxenrunner`.
    * The environment variable `ITSETUP_BOX_MOUNTS` defaults to `-v /home:/home -v /media:/media -v /run/user:/run/user` and can be used to adjust exposed mounts.
    * The environment variable `ITSETUP_BOX_ETCFILES` defaults to `-v /etc/passwd:/etc/passwd:ro -v /etc/group:/etc/group:ro` and adjusts exposed etcfiles.
* `box=distrobox`: Uses `distrobox` (which you must have).
    * _Set `DBX_CONTAINER_MANAGER=docker` in your environment!_ `distrobox` likes to prefer `podman`, but it's not actually a good idea to use podman for this.
        * I am considering the merits of simply requiring `distrobox` use `docker`, but this seems inflexible.
    * **DO NOT USE `podman` FOR THIS.** `podman` will download 800MB then throw it all away because you don't have subuid/subgid setup.
        * Rootlessness isn't even _possible_ here because `distrobox` will run your container `--privileged`. This will cause `podman` to require authentication.
    * **DO NOT USE `lilipod` FOR THIS.** `lilipod` has bad diagnostics; I _think_ this was also the subuid/subgid thing (I tried `podman` after).
