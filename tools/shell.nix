let
  nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/tarball/nixos-25.05";
  pkgs = import nixpkgs { config = {}; overlays = []; };
in

pkgs.mkShell {
  packages = with pkgs; [
    autoconf
    automake
    libtool
    cmake
    curl
    gcc
    git
    gnumake
    meson
    nasm
    ninja
    gnupatch
    perl
    pkg-config
    unzip

    SDL2

    p7zip
    ccache
    gettext
    luajitPackages.luacheck
  ];

  shellHook = ''
    export LD_LIBRARY_PATH=${pkgs.SDL2}/lib:$LD_LIBRARY_PATH
  '';
}
