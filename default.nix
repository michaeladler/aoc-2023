let
  pkgs = import <nixpkgs> { };
in
  pkgs.callPackage ./pkg.nix { }
