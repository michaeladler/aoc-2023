{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  inputs.flake-utils.url = "github:numtide/flake-utils";
  inputs.mini-compile-commands = { url = github:danielbarter/mini_compile_commands; flake = false; };

  outputs = { self, nixpkgs, flake-utils, mini-compile-commands }:
    flake-utils.lib.eachDefaultSystem (system:
      let pkgs = nixpkgs.legacyPackages.${system};

      in
      rec {

        devShell = with pkgs;
          let
            mcc-env = (callPackage mini-compile-commands { }).wrap clangStdenv;
          in
          (mkShell.override { stdenv = mcc-env; }) {
            nativeBuildInputs = [ meson ninja clang-analyzer pkg-config ];
            buildInputs = [ xxHash ];
          };

        packages.default = pkgs.callPackage ./pkg.nix { };

      });
}
